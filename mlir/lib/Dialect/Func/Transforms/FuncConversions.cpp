//===- FuncConversions.cpp - Function conversions -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/Func/Transforms/FuncConversions.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;
using namespace mlir::func;

/// Flatten the given value ranges into a single vector of values.
static SmallVector<Value> flattenValues(ArrayRef<ValueRange> values) {
  SmallVector<Value> result;
  for (const auto &vals : values)
    llvm::append_range(result, vals);
  return result;
}

namespace {
/// Converts the operand and result types of the CallOp, used together with the
/// FuncOpSignatureConversion.
struct CallOpSignatureConversion : public OpConversionPattern<CallOp> {
  using OpConversionPattern<CallOp>::OpConversionPattern;

  /// Hook for derived classes to implement combined matching and rewriting.
  LogicalResult
  matchAndRewrite(CallOp callOp, OneToNOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    // Convert the original function results. Keep track of how many result
    // types an original result type is converted into.
    SmallVector<size_t> numResultsReplacments;
    SmallVector<Type, 1> convertedResults;
    size_t numFlattenedResults = 0;
    for (auto [idx, type] : llvm::enumerate(callOp.getResultTypes())) {
      if (failed(typeConverter->convertTypes(type, convertedResults)))
        return failure();
      numResultsReplacments.push_back(convertedResults.size() -
                                      numFlattenedResults);
      numFlattenedResults = convertedResults.size();
    }

    // Substitute with the new result types from the corresponding FuncType
    // conversion.
    auto newCallOp =
        CallOp::create(rewriter, callOp.getLoc(), callOp.getCallee(),
                       convertedResults, flattenValues(adaptor.getOperands()));
    SmallVector<ValueRange> replacements;
    size_t offset = 0;
    for (int i = 0, e = callOp->getNumResults(); i < e; ++i) {
      replacements.push_back(
          newCallOp->getResults().slice(offset, numResultsReplacments[i]));
      offset += numResultsReplacments[i];
    }
    assert(offset == convertedResults.size() &&
           "expected that all converted results are used");
    rewriter.replaceOpWithMultiple(callOp, replacements);
    return success();
  }
};
} // namespace

void mlir::populateCallOpTypeConversionPattern(RewritePatternSet &patterns,
                                               const TypeConverter &converter) {
  patterns.add<CallOpSignatureConversion>(converter, patterns.getContext());
}

namespace {
/// Only needed to support partial conversion of functions where this pattern
/// ensures that the branch operation arguments matches up with the succesor
/// block arguments.
class BranchOpInterfaceTypeConversion
    : public OpInterfaceConversionPattern<BranchOpInterface> {
public:
  using OpInterfaceConversionPattern<
      BranchOpInterface>::OpInterfaceConversionPattern;

  BranchOpInterfaceTypeConversion(
      const TypeConverter &typeConverter, MLIRContext *ctx,
      function_ref<bool(BranchOpInterface, int)> shouldConvertBranchOperand)
      : OpInterfaceConversionPattern(typeConverter, ctx, /*benefit=*/1),
        shouldConvertBranchOperand(shouldConvertBranchOperand) {}

  LogicalResult
  matchAndRewrite(BranchOpInterface op, ArrayRef<Value> operands,
                  ConversionPatternRewriter &rewriter) const final {
    // For a branch operation, only some operands go to the target blocks, so
    // only rewrite those.
    SmallVector<Value, 4> newOperands(op->operand_begin(), op->operand_end());
    for (int succIdx = 0, succEnd = op->getBlock()->getNumSuccessors();
         succIdx < succEnd; ++succIdx) {
      OperandRange forwardedOperands =
          op.getSuccessorOperands(succIdx).getForwardedOperands();
      if (forwardedOperands.empty())
        continue;

      for (int idx = forwardedOperands.getBeginOperandIndex(),
               eidx = idx + forwardedOperands.size();
           idx < eidx; ++idx) {
        if (!shouldConvertBranchOperand || shouldConvertBranchOperand(op, idx))
          newOperands[idx] = operands[idx];
      }
    }
    rewriter.modifyOpInPlace(
        op, [newOperands, op]() { op->setOperands(newOperands); });
    return success();
  }

private:
  function_ref<bool(BranchOpInterface, int)> shouldConvertBranchOperand;
};
} // namespace

namespace {
/// Only needed to support partial conversion of functions where this pattern
/// ensures that the branch operation arguments matches up with the succesor
/// block arguments.
class ReturnOpTypeConversion : public OpConversionPattern<ReturnOp> {
public:
  using OpConversionPattern<ReturnOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(ReturnOp op, OneToNOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const final {
    rewriter.replaceOpWithNewOp<ReturnOp>(op,
                                          flattenValues(adaptor.getOperands()));
    return success();
  }
};
} // namespace

void mlir::populateBranchOpInterfaceTypeConversionPattern(
    RewritePatternSet &patterns, const TypeConverter &typeConverter,
    function_ref<bool(BranchOpInterface, int)> shouldConvertBranchOperand) {
  patterns.add<BranchOpInterfaceTypeConversion>(
      typeConverter, patterns.getContext(), shouldConvertBranchOperand);
}

bool mlir::isLegalForBranchOpInterfaceTypeConversionPattern(
    Operation *op, const TypeConverter &converter) {
  // All successor operands of branch like operations must be rewritten.
  if (auto branchOp = dyn_cast<BranchOpInterface>(op)) {
    for (int p = 0, e = op->getBlock()->getNumSuccessors(); p < e; ++p) {
      auto successorOperands = branchOp.getSuccessorOperands(p);
      if (!converter.isLegal(
              successorOperands.getForwardedOperands().getTypes()))
        return false;
    }
    return true;
  }

  return false;
}

void mlir::populateReturnOpTypeConversionPattern(
    RewritePatternSet &patterns, const TypeConverter &typeConverter) {
  patterns.add<ReturnOpTypeConversion>(typeConverter, patterns.getContext());
}

bool mlir::isLegalForReturnOpTypeConversionPattern(
    Operation *op, const TypeConverter &converter, bool returnOpAlwaysLegal) {
  // If this is a `return` and the user pass wants to convert/transform across
  // function boundaries, then `converter` is invoked to check whether the
  // `return` op is legal.
  if (isa<ReturnOp>(op) && !returnOpAlwaysLegal)
    return converter.isLegal(op);

  // ReturnLike operations have to be legalized with their parent. For
  // return this is handled, for other ops they remain as is.
  return op->hasTrait<OpTrait::ReturnLike>();
}

bool mlir::isNotBranchOpInterfaceOrReturnLikeOp(Operation *op) {
  // If it is not a terminator, ignore it.
  if (!op->mightHaveTrait<OpTrait::IsTerminator>())
    return true;

  // If it is not the last operation in the block, also ignore it. We do
  // this to handle unknown operations, as well.
  Block *block = op->getBlock();
  if (!block || &block->back() != op)
    return true;

  // We don't want to handle terminators in nested regions, assume they are
  // always legal.
  if (!isa_and_nonnull<FuncOp>(op->getParentOp()))
    return true;

  return false;
}

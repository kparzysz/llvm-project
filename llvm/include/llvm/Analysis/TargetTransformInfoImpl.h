//===- TargetTransformInfoImpl.h --------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file provides helpers for the implementation of
/// a TargetTransformInfo-conforming class.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_TARGETTRANSFORMINFOIMPL_H
#define LLVM_ANALYSIS_TARGETTRANSFORMINFOIMPL_H

#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/VectorUtils.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/PatternMatch.h"
#include <optional>
#include <utility>

namespace llvm {

class Function;

/// Base class for use as a mix-in that aids implementing
/// a TargetTransformInfo-compatible class.
class TargetTransformInfoImplBase {

protected:
  typedef TargetTransformInfo TTI;

  const DataLayout &DL;

  explicit TargetTransformInfoImplBase(const DataLayout &DL) : DL(DL) {}

public:
  virtual ~TargetTransformInfoImplBase();

  // Provide value semantics. MSVC requires that we spell all of these out.
  TargetTransformInfoImplBase(const TargetTransformInfoImplBase &Arg) = default;
  TargetTransformInfoImplBase(TargetTransformInfoImplBase &&Arg) : DL(Arg.DL) {}

  virtual const DataLayout &getDataLayout() const { return DL; }

  // FIXME: It looks like this implementation is dead. All clients appear to
  //  use the (non-const) version from `TargetTransformInfoImplCRTPBase`.
  virtual InstructionCost getGEPCost(Type *PointeeType, const Value *Ptr,
                                     ArrayRef<const Value *> Operands,
                                     Type *AccessType,
                                     TTI::TargetCostKind CostKind) const {
    // In the basic model, we just assume that all-constant GEPs will be folded
    // into their uses via addressing modes.
    for (const Value *Operand : Operands)
      if (!isa<Constant>(Operand))
        return TTI::TCC_Basic;

    return TTI::TCC_Free;
  }

  virtual InstructionCost
  getPointersChainCost(ArrayRef<const Value *> Ptrs, const Value *Base,
                       const TTI::PointersChainInfo &Info, Type *AccessTy,
                       TTI::TargetCostKind CostKind) const {
    llvm_unreachable("Not implemented");
  }

  virtual unsigned
  getEstimatedNumberOfCaseClusters(const SwitchInst &SI, unsigned &JTSize,
                                   ProfileSummaryInfo *PSI,
                                   BlockFrequencyInfo *BFI) const {
    (void)PSI;
    (void)BFI;
    JTSize = 0;
    return SI.getNumCases();
  }

  virtual InstructionCost
  getInstructionCost(const User *U, ArrayRef<const Value *> Operands,
                     TTI::TargetCostKind CostKind) const {
    llvm_unreachable("Not implemented");
  }

  virtual unsigned getInliningThresholdMultiplier() const { return 1; }
  virtual unsigned getInliningCostBenefitAnalysisSavingsMultiplier() const {
    return 8;
  }
  virtual unsigned getInliningCostBenefitAnalysisProfitableMultiplier() const {
    return 8;
  }
  virtual int getInliningLastCallToStaticBonus() const {
    // This is the value of InlineConstants::LastCallToStaticBonus before it was
    // removed along with the introduction of this function.
    return 15000;
  }
  virtual unsigned adjustInliningThreshold(const CallBase *CB) const {
    return 0;
  }
  virtual unsigned getCallerAllocaCost(const CallBase *CB,
                                       const AllocaInst *AI) const {
    return 0;
  };

  virtual int getInlinerVectorBonusPercent() const { return 150; }

  virtual InstructionCost getMemcpyCost(const Instruction *I) const {
    return TTI::TCC_Expensive;
  }

  virtual uint64_t getMaxMemIntrinsicInlineSizeThreshold() const { return 64; }

  // Although this default value is arbitrary, it is not random. It is assumed
  // that a condition that evaluates the same way by a higher percentage than
  // this is best represented as control flow. Therefore, the default value N
  // should be set such that the win from N% correct executions is greater than
  // the loss from (100 - N)% mispredicted executions for the majority of
  //  intended targets.
  virtual BranchProbability getPredictableBranchThreshold() const {
    return BranchProbability(99, 100);
  }

  virtual InstructionCost getBranchMispredictPenalty() const { return 0; }

  virtual bool hasBranchDivergence(const Function *F = nullptr) const {
    return false;
  }

  virtual bool isSourceOfDivergence(const Value *V) const { return false; }

  virtual bool isAlwaysUniform(const Value *V) const { return false; }

  virtual bool isValidAddrSpaceCast(unsigned FromAS, unsigned ToAS) const {
    return false;
  }

  virtual bool addrspacesMayAlias(unsigned AS0, unsigned AS1) const {
    return true;
  }

  virtual unsigned getFlatAddressSpace() const { return -1; }

  virtual bool collectFlatAddressOperands(SmallVectorImpl<int> &OpIndexes,
                                          Intrinsic::ID IID) const {
    return false;
  }

  virtual bool isNoopAddrSpaceCast(unsigned, unsigned) const { return false; }
  virtual bool
  canHaveNonUndefGlobalInitializerInAddressSpace(unsigned AS) const {
    return AS == 0;
  };

  virtual unsigned getAssumedAddrSpace(const Value *V) const { return -1; }

  virtual bool isSingleThreaded() const { return false; }

  virtual std::pair<const Value *, unsigned>
  getPredicatedAddrSpace(const Value *V) const {
    return std::make_pair(nullptr, -1);
  }

  virtual Value *rewriteIntrinsicWithAddressSpace(IntrinsicInst *II,
                                                  Value *OldV,
                                                  Value *NewV) const {
    return nullptr;
  }

  virtual bool isLoweredToCall(const Function *F) const {
    assert(F && "A concrete function must be provided to this routine.");

    // FIXME: These should almost certainly not be handled here, and instead
    // handled with the help of TLI or the target itself. This was largely
    // ported from existing analysis heuristics here so that such refactorings
    // can take place in the future.

    if (F->isIntrinsic())
      return false;

    if (F->hasLocalLinkage() || !F->hasName())
      return true;

    StringRef Name = F->getName();

    // These will all likely lower to a single selection DAG node.
    // clang-format off
    if (Name == "copysign" || Name == "copysignf" || Name == "copysignl" ||
        Name == "fabs"  || Name == "fabsf"  || Name == "fabsl" ||
        Name == "fmin"  || Name == "fminf"  || Name == "fminl" ||
        Name == "fmax"  || Name == "fmaxf"  || Name == "fmaxl" ||
        Name == "sin"   || Name == "sinf"   || Name == "sinl"  ||
        Name == "cos"   || Name == "cosf"   || Name == "cosl"  ||
        Name == "tan"   || Name == "tanf"   || Name == "tanl"  ||
        Name == "asin"  || Name == "asinf"  || Name == "asinl" ||
        Name == "acos"  || Name == "acosf"  || Name == "acosl" ||
        Name == "atan"  || Name == "atanf"  || Name == "atanl" ||
        Name == "atan2" || Name == "atan2f" || Name == "atan2l"||
        Name == "sinh"  || Name == "sinhf"  || Name == "sinhl" ||
        Name == "cosh"  || Name == "coshf"  || Name == "coshl" ||
        Name == "tanh"  || Name == "tanhf"  || Name == "tanhl" ||
        Name == "sqrt"  || Name == "sqrtf"  || Name == "sqrtl" ||
        Name == "exp10"  || Name == "exp10l"  || Name == "exp10f")
      return false;
    // clang-format on
    // These are all likely to be optimized into something smaller.
    if (Name == "pow" || Name == "powf" || Name == "powl" || Name == "exp2" ||
        Name == "exp2l" || Name == "exp2f" || Name == "floor" ||
        Name == "floorf" || Name == "ceil" || Name == "round" ||
        Name == "ffs" || Name == "ffsl" || Name == "abs" || Name == "labs" ||
        Name == "llabs")
      return false;

    return true;
  }

  virtual bool isHardwareLoopProfitable(Loop *L, ScalarEvolution &SE,
                                        AssumptionCache &AC,
                                        TargetLibraryInfo *LibInfo,
                                        HardwareLoopInfo &HWLoopInfo) const {
    return false;
  }

  virtual unsigned getEpilogueVectorizationMinVF() const { return 16; }

  virtual bool preferPredicateOverEpilogue(TailFoldingInfo *TFI) const {
    return false;
  }

  virtual TailFoldingStyle
  getPreferredTailFoldingStyle(bool IVUpdateMayOverflow = true) const {
    return TailFoldingStyle::DataWithoutLaneMask;
  }

  virtual std::optional<Instruction *>
  instCombineIntrinsic(InstCombiner &IC, IntrinsicInst &II) const {
    return std::nullopt;
  }

  virtual std::optional<Value *>
  simplifyDemandedUseBitsIntrinsic(InstCombiner &IC, IntrinsicInst &II,
                                   APInt DemandedMask, KnownBits &Known,
                                   bool &KnownBitsComputed) const {
    return std::nullopt;
  }

  virtual std::optional<Value *> simplifyDemandedVectorEltsIntrinsic(
      InstCombiner &IC, IntrinsicInst &II, APInt DemandedElts, APInt &UndefElts,
      APInt &UndefElts2, APInt &UndefElts3,
      std::function<void(Instruction *, unsigned, APInt, APInt &)>
          SimplifyAndSetOp) const {
    return std::nullopt;
  }

  virtual void getUnrollingPreferences(Loop *, ScalarEvolution &,
                                       TTI::UnrollingPreferences &,
                                       OptimizationRemarkEmitter *) const {}

  virtual void getPeelingPreferences(Loop *, ScalarEvolution &,
                                     TTI::PeelingPreferences &) const {}

  virtual bool isLegalAddImmediate(int64_t Imm) const { return false; }

  virtual bool isLegalAddScalableImmediate(int64_t Imm) const { return false; }

  virtual bool isLegalICmpImmediate(int64_t Imm) const { return false; }

  virtual bool isLegalAddressingMode(Type *Ty, GlobalValue *BaseGV,
                                     int64_t BaseOffset, bool HasBaseReg,
                                     int64_t Scale, unsigned AddrSpace,
                                     Instruction *I = nullptr,
                                     int64_t ScalableOffset = 0) const {
    // Guess that only reg and reg+reg addressing is allowed. This heuristic is
    // taken from the implementation of LSR.
    return !BaseGV && BaseOffset == 0 && (Scale == 0 || Scale == 1);
  }

  virtual bool isLSRCostLess(const TTI::LSRCost &C1,
                             const TTI::LSRCost &C2) const {
    return std::tie(C1.NumRegs, C1.AddRecCost, C1.NumIVMuls, C1.NumBaseAdds,
                    C1.ScaleCost, C1.ImmCost, C1.SetupCost) <
           std::tie(C2.NumRegs, C2.AddRecCost, C2.NumIVMuls, C2.NumBaseAdds,
                    C2.ScaleCost, C2.ImmCost, C2.SetupCost);
  }

  virtual bool isNumRegsMajorCostOfLSR() const { return true; }

  virtual bool shouldDropLSRSolutionIfLessProfitable() const { return false; }

  virtual bool isProfitableLSRChainElement(Instruction *I) const {
    return false;
  }

  virtual bool canMacroFuseCmp() const { return false; }

  virtual bool canSaveCmp(Loop *L, BranchInst **BI, ScalarEvolution *SE,
                          LoopInfo *LI, DominatorTree *DT, AssumptionCache *AC,
                          TargetLibraryInfo *LibInfo) const {
    return false;
  }

  virtual TTI::AddressingModeKind
  getPreferredAddressingMode(const Loop *L, ScalarEvolution *SE) const {
    return TTI::AMK_None;
  }

  virtual bool isLegalMaskedStore(Type *DataType, Align Alignment,
                                  unsigned AddressSpace) const {
    return false;
  }

  virtual bool isLegalMaskedLoad(Type *DataType, Align Alignment,
                                 unsigned AddressSpace) const {
    return false;
  }

  virtual bool isLegalNTStore(Type *DataType, Align Alignment) const {
    // By default, assume nontemporal memory stores are available for stores
    // that are aligned and have a size that is a power of 2.
    unsigned DataSize = DL.getTypeStoreSize(DataType);
    return Alignment >= DataSize && isPowerOf2_32(DataSize);
  }

  virtual bool isLegalNTLoad(Type *DataType, Align Alignment) const {
    // By default, assume nontemporal memory loads are available for loads that
    // are aligned and have a size that is a power of 2.
    unsigned DataSize = DL.getTypeStoreSize(DataType);
    return Alignment >= DataSize && isPowerOf2_32(DataSize);
  }

  virtual bool isLegalBroadcastLoad(Type *ElementTy,
                                    ElementCount NumElements) const {
    return false;
  }

  virtual bool isLegalMaskedScatter(Type *DataType, Align Alignment) const {
    return false;
  }

  virtual bool isLegalMaskedGather(Type *DataType, Align Alignment) const {
    return false;
  }

  virtual bool forceScalarizeMaskedGather(VectorType *DataType,
                                          Align Alignment) const {
    return false;
  }

  virtual bool forceScalarizeMaskedScatter(VectorType *DataType,
                                           Align Alignment) const {
    return false;
  }

  virtual bool isLegalMaskedCompressStore(Type *DataType,
                                          Align Alignment) const {
    return false;
  }

  virtual bool isLegalAltInstr(VectorType *VecTy, unsigned Opcode0,
                               unsigned Opcode1,
                               const SmallBitVector &OpcodeMask) const {
    return false;
  }

  virtual bool isLegalMaskedExpandLoad(Type *DataType, Align Alignment) const {
    return false;
  }

  virtual bool isLegalStridedLoadStore(Type *DataType, Align Alignment) const {
    return false;
  }

  virtual bool isLegalInterleavedAccessType(VectorType *VTy, unsigned Factor,
                                            Align Alignment,
                                            unsigned AddrSpace) const {
    return false;
  }

  virtual bool isLegalMaskedVectorHistogram(Type *AddrType,
                                            Type *DataType) const {
    return false;
  }

  virtual bool enableOrderedReductions() const { return false; }

  virtual bool hasDivRemOp(Type *DataType, bool IsSigned) const {
    return false;
  }

  virtual bool hasVolatileVariant(Instruction *I, unsigned AddrSpace) const {
    return false;
  }

  virtual bool prefersVectorizedAddressing() const { return true; }

  virtual InstructionCost getScalingFactorCost(Type *Ty, GlobalValue *BaseGV,
                                               StackOffset BaseOffset,
                                               bool HasBaseReg, int64_t Scale,
                                               unsigned AddrSpace) const {
    // Guess that all legal addressing mode are free.
    if (isLegalAddressingMode(Ty, BaseGV, BaseOffset.getFixed(), HasBaseReg,
                              Scale, AddrSpace, /*I=*/nullptr,
                              BaseOffset.getScalable()))
      return 0;
    return InstructionCost::getInvalid();
  }

  virtual bool LSRWithInstrQueries() const { return false; }

  virtual bool isTruncateFree(Type *Ty1, Type *Ty2) const { return false; }

  virtual bool isProfitableToHoist(Instruction *I) const { return true; }

  virtual bool useAA() const { return false; }

  virtual bool isTypeLegal(Type *Ty) const { return false; }

  virtual unsigned getRegUsageForType(Type *Ty) const { return 1; }

  virtual bool shouldBuildLookupTables() const { return true; }

  virtual bool shouldBuildLookupTablesForConstant(Constant *C) const {
    return true;
  }

  virtual bool shouldBuildRelLookupTables() const { return false; }

  virtual bool useColdCCForColdCall(Function &F) const { return false; }

  virtual bool isTargetIntrinsicTriviallyScalarizable(Intrinsic::ID ID) const {
    return false;
  }

  virtual bool isTargetIntrinsicWithScalarOpAtArg(Intrinsic::ID ID,
                                                  unsigned ScalarOpdIdx) const {
    return false;
  }

  virtual bool isTargetIntrinsicWithOverloadTypeAtArg(Intrinsic::ID ID,
                                                      int OpdIdx) const {
    return OpdIdx == -1;
  }

  virtual bool
  isTargetIntrinsicWithStructReturnOverloadAtField(Intrinsic::ID ID,
                                                   int RetIdx) const {
    return RetIdx == 0;
  }

  virtual InstructionCost getScalarizationOverhead(
      VectorType *Ty, const APInt &DemandedElts, bool Insert, bool Extract,
      TTI::TargetCostKind CostKind, bool ForPoisonSrc = true,
      ArrayRef<Value *> VL = {}) const {
    return 0;
  }

  virtual InstructionCost
  getOperandsScalarizationOverhead(ArrayRef<const Value *> Args,
                                   ArrayRef<Type *> Tys,
                                   TTI::TargetCostKind CostKind) const {
    return 0;
  }

  virtual bool supportsEfficientVectorElementLoadStore() const { return false; }

  virtual bool supportsTailCalls() const { return true; }

  virtual bool supportsTailCallFor(const CallBase *CB) const {
    llvm_unreachable("Not implemented");
  }

  virtual bool enableAggressiveInterleaving(bool LoopHasReductions) const {
    return false;
  }

  virtual TTI::MemCmpExpansionOptions
  enableMemCmpExpansion(bool OptSize, bool IsZeroCmp) const {
    return {};
  }

  virtual bool enableSelectOptimize() const { return true; }

  virtual bool shouldTreatInstructionLikeSelect(const Instruction *I) const {
    // A select with two constant operands will usually be better left as a
    // select.
    using namespace llvm::PatternMatch;
    if (match(I, m_Select(m_Value(), m_Constant(), m_Constant())))
      return false;
    // If the select is a logical-and/logical-or then it is better treated as a
    // and/or by the backend.
    return isa<SelectInst>(I) &&
           !match(I, m_CombineOr(m_LogicalAnd(m_Value(), m_Value()),
                                 m_LogicalOr(m_Value(), m_Value())));
  }

  virtual bool enableInterleavedAccessVectorization() const { return false; }

  virtual bool enableMaskedInterleavedAccessVectorization() const {
    return false;
  }

  virtual bool isFPVectorizationPotentiallyUnsafe() const { return false; }

  virtual bool allowsMisalignedMemoryAccesses(LLVMContext &Context,
                                              unsigned BitWidth,
                                              unsigned AddressSpace,
                                              Align Alignment,
                                              unsigned *Fast) const {
    return false;
  }

  virtual TTI::PopcntSupportKind
  getPopcntSupport(unsigned IntTyWidthInBit) const {
    return TTI::PSK_Software;
  }

  virtual bool haveFastSqrt(Type *Ty) const { return false; }

  virtual bool isExpensiveToSpeculativelyExecute(const Instruction *I) const {
    return true;
  }

  virtual bool isFCmpOrdCheaperThanFCmpZero(Type *Ty) const { return true; }

  virtual InstructionCost getFPOpCost(Type *Ty) const {
    return TargetTransformInfo::TCC_Basic;
  }

  virtual InstructionCost getIntImmCodeSizeCost(unsigned Opcode, unsigned Idx,
                                                const APInt &Imm,
                                                Type *Ty) const {
    return 0;
  }

  virtual InstructionCost getIntImmCost(const APInt &Imm, Type *Ty,
                                        TTI::TargetCostKind CostKind) const {
    return TTI::TCC_Basic;
  }

  virtual InstructionCost getIntImmCostInst(unsigned Opcode, unsigned Idx,
                                            const APInt &Imm, Type *Ty,
                                            TTI::TargetCostKind CostKind,
                                            Instruction *Inst = nullptr) const {
    return TTI::TCC_Free;
  }

  virtual InstructionCost
  getIntImmCostIntrin(Intrinsic::ID IID, unsigned Idx, const APInt &Imm,
                      Type *Ty, TTI::TargetCostKind CostKind) const {
    return TTI::TCC_Free;
  }

  virtual bool preferToKeepConstantsAttached(const Instruction &Inst,
                                             const Function &Fn) const {
    return false;
  }

  virtual unsigned getNumberOfRegisters(unsigned ClassID) const { return 8; }
  virtual bool hasConditionalLoadStoreForType(Type *Ty, bool IsStore) const {
    return false;
  }

  virtual unsigned getRegisterClassForType(bool Vector,
                                           Type *Ty = nullptr) const {
    return Vector ? 1 : 0;
  }

  virtual const char *getRegisterClassName(unsigned ClassID) const {
    switch (ClassID) {
    default:
      return "Generic::Unknown Register Class";
    case 0:
      return "Generic::ScalarRC";
    case 1:
      return "Generic::VectorRC";
    }
  }

  virtual TypeSize
  getRegisterBitWidth(TargetTransformInfo::RegisterKind K) const {
    return TypeSize::getFixed(32);
  }

  virtual unsigned getMinVectorRegisterBitWidth() const { return 128; }

  virtual std::optional<unsigned> getMaxVScale() const { return std::nullopt; }
  virtual std::optional<unsigned> getVScaleForTuning() const {
    return std::nullopt;
  }
  virtual bool isVScaleKnownToBeAPowerOfTwo() const { return false; }

  virtual bool
  shouldMaximizeVectorBandwidth(TargetTransformInfo::RegisterKind K) const {
    return false;
  }

  virtual ElementCount getMinimumVF(unsigned ElemWidth, bool IsScalable) const {
    return ElementCount::get(0, IsScalable);
  }

  virtual unsigned getMaximumVF(unsigned ElemWidth, unsigned Opcode) const {
    return 0;
  }
  virtual unsigned getStoreMinimumVF(unsigned VF, Type *, Type *) const {
    return VF;
  }

  virtual bool shouldConsiderAddressTypePromotion(
      const Instruction &I, bool &AllowPromotionWithoutCommonHeader) const {
    AllowPromotionWithoutCommonHeader = false;
    return false;
  }

  virtual unsigned getCacheLineSize() const { return 0; }
  virtual std::optional<unsigned>
  getCacheSize(TargetTransformInfo::CacheLevel Level) const {
    switch (Level) {
    case TargetTransformInfo::CacheLevel::L1D:
      [[fallthrough]];
    case TargetTransformInfo::CacheLevel::L2D:
      return std::nullopt;
    }
    llvm_unreachable("Unknown TargetTransformInfo::CacheLevel");
  }

  virtual std::optional<unsigned>
  getCacheAssociativity(TargetTransformInfo::CacheLevel Level) const {
    switch (Level) {
    case TargetTransformInfo::CacheLevel::L1D:
      [[fallthrough]];
    case TargetTransformInfo::CacheLevel::L2D:
      return std::nullopt;
    }

    llvm_unreachable("Unknown TargetTransformInfo::CacheLevel");
  }

  virtual std::optional<unsigned> getMinPageSize() const { return {}; }

  virtual unsigned getPrefetchDistance() const { return 0; }
  virtual unsigned getMinPrefetchStride(unsigned NumMemAccesses,
                                        unsigned NumStridedMemAccesses,
                                        unsigned NumPrefetches,
                                        bool HasCall) const {
    return 1;
  }
  virtual unsigned getMaxPrefetchIterationsAhead() const { return UINT_MAX; }
  virtual bool enableWritePrefetching() const { return false; }
  virtual bool shouldPrefetchAddressSpace(unsigned AS) const { return !AS; }

  virtual InstructionCost getPartialReductionCost(
      unsigned Opcode, Type *InputTypeA, Type *InputTypeB, Type *AccumType,
      ElementCount VF, TTI::PartialReductionExtendKind OpAExtend,
      TTI::PartialReductionExtendKind OpBExtend, std::optional<unsigned> BinOp,
      TTI::TargetCostKind CostKind) const {
    return InstructionCost::getInvalid();
  }

  virtual unsigned getMaxInterleaveFactor(ElementCount VF) const { return 1; }

  virtual InstructionCost getArithmeticInstrCost(
      unsigned Opcode, Type *Ty, TTI::TargetCostKind CostKind,
      TTI::OperandValueInfo Opd1Info, TTI::OperandValueInfo Opd2Info,
      ArrayRef<const Value *> Args, const Instruction *CxtI = nullptr) const {
    // Widenable conditions will eventually lower into constants, so some
    // operations with them will be trivially optimized away.
    auto IsWidenableCondition = [](const Value *V) {
      if (auto *II = dyn_cast<IntrinsicInst>(V))
        if (II->getIntrinsicID() == Intrinsic::experimental_widenable_condition)
          return true;
      return false;
    };
    // FIXME: A number of transformation tests seem to require these values
    // which seems a little odd for how arbitary there are.
    switch (Opcode) {
    default:
      break;
    case Instruction::FDiv:
    case Instruction::FRem:
    case Instruction::SDiv:
    case Instruction::SRem:
    case Instruction::UDiv:
    case Instruction::URem:
      // FIXME: Unlikely to be true for CodeSize.
      return TTI::TCC_Expensive;
    case Instruction::And:
    case Instruction::Or:
      if (any_of(Args, IsWidenableCondition))
        return TTI::TCC_Free;
      break;
    }

    // Assume a 3cy latency for fp arithmetic ops.
    if (CostKind == TTI::TCK_Latency)
      if (Ty->getScalarType()->isFloatingPointTy())
        return 3;

    return 1;
  }

  virtual InstructionCost getAltInstrCost(VectorType *VecTy, unsigned Opcode0,
                                          unsigned Opcode1,
                                          const SmallBitVector &OpcodeMask,
                                          TTI::TargetCostKind CostKind) const {
    return InstructionCost::getInvalid();
  }

  virtual InstructionCost
  getShuffleCost(TTI::ShuffleKind Kind, VectorType *DstTy, VectorType *SrcTy,
                 ArrayRef<int> Mask, TTI::TargetCostKind CostKind, int Index,
                 VectorType *SubTp, ArrayRef<const Value *> Args = {},
                 const Instruction *CxtI = nullptr) const {
    return 1;
  }

  virtual InstructionCost getCastInstrCost(unsigned Opcode, Type *Dst,
                                           Type *Src, TTI::CastContextHint CCH,
                                           TTI::TargetCostKind CostKind,
                                           const Instruction *I) const {
    switch (Opcode) {
    default:
      break;
    case Instruction::IntToPtr: {
      unsigned SrcSize = Src->getScalarSizeInBits();
      if (DL.isLegalInteger(SrcSize) &&
          SrcSize <= DL.getPointerTypeSizeInBits(Dst))
        return 0;
      break;
    }
    case Instruction::PtrToInt: {
      unsigned DstSize = Dst->getScalarSizeInBits();
      if (DL.isLegalInteger(DstSize) &&
          DstSize >= DL.getPointerTypeSizeInBits(Src))
        return 0;
      break;
    }
    case Instruction::BitCast:
      if (Dst == Src || (Dst->isPointerTy() && Src->isPointerTy()))
        // Identity and pointer-to-pointer casts are free.
        return 0;
      break;
    case Instruction::Trunc: {
      // trunc to a native type is free (assuming the target has compare and
      // shift-right of the same width).
      TypeSize DstSize = DL.getTypeSizeInBits(Dst);
      if (!DstSize.isScalable() && DL.isLegalInteger(DstSize.getFixedValue()))
        return 0;
      break;
    }
    }
    return 1;
  }

  virtual InstructionCost
  getExtractWithExtendCost(unsigned Opcode, Type *Dst, VectorType *VecTy,
                           unsigned Index, TTI::TargetCostKind CostKind) const {
    return 1;
  }

  virtual InstructionCost getCFInstrCost(unsigned Opcode,
                                         TTI::TargetCostKind CostKind,
                                         const Instruction *I = nullptr) const {
    // A phi would be free, unless we're costing the throughput because it
    // will require a register.
    if (Opcode == Instruction::PHI && CostKind != TTI::TCK_RecipThroughput)
      return 0;
    return 1;
  }

  virtual InstructionCost getCmpSelInstrCost(
      unsigned Opcode, Type *ValTy, Type *CondTy, CmpInst::Predicate VecPred,
      TTI::TargetCostKind CostKind, TTI::OperandValueInfo Op1Info,
      TTI::OperandValueInfo Op2Info, const Instruction *I) const {
    return 1;
  }

  virtual InstructionCost getVectorInstrCost(unsigned Opcode, Type *Val,
                                             TTI::TargetCostKind CostKind,
                                             unsigned Index, const Value *Op0,
                                             const Value *Op1) const {
    return 1;
  }

  /// \param ScalarUserAndIdx encodes the information about extracts from a
  /// vector with 'Scalar' being the value being extracted,'User' being the user
  /// of the extract(nullptr if user is not known before vectorization) and
  /// 'Idx' being the extract lane.
  virtual InstructionCost getVectorInstrCost(
      unsigned Opcode, Type *Val, TTI::TargetCostKind CostKind, unsigned Index,
      Value *Scalar,
      ArrayRef<std::tuple<Value *, User *, int>> ScalarUserAndIdx) const {
    return 1;
  }

  virtual InstructionCost getVectorInstrCost(const Instruction &I, Type *Val,
                                             TTI::TargetCostKind CostKind,
                                             unsigned Index) const {
    return 1;
  }

  virtual InstructionCost
  getReplicationShuffleCost(Type *EltTy, int ReplicationFactor, int VF,
                            const APInt &DemandedDstElts,
                            TTI::TargetCostKind CostKind) const {
    return 1;
  }

  virtual InstructionCost
  getInsertExtractValueCost(unsigned Opcode,
                            TTI::TargetCostKind CostKind) const {
    // Note: The `insertvalue` cost here is chosen to match the default case of
    // getInstructionCost() -- as prior to adding this helper `insertvalue` was
    // not handled.
    if (Opcode == Instruction::InsertValue &&
        CostKind != TTI::TCK_RecipThroughput)
      return TTI::TCC_Basic;
    return TTI::TCC_Free;
  }

  virtual InstructionCost
  getMemoryOpCost(unsigned Opcode, Type *Src, Align Alignment,
                  unsigned AddressSpace, TTI::TargetCostKind CostKind,
                  TTI::OperandValueInfo OpInfo, const Instruction *I) const {
    return 1;
  }

  virtual InstructionCost getVPMemoryOpCost(unsigned Opcode, Type *Src,
                                            Align Alignment,
                                            unsigned AddressSpace,
                                            TTI::TargetCostKind CostKind,
                                            const Instruction *I) const {
    return 1;
  }

  virtual InstructionCost
  getMaskedMemoryOpCost(unsigned Opcode, Type *Src, Align Alignment,
                        unsigned AddressSpace,
                        TTI::TargetCostKind CostKind) const {
    return 1;
  }

  virtual InstructionCost
  getGatherScatterOpCost(unsigned Opcode, Type *DataTy, const Value *Ptr,
                         bool VariableMask, Align Alignment,
                         TTI::TargetCostKind CostKind,
                         const Instruction *I = nullptr) const {
    return 1;
  }

  virtual InstructionCost getExpandCompressMemoryOpCost(
      unsigned Opcode, Type *DataTy, bool VariableMask, Align Alignment,
      TTI::TargetCostKind CostKind, const Instruction *I = nullptr) const {
    return 1;
  }

  virtual InstructionCost
  getStridedMemoryOpCost(unsigned Opcode, Type *DataTy, const Value *Ptr,
                         bool VariableMask, Align Alignment,
                         TTI::TargetCostKind CostKind,
                         const Instruction *I = nullptr) const {
    return InstructionCost::getInvalid();
  }

  virtual InstructionCost getInterleavedMemoryOpCost(
      unsigned Opcode, Type *VecTy, unsigned Factor, ArrayRef<unsigned> Indices,
      Align Alignment, unsigned AddressSpace, TTI::TargetCostKind CostKind,
      bool UseMaskForCond, bool UseMaskForGaps) const {
    return 1;
  }

  virtual InstructionCost
  getIntrinsicInstrCost(const IntrinsicCostAttributes &ICA,
                        TTI::TargetCostKind CostKind) const {
    switch (ICA.getID()) {
    default:
      break;
    case Intrinsic::experimental_vector_histogram_add:
      // For now, we want explicit support from the target for histograms.
      return InstructionCost::getInvalid();
    case Intrinsic::allow_runtime_check:
    case Intrinsic::allow_ubsan_check:
    case Intrinsic::annotation:
    case Intrinsic::assume:
    case Intrinsic::sideeffect:
    case Intrinsic::pseudoprobe:
    case Intrinsic::arithmetic_fence:
    case Intrinsic::dbg_assign:
    case Intrinsic::dbg_declare:
    case Intrinsic::dbg_value:
    case Intrinsic::dbg_label:
    case Intrinsic::invariant_start:
    case Intrinsic::invariant_end:
    case Intrinsic::launder_invariant_group:
    case Intrinsic::strip_invariant_group:
    case Intrinsic::is_constant:
    case Intrinsic::lifetime_start:
    case Intrinsic::lifetime_end:
    case Intrinsic::experimental_noalias_scope_decl:
    case Intrinsic::objectsize:
    case Intrinsic::ptr_annotation:
    case Intrinsic::var_annotation:
    case Intrinsic::experimental_gc_result:
    case Intrinsic::experimental_gc_relocate:
    case Intrinsic::coro_alloc:
    case Intrinsic::coro_begin:
    case Intrinsic::coro_begin_custom_abi:
    case Intrinsic::coro_free:
    case Intrinsic::coro_end:
    case Intrinsic::coro_frame:
    case Intrinsic::coro_size:
    case Intrinsic::coro_align:
    case Intrinsic::coro_suspend:
    case Intrinsic::coro_subfn_addr:
    case Intrinsic::threadlocal_address:
    case Intrinsic::experimental_widenable_condition:
    case Intrinsic::ssa_copy:
      // These intrinsics don't actually represent code after lowering.
      return 0;
    }
    return 1;
  }

  virtual InstructionCost getCallInstrCost(Function *F, Type *RetTy,
                                           ArrayRef<Type *> Tys,
                                           TTI::TargetCostKind CostKind) const {
    return 1;
  }

  // Assume that we have a register of the right size for the type.
  virtual unsigned getNumberOfParts(Type *Tp) const { return 1; }

  virtual InstructionCost getAddressComputationCost(Type *Tp, ScalarEvolution *,
                                                    const SCEV *) const {
    return 0;
  }

  virtual InstructionCost
  getArithmeticReductionCost(unsigned, VectorType *,
                             std::optional<FastMathFlags> FMF,
                             TTI::TargetCostKind) const {
    return 1;
  }

  virtual InstructionCost getMinMaxReductionCost(Intrinsic::ID IID,
                                                 VectorType *, FastMathFlags,
                                                 TTI::TargetCostKind) const {
    return 1;
  }

  virtual InstructionCost
  getExtendedReductionCost(unsigned Opcode, bool IsUnsigned, Type *ResTy,
                           VectorType *Ty, std::optional<FastMathFlags> FMF,
                           TTI::TargetCostKind CostKind) const {
    return 1;
  }

  virtual InstructionCost
  getMulAccReductionCost(bool IsUnsigned, Type *ResTy, VectorType *Ty,
                         TTI::TargetCostKind CostKind) const {
    return 1;
  }

  virtual InstructionCost
  getCostOfKeepingLiveOverCall(ArrayRef<Type *> Tys) const {
    return 0;
  }

  virtual bool getTgtMemIntrinsic(IntrinsicInst *Inst,
                                  MemIntrinsicInfo &Info) const {
    return false;
  }

  virtual unsigned getAtomicMemIntrinsicMaxElementSize() const {
    // Note for overrides: You must ensure for all element unordered-atomic
    // memory intrinsics that all power-of-2 element sizes up to, and
    // including, the return value of this method have a corresponding
    // runtime lib call. These runtime lib call definitions can be found
    // in RuntimeLibcalls.h
    return 0;
  }

  virtual Value *
  getOrCreateResultFromMemIntrinsic(IntrinsicInst *Inst, Type *ExpectedType,
                                    bool CanCreate = true) const {
    return nullptr;
  }

  virtual Type *
  getMemcpyLoopLoweringType(LLVMContext &Context, Value *Length,
                            unsigned SrcAddrSpace, unsigned DestAddrSpace,
                            Align SrcAlign, Align DestAlign,
                            std::optional<uint32_t> AtomicElementSize) const {
    return AtomicElementSize ? Type::getIntNTy(Context, *AtomicElementSize * 8)
                             : Type::getInt8Ty(Context);
  }

  virtual void getMemcpyLoopResidualLoweringType(
      SmallVectorImpl<Type *> &OpsOut, LLVMContext &Context,
      unsigned RemainingBytes, unsigned SrcAddrSpace, unsigned DestAddrSpace,
      Align SrcAlign, Align DestAlign,
      std::optional<uint32_t> AtomicCpySize) const {
    unsigned OpSizeInBytes = AtomicCpySize.value_or(1);
    Type *OpType = Type::getIntNTy(Context, OpSizeInBytes * 8);
    for (unsigned i = 0; i != RemainingBytes; i += OpSizeInBytes)
      OpsOut.push_back(OpType);
  }

  virtual bool areInlineCompatible(const Function *Caller,
                                   const Function *Callee) const {
    return (Caller->getFnAttribute("target-cpu") ==
            Callee->getFnAttribute("target-cpu")) &&
           (Caller->getFnAttribute("target-features") ==
            Callee->getFnAttribute("target-features"));
  }

  virtual unsigned getInlineCallPenalty(const Function *F, const CallBase &Call,
                                        unsigned DefaultCallPenalty) const {
    return DefaultCallPenalty;
  }

  virtual bool areTypesABICompatible(const Function *Caller,
                                     const Function *Callee,
                                     const ArrayRef<Type *> &Types) const {
    return (Caller->getFnAttribute("target-cpu") ==
            Callee->getFnAttribute("target-cpu")) &&
           (Caller->getFnAttribute("target-features") ==
            Callee->getFnAttribute("target-features"));
  }

  virtual bool isIndexedLoadLegal(TTI::MemIndexedMode Mode, Type *Ty) const {
    return false;
  }

  virtual bool isIndexedStoreLegal(TTI::MemIndexedMode Mode, Type *Ty) const {
    return false;
  }

  virtual unsigned getLoadStoreVecRegBitWidth(unsigned AddrSpace) const {
    return 128;
  }

  virtual bool isLegalToVectorizeLoad(LoadInst *LI) const { return true; }

  virtual bool isLegalToVectorizeStore(StoreInst *SI) const { return true; }

  virtual bool isLegalToVectorizeLoadChain(unsigned ChainSizeInBytes,
                                           Align Alignment,
                                           unsigned AddrSpace) const {
    return true;
  }

  virtual bool isLegalToVectorizeStoreChain(unsigned ChainSizeInBytes,
                                            Align Alignment,
                                            unsigned AddrSpace) const {
    return true;
  }

  virtual bool isLegalToVectorizeReduction(const RecurrenceDescriptor &RdxDesc,
                                           ElementCount VF) const {
    return true;
  }

  virtual bool isElementTypeLegalForScalableVector(Type *Ty) const {
    return true;
  }

  virtual unsigned getLoadVectorFactor(unsigned VF, unsigned LoadSize,
                                       unsigned ChainSizeInBytes,
                                       VectorType *VecTy) const {
    return VF;
  }

  virtual unsigned getStoreVectorFactor(unsigned VF, unsigned StoreSize,
                                        unsigned ChainSizeInBytes,
                                        VectorType *VecTy) const {
    return VF;
  }

  virtual bool preferFixedOverScalableIfEqualCost() const { return false; }

  virtual bool preferInLoopReduction(RecurKind Kind, Type *Ty) const {
    return false;
  }
  virtual bool preferAlternateOpcodeVectorization() const { return true; }

  virtual bool preferPredicatedReductionSelect() const { return false; }

  virtual bool preferEpilogueVectorization() const { return true; }

  virtual bool shouldExpandReduction(const IntrinsicInst *II) const {
    return true;
  }

  virtual TTI::ReductionShuffle
  getPreferredExpandedReductionShuffle(const IntrinsicInst *II) const {
    return TTI::ReductionShuffle::SplitHalf;
  }

  virtual unsigned getGISelRematGlobalCost() const { return 1; }

  virtual unsigned getMinTripCountTailFoldingThreshold() const { return 0; }

  virtual bool supportsScalableVectors() const { return false; }

  virtual bool enableScalableVectorization() const { return false; }

  virtual bool hasActiveVectorLength() const { return false; }

  virtual bool isProfitableToSinkOperands(Instruction *I,
                                          SmallVectorImpl<Use *> &Ops) const {
    return false;
  }

  virtual bool isVectorShiftByScalarCheap(Type *Ty) const { return false; }

  virtual TargetTransformInfo::VPLegalization
  getVPLegalizationStrategy(const VPIntrinsic &PI) const {
    return TargetTransformInfo::VPLegalization(
        /* EVLParamStrategy */ TargetTransformInfo::VPLegalization::Discard,
        /* OperatorStrategy */ TargetTransformInfo::VPLegalization::Convert);
  }

  virtual bool hasArmWideBranch(bool) const { return false; }

  virtual APInt getFeatureMask(const Function &F) const {
    return APInt::getZero(32);
  }

  virtual bool isMultiversionedFunction(const Function &F) const {
    return false;
  }

  virtual unsigned getMaxNumArgs() const { return UINT_MAX; }

  virtual unsigned getNumBytesToPadGlobalArray(unsigned Size,
                                               Type *ArrayType) const {
    return 0;
  }

  virtual void collectKernelLaunchBounds(
      const Function &F,
      SmallVectorImpl<std::pair<StringRef, int64_t>> &LB) const {}

  virtual bool allowVectorElementIndexingUsingGEP() const { return true; }

protected:
  // Obtain the minimum required size to hold the value (without the sign)
  // In case of a vector it returns the min required size for one element.
  unsigned minRequiredElementSize(const Value *Val, bool &isSigned) const {
    if (isa<ConstantDataVector>(Val) || isa<ConstantVector>(Val)) {
      const auto *VectorValue = cast<Constant>(Val);

      // In case of a vector need to pick the max between the min
      // required size for each element
      auto *VT = cast<FixedVectorType>(Val->getType());

      // Assume unsigned elements
      isSigned = false;

      // The max required size is the size of the vector element type
      unsigned MaxRequiredSize =
          VT->getElementType()->getPrimitiveSizeInBits().getFixedValue();

      unsigned MinRequiredSize = 0;
      for (unsigned i = 0, e = VT->getNumElements(); i < e; ++i) {
        if (auto *IntElement =
                dyn_cast<ConstantInt>(VectorValue->getAggregateElement(i))) {
          bool signedElement = IntElement->getValue().isNegative();
          // Get the element min required size.
          unsigned ElementMinRequiredSize =
              IntElement->getValue().getSignificantBits() - 1;
          // In case one element is signed then all the vector is signed.
          isSigned |= signedElement;
          // Save the max required bit size between all the elements.
          MinRequiredSize = std::max(MinRequiredSize, ElementMinRequiredSize);
        } else {
          // not an int constant element
          return MaxRequiredSize;
        }
      }
      return MinRequiredSize;
    }

    if (const auto *CI = dyn_cast<ConstantInt>(Val)) {
      isSigned = CI->getValue().isNegative();
      return CI->getValue().getSignificantBits() - 1;
    }

    if (const auto *Cast = dyn_cast<SExtInst>(Val)) {
      isSigned = true;
      return Cast->getSrcTy()->getScalarSizeInBits() - 1;
    }

    if (const auto *Cast = dyn_cast<ZExtInst>(Val)) {
      isSigned = false;
      return Cast->getSrcTy()->getScalarSizeInBits();
    }

    isSigned = false;
    return Val->getType()->getScalarSizeInBits();
  }

  bool isStridedAccess(const SCEV *Ptr) const {
    return Ptr && isa<SCEVAddRecExpr>(Ptr);
  }

  const SCEVConstant *getConstantStrideStep(ScalarEvolution *SE,
                                            const SCEV *Ptr) const {
    if (!isStridedAccess(Ptr))
      return nullptr;
    const SCEVAddRecExpr *AddRec = cast<SCEVAddRecExpr>(Ptr);
    return dyn_cast<SCEVConstant>(AddRec->getStepRecurrence(*SE));
  }

  bool isConstantStridedAccessLessThan(ScalarEvolution *SE, const SCEV *Ptr,
                                       int64_t MergeDistance) const {
    const SCEVConstant *Step = getConstantStrideStep(SE, Ptr);
    if (!Step)
      return false;
    APInt StrideVal = Step->getAPInt();
    if (StrideVal.getBitWidth() > 64)
      return false;
    // FIXME: Need to take absolute value for negative stride case.
    return StrideVal.getSExtValue() < MergeDistance;
  }
};

/// CRTP base class for use as a mix-in that aids implementing
/// a TargetTransformInfo-compatible class.
template <typename T>
class TargetTransformInfoImplCRTPBase : public TargetTransformInfoImplBase {
private:
  typedef TargetTransformInfoImplBase BaseT;

protected:
  explicit TargetTransformInfoImplCRTPBase(const DataLayout &DL) : BaseT(DL) {}

public:
  InstructionCost getGEPCost(Type *PointeeType, const Value *Ptr,
                             ArrayRef<const Value *> Operands, Type *AccessType,
                             TTI::TargetCostKind CostKind) const override {
    assert(PointeeType && Ptr && "can't get GEPCost of nullptr");
    auto *BaseGV = dyn_cast<GlobalValue>(Ptr->stripPointerCasts());
    bool HasBaseReg = (BaseGV == nullptr);

    auto PtrSizeBits = DL.getPointerTypeSizeInBits(Ptr->getType());
    APInt BaseOffset(PtrSizeBits, 0);
    int64_t Scale = 0;

    auto GTI = gep_type_begin(PointeeType, Operands);
    Type *TargetType = nullptr;

    // Handle the case where the GEP instruction has a single operand,
    // the basis, therefore TargetType is a nullptr.
    if (Operands.empty())
      return !BaseGV ? TTI::TCC_Free : TTI::TCC_Basic;

    for (auto I = Operands.begin(); I != Operands.end(); ++I, ++GTI) {
      TargetType = GTI.getIndexedType();
      // We assume that the cost of Scalar GEP with constant index and the
      // cost of Vector GEP with splat constant index are the same.
      const ConstantInt *ConstIdx = dyn_cast<ConstantInt>(*I);
      if (!ConstIdx)
        if (auto Splat = getSplatValue(*I))
          ConstIdx = dyn_cast<ConstantInt>(Splat);
      if (StructType *STy = GTI.getStructTypeOrNull()) {
        // For structures the index is always splat or scalar constant
        assert(ConstIdx && "Unexpected GEP index");
        uint64_t Field = ConstIdx->getZExtValue();
        BaseOffset += DL.getStructLayout(STy)->getElementOffset(Field);
      } else {
        // If this operand is a scalable type, bail out early.
        // TODO: Make isLegalAddressingMode TypeSize aware.
        if (TargetType->isScalableTy())
          return TTI::TCC_Basic;
        int64_t ElementSize =
            GTI.getSequentialElementStride(DL).getFixedValue();
        if (ConstIdx) {
          BaseOffset +=
              ConstIdx->getValue().sextOrTrunc(PtrSizeBits) * ElementSize;
        } else {
          // Needs scale register.
          if (Scale != 0)
            // No addressing mode takes two scale registers.
            return TTI::TCC_Basic;
          Scale = ElementSize;
        }
      }
    }

    // If we haven't been provided a hint, use the target type for now.
    //
    // TODO: Take a look at potentially removing this: This is *slightly* wrong
    // as it's possible to have a GEP with a foldable target type but a memory
    // access that isn't foldable. For example, this load isn't foldable on
    // RISC-V:
    //
    // %p = getelementptr i32, ptr %base, i32 42
    // %x = load <2 x i32>, ptr %p
    if (!AccessType)
      AccessType = TargetType;

    // If the final address of the GEP is a legal addressing mode for the given
    // access type, then we can fold it into its users.
    if (static_cast<const T *>(this)->isLegalAddressingMode(
            AccessType, const_cast<GlobalValue *>(BaseGV),
            BaseOffset.sextOrTrunc(64).getSExtValue(), HasBaseReg, Scale,
            Ptr->getType()->getPointerAddressSpace()))
      return TTI::TCC_Free;

    // TODO: Instead of returning TCC_Basic here, we should use
    // getArithmeticInstrCost. Or better yet, provide a hook to let the target
    // model it.
    return TTI::TCC_Basic;
  }

  InstructionCost
  getPointersChainCost(ArrayRef<const Value *> Ptrs, const Value *Base,
                       const TTI::PointersChainInfo &Info, Type *AccessTy,
                       TTI::TargetCostKind CostKind) const override {
    InstructionCost Cost = TTI::TCC_Free;
    // In the basic model we take into account GEP instructions only
    // (although here can come alloca instruction, a value, constants and/or
    // constant expressions, PHIs, bitcasts ... whatever allowed to be used as a
    // pointer). Typically, if Base is a not a GEP-instruction and all the
    // pointers are relative to the same base address, all the rest are
    // either GEP instructions, PHIs, bitcasts or constants. When we have same
    // base, we just calculate cost of each non-Base GEP as an ADD operation if
    // any their index is a non-const.
    // If no known dependecies between the pointers cost is calculated as a sum
    // of costs of GEP instructions.
    for (const Value *V : Ptrs) {
      const auto *GEP = dyn_cast<GetElementPtrInst>(V);
      if (!GEP)
        continue;
      if (Info.isSameBase() && V != Base) {
        if (GEP->hasAllConstantIndices())
          continue;
        Cost += static_cast<const T *>(this)->getArithmeticInstrCost(
            Instruction::Add, GEP->getType(), CostKind,
            {TTI::OK_AnyValue, TTI::OP_None}, {TTI::OK_AnyValue, TTI::OP_None},
            {});
      } else {
        SmallVector<const Value *> Indices(GEP->indices());
        Cost += static_cast<const T *>(this)->getGEPCost(
            GEP->getSourceElementType(), GEP->getPointerOperand(), Indices,
            AccessTy, CostKind);
      }
    }
    return Cost;
  }

  InstructionCost
  getInstructionCost(const User *U, ArrayRef<const Value *> Operands,
                     TTI::TargetCostKind CostKind) const override {
    using namespace llvm::PatternMatch;

    auto *TargetTTI = static_cast<const T *>(this);
    // Handle non-intrinsic calls, invokes, and callbr.
    // FIXME: Unlikely to be true for anything but CodeSize.
    auto *CB = dyn_cast<CallBase>(U);
    if (CB && !isa<IntrinsicInst>(U)) {
      if (const Function *F = CB->getCalledFunction()) {
        if (!TargetTTI->isLoweredToCall(F))
          return TTI::TCC_Basic; // Give a basic cost if it will be lowered

        return TTI::TCC_Basic * (F->getFunctionType()->getNumParams() + 1);
      }
      // For indirect or other calls, scale cost by number of arguments.
      return TTI::TCC_Basic * (CB->arg_size() + 1);
    }

    Type *Ty = U->getType();
    unsigned Opcode = Operator::getOpcode(U);
    auto *I = dyn_cast<Instruction>(U);
    switch (Opcode) {
    default:
      break;
    case Instruction::Call: {
      assert(isa<IntrinsicInst>(U) && "Unexpected non-intrinsic call");
      auto *Intrinsic = cast<IntrinsicInst>(U);
      IntrinsicCostAttributes CostAttrs(Intrinsic->getIntrinsicID(), *CB);
      return TargetTTI->getIntrinsicInstrCost(CostAttrs, CostKind);
    }
    case Instruction::Br:
    case Instruction::Ret:
    case Instruction::PHI:
    case Instruction::Switch:
      return TargetTTI->getCFInstrCost(Opcode, CostKind, I);
    case Instruction::Freeze:
      return TTI::TCC_Free;
    case Instruction::ExtractValue:
    case Instruction::InsertValue:
      return TargetTTI->getInsertExtractValueCost(Opcode, CostKind);
    case Instruction::Alloca:
      if (cast<AllocaInst>(U)->isStaticAlloca())
        return TTI::TCC_Free;
      break;
    case Instruction::GetElementPtr: {
      const auto *GEP = cast<GEPOperator>(U);
      Type *AccessType = nullptr;
      // For now, only provide the AccessType in the simple case where the GEP
      // only has one user.
      if (GEP->hasOneUser() && I)
        AccessType = I->user_back()->getAccessType();

      return TargetTTI->getGEPCost(GEP->getSourceElementType(),
                                   Operands.front(), Operands.drop_front(),
                                   AccessType, CostKind);
    }
    case Instruction::Add:
    case Instruction::FAdd:
    case Instruction::Sub:
    case Instruction::FSub:
    case Instruction::Mul:
    case Instruction::FMul:
    case Instruction::UDiv:
    case Instruction::SDiv:
    case Instruction::FDiv:
    case Instruction::URem:
    case Instruction::SRem:
    case Instruction::FRem:
    case Instruction::Shl:
    case Instruction::LShr:
    case Instruction::AShr:
    case Instruction::And:
    case Instruction::Or:
    case Instruction::Xor:
    case Instruction::FNeg: {
      const TTI::OperandValueInfo Op1Info = TTI::getOperandInfo(Operands[0]);
      TTI::OperandValueInfo Op2Info;
      if (Opcode != Instruction::FNeg)
        Op2Info = TTI::getOperandInfo(Operands[1]);
      return TargetTTI->getArithmeticInstrCost(Opcode, Ty, CostKind, Op1Info,
                                               Op2Info, Operands, I);
    }
    case Instruction::IntToPtr:
    case Instruction::PtrToInt:
    case Instruction::SIToFP:
    case Instruction::UIToFP:
    case Instruction::FPToUI:
    case Instruction::FPToSI:
    case Instruction::Trunc:
    case Instruction::FPTrunc:
    case Instruction::BitCast:
    case Instruction::FPExt:
    case Instruction::SExt:
    case Instruction::ZExt:
    case Instruction::AddrSpaceCast: {
      Type *OpTy = Operands[0]->getType();
      return TargetTTI->getCastInstrCost(
          Opcode, Ty, OpTy, TTI::getCastContextHint(I), CostKind, I);
    }
    case Instruction::Store: {
      auto *SI = cast<StoreInst>(U);
      Type *ValTy = Operands[0]->getType();
      TTI::OperandValueInfo OpInfo = TTI::getOperandInfo(Operands[0]);
      return TargetTTI->getMemoryOpCost(Opcode, ValTy, SI->getAlign(),
                                        SI->getPointerAddressSpace(), CostKind,
                                        OpInfo, I);
    }
    case Instruction::Load: {
      // FIXME: Arbitary cost which could come from the backend.
      if (CostKind == TTI::TCK_Latency)
        return 4;
      auto *LI = cast<LoadInst>(U);
      Type *LoadType = U->getType();
      // If there is a non-register sized type, the cost estimation may expand
      // it to be several instructions to load into multiple registers on the
      // target.  But, if the only use of the load is a trunc instruction to a
      // register sized type, the instruction selector can combine these
      // instructions to be a single load.  So, in this case, we use the
      // destination type of the trunc instruction rather than the load to
      // accurately estimate the cost of this load instruction.
      if (CostKind == TTI::TCK_CodeSize && LI->hasOneUse() &&
          !LoadType->isVectorTy()) {
        if (const TruncInst *TI = dyn_cast<TruncInst>(*LI->user_begin()))
          LoadType = TI->getDestTy();
      }
      return TargetTTI->getMemoryOpCost(Opcode, LoadType, LI->getAlign(),
                                        LI->getPointerAddressSpace(), CostKind,
                                        {TTI::OK_AnyValue, TTI::OP_None}, I);
    }
    case Instruction::Select: {
      const Value *Op0, *Op1;
      if (match(U, m_LogicalAnd(m_Value(Op0), m_Value(Op1))) ||
          match(U, m_LogicalOr(m_Value(Op0), m_Value(Op1)))) {
        // select x, y, false --> x & y
        // select x, true, y --> x | y
        const auto Op1Info = TTI::getOperandInfo(Op0);
        const auto Op2Info = TTI::getOperandInfo(Op1);
        assert(Op0->getType()->getScalarSizeInBits() == 1 &&
               Op1->getType()->getScalarSizeInBits() == 1);

        SmallVector<const Value *, 2> Operands{Op0, Op1};
        return TargetTTI->getArithmeticInstrCost(
            match(U, m_LogicalOr()) ? Instruction::Or : Instruction::And, Ty,
            CostKind, Op1Info, Op2Info, Operands, I);
      }
      const auto Op1Info = TTI::getOperandInfo(Operands[1]);
      const auto Op2Info = TTI::getOperandInfo(Operands[2]);
      Type *CondTy = Operands[0]->getType();
      return TargetTTI->getCmpSelInstrCost(Opcode, U->getType(), CondTy,
                                           CmpInst::BAD_ICMP_PREDICATE,
                                           CostKind, Op1Info, Op2Info, I);
    }
    case Instruction::ICmp:
    case Instruction::FCmp: {
      const auto Op1Info = TTI::getOperandInfo(Operands[0]);
      const auto Op2Info = TTI::getOperandInfo(Operands[1]);
      Type *ValTy = Operands[0]->getType();
      // TODO: Also handle ICmp/FCmp constant expressions.
      return TargetTTI->getCmpSelInstrCost(Opcode, ValTy, U->getType(),
                                           I ? cast<CmpInst>(I)->getPredicate()
                                             : CmpInst::BAD_ICMP_PREDICATE,
                                           CostKind, Op1Info, Op2Info, I);
    }
    case Instruction::InsertElement: {
      auto *IE = dyn_cast<InsertElementInst>(U);
      if (!IE)
        return TTI::TCC_Basic; // FIXME
      unsigned Idx = -1;
      if (auto *CI = dyn_cast<ConstantInt>(Operands[2]))
        if (CI->getValue().getActiveBits() <= 32)
          Idx = CI->getZExtValue();
      return TargetTTI->getVectorInstrCost(*IE, Ty, CostKind, Idx);
    }
    case Instruction::ShuffleVector: {
      auto *Shuffle = dyn_cast<ShuffleVectorInst>(U);
      if (!Shuffle)
        return TTI::TCC_Basic; // FIXME

      auto *VecTy = cast<VectorType>(U->getType());
      auto *VecSrcTy = cast<VectorType>(Operands[0]->getType());
      ArrayRef<int> Mask = Shuffle->getShuffleMask();
      int NumSubElts, SubIndex;

      // Treat undef/poison mask as free (no matter the length).
      if (all_of(Mask, [](int M) { return M < 0; }))
        return TTI::TCC_Free;

      // TODO: move more of this inside improveShuffleKindFromMask.
      if (Shuffle->changesLength()) {
        // Treat a 'subvector widening' as a free shuffle.
        if (Shuffle->increasesLength() && Shuffle->isIdentityWithPadding())
          return TTI::TCC_Free;

        if (Shuffle->isExtractSubvectorMask(SubIndex))
          return TargetTTI->getShuffleCost(TTI::SK_ExtractSubvector, VecTy,
                                           VecSrcTy, Mask, CostKind, SubIndex,
                                           VecTy, Operands, Shuffle);

        if (Shuffle->isInsertSubvectorMask(NumSubElts, SubIndex))
          return TargetTTI->getShuffleCost(
              TTI::SK_InsertSubvector, VecTy, VecSrcTy, Mask, CostKind,
              SubIndex,
              FixedVectorType::get(VecTy->getScalarType(), NumSubElts),
              Operands, Shuffle);

        int ReplicationFactor, VF;
        if (Shuffle->isReplicationMask(ReplicationFactor, VF)) {
          APInt DemandedDstElts = APInt::getZero(Mask.size());
          for (auto I : enumerate(Mask)) {
            if (I.value() != PoisonMaskElem)
              DemandedDstElts.setBit(I.index());
          }
          return TargetTTI->getReplicationShuffleCost(
              VecSrcTy->getElementType(), ReplicationFactor, VF,
              DemandedDstElts, CostKind);
        }

        bool IsUnary = isa<UndefValue>(Operands[1]);
        NumSubElts = VecSrcTy->getElementCount().getKnownMinValue();
        SmallVector<int, 16> AdjustMask(Mask);

        // Widening shuffle - widening the source(s) to the new length
        // (treated as free - see above), and then perform the adjusted
        // shuffle at that width.
        if (Shuffle->increasesLength()) {
          for (int &M : AdjustMask)
            M = M >= NumSubElts ? (M + (Mask.size() - NumSubElts)) : M;

          return TargetTTI->getShuffleCost(
              IsUnary ? TTI::SK_PermuteSingleSrc : TTI::SK_PermuteTwoSrc, VecTy,
              VecTy, AdjustMask, CostKind, 0, nullptr, Operands, Shuffle);
        }

        // Narrowing shuffle - perform shuffle at original wider width and
        // then extract the lower elements.
        // FIXME: This can assume widening, which is not true of all vector
        // architectures (and is not even the default).
        AdjustMask.append(NumSubElts - Mask.size(), PoisonMaskElem);

        InstructionCost ShuffleCost = TargetTTI->getShuffleCost(
            IsUnary ? TTI::SK_PermuteSingleSrc : TTI::SK_PermuteTwoSrc,
            VecSrcTy, VecSrcTy, AdjustMask, CostKind, 0, nullptr, Operands,
            Shuffle);

        SmallVector<int, 16> ExtractMask(Mask.size());
        std::iota(ExtractMask.begin(), ExtractMask.end(), 0);
        return ShuffleCost + TargetTTI->getShuffleCost(
                                 TTI::SK_ExtractSubvector, VecTy, VecSrcTy,
                                 ExtractMask, CostKind, 0, VecTy, {}, Shuffle);
      }

      if (Shuffle->isIdentity())
        return TTI::TCC_Free;

      if (Shuffle->isReverse())
        return TargetTTI->getShuffleCost(TTI::SK_Reverse, VecTy, VecSrcTy, Mask,
                                         CostKind, 0, nullptr, Operands,
                                         Shuffle);

      if (Shuffle->isTranspose())
        return TargetTTI->getShuffleCost(TTI::SK_Transpose, VecTy, VecSrcTy,
                                         Mask, CostKind, 0, nullptr, Operands,
                                         Shuffle);

      if (Shuffle->isZeroEltSplat())
        return TargetTTI->getShuffleCost(TTI::SK_Broadcast, VecTy, VecSrcTy,
                                         Mask, CostKind, 0, nullptr, Operands,
                                         Shuffle);

      if (Shuffle->isSingleSource())
        return TargetTTI->getShuffleCost(TTI::SK_PermuteSingleSrc, VecTy,
                                         VecSrcTy, Mask, CostKind, 0, nullptr,
                                         Operands, Shuffle);

      if (Shuffle->isInsertSubvectorMask(NumSubElts, SubIndex))
        return TargetTTI->getShuffleCost(
            TTI::SK_InsertSubvector, VecTy, VecSrcTy, Mask, CostKind, SubIndex,
            FixedVectorType::get(VecTy->getScalarType(), NumSubElts), Operands,
            Shuffle);

      if (Shuffle->isSelect())
        return TargetTTI->getShuffleCost(TTI::SK_Select, VecTy, VecSrcTy, Mask,
                                         CostKind, 0, nullptr, Operands,
                                         Shuffle);

      if (Shuffle->isSplice(SubIndex))
        return TargetTTI->getShuffleCost(TTI::SK_Splice, VecTy, VecSrcTy, Mask,
                                         CostKind, SubIndex, nullptr, Operands,
                                         Shuffle);

      return TargetTTI->getShuffleCost(TTI::SK_PermuteTwoSrc, VecTy, VecSrcTy,
                                       Mask, CostKind, 0, nullptr, Operands,
                                       Shuffle);
    }
    case Instruction::ExtractElement: {
      auto *EEI = dyn_cast<ExtractElementInst>(U);
      if (!EEI)
        return TTI::TCC_Basic; // FIXME
      unsigned Idx = -1;
      if (auto *CI = dyn_cast<ConstantInt>(Operands[1]))
        if (CI->getValue().getActiveBits() <= 32)
          Idx = CI->getZExtValue();
      Type *DstTy = Operands[0]->getType();
      return TargetTTI->getVectorInstrCost(*EEI, DstTy, CostKind, Idx);
    }
    }

    // By default, just classify everything remaining as 'basic'.
    return TTI::TCC_Basic;
  }

  bool isExpensiveToSpeculativelyExecute(const Instruction *I) const override {
    auto *TargetTTI = static_cast<const T *>(this);
    SmallVector<const Value *, 4> Ops(I->operand_values());
    InstructionCost Cost = TargetTTI->getInstructionCost(
        I, Ops, TargetTransformInfo::TCK_SizeAndLatency);
    return Cost >= TargetTransformInfo::TCC_Expensive;
  }

  bool supportsTailCallFor(const CallBase *CB) const override {
    return static_cast<const T *>(this)->supportsTailCalls();
  }
};
} // namespace llvm

#endif

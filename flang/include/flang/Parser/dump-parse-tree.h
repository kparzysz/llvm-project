//===-- include/flang/Parser/dump-parse-tree.h ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef FORTRAN_PARSER_DUMP_PARSE_TREE_H_
#define FORTRAN_PARSER_DUMP_PARSE_TREE_H_

#include "format-specification.h"
#include "parse-tree-visitor.h"
#include "parse-tree.h"
#include "tools.h"
#include "unparse.h"
#include "flang/Common/idioms.h"
#include "flang/Common/indirection.h"
#include "flang/Support/Fortran.h"
#include "llvm/Frontend/OpenMP/OMP.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <type_traits>

namespace Fortran::parser {

//
// Dump the Parse Tree hierarchy of any node 'x' of the parse tree.
//

class ParseTreeDumper {
public:
  explicit ParseTreeDumper(llvm::raw_ostream &out,
      const AnalyzedObjectsAsFortran *asFortran = nullptr)
      : out_(out), asFortran_{asFortran} {}

  static constexpr const char *GetNodeName(const char *) { return "char *"; }
#define NODE_NAME(T, N) \
  static constexpr const char *GetNodeName(const T &) { return N; }
#define NODE_ENUM(T, E) \
  static std::string GetNodeName(const T::E &x) { \
    return #E " = "s + std::string{T::EnumToString(x)}; \
  }
#define NODE(T1, T2) NODE_NAME(T1::T2, #T2)
  NODE_NAME(bool, "bool")
  NODE_NAME(int, "int")
  NODE(std, string)
  NODE(std, int64_t)
  NODE(std, uint64_t)
  NODE_ENUM(common, CUDADataAttr)
  NODE_ENUM(common, CUDASubprogramAttrs)
  NODE_ENUM(common, OpenACCDeviceType)
  NODE(format, ControlEditDesc)
  NODE(format::ControlEditDesc, Kind)
  NODE(format, DerivedTypeDataEditDesc)
  NODE(format, FormatItem)
  NODE(format, FormatSpecification)
  NODE(format, IntrinsicTypeDataEditDesc)
  NODE(format::IntrinsicTypeDataEditDesc, Kind)
  NODE(parser, Abstract)
  NODE(parser, AccAtomicCapture)
  NODE(AccAtomicCapture, Stmt1)
  NODE(AccAtomicCapture, Stmt2)
  NODE(parser, AccAtomicRead)
  NODE(parser, AccAtomicUpdate)
  NODE(parser, AccAtomicWrite)
  NODE(parser, AccBeginBlockDirective)
  NODE(parser, AccBeginCombinedDirective)
  NODE(parser, AccBeginLoopDirective)
  NODE(parser, AccBlockDirective)
  NODE(parser, AccClause)
#define GEN_FLANG_DUMP_PARSE_TREE_CLAUSES
#include "llvm/Frontend/OpenACC/ACC.inc"
  NODE(parser, AccBindClause)
  NODE(parser, AccDefaultClause)
  static std::string GetNodeName(const llvm::acc::DefaultValue &x) {
    return llvm::Twine(
        "llvm::acc::DefaultValue = ", llvm::acc::getOpenACCDefaultValueName(x))
        .str();
  }
  NODE(parser, AccClauseList)
  NODE(parser, AccCombinedDirective)
  NODE(parser, AccDataModifier)
  NODE_ENUM(parser::AccDataModifier, Modifier)
  NODE(parser, AccDeclarativeDirective)
  NODE(parser, AccEndAtomic)
  NODE(parser, AccEndBlockDirective)
  NODE(parser, AccEndCombinedDirective)
  NODE(parser, AccCollapseArg)
  NODE(parser, AccGangArg)
  NODE(AccGangArg, Num)
  NODE(AccGangArg, Dim)
  NODE(AccGangArg, Static)
  NODE(parser, AccGangArgList)
  NODE(parser, AccObject)
  NODE(parser, AccObjectList)
  NODE(parser, AccObjectListWithModifier)
  NODE(parser, AccObjectListWithReduction)
  NODE(parser, AccSizeExpr)
  NODE(parser, AccSizeExprList)
  NODE(parser, AccSelfClause)
  NODE(parser, AccStandaloneDirective)
  NODE(parser, AccDeviceTypeExpr)

  NODE(parser, AccDeviceTypeExprList)
  NODE(parser, AccTileExpr)
  NODE(parser, AccTileExprList)
  NODE(parser, AccLoopDirective)
  NODE(parser, AccEndLoop)
  NODE(parser, AccWaitArgument)
  static std::string GetNodeName(const llvm::acc::Directive &x) {
    return llvm::Twine(
        "llvm::acc::Directive = ", llvm::acc::getOpenACCDirectiveName(x))
        .str();
  }
  NODE(parser, AcImpliedDo)
  NODE(parser, AcImpliedDoControl)
  NODE(parser, AcValue)
  NODE(parser, AccessStmt)
  NODE(parser, AccessId)
  NODE(parser, AccessSpec)
  NODE_ENUM(AccessSpec, Kind)
  NODE(parser, AcSpec)
  NODE(parser, ActionStmt)
  NODE(parser, ActualArg)
  NODE(ActualArg, PercentRef)
  NODE(ActualArg, PercentVal)
  NODE(parser, ActualArgSpec)
  NODE(AcValue, Triplet)
  NODE(parser, AllocOpt)
  NODE(AllocOpt, Mold)
  NODE(AllocOpt, Source)
  NODE(AllocOpt, Stream)
  NODE(AllocOpt, Pinned)
  NODE(parser, Allocatable)
  NODE(parser, AllocatableStmt)
  NODE(parser, AllocateCoarraySpec)
  NODE(parser, AllocateObject)
  NODE(parser, AllocateShapeSpec)
  NODE(parser, AllocateStmt)
  NODE(parser, Allocation)
  NODE(parser, AltReturnSpec)
  NODE(parser, ArithmeticIfStmt)
  NODE(parser, ArrayConstructor)
  NODE(parser, ArrayElement)
  NODE(parser, ArraySpec)
  NODE(parser, AssignStmt)
  NODE(parser, AssignedGotoStmt)
  NODE(parser, AssignmentStmt)
  NODE(parser, AssociateConstruct)
  NODE(parser, AssociateStmt)
  NODE(parser, Association)
  NODE(parser, AssumedImpliedSpec)
  NODE(parser, AssumedRankSpec)
  NODE(parser, AssumedShapeSpec)
  NODE(parser, AssumedSizeSpec)
  NODE(parser, Asynchronous)
  NODE(parser, AsynchronousStmt)
  NODE(parser, AttrSpec)
  NODE(parser, BOZLiteralConstant)
  NODE(parser, BackspaceStmt)
  NODE(parser, BasedPointer)
  NODE(parser, BasedPointerStmt)
  NODE(parser, BindAttr)
  NODE(BindAttr, Deferred)
  NODE(BindAttr, Non_Overridable)
  NODE(parser, BindEntity)
  NODE_ENUM(BindEntity, Kind)
  NODE(parser, BindStmt)
  NODE(parser, Block)
  NODE(parser, BlockConstruct)
  NODE(parser, BlockData)
  NODE(parser, BlockDataStmt)
  NODE(parser, BlockSpecificationPart)
  NODE(parser, BlockStmt)
  NODE(parser, BoundsRemapping)
  NODE(parser, BoundsSpec)
  NODE(parser, Call)
  NODE(parser, CallStmt)
  NODE(CallStmt, Chevrons)
  NODE(CallStmt, StarOrExpr)
  NODE(parser, CaseConstruct)
  NODE(CaseConstruct, Case)
  NODE(parser, CaseSelector)
  NODE(parser, CaseStmt)
  NODE(parser, CaseValueRange)
  NODE(CaseValueRange, Range)
  NODE(parser, ChangeTeamConstruct)
  NODE(parser, ChangeTeamStmt)
  NODE(parser, CharLength)
  NODE(parser, CharLiteralConstant)
  NODE(parser, CharLiteralConstantSubstring)
  NODE(parser, CharSelector)
  NODE(CharSelector, LengthAndKind)
  NODE(parser, CloseStmt)
  NODE(CloseStmt, CloseSpec)
  NODE(parser, CoarrayAssociation)
  NODE(parser, CoarraySpec)
  NODE(parser, CodimensionDecl)
  NODE(parser, CodimensionStmt)
  NODE(parser, CoindexedNamedObject)
  NODE(parser, CommonBlockObject)
  NODE(parser, CommonStmt)
  NODE(CommonStmt, Block)
  NODE(parser, CompilerDirective)
  NODE(CompilerDirective, AssumeAligned)
  NODE(CompilerDirective, IgnoreTKR)
  NODE(CompilerDirective, LoopCount)
  NODE(CompilerDirective, NameValue)
  NODE(CompilerDirective, Unrecognized)
  NODE(CompilerDirective, VectorAlways)
  NODE(CompilerDirective, Unroll)
  NODE(CompilerDirective, UnrollAndJam)
  NODE(CompilerDirective, NoVector)
  NODE(CompilerDirective, NoUnroll)
  NODE(CompilerDirective, NoUnrollAndJam)
  NODE(parser, ComplexLiteralConstant)
  NODE(parser, ComplexPart)
  NODE(parser, ComponentArraySpec)
  NODE(parser, ComponentAttrSpec)
  NODE(parser, ComponentDataSource)
  NODE(parser, ComponentDecl)
  NODE(parser, FillDecl)
  NODE(parser, ComponentOrFill)
  NODE(parser, ComponentDefStmt)
  NODE(parser, ComponentSpec)
  NODE(parser, ComputedGotoStmt)
  NODE(parser, ConcurrentControl)
  NODE(parser, ConcurrentHeader)
  NODE(parser, ConnectSpec)
  NODE(ConnectSpec, CharExpr)
  NODE_ENUM(ConnectSpec::CharExpr, Kind)
  NODE(ConnectSpec, Newunit)
  NODE(ConnectSpec, Recl)
  NODE(parser, ContainsStmt)
  NODE(parser, Contiguous)
  NODE(parser, ContiguousStmt)
  NODE(parser, ContinueStmt)
  NODE(parser, CriticalConstruct)
  NODE(parser, CriticalStmt)
  NODE(parser, CUDAAttributesStmt)
  NODE(parser, CUFKernelDoConstruct)
  NODE(CUFKernelDoConstruct, StarOrExpr)
  NODE(CUFKernelDoConstruct, Directive)
  NODE(CUFKernelDoConstruct, LaunchConfiguration)
  NODE(parser, CUFReduction)
  NODE(parser, CycleStmt)
  NODE(parser, DataComponentDefStmt)
  NODE(parser, DataIDoObject)
  NODE(parser, DataImpliedDo)
  NODE(parser, DataRef)
  NODE(parser, DataStmt)
  NODE(parser, DataStmtConstant)
  NODE(parser, DataStmtObject)
  NODE(parser, DataStmtRepeat)
  NODE(parser, DataStmtSet)
  NODE(parser, DataStmtValue)
  NODE(parser, DeallocateStmt)
  NODE(parser, DeclarationConstruct)
  NODE(parser, DeclarationTypeSpec)
  NODE(DeclarationTypeSpec, Class)
  NODE(DeclarationTypeSpec, ClassStar)
  NODE(DeclarationTypeSpec, Record)
  NODE(DeclarationTypeSpec, Type)
  NODE(DeclarationTypeSpec, TypeStar)
  NODE(parser, Default)
  NODE(parser, DeferredCoshapeSpecList)
  NODE(parser, DeferredShapeSpecList)
  NODE(parser, DefinedOpName)
  NODE(parser, DefinedOperator)
  NODE_ENUM(DefinedOperator, IntrinsicOperator)
  NODE(parser, DerivedTypeDef)
  NODE(parser, DerivedTypeSpec)
  NODE(parser, DerivedTypeStmt)
  NODE(parser, Designator)
  NODE(parser, DimensionStmt)
  NODE(DimensionStmt, Declaration)
  NODE(parser, DoConstruct)
  NODE(parser, DummyArg)
  NODE(parser, ElseIfStmt)
  NODE(parser, ElseStmt)
  NODE(parser, ElsewhereStmt)
  NODE(parser, EndAssociateStmt)
  NODE(parser, EndBlockDataStmt)
  NODE(parser, EndBlockStmt)
  NODE(parser, EndChangeTeamStmt)
  NODE(parser, EndCriticalStmt)
  NODE(parser, EndDoStmt)
  NODE(parser, EndEnumStmt)
  NODE(parser, EndForallStmt)
  NODE(parser, EndFunctionStmt)
  NODE(parser, EndIfStmt)
  NODE(parser, EndInterfaceStmt)
  NODE(parser, EndLabel)
  NODE(parser, EndModuleStmt)
  NODE(parser, EndMpSubprogramStmt)
  NODE(parser, EndProgramStmt)
  NODE(parser, EndSelectStmt)
  NODE(parser, EndSubmoduleStmt)
  NODE(parser, EndSubroutineStmt)
  NODE(parser, EndTypeStmt)
  NODE(parser, EndWhereStmt)
  NODE(parser, EndfileStmt)
  NODE(parser, EntityDecl)
  NODE(parser, EntryStmt)
  NODE(parser, EnumDef)
  NODE(parser, EnumDefStmt)
  NODE(parser, Enumerator)
  NODE(parser, EnumeratorDefStmt)
  NODE(parser, EorLabel)
  NODE(parser, EquivalenceObject)
  NODE(parser, EquivalenceStmt)
  NODE(parser, ErrLabel)
  NODE(parser, ErrorRecovery)
  NODE(parser, EventPostStmt)
  NODE(parser, EventWaitSpec)
  NODE(parser, EventWaitStmt)
  NODE(parser, ExecutableConstruct)
  NODE(parser, ExecutionPart)
  NODE(parser, ExecutionPartConstruct)
  NODE(parser, ExitStmt)
  NODE(parser, ExplicitCoshapeSpec)
  NODE(parser, ExplicitShapeSpec)
  NODE(parser, Expr)
  NODE(Expr, Parentheses)
  NODE(Expr, UnaryPlus)
  NODE(Expr, Negate)
  NODE(Expr, NOT)
  NODE(Expr, PercentLoc)
  NODE(Expr, DefinedUnary)
  NODE(Expr, Power)
  NODE(Expr, Multiply)
  NODE(Expr, Divide)
  NODE(Expr, Add)
  NODE(Expr, Subtract)
  NODE(Expr, Concat)
  NODE(Expr, LT)
  NODE(Expr, LE)
  NODE(Expr, EQ)
  NODE(Expr, NE)
  NODE(Expr, GE)
  NODE(Expr, GT)
  NODE(Expr, AND)
  NODE(Expr, OR)
  NODE(Expr, EQV)
  NODE(Expr, NEQV)
  NODE(Expr, DefinedBinary)
  NODE(Expr, ComplexConstructor)
  NODE(parser, External)
  NODE(parser, ExternalStmt)
  NODE(parser, FailImageStmt)
  NODE(parser, FileUnitNumber)
  NODE(parser, FinalProcedureStmt)
  NODE(parser, FlushStmt)
  NODE(parser, ForallAssignmentStmt)
  NODE(parser, ForallBodyConstruct)
  NODE(parser, ForallConstruct)
  NODE(parser, ForallConstructStmt)
  NODE(parser, ForallStmt)
  NODE(parser, FormTeamStmt)
  NODE(FormTeamStmt, FormTeamSpec)
  NODE(parser, Format)
  NODE(parser, FormatStmt)
  NODE(parser, FunctionReference)
  NODE(parser, FunctionStmt)
  NODE(parser, FunctionSubprogram)
  NODE(parser, GenericSpec)
  NODE(GenericSpec, Assignment)
  NODE(GenericSpec, ReadFormatted)
  NODE(GenericSpec, ReadUnformatted)
  NODE(GenericSpec, WriteFormatted)
  NODE(GenericSpec, WriteUnformatted)
  NODE(parser, GenericStmt)
  NODE(parser, GotoStmt)
  NODE(parser, HollerithLiteralConstant)
  NODE(parser, IdExpr)
  NODE(parser, IdVariable)
  NODE(parser, IfConstruct)
  NODE(IfConstruct, ElseBlock)
  NODE(IfConstruct, ElseIfBlock)
  NODE(parser, IfStmt)
  NODE(parser, IfThenStmt)
  NODE(parser, TeamValue)
  NODE(parser, ImageSelector)
  NODE(parser, ImageSelectorSpec)
  NODE(ImageSelectorSpec, Stat)
  NODE(ImageSelectorSpec, Team_Number)
  NODE(parser, ImplicitPart)
  NODE(parser, ImplicitPartStmt)
  NODE(parser, ImplicitSpec)
  NODE(parser, ImplicitStmt)
  NODE_ENUM(ImplicitStmt, ImplicitNoneNameSpec)
  NODE(parser, ImpliedShapeSpec)
  NODE(parser, ImportStmt)
  NODE(parser, Initialization)
  NODE(parser, InputImpliedDo)
  NODE(parser, InputItem)
  NODE(parser, InquireSpec)
  NODE(InquireSpec, CharVar)
  NODE_ENUM(InquireSpec::CharVar, Kind)
  NODE(InquireSpec, IntVar)
  NODE_ENUM(InquireSpec::IntVar, Kind)
  NODE(InquireSpec, LogVar)
  NODE_ENUM(InquireSpec::LogVar, Kind)
  NODE(parser, InquireStmt)
  NODE(InquireStmt, Iolength)
  NODE(parser, IntegerTypeSpec)
  NODE(parser, IntentSpec)
  NODE_ENUM(IntentSpec, Intent)
  NODE(parser, IntentStmt)
  NODE(parser, InterfaceBlock)
  NODE(parser, InterfaceBody)
  NODE(InterfaceBody, Function)
  NODE(InterfaceBody, Subroutine)
  NODE(parser, InterfaceSpecification)
  NODE(parser, InterfaceStmt)
  NODE(parser, InternalSubprogram)
  NODE(parser, InternalSubprogramPart)
  NODE(parser, Intrinsic)
  NODE(parser, IntrinsicStmt)
  NODE(parser, IntrinsicTypeSpec)
  NODE(IntrinsicTypeSpec, Character)
  NODE(IntrinsicTypeSpec, Complex)
  NODE(IntrinsicTypeSpec, DoubleComplex)
  NODE(IntrinsicTypeSpec, DoublePrecision)
  NODE(IntrinsicTypeSpec, Logical)
  NODE(IntrinsicTypeSpec, Real)
  NODE(parser, IoControlSpec)
  NODE(IoControlSpec, Asynchronous)
  NODE(IoControlSpec, CharExpr)
  NODE_ENUM(IoControlSpec::CharExpr, Kind)
  NODE(IoControlSpec, Pos)
  NODE(IoControlSpec, Rec)
  NODE(IoControlSpec, Size)
  NODE(parser, IoUnit)
  NODE(parser, Keyword)
  NODE(parser, KindParam)
  NODE(parser, KindSelector)
  NODE(KindSelector, StarSize)
  NODE(parser, LabelDoStmt)
  NODE(parser, LanguageBindingSpec)
  NODE(parser, LengthSelector)
  NODE(parser, LetterSpec)
  NODE(parser, LiteralConstant)
  NODE(parser, IntLiteralConstant)
  NODE(parser, ReductionOperator)
  NODE_ENUM(parser::ReductionOperator, Operator)
  NODE(parser, LocalitySpec)
  NODE(LocalitySpec, DefaultNone)
  NODE(LocalitySpec, Local)
  NODE(LocalitySpec, LocalInit)
  NODE(LocalitySpec, Reduce)
  NODE(LocalitySpec, Shared)
  NODE(parser, LockStmt)
  NODE(LockStmt, LockStat)
  NODE(parser, LogicalLiteralConstant)
  NODE_NAME(LoopControl::Bounds, "LoopBounds")
  NODE_NAME(AcImpliedDoControl::Bounds, "LoopBounds")
  NODE_NAME(DataImpliedDo::Bounds, "LoopBounds")
  NODE(parser, LoopControl)
  NODE(LoopControl, Concurrent)
  NODE(parser, MainProgram)
  NODE(parser, Map)
  NODE(Map, EndMapStmt)
  NODE(Map, MapStmt)
  NODE(parser, MaskedElsewhereStmt)
  NODE(parser, Module)
  NODE(parser, ModuleStmt)
  NODE(parser, ModuleSubprogram)
  NODE(parser, ModuleSubprogramPart)
  NODE(parser, MpSubprogramStmt)
  NODE(parser, MsgVariable)
  NODE(parser, Name)
  NODE(parser, NamedConstant)
  NODE(parser, NamedConstantDef)
  NODE(parser, NamelistStmt)
  NODE(NamelistStmt, Group)
  NODE(parser, NonLabelDoStmt)
  NODE(parser, NoPass)
  NODE(parser, NotifyWaitStmt)
  NODE(parser, NullifyStmt)
  NODE(parser, NullInit)
  NODE(parser, ObjectDecl)
  NODE(parser, OldParameterStmt)
  NODE(parser, OmpTypeSpecifier)
  NODE(parser, OmpTypeNameList)
  NODE(parser, OmpAdjustArgsClause)
  NODE(OmpAdjustArgsClause, OmpAdjustOp)
  NODE_ENUM(OmpAdjustArgsClause::OmpAdjustOp, Value)
  NODE(parser, OmpAppendArgsClause)
  NODE(OmpAppendArgsClause, OmpAppendOp)
  NODE(parser, OmpLocator)
  NODE(parser, OmpLocatorList)
  NODE(parser, OmpReductionSpecifier)
  NODE(parser, OmpArgument)
  NODE(parser, OmpArgumentList)
  NODE(parser, OmpMetadirectiveDirective)
  NODE(parser, OmpMatchClause)
  NODE(parser, OmpOtherwiseClause)
  NODE(parser, OmpWhenClause)
  NODE(OmpWhenClause, Modifier)
  NODE(parser, OmpDirectiveName)
  NODE(parser, OmpDirectiveSpecification)
  NODE_ENUM(OmpDirectiveSpecification, Flags)
  NODE(parser, OmpTraitPropertyName)
  NODE(parser, OmpTraitScore)
  NODE(parser, OmpTraitPropertyExtension)
  NODE(OmpTraitPropertyExtension, Complex)
  NODE(parser, OmpTraitProperty)
  NODE(parser, OmpTraitSelectorName)
  NODE_ENUM(OmpTraitSelectorName, Value)
  NODE(parser, OmpTraitSelector)
  NODE(OmpTraitSelector, Properties)
  NODE(parser, OmpTraitSetSelectorName)
  NODE_ENUM(OmpTraitSetSelectorName, Value)
  NODE(parser, OmpTraitSetSelector)
  NODE(parser, OmpContextSelectorSpecification)
  NODE(parser, OmpMapper)
  NODE(parser, OmpMapType)
  NODE_ENUM(OmpMapType, Value)
  NODE(parser, OmpMapTypeModifier)
  NODE_ENUM(OmpMapTypeModifier, Value)
  NODE(parser, OmpIteratorSpecifier)
  NODE(parser, OmpIterator)
  NODE(parser, OmpAbsentClause)
  NODE(parser, OmpAffinityClause)
  NODE(OmpAffinityClause, Modifier)
  NODE(parser, OmpAlignment)
  NODE(parser, OmpAlignClause)
  NODE(parser, OmpAlignedClause)
  NODE(OmpAlignedClause, Modifier)
  NODE(parser, OmpAlwaysModifier)
  NODE_ENUM(OmpAlwaysModifier, Value)
  NODE(parser, OmpAtClause)
  NODE_ENUM(OmpAtClause, ActionTime)
  NODE_ENUM(OmpSeverityClause, Severity)
  NODE(parser, OmpBeginBlockDirective)
  NODE(parser, OmpBeginLoopDirective)
  NODE(parser, OmpBeginSectionsDirective)
  NODE(parser, OmpBlockDirective)
  static std::string GetNodeName(const llvm::omp::Directive &x) {
    return llvm::Twine("llvm::omp::Directive = ",
        llvm::omp::getOpenMPDirectiveName(x, llvm::omp::FallbackVersion))
        .str();
  }
  NODE(parser, OmpClause)
#define GEN_FLANG_DUMP_PARSE_TREE_CLAUSES
#include "llvm/Frontend/OpenMP/OMP.inc"
  NODE(parser, OmpClauseList)
  NODE(parser, OmpCancellationConstructTypeClause)
  NODE(parser, OmpCloseModifier)
  NODE_ENUM(OmpCloseModifier, Value)
  NODE(parser, OmpContainsClause)
  NODE(parser, OmpCriticalDirective)
  NODE(parser, OmpErrorDirective)
  NODE(parser, OmpNothingDirective)
  NODE(parser, OmpDeclareTargetSpecifier)
  NODE(parser, OmpDeclareTargetWithClause)
  NODE(parser, OmpDeclareTargetWithList)
  NODE(parser, OmpMapperSpecifier)
  NODE(parser, OmpDefaultClause)
  NODE_ENUM(OmpDefaultClause, DataSharingAttribute)
  NODE(parser, OmpVariableCategory)
  NODE_ENUM(OmpVariableCategory, Value)
  NODE(parser, OmpDefaultmapClause)
  NODE_ENUM(OmpDefaultmapClause, ImplicitBehavior)
  NODE(OmpDefaultmapClause, Modifier)
  NODE(parser, OmpDeleteModifier)
  NODE_ENUM(OmpDeleteModifier, Value)
  NODE(parser, OmpDependenceType)
  NODE_ENUM(OmpDependenceType, Value)
  NODE(parser, OmpTaskDependenceType)
  NODE_ENUM(OmpTaskDependenceType, Value)
  NODE(parser, OmpIndirectClause)
  NODE(parser, OmpIterationOffset)
  NODE(parser, OmpIteration)
  NODE(parser, OmpIterationVector)
  NODE(parser, OmpDoacross)
  NODE(OmpDoacross, Sink)
  NODE(OmpDoacross, Source)
  NODE(parser, OmpDependClause)
  NODE(OmpDependClause, TaskDep)
  NODE(OmpDependClause::TaskDep, Modifier)
  NODE(parser, OmpDetachClause)
  NODE(parser, OmpDoacrossClause)
  NODE(parser, OmpDestroyClause)
  NODE(parser, OmpEndBlockDirective)
  NODE(parser, OmpEndCriticalDirective)
  NODE(parser, OmpEndLoopDirective)
  NODE(parser, OmpEndSectionsDirective)
  NODE(parser, OmpFailClause)
  NODE(parser, OmpFromClause)
  NODE(OmpFromClause, Modifier)
  NODE(parser, OmpExpectation)
  NODE_ENUM(OmpExpectation, Value)
  NODE(parser, OmpHintClause)
  NODE(parser, OmpHoldsClause)
  NODE(parser, OmpIfClause)
  NODE(OmpIfClause, Modifier)
  NODE(parser, OmpLastprivateClause)
  NODE(OmpLastprivateClause, Modifier)
  NODE(parser, OmpLastprivateModifier)
  NODE_ENUM(OmpLastprivateModifier, Value)
  NODE(parser, OmpLinearClause)
  NODE(OmpLinearClause, Modifier)
  NODE(parser, OmpLinearModifier)
  NODE_ENUM(OmpLinearModifier, Value)
  NODE(parser, OmpStepComplexModifier)
  NODE(parser, OmpStepSimpleModifier)
  NODE(parser, OmpLoopDirective)
  NODE(parser, OmpMapClause)
  NODE(parser, OmpMessageClause)
  NODE(OmpMapClause, Modifier)
  static std::string GetNodeName(const llvm::omp::Clause &x) {
    return llvm::Twine(
        "llvm::omp::Clause = ", llvm::omp::getOpenMPClauseName(x))
        .str();
  }
  NODE(parser, OmpObject)
  NODE(parser, OmpObjectList)
  NODE(parser, OmpNoOpenMPClause)
  NODE(parser, OmpNoOpenMPRoutinesClause)
  NODE(parser, OmpNoParallelismClause)
  NODE(parser, OmpOrderClause)
  NODE(OmpOrderClause, Modifier)
  NODE_ENUM(OmpOrderClause, Ordering)
  NODE(parser, OmpOrderModifier)
  NODE_ENUM(OmpOrderModifier, Value)
  NODE(parser, OmpGrainsizeClause)
  NODE(OmpGrainsizeClause, Modifier)
  NODE(parser, OmpPrescriptiveness)
  NODE_ENUM(OmpPrescriptiveness, Value)
  NODE(parser, OmpNumTasksClause)
  NODE(OmpNumTasksClause, Modifier)
  NODE(parser, OmpBindClause)
  NODE_ENUM(OmpBindClause, Binding)
  NODE(parser, OmpPresentModifier)
  NODE_ENUM(OmpPresentModifier, Value)
  NODE(parser, OmpProcBindClause)
  NODE_ENUM(OmpProcBindClause, AffinityPolicy)
  NODE(parser, OmpReductionModifier)
  NODE_ENUM(OmpReductionModifier, Value)
  NODE(parser, OmpReductionClause)
  NODE(OmpReductionClause, Modifier)
  NODE(parser, OmpInReductionClause)
  NODE(OmpInReductionClause, Modifier)
  NODE(parser, OmpReductionCombiner)
  NODE(parser, OmpRefModifier)
  NODE_ENUM(OmpRefModifier, Value)
  NODE(parser, OmpSelfModifier)
  NODE_ENUM(OmpSelfModifier, Value)
  NODE(parser, OmpTaskReductionClause)
  NODE(OmpTaskReductionClause, Modifier)
  NODE(parser, OmpInitializerProc)
  NODE(parser, OmpInitializerClause)
  NODE(parser, OmpReductionIdentifier)
  NODE(parser, OmpAllocateClause)
  NODE(OmpAllocateClause, Modifier)
  NODE(parser, OmpAlignModifier)
  NODE(parser, OmpAllocatorComplexModifier)
  NODE(parser, OmpAllocatorSimpleModifier)
  NODE(parser, OmpScheduleClause)
  NODE(OmpScheduleClause, Modifier)
  NODE_ENUM(OmpScheduleClause, Kind)
  NODE(parser, OmpSeverityClause)
  NODE(parser, OmpDeviceClause)
  NODE(OmpDeviceClause, Modifier)
  NODE(parser, OmpDeviceModifier)
  NODE_ENUM(OmpDeviceModifier, Value)
  NODE(parser, OmpDeviceTypeClause)
  NODE_ENUM(OmpDeviceTypeClause, DeviceTypeDescription)
  NODE(parser, OmpInteropRuntimeIdentifier)
  NODE(parser, OmpInteropPreference)
  NODE(parser, OmpInteropType)
  NODE_ENUM(OmpInteropType, Value)
  NODE(parser, OmpInitClause)
  NODE(OmpInitClause, Modifier)
  NODE(parser, OmpUseClause)
  NODE(parser, OmpUpdateClause)
  NODE(parser, OmpChunkModifier)
  NODE_ENUM(OmpChunkModifier, Value)
  NODE(parser, OmpOrderingModifier)
  NODE_ENUM(OmpOrderingModifier, Value)
  NODE(parser, OmpSectionsDirective)
  NODE(parser, OmpToClause)
  NODE(OmpToClause, Modifier)
  NODE(parser, OmpxHoldModifier)
  NODE_ENUM(OmpxHoldModifier, Value)
  NODE(parser, Only)
  NODE(parser, OpenACCAtomicConstruct)
  NODE(parser, OpenACCBlockConstruct)
  NODE(parser, OpenACCCacheConstruct)
  NODE(parser, OpenACCCombinedConstruct)
  NODE(parser, OpenACCConstruct)
  NODE(parser, OpenACCDeclarativeConstruct)
  NODE(parser, OpenACCEndConstruct)
  NODE(parser, OpenACCLoopConstruct)
  NODE(parser, OpenMPInteropConstruct)
  NODE(parser, OpenACCRoutineConstruct)
  NODE(parser, OpenACCStandaloneDeclarativeConstruct)
  NODE(parser, OpenACCStandaloneConstruct)
  NODE(parser, OpenACCWaitConstruct)
  NODE(parser, OpenMPAssumeConstruct)
  NODE(parser, OpenMPDeclarativeAssumes)
  NODE(parser, OmpAssumeDirective)
  NODE(parser, OmpEndAssumeDirective)
  NODE(parser, OpenMPAtomicConstruct)
  NODE(parser, OpenMPBlockConstruct)
  NODE(parser, OpenMPCancelConstruct)
  NODE(parser, OpenMPCancellationPointConstruct)
  NODE(parser, OpenMPConstruct)
  NODE(parser, OpenMPCriticalConstruct)
  NODE(parser, OpenMPDeclarativeAllocate)
  NODE(parser, OpenMPDeclarativeConstruct)
  NODE(parser, OmpDeclareVariantDirective)
  NODE(parser, OpenMPDeclareReductionConstruct)
  NODE(parser, OpenMPDeclareSimdConstruct)
  NODE(parser, OpenMPDeclareTargetConstruct)
  NODE(parser, OpenMPDeclareMapperConstruct)
  NODE_ENUM(common, OmpMemoryOrderType)
  NODE(parser, OmpMemoryOrderClause)
  NODE(parser, OmpAtomicDefaultMemOrderClause)
  NODE(parser, OpenMPDepobjConstruct)
  NODE(parser, OpenMPUtilityConstruct)
  NODE(parser, OpenMPDispatchConstruct)
  NODE(parser, OpenMPFlushConstruct)
  NODE(parser, OpenMPLoopConstruct)
  NODE(parser, OpenMPExecutableAllocate)
  NODE(parser, OpenMPAllocatorsConstruct)
  NODE(parser, OpenMPRequiresConstruct)
  NODE(parser, OpenMPSimpleStandaloneConstruct)
  NODE(parser, OpenMPStandaloneConstruct)
  NODE(parser, OpenMPSectionConstruct)
  NODE(parser, OpenMPSectionsConstruct)
  NODE(parser, OpenMPThreadprivate)
  NODE(parser, OpenStmt)
  NODE(parser, Optional)
  NODE(parser, OptionalStmt)
  NODE(parser, OtherSpecificationStmt)
  NODE(parser, OutputImpliedDo)
  NODE(parser, OutputItem)
  NODE(parser, Parameter)
  NODE(parser, ParameterStmt)
  NODE(parser, ParentIdentifier)
  NODE(parser, Pass)
  NODE(parser, PauseStmt)
  NODE(parser, Pointer)
  NODE(parser, PointerAssignmentStmt)
  NODE(PointerAssignmentStmt, Bounds)
  NODE(parser, PointerDecl)
  NODE(parser, PointerObject)
  NODE(parser, PointerStmt)
  NODE(parser, PositionOrFlushSpec)
  NODE(parser, PrefixSpec)
  NODE(PrefixSpec, Elemental)
  NODE(PrefixSpec, Impure)
  NODE(PrefixSpec, Module)
  NODE(PrefixSpec, Non_Recursive)
  NODE(PrefixSpec, Pure)
  NODE(PrefixSpec, Recursive)
  NODE(PrefixSpec, Attributes)
  NODE(PrefixSpec, Launch_Bounds)
  NODE(PrefixSpec, Cluster_Dims)
  NODE(parser, PrintStmt)
  NODE(parser, PrivateStmt)
  NODE(parser, PrivateOrSequence)
  NODE(parser, ProcAttrSpec)
  NODE(parser, ProcComponentAttrSpec)
  NODE(parser, ProcComponentDefStmt)
  NODE(parser, ProcComponentRef)
  NODE(parser, ProcDecl)
  NODE(parser, ProcInterface)
  NODE(parser, ProcPointerInit)
  NODE(parser, ProcedureDeclarationStmt)
  NODE(parser, ProcedureDesignator)
  NODE(parser, ProcedureStmt)
  NODE_ENUM(ProcedureStmt, Kind)
  NODE(parser, Program)
  NODE(parser, ProgramStmt)
  NODE(parser, ProgramUnit)
  NODE(parser, Protected)
  NODE(parser, ProtectedStmt)
  NODE(parser, ReadStmt)
  NODE(parser, RealLiteralConstant)
  NODE(RealLiteralConstant, Real)
  NODE(parser, Rename)
  NODE(Rename, Names)
  NODE(Rename, Operators)
  NODE(parser, ReturnStmt)
  NODE(parser, RewindStmt)
  NODE(parser, Save)
  NODE(parser, SaveStmt)
  NODE(parser, SavedEntity)
  NODE_ENUM(SavedEntity, Kind)
  NODE(parser, SectionSubscript)
  NODE(parser, SelectCaseStmt)
  NODE(parser, SelectRankCaseStmt)
  NODE(SelectRankCaseStmt, Rank)
  NODE(parser, SelectRankConstruct)
  NODE(SelectRankConstruct, RankCase)
  NODE(parser, SelectRankStmt)
  NODE(parser, SelectTypeConstruct)
  NODE(SelectTypeConstruct, TypeCase)
  NODE(parser, SelectTypeStmt)
  NODE(parser, Selector)
  NODE(parser, SeparateModuleSubprogram)
  NODE(parser, SequenceStmt)
  NODE(parser, Sign)
  NODE(parser, SignedComplexLiteralConstant)
  NODE(parser, SignedIntLiteralConstant)
  NODE(parser, SignedRealLiteralConstant)
  NODE(parser, SpecificationConstruct)
  NODE(parser, SpecificationExpr)
  NODE(parser, SpecificationPart)
  NODE(parser, Star)
  NODE(parser, StatOrErrmsg)
  NODE(parser, StatVariable)
  NODE(parser, StatusExpr)
  NODE(parser, StmtFunctionStmt)
  NODE(parser, StopCode)
  NODE(parser, StopStmt)
  NODE_ENUM(StopStmt, Kind)
  NODE(parser, StructureComponent)
  NODE(parser, StructureConstructor)
  NODE(parser, StructureDef)
  NODE(StructureDef, EndStructureStmt)
  NODE(parser, StructureField)
  NODE(parser, StructureStmt)
  NODE(parser, Submodule)
  NODE(parser, SubmoduleStmt)
  NODE(parser, SubroutineStmt)
  NODE(parser, SubroutineSubprogram)
  NODE(parser, SubscriptTriplet)
  NODE(parser, Substring)
  NODE(parser, SubstringInquiry)
  NODE(parser, SubstringRange)
  NODE(parser, Suffix)
  NODE(parser, SyncAllStmt)
  NODE(parser, SyncImagesStmt)
  NODE(SyncImagesStmt, ImageSet)
  NODE(parser, SyncMemoryStmt)
  NODE(parser, SyncTeamStmt)
  NODE(parser, Target)
  NODE(parser, TargetStmt)
  NODE(parser, TypeAttrSpec)
  NODE(TypeAttrSpec, BindC)
  NODE(TypeAttrSpec, Extends)
  NODE(parser, TypeBoundGenericStmt)
  NODE(parser, TypeBoundProcBinding)
  NODE(parser, TypeBoundProcDecl)
  NODE(parser, TypeBoundProcedurePart)
  NODE(parser, TypeBoundProcedureStmt)
  NODE(TypeBoundProcedureStmt, WithInterface)
  NODE(TypeBoundProcedureStmt, WithoutInterface)
  NODE(parser, TypeDeclarationStmt)
  NODE(parser, TypeGuardStmt)
  NODE(TypeGuardStmt, Guard)
  NODE(parser, TypeParamDecl)
  NODE(parser, TypeParamDefStmt)
  NODE(common, TypeParamAttr)
  NODE(parser, TypeParamSpec)
  NODE(parser, TypeParamValue)
  NODE(TypeParamValue, Deferred)
  NODE(parser, TypeSpec)
  NODE(parser, Union)
  NODE(Union, EndUnionStmt)
  NODE(Union, UnionStmt)
  NODE(parser, UnlockStmt)
  NODE(parser, UnsignedLiteralConstant)
  NODE(parser, UnsignedTypeSpec)
  NODE(parser, UseStmt)
  NODE_ENUM(UseStmt, ModuleNature)
  NODE(parser, Value)
  NODE(parser, ValueStmt)
  NODE(parser, Variable)
  NODE(parser, VectorTypeSpec)
  NODE(VectorTypeSpec, PairVectorTypeSpec)
  NODE(VectorTypeSpec, QuadVectorTypeSpec)
  NODE(parser, IntrinsicVectorTypeSpec)
  NODE(parser, VectorElementType)
  NODE(parser, Verbatim)
  NODE(parser, Volatile)
  NODE(parser, VolatileStmt)
  NODE(parser, WaitSpec)
  NODE(parser, WaitStmt)
  NODE(parser, WhereBodyConstruct)
  NODE(parser, WhereConstruct)
  NODE(WhereConstruct, Elsewhere)
  NODE(WhereConstruct, MaskedElsewhere)
  NODE(parser, WhereConstructStmt)
  NODE(parser, WhereStmt)
  NODE(parser, WriteStmt)
#undef NODE
#undef NODE_NAME

  template <typename T> bool Pre(const T &x) {
    std::string fortran{AsFortran<T>(x)};
    if (fortran.empty() && (UnionTrait<T> || WrapperTrait<T>)) {
      Prefix(GetNodeName(x));
    } else {
      IndentEmptyLine();
      out_ << GetNodeName(x);
      if (!fortran.empty()) {
        out_ << " = '" << fortran << '\'';
      }
      EndLine();
      ++indent_;
    }
    return true;
  }

  template <typename T> void Post(const T &x) {
    if (AsFortran<T>(x).empty() && (UnionTrait<T> || WrapperTrait<T>)) {
      EndLineIfNonempty();
    } else {
      --indent_;
    }
  }

  // A few types we want to ignore

  bool Pre(const CharBlock &) { return true; }
  void Post(const CharBlock &) {}

  template <typename T> bool Pre(const Statement<T> &) { return true; }
  template <typename T> void Post(const Statement<T> &) {}
  template <typename T> bool Pre(const UnlabeledStatement<T> &) { return true; }
  template <typename T> void Post(const UnlabeledStatement<T> &) {}

  template <typename T> bool Pre(const common::Indirection<T> &) {
    return true;
  }
  template <typename T> void Post(const common::Indirection<T> &) {}

  template <typename A> bool Pre(const Scalar<A> &) {
    Prefix("Scalar");
    return true;
  }
  template <typename A> void Post(const Scalar<A> &) { EndLineIfNonempty(); }

  template <typename A> bool Pre(const Constant<A> &) {
    Prefix("Constant");
    return true;
  }
  template <typename A> void Post(const Constant<A> &) { EndLineIfNonempty(); }

  template <typename A> bool Pre(const Integer<A> &) {
    Prefix("Integer");
    return true;
  }
  template <typename A> void Post(const Integer<A> &) { EndLineIfNonempty(); }

  template <typename A> bool Pre(const Logical<A> &) {
    Prefix("Logical");
    return true;
  }
  template <typename A> void Post(const Logical<A> &) { EndLineIfNonempty(); }

  template <typename A> bool Pre(const DefaultChar<A> &) {
    Prefix("DefaultChar");
    return true;
  }
  template <typename A> void Post(const DefaultChar<A> &) {
    EndLineIfNonempty();
  }

  template <typename... A> bool Pre(const std::tuple<A...> &) { return true; }
  template <typename... A> void Post(const std::tuple<A...> &) {}

  template <typename... A> bool Pre(const std::variant<A...> &) { return true; }
  template <typename... A> void Post(const std::variant<A...> &) {}

protected:
  // Return a Fortran representation of this node to include in the dump
  template <typename T> std::string AsFortran(const T &x) {
    std::string buf;
    llvm::raw_string_ostream ss{buf};
    if constexpr (HasTypedExpr<T>::value) {
      if (asFortran_ && x.typedExpr) {
        asFortran_->expr(ss, *x.typedExpr);
      }
    } else if constexpr (std::is_same_v<T, AssignmentStmt> ||
        std::is_same_v<T, PointerAssignmentStmt>) {
      if (asFortran_ && x.typedAssignment) {
        asFortran_->assignment(ss, *x.typedAssignment);
      }
    } else if constexpr (std::is_same_v<T, CallStmt>) {
      if (asFortran_ && x.typedCall) {
        asFortran_->call(ss, *x.typedCall);
      }
    } else if constexpr (std::is_same_v<T, IntLiteralConstant> ||
        std::is_same_v<T, SignedIntLiteralConstant> ||
        std::is_same_v<T, UnsignedLiteralConstant>) {
      ss << std::get<CharBlock>(x.t);
    } else if constexpr (std::is_same_v<T, RealLiteralConstant::Real>) {
      ss << x.source;
    } else if constexpr (std::is_same_v<T, std::string> ||
        std::is_same_v<T, std::int64_t> || std::is_same_v<T, std::uint64_t>) {
      ss << x;
    }
    if (ss.tell()) {
      return buf;
    }
    if constexpr (std::is_same_v<T, Name>) {
      return x.source.ToString();
#ifdef SHOW_ALL_SOURCE_MEMBERS
    } else if constexpr (HasSource<T>::value) {
      return x.source.ToString();
#endif
    } else if constexpr (std::is_same_v<T, int>) {
      return std::to_string(x);
    } else if constexpr (std::is_same_v<T, bool>) {
      return x ? "true" : "false";
    } else {
      return "";
    }
  }

  void IndentEmptyLine() {
    if (emptyline_ && indent_ > 0) {
      for (int i{0}; i < indent_; ++i) {
        out_ << "| ";
      }
      emptyline_ = false;
    }
  }

  void Prefix(const char *str) {
    IndentEmptyLine();
    out_ << str << " -> ";
    emptyline_ = false;
  }

  void Prefix(const std::string &str) {
    IndentEmptyLine();
    out_ << str << " -> ";
    emptyline_ = false;
  }

  void EndLine() {
    out_ << '\n';
    emptyline_ = true;
  }

  void EndLineIfNonempty() {
    if (!emptyline_) {
      EndLine();
    }
  }

private:
  int indent_{0};
  llvm::raw_ostream &out_;
  const AnalyzedObjectsAsFortran *const asFortran_;
  bool emptyline_{false};
};

template <typename T>
llvm::raw_ostream &DumpTree(llvm::raw_ostream &out, const T &x,
    const AnalyzedObjectsAsFortran *asFortran = nullptr) {
  ParseTreeDumper dumper{out, asFortran};
  Walk(x, dumper);
  return out;
}

} // namespace Fortran::parser
#endif // FORTRAN_PARSER_DUMP_PARSE_TREE_H_

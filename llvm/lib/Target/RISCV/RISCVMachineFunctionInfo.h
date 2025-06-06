//=- RISCVMachineFunctionInfo.h - RISC-V machine function info ----*- C++ -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares RISCV-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCV_RISCVMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_RISCV_RISCVMACHINEFUNCTIONINFO_H

#include "RISCVSubtarget.h"
#include "llvm/CodeGen/MIRYamlMapping.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class RISCVMachineFunctionInfo;

namespace yaml {
struct RISCVMachineFunctionInfo final : public yaml::MachineFunctionInfo {
  int VarArgsFrameIndex;
  int VarArgsSaveSize;

  RISCVMachineFunctionInfo() = default;
  RISCVMachineFunctionInfo(const llvm::RISCVMachineFunctionInfo &MFI);

  void mappingImpl(yaml::IO &YamlIO) override;
  ~RISCVMachineFunctionInfo() = default;
};

template <> struct MappingTraits<RISCVMachineFunctionInfo> {
  static void mapping(IO &YamlIO, RISCVMachineFunctionInfo &MFI) {
    YamlIO.mapOptional("varArgsFrameIndex", MFI.VarArgsFrameIndex);
    YamlIO.mapOptional("varArgsSaveSize", MFI.VarArgsSaveSize);
  }
};
} // end namespace yaml

/// RISCVMachineFunctionInfo - This class is derived from MachineFunctionInfo
/// and contains private RISCV-specific information for each MachineFunction.
class RISCVMachineFunctionInfo : public MachineFunctionInfo {
private:
  /// FrameIndex for start of varargs area
  int VarArgsFrameIndex = 0;
  /// Size of the save area used for varargs
  int VarArgsSaveSize = 0;
  /// FrameIndex used for transferring values between 64-bit FPRs and a pair
  /// of 32-bit GPRs via the stack.
  int MoveF64FrameIndex = -1;
  /// FrameIndex of the spill slot for the scratch register in BranchRelaxation.
  int BranchRelaxationScratchFrameIndex = -1;
  /// Size of any opaque stack adjustment due to save/restore libcalls.
  unsigned LibCallStackSize = 0;
  /// Size of RVV stack.
  uint64_t RVVStackSize = 0;
  /// Alignment of RVV stack.
  Align RVVStackAlign;
  /// Padding required to keep RVV stack aligned within the main stack.
  uint64_t RVVPadding = 0;
  /// Size of stack frame to save callee saved registers
  unsigned CalleeSavedStackSize = 0;
  /// Is there any vector argument or return?
  bool IsVectorCall = false;

  /// Registers that have been sign extended from i32.
  SmallVector<Register, 8> SExt32Registers;

  /// Size of stack frame for Zcmp PUSH/POP
  unsigned RVPushStackSize = 0;
  unsigned RVPushRegs = 0;

  /// Size of any opaque stack adjustment due to QCI Interrupt instructions.
  unsigned QCIInterruptStackSize = 0;

  /// Store Frame Indexes for Interrupt-Related CSR Spills.
  SmallVector<int, 2> InterruptCSRFrameIndexes;

  int64_t StackProbeSize = 0;

  /// Does it probe the stack for a dynamic allocation?
  bool HasDynamicAllocation = false;

public:
  RISCVMachineFunctionInfo(const Function &F, const RISCVSubtarget *STI);

  MachineFunctionInfo *
  clone(BumpPtrAllocator &Allocator, MachineFunction &DestMF,
        const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
      const override;

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  unsigned getVarArgsSaveSize() const { return VarArgsSaveSize; }
  void setVarArgsSaveSize(int Size) { VarArgsSaveSize = Size; }

  int getMoveF64FrameIndex(MachineFunction &MF) {
    if (MoveF64FrameIndex == -1)
      MoveF64FrameIndex =
          MF.getFrameInfo().CreateStackObject(8, Align(8), false);
    return MoveF64FrameIndex;
  }

  int getBranchRelaxationScratchFrameIndex() const {
    return BranchRelaxationScratchFrameIndex;
  }
  void setBranchRelaxationScratchFrameIndex(int Index) {
    BranchRelaxationScratchFrameIndex = Index;
  }

  unsigned getReservedSpillsSize() const {
    return LibCallStackSize + RVPushStackSize + QCIInterruptStackSize;
  }

  unsigned getLibCallStackSize() const { return LibCallStackSize; }
  void setLibCallStackSize(unsigned Size) { LibCallStackSize = Size; }

  bool useSaveRestoreLibCalls(const MachineFunction &MF) const {
    // We cannot use fixed locations for the callee saved spill slots if the
    // function uses a varargs save area, or is an interrupt handler.
    return !isPushable(MF) &&
           MF.getSubtarget<RISCVSubtarget>().enableSaveRestore() &&
           VarArgsSaveSize == 0 && !MF.getFrameInfo().hasTailCall() &&
           !MF.getFunction().hasFnAttribute("interrupt");
  }

  uint64_t getRVVStackSize() const { return RVVStackSize; }
  void setRVVStackSize(uint64_t Size) { RVVStackSize = Size; }

  Align getRVVStackAlign() const { return RVVStackAlign; }
  void setRVVStackAlign(Align StackAlign) { RVVStackAlign = StackAlign; }

  uint64_t getRVVPadding() const { return RVVPadding; }
  void setRVVPadding(uint64_t Padding) { RVVPadding = Padding; }

  unsigned getCalleeSavedStackSize() const { return CalleeSavedStackSize; }
  void setCalleeSavedStackSize(unsigned Size) { CalleeSavedStackSize = Size; }

  enum class PushPopKind { None = 0, StdExtZcmp, VendorXqccmp };

  PushPopKind getPushPopKind(const MachineFunction &MF) const;

  bool isPushable(const MachineFunction &MF) const {
    return getPushPopKind(MF) != PushPopKind::None;
  }

  unsigned getRVPushRegs() const { return RVPushRegs; }
  void setRVPushRegs(unsigned Regs) { RVPushRegs = Regs; }

  unsigned getRVPushStackSize() const { return RVPushStackSize; }
  void setRVPushStackSize(unsigned Size) { RVPushStackSize = Size; }

  enum class InterruptStackKind {
    None = 0,
    QCINest,
    QCINoNest,
    SiFiveCLICPreemptible,
    SiFiveCLICStackSwap,
    SiFiveCLICPreemptibleStackSwap
  };

  InterruptStackKind getInterruptStackKind(const MachineFunction &MF) const;

  bool useQCIInterrupt(const MachineFunction &MF) const {
    InterruptStackKind Kind = getInterruptStackKind(MF);
    return Kind == InterruptStackKind::QCINest ||
           Kind == InterruptStackKind::QCINoNest;
  }

  unsigned getQCIInterruptStackSize() const { return QCIInterruptStackSize; }
  void setQCIInterruptStackSize(unsigned Size) { QCIInterruptStackSize = Size; }

  bool useSiFiveInterrupt(const MachineFunction &MF) const {
    InterruptStackKind Kind = getInterruptStackKind(MF);
    return Kind == InterruptStackKind::SiFiveCLICPreemptible ||
           Kind == InterruptStackKind::SiFiveCLICStackSwap ||
           Kind == InterruptStackKind::SiFiveCLICPreemptibleStackSwap;
  }

  bool isSiFivePreemptibleInterrupt(const MachineFunction &MF) const {
    InterruptStackKind Kind = getInterruptStackKind(MF);
    return Kind == InterruptStackKind::SiFiveCLICPreemptible ||
           Kind == InterruptStackKind::SiFiveCLICPreemptibleStackSwap;
  }

  bool isSiFiveStackSwapInterrupt(const MachineFunction &MF) const {
    InterruptStackKind Kind = getInterruptStackKind(MF);
    return Kind == InterruptStackKind::SiFiveCLICStackSwap ||
           Kind == InterruptStackKind::SiFiveCLICPreemptibleStackSwap;
  }

  void pushInterruptCSRFrameIndex(int FI) {
    InterruptCSRFrameIndexes.push_back(FI);
  }
  int getInterruptCSRFrameIndex(size_t Idx) const {
    return InterruptCSRFrameIndexes[Idx];
  }

  // Some Stack Management Variants automatically update FP in a frame-pointer
  // convention compatible way - which means we don't need to manually update
  // the FP, but we still need to emit the correct CFI information for
  // calculating the CFA based on FP.
  bool hasImplicitFPUpdates(const MachineFunction &MF) const;

  void initializeBaseYamlFields(const yaml::RISCVMachineFunctionInfo &YamlMFI);

  void addSExt32Register(Register Reg);
  bool isSExt32Register(Register Reg) const;

  bool isVectorCall() const { return IsVectorCall; }
  void setIsVectorCall() { IsVectorCall = true; }

  bool hasDynamicAllocation() const { return HasDynamicAllocation; }
  void setDynamicAllocation() { HasDynamicAllocation = true; }
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RISCV_RISCVMACHINEFUNCTIONINFO_H

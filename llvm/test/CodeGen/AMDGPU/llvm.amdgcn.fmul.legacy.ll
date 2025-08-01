; RUN: llc -mtriple=amdgcn -mcpu=tahiti  < %s | FileCheck -check-prefixes=GCN,MADMACF32,GFX6 %s
; RUN: llc -mtriple=amdgcn -mcpu=tonga   < %s | FileCheck -check-prefixes=GCN,MADMACF32,GFX8 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx900  < %s | FileCheck -check-prefixes=GCN,MADMACF32,GFX9 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx90a  < %s | FileCheck -check-prefixes=GCN,MADMACF32,GFX9 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx1010 < %s | FileCheck -check-prefixes=GCN,MADMACF32,GFX101 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx1030 < %s | FileCheck -check-prefixes=GCN,NOMADMACF32,GFX103 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx1100 < %s | FileCheck -check-prefixes=GFX11 %s

; GCN-LABEL: {{^}}test_mul_legacy_f32:
; GCN: v_mul_legacy_f32{{[_e3264]*}} v{{[0-9]+}}, s{{[0-9]+}}, {{[sv][0-9]+}}
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
define amdgpu_kernel void @test_mul_legacy_f32(ptr addrspace(1) %out, float %a, float %b) #0 {
  %result = call float @llvm.amdgcn.fmul.legacy(float %a, float %b)
  store float %result, ptr addrspace(1) %out, align 4
  ret void
}

; GCN-LABEL: {{^}}test_mul_legacy_undef0_f32:
; GCN: v_mul_legacy_f32{{[_e3264]*}} v{{[0-9]+}}, s{{[0-9]+}}, {{[sv][0-9]+}}
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
define amdgpu_kernel void @test_mul_legacy_undef0_f32(ptr addrspace(1) %out, float %a) #0 {
  %result = call float @llvm.amdgcn.fmul.legacy(float poison, float %a)
  store float %result, ptr addrspace(1) %out, align 4
  ret void
}

; GCN-LABEL: {{^}}test_mul_legacy_undef1_f32:
; GCN: v_mul_legacy_f32{{[_e3264]*}} v{{[0-9]+}}, s{{[0-9]+}}, {{[sv][0-9]+}}
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
define amdgpu_kernel void @test_mul_legacy_undef1_f32(ptr addrspace(1) %out, float %a) #0 {
  %result = call float @llvm.amdgcn.fmul.legacy(float %a, float poison)
  store float %result, ptr addrspace(1) %out, align 4
  ret void
}

; GCN-LABEL: {{^}}test_mul_legacy_fabs_f32:
; GCN: v_mul_legacy_f32{{[_e3264]*}} v{{[0-9]+}}, |s{{[0-9]+}}|, |{{[sv][0-9]+}}|
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, |s{{[0-9]+}}|, |s{{[0-9]+}}|
define amdgpu_kernel void @test_mul_legacy_fabs_f32(ptr addrspace(1) %out, float %a, float %b) #0 {
  %a.fabs = call float @llvm.fabs.f32(float %a)
  %b.fabs = call float @llvm.fabs.f32(float %b)
  %result = call float @llvm.amdgcn.fmul.legacy(float %a.fabs, float %b.fabs)
  store float %result, ptr addrspace(1) %out, align 4
  ret void
}

; Don't form mad/mac instructions because they don't support denormals.
; GCN-LABEL: {{^}}test_add_mul_legacy_f32:
; GCN: v_mul_legacy_f32{{[_e3264]*}} v{{[0-9]+}}, s{{[0-9]+}}, {{[sv][0-9]+}}
; GCN: v_add_f32_e{{(32|64)}} v{{[0-9]+}}, s{{[0-9]+}}, {{[sv][0-9]+}}
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
; GFX11: v_dual_mov_b32 v{{[0-9]+}}, 0 :: v_dual_add_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
define amdgpu_kernel void @test_add_mul_legacy_f32(ptr addrspace(1) %out, float %a, float %b, float %c) #0 {
  %mul = call float @llvm.amdgcn.fmul.legacy(float %a, float %b)
  %add = fadd float %mul, %c
  store float %add, ptr addrspace(1) %out, align 4
  ret void
}

; GCN-LABEL: {{^}}test_mad_legacy_f32:
; GFX6: v_mac_legacy_f32_e32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX8: v_mad_legacy_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX9: v_mad_legacy_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX101: v_mac_legacy_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
; GFX103: v_mul_legacy_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
; GFX103: v_add_f32_e32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, s{{[0-9]+}}, s{{[0-9]+}}
; GFX11: v_dual_mov_b32 v{{[0-9]+}}, 0 :: v_dual_add_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
define amdgpu_kernel void @test_mad_legacy_f32(ptr addrspace(1) %out, float %a, float %b, float %c) #2 {
  %mul = call float @llvm.amdgcn.fmul.legacy(float %a, float %b)
  %add = fadd float %mul, %c
  store float %add, ptr addrspace(1) %out, align 4
  ret void
}

; GCN-LABEL: {{^}}test_mad_legacy_f32_imm:
; GFX6: v_mad_legacy_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX8: v_mad_legacy_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX9: v_mad_legacy_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX101: v_mad_legacy_f32 v{{[0-9]+}}, 0x41200000, s{{[0-9]+}}
; GFX103: v_mul_legacy_f32_e64 v{{[0-9]+}}, 0x41200000, s{{[0-9]+}}
; GFX103: v_add_f32_e32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
define amdgpu_kernel void @test_mad_legacy_f32_imm(ptr addrspace(1) %out, float %a, float %c) #2 {
  %mul = call float @llvm.amdgcn.fmul.legacy(float %a, float 10.0)
  %add = fadd float %mul, %c
  store float %add, ptr addrspace(1) %out, align 4
  ret void
}

; GCN-LABEL: {{^}}test_mad_legacy_fneg_f32:
; MADMACF32: v_mad_legacy_f32 v{{[0-9]+}}, -s{{[0-9]+}}, -{{[sv][0-9]+}}, v{{[0-9]+}}
; NOMADMACF32: v_mul_legacy_f32_e64 v{{[0-9]+}}, -s{{[0-9]+}}, -s{{[0-9]+}}
; NOMADMACF32: v_add_f32_e32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
; GFX11: v_mul_dx9_zero_f32_e64 v{{[0-9]+}}, -s{{[0-9]+}}, -s{{[0-9]+}}
; GFX11: v_dual_mov_b32 v{{[0-9]+}}, 0 :: v_dual_add_f32 v{{[0-9]+}}, s{{[0-9]+}}, v{{[0-9]+}}
define amdgpu_kernel void @test_mad_legacy_fneg_f32(ptr addrspace(1) %out, float %a, float %b, float %c) #2 {
  %a.fneg = fneg float %a
  %b.fneg = fneg float %b
  %mul = call float @llvm.amdgcn.fmul.legacy(float %a.fneg, float %b.fneg)
  %add = fadd float %mul, %c
  store float %add, ptr addrspace(1) %out, align 4
  ret void
}

declare float @llvm.fabs.f32(float) #1
declare float @llvm.amdgcn.fmul.legacy(float, float) #1

attributes #0 = { nounwind }
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind "denormal-fp-math"="preserve-sign" "amdgpu-no-dispatch-id" "amdgpu-no-dispatch-ptr" "amdgpu-no-implicitarg-ptr" "amdgpu-no-lds-kernel-id" "amdgpu-no-queue-ptr" "amdgpu-no-workgroup-id-x" "amdgpu-no-workgroup-id-y" "amdgpu-no-workgroup-id-z" "amdgpu-no-workitem-id-x" "amdgpu-no-workitem-id-y" "amdgpu-no-workitem-id-z" }

; RUN: llc -mtriple=amdgcn < %s | FileCheck -check-prefix=GCN %s
; RUN: llc -mtriple=amdgcn -mcpu=tonga < %s  | FileCheck -check-prefix=GCN %s

declare float @llvm.fabs.f32(float) #0
declare double @llvm.fabs.f64(double) #0
declare float @llvm.amdgcn.frexp.mant.f32(float) #0
declare double @llvm.amdgcn.frexp.mant.f64(double) #0

; GCN-LABEL: {{^}}s_test_frexp_mant_f32:
; GCN: v_frexp_mant_f32_e32 {{v[0-9]+}}, {{s[0-9]+}}
define amdgpu_kernel void @s_test_frexp_mant_f32(ptr addrspace(1) %out, float %src) #1 {
  %frexp.mant = call float @llvm.amdgcn.frexp.mant.f32(float %src)
  store float %frexp.mant, ptr addrspace(1) %out
  ret void
}

; GCN-LABEL: {{^}}s_test_fabs_frexp_mant_f32:
; GCN: v_frexp_mant_f32_e64 {{v[0-9]+}}, |{{s[0-9]+}}|
define amdgpu_kernel void @s_test_fabs_frexp_mant_f32(ptr addrspace(1) %out, float %src) #1 {
  %fabs.src = call float @llvm.fabs.f32(float %src)
  %frexp.mant = call float @llvm.amdgcn.frexp.mant.f32(float %fabs.src)
  store float %frexp.mant, ptr addrspace(1) %out
  ret void
}

; GCN-LABEL: {{^}}s_test_fneg_fabs_frexp_mant_f32:
; GCN: v_frexp_mant_f32_e64 {{v[0-9]+}}, -|{{s[0-9]+}}|
define amdgpu_kernel void @s_test_fneg_fabs_frexp_mant_f32(ptr addrspace(1) %out, float %src) #1 {
  %fabs.src = call float @llvm.fabs.f32(float %src)
  %fneg.fabs.src = fneg float %fabs.src
  %frexp.mant = call float @llvm.amdgcn.frexp.mant.f32(float %fneg.fabs.src)
  store float %frexp.mant, ptr addrspace(1) %out
  ret void
}

; GCN-LABEL: {{^}}s_test_frexp_mant_f64:
; GCN: v_frexp_mant_f64_e32 {{v\[[0-9]+:[0-9]+\]}}, {{s\[[0-9]+:[0-9]+\]}}
define amdgpu_kernel void @s_test_frexp_mant_f64(ptr addrspace(1) %out, double %src) #1 {
  %frexp.mant = call double @llvm.amdgcn.frexp.mant.f64(double %src)
  store double %frexp.mant, ptr addrspace(1) %out
  ret void
}

; GCN-LABEL: {{^}}s_test_fabs_frexp_mant_f64:
; GCN: v_frexp_mant_f64_e64 {{v\[[0-9]+:[0-9]+\]}}, |{{s\[[0-9]+:[0-9]+\]}}|
define amdgpu_kernel void @s_test_fabs_frexp_mant_f64(ptr addrspace(1) %out, double %src) #1 {
  %fabs.src = call double @llvm.fabs.f64(double %src)
  %frexp.mant = call double @llvm.amdgcn.frexp.mant.f64(double %fabs.src)
  store double %frexp.mant, ptr addrspace(1) %out
  ret void
}

; GCN-LABEL: {{^}}s_test_fneg_fabs_frexp_mant_f64:
; GCN: v_frexp_mant_f64_e64 {{v\[[0-9]+:[0-9]+\]}}, -|{{s\[[0-9]+:[0-9]+\]}}|
define amdgpu_kernel void @s_test_fneg_fabs_frexp_mant_f64(ptr addrspace(1) %out, double %src) #1 {
  %fabs.src = call double @llvm.fabs.f64(double %src)
  %fneg.fabs.src = fneg double %fabs.src
  %frexp.mant = call double @llvm.amdgcn.frexp.mant.f64(double %fneg.fabs.src)
  store double %frexp.mant, ptr addrspace(1) %out
  ret void
}

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind }

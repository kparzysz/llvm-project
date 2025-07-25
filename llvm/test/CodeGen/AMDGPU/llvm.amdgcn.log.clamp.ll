; RUN: llc -mtriple=amdgcn < %s | FileCheck -check-prefix=GCN %s
; RUN: not llc -mtriple=amdgcn -mcpu=tonga < %s 2>&1 | FileCheck -check-prefix=ERR %s

; ERR: intrinsic not supported on subtarget

declare float @llvm.amdgcn.log.clamp.f32(float) #0

; GCN-LABEL: {{^}}v_log_clamp_f32:
; GCN: v_log_clamp_f32_e32 {{v[0-9]+}}, {{s[0-9]+}}
define amdgpu_kernel void @v_log_clamp_f32(ptr addrspace(1) %out, float %src) #1 {
  %log.clamp = call float @llvm.amdgcn.log.clamp.f32(float %src) #0
  store float %log.clamp, ptr addrspace(1) %out
  ret void
}

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind }

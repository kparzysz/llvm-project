; RUN: llc -mtriple=amdgcn -mcpu=verde < %s | FileCheck -check-prefixes=GCN,GFX6789 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx900 < %s | FileCheck -check-prefixes=GCN,GFX6789 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx90a < %s | FileCheck -check-prefixes=GCN,GFX90A %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx90a -early-live-intervals < %s | FileCheck -check-prefixes=GCN,GFX90A %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx1010 -show-mc-encoding < %s | FileCheck -check-prefixes=GCN,GFX10 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx1100 -show-mc-encoding < %s | FileCheck -check-prefixes=GCN,GFX10 %s
; RUN: llc -mtriple=amdgcn -mcpu=gfx1200 -show-mc-encoding < %s | FileCheck -check-prefixes=GCN,GFX12 %s

; GCN-LABEL: {{^}}atomic_swap_1d:
; GFX6789: image_atomic_swap v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_swap v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_swap v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_swap v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_swap_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.swap.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_swap_1d_i64:
; GFX6789: image_atomic_swap v[0:1], v2, s[0:7] dmask:0x3 unorm glc{{$}}
; GFX90A: image_atomic_swap v[0:1], v2, s[0:7] dmask:0x3 unorm glc{{$}}
; GFX10: image_atomic_swap v[0:1], v2, s[0:7] dmask:0x3 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_swap v[0:1], v2, s[0:7] dmask:0x3 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps <2 x float> @atomic_swap_1d_i64(<8 x i32> inreg %rsrc, i64 %data, i32 %s) {
main_body:
  %v = call i64 @llvm.amdgcn.image.atomic.swap.1d.i64.i32(i64 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i64 %v to <2 x float>
  ret <2 x float> %out
}

; GCN-LABEL: {{^}}atomic_swap_1d_float:
; GFX6789: image_atomic_swap v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_swap v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_swap v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_swap v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_swap_1d_float(<8 x i32> inreg %rsrc, float %data, i32 %s) {
main_body:
  %v = call float @llvm.amdgcn.image.atomic.swap.1d.f32.i32(float %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  ret float %v
}

; GCN-LABEL: {{^}}atomic_add_1d:
; GFX6789: image_atomic_add v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_add v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_add v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_add_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_sub_1d:
; GFX6789: image_atomic_sub v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_sub v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_sub v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_sub_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_sub_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.sub.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_smin_1d:
; GFX6789: image_atomic_smin v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_smin v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_smin v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_min_int v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_smin_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.smin.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_umin_1d:
; GFX6789: image_atomic_umin v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_umin v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_umin v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_min_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_umin_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.umin.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_smax_1d:
; GFX6789: image_atomic_smax v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_smax v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_smax v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_max_int v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_smax_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.smax.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_umax_1d:
; GFX6789: image_atomic_umax v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_umax v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_umax v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_max_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_umax_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.umax.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_and_1d:
; GFX6789: image_atomic_and v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_and v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_and v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_and v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_and_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.and.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_or_1d:
; GFX6789: image_atomic_or v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_or v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_or v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_or v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_or_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.or.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_xor_1d:
; GFX6789: image_atomic_xor v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_xor v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_xor v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_xor v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_xor_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.xor.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_inc_1d:
; GFX6789: image_atomic_inc v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_inc v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_inc v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_inc_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_inc_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.inc.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_dec_1d:
; GFX6789: image_atomic_dec v0, v1, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_dec v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_dec v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_dec_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_dec_1d(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.dec.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_cmpswap_1d:
; GFX6789: image_atomic_cmpswap v[0:1], v2, s[0:7] dmask:0x3 unorm glc{{$}}
; GFX90A: image_atomic_cmpswap v[0:1], v2, s[0:7] dmask:0x3 unorm glc{{$}}
; GFX10: image_atomic_cmpswap v[0:1], v2, s[0:7] dmask:0x3 dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_cmpswap v[0:1], v2, s[0:7] dmask:0x3 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_cmpswap_1d(<8 x i32> inreg %rsrc, i32 %cmp, i32 %swap, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.cmpswap.1d.i32.i32(i32 %cmp, i32 %swap, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_cmpswap_1d_64:
; GFX6789: image_atomic_cmpswap v[0:3], v4, s[0:7] dmask:0xf unorm glc{{$}}
; GFX90A: image_atomic_cmpswap v[0:3], v4, s[0:7] dmask:0xf unorm glc{{$}}
; GFX10: image_atomic_cmpswap v[0:3], v4, s[0:7] dmask:0xf dim:SQ_RSRC_IMG_1D unorm glc ;
; GFX12: image_atomic_cmpswap v[0:3], v4, s[0:7] dmask:0xf dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_RETURN ;
define amdgpu_ps <2 x float> @atomic_cmpswap_1d_64(<8 x i32> inreg %rsrc, i64 %cmp, i64 %swap, i32 %s) {
main_body:
  %v = call i64 @llvm.amdgcn.image.atomic.cmpswap.1d.i64.i32(i64 %cmp, i64 %swap, i32 %s, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i64 %v to <2 x float>
  ret <2 x float> %out
}

; GCN-LABEL: {{^}}atomic_add_2d:
; GFX6789: image_atomic_add v0, v[1:2], s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[13579]}}], s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_add v0, v[1:2], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_2d(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %t) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.2d.i32.i32(i32 %data, i32 %s, i32 %t, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_3d:
; GFX6789: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[02468]}}], s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_3D unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2, v3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_3D th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_3d(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %t, i32 %r) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.3d.i32.i32(i32 %data, i32 %s, i32 %t, i32 %r, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_cube:
; GFX6789: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[02468]}}], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX10: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_CUBE unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2, v3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_CUBE th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_cube(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %t, i32 %face) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.cube.i32.i32(i32 %data, i32 %s, i32 %t, i32 %face, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_1darray:
; GFX6789: image_atomic_add v0, v[1:2], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[13579]}}], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX10: image_atomic_add v0, v[1:2], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D_ARRAY unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D_ARRAY th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_1darray(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %slice) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.1darray.i32.i32(i32 %data, i32 %s, i32 %slice, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_2darray:
; GFX6789: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[02468]}}], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX10: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D_ARRAY unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2, v3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D_ARRAY th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_2darray(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %t, i32 %slice) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.2darray.i32.i32(i32 %data, i32 %s, i32 %t, i32 %slice, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_2dmsaa:
; GFX6789: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 unorm glc{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[02468]}}], s[0:7] dmask:0x1 unorm glc{{$}}
; GFX10: image_atomic_add v0, v[1:3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D_MSAA unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2, v3], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D_MSAA th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_2dmsaa(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %t, i32 %fragid) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.2dmsaa.i32.i32(i32 %data, i32 %s, i32 %t, i32 %fragid, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_2darraymsaa:
; GFX6789: image_atomic_add v0, v[1:4], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX90A: image_atomic_add v0, v[{{[02468]}}:{{[13579]}}], s[0:7] dmask:0x1 unorm glc da{{$}}
; GFX10: image_atomic_add v0, v[1:4], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D_MSAA_ARRAY unorm glc ;
; GFX12: image_atomic_add_uint v0, [v1, v2, v3, v4], s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_2D_MSAA_ARRAY th:TH_ATOMIC_RETURN ;
define amdgpu_ps float @atomic_add_2darraymsaa(<8 x i32> inreg %rsrc, i32 %data, i32 %s, i32 %t, i32 %slice, i32 %fragid) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.2darraymsaa.i32.i32(i32 %data, i32 %s, i32 %t, i32 %slice, i32 %fragid, <8 x i32> %rsrc, i32 0, i32 0)
  %out = bitcast i32 %v to float
  ret float %out
}

; GCN-LABEL: {{^}}atomic_add_1d_slc:
; GFX6789: image_atomic_add v0, v1, s[0:7] dmask:0x1 unorm glc slc{{$}}
; GFX90A: image_atomic_add v0, v{{[02468]}}, s[0:7] dmask:0x1 unorm glc slc{{$}}
; GFX10: image_atomic_add v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D unorm glc slc ;
; GFX12: image_atomic_add_uint v0, v1, s[0:7] dmask:0x1 dim:SQ_RSRC_IMG_1D th:TH_ATOMIC_NT_RETURN ;
define amdgpu_ps float @atomic_add_1d_slc(<8 x i32> inreg %rsrc, i32 %data, i32 %s) {
main_body:
  %v = call i32 @llvm.amdgcn.image.atomic.add.1d.i32.i32(i32 %data, i32 %s, <8 x i32> %rsrc, i32 0, i32 2)
  %out = bitcast i32 %v to float
  ret float %out
}

declare i32 @llvm.amdgcn.image.atomic.swap.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.sub.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.smin.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.umin.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.smax.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.umax.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.and.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.or.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.xor.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.inc.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.dec.1d.i32.i32(i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.cmpswap.1d.i32.i32(i32, i32, i32, <8 x i32>, i32, i32) #0

declare i64 @llvm.amdgcn.image.atomic.swap.1d.i64.i32(i64, i32, <8 x i32>, i32, i32) #0
declare i64 @llvm.amdgcn.image.atomic.cmpswap.1d.i64.i32(i64, i64, i32, <8 x i32>, i32, i32) #0

declare float @llvm.amdgcn.image.atomic.swap.1d.f32.i32(float, i32, <8 x i32>, i32, i32) #0

declare i32 @llvm.amdgcn.image.atomic.add.2d.i32.i32(i32, i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.3d.i32.i32(i32, i32, i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.cube.i32.i32(i32, i32, i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.1darray.i32.i32(i32, i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.2darray.i32.i32(i32, i32, i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.2dmsaa.i32.i32(i32, i32, i32, i32, <8 x i32>, i32, i32) #0
declare i32 @llvm.amdgcn.image.atomic.add.2darraymsaa.i32.i32(i32, i32, i32, i32, i32, <8 x i32>, i32, i32) #0

attributes #0 = { nounwind }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind readnone }

// RUN: %clang_cc1 -emit-llvm %s -o - -ffreestanding -triple=i386-pc-win32       | FileCheck %s --check-prefix=X86
// RUN: %clang_cc1 -emit-llvm %s -o - -ffreestanding -triple=x86_64-pc-win32     | FileCheck %s --check-prefix=X64

void __vectorcall v1(int a, int b) {}
// X86: define dso_local x86_vectorcallcc void @"\01v1@@8"(i32 inreg noundef %a, i32 inreg noundef %b)
// X64: define dso_local x86_vectorcallcc void @"\01v1@@16"(i32 noundef %a, i32 noundef %b)

void __vectorcall v2(char a, char b) {}
// X86: define dso_local x86_vectorcallcc void @"\01v2@@8"(i8 inreg noundef signext %a, i8 inreg noundef signext %b)
// X64: define dso_local x86_vectorcallcc void @"\01v2@@16"(i8 noundef %a, i8 noundef %b)

struct Small { int x; };
void __vectorcall v3(int a, struct Small b, int c) {}
// X86: define dso_local x86_vectorcallcc void @"\01v3@@12"(i32 inreg noundef %a, i32 %b.0, i32 inreg noundef %c)
// X64: define dso_local x86_vectorcallcc void @"\01v3@@24"(i32 noundef %a, i32 %b.coerce, i32 noundef %c)

struct Large { int a[5]; };
void __vectorcall v4(int a, struct Large b, int c) {}
// X86: define dso_local x86_vectorcallcc void @"\01v4@@28"(i32 inreg noundef %a, ptr noundef byval(%struct.Large) align 4 %b, i32 inreg noundef %c)
// X64: define dso_local x86_vectorcallcc void @"\01v4@@40"(i32 noundef %a, ptr dead_on_return noundef %b, i32 noundef %c)

void __vectorcall v5(long long a, int b, int c) {}
// X86: define dso_local x86_vectorcallcc void @"\01v5@@16"(i64 noundef %a, i32 inreg noundef %b, i32 inreg noundef %c)
// X64: define dso_local x86_vectorcallcc void @"\01v5@@24"(i64 noundef %a, i32 noundef %b, i32 noundef %c)

struct HFA2 { double x, y; };
struct HFA4 { double w, x, y, z; };
struct HFA5 { double v, w, x, y, z; };

void __vectorcall hfa1(int a, struct HFA4 b, int c) {}
// X86: define dso_local x86_vectorcallcc void @"\01hfa1@@40"(i32 inreg noundef %a, %struct.HFA4 inreg %b.coerce, i32 inreg noundef %c)
// X64: define dso_local x86_vectorcallcc void @"\01hfa1@@48"(i32 noundef %a, %struct.HFA4 inreg %b.coerce, i32 noundef %c)

// HFAs that would require more than six total SSE registers are passed
// indirectly. Additional vector arguments can consume the rest of the SSE
// registers.
void __vectorcall hfa2(struct HFA4 a, struct HFA4 b, double c) {}
// X86: define dso_local x86_vectorcallcc void @"\01hfa2@@72"(%struct.HFA4 inreg %a.coerce, ptr dead_on_return inreg noundef %b, double inreg noundef %c)
// X64: define dso_local x86_vectorcallcc void @"\01hfa2@@72"(%struct.HFA4 inreg %a.coerce, ptr dead_on_return noundef %b, double noundef %c)

// Ensure that we pass builtin types directly while counting them against the
// SSE register usage.
void __vectorcall hfa3(double a, double b, double c, double d, double e, struct HFA2 f) {}
// X86: define dso_local x86_vectorcallcc void @"\01hfa3@@56"(double inreg noundef %a, double inreg noundef %b, double inreg noundef %c, double inreg noundef %d, double inreg noundef %e, ptr dead_on_return inreg noundef %f)
// X64: define dso_local x86_vectorcallcc void @"\01hfa3@@56"(double noundef %a, double noundef %b, double noundef %c, double noundef %d, double noundef %e, ptr dead_on_return noundef %f)

// Aggregates with more than four elements are not HFAs and are passed byval.
// Because they are not classified as homogeneous, they don't get special
// handling to ensure alignment.
void __vectorcall hfa4(struct HFA5 a) {}
// X86: define dso_local x86_vectorcallcc void @"\01hfa4@@40"(ptr noundef byval(%struct.HFA5) align 4 %0)
// X64: define dso_local x86_vectorcallcc void @"\01hfa4@@40"(ptr dead_on_return noundef %a)

// Return HFAs of 4 or fewer elements in registers.
static struct HFA2 g_hfa2;
struct HFA2 __vectorcall hfa5(void) { return g_hfa2; }
// X86: define dso_local x86_vectorcallcc %struct.HFA2 @"\01hfa5@@0"()
// X64: define dso_local x86_vectorcallcc %struct.HFA2 @"\01hfa5@@0"()

typedef float __attribute__((vector_size(16))) v4f32;
struct HVA2 { v4f32 x, y; };
struct HVA3 { v4f32 w, x, y; };
struct HVA4 { v4f32 w, x, y, z; };
struct HVA5 { v4f32 w, x, y, z, p; };

v4f32 __vectorcall hva1(int a, struct HVA4 b, int c) {return b.w;}
// X86: define dso_local x86_vectorcallcc <4 x float> @"\01hva1@@72"(i32 inreg noundef %a, %struct.HVA4 inreg %b.coerce, i32 inreg noundef %c)
// X64: define dso_local x86_vectorcallcc <4 x float> @"\01hva1@@80"(i32 noundef %a, %struct.HVA4 inreg %b.coerce, i32 noundef %c)

v4f32 __vectorcall hva2(struct HVA4 a, struct HVA4 b, v4f32 c) {return c;}
// X86: define dso_local x86_vectorcallcc <4 x float> @"\01hva2@@144"(%struct.HVA4 inreg %a.coerce, ptr dead_on_return inreg noundef %b, <4 x float> inreg noundef %c)
// X64: define dso_local x86_vectorcallcc <4 x float> @"\01hva2@@144"(%struct.HVA4 inreg %a.coerce, ptr dead_on_return noundef %b, <4 x float> noundef %c)

v4f32 __vectorcall hva3(v4f32 a, v4f32 b, v4f32 c, v4f32 d, v4f32 e, struct HVA2 f) {return f.x;}
// X86: define dso_local x86_vectorcallcc <4 x float> @"\01hva3@@112"(<4 x float> inreg noundef %a, <4 x float> inreg noundef %b, <4 x float> inreg noundef %c, <4 x float> inreg noundef %d, <4 x float> inreg noundef %e, ptr dead_on_return inreg noundef %f)
// X64: define dso_local x86_vectorcallcc <4 x float> @"\01hva3@@112"(<4 x float> noundef %a, <4 x float> noundef %b, <4 x float> noundef %c, <4 x float> noundef %d, <4 x float> noundef %e, ptr dead_on_return noundef %f)

// vector types have higher priority then HVA structures, So vector types are allocated first
// and HVAs are allocated if enough registers are available
v4f32 __vectorcall hva4(struct HVA4 a, struct HVA2 b, v4f32 c) {return b.y;}
// X86: define dso_local x86_vectorcallcc <4 x float> @"\01hva4@@112"(%struct.HVA4 inreg %a.coerce, ptr dead_on_return inreg noundef %b, <4 x float> inreg noundef %c)
// X64: define dso_local x86_vectorcallcc <4 x float> @"\01hva4@@112"(%struct.HVA4 inreg %a.coerce, ptr dead_on_return noundef %b, <4 x float> noundef %c)

v4f32 __vectorcall hva5(struct HVA3 a, struct HVA3 b, v4f32 c, struct HVA2 d) {return d.y;}
// X86: define dso_local x86_vectorcallcc <4 x float> @"\01hva5@@144"(%struct.HVA3 inreg %a.coerce, ptr dead_on_return inreg noundef %b, <4 x float> inreg noundef %c, %struct.HVA2 inreg %d.coerce)
// X64: define dso_local x86_vectorcallcc <4 x float> @"\01hva5@@144"(%struct.HVA3 inreg %a.coerce, ptr dead_on_return noundef %b, <4 x float> noundef %c, %struct.HVA2 inreg %d.coerce)

struct HVA4 __vectorcall hva6(struct HVA4 a, struct HVA4 b) { return b;}
// X86: define dso_local x86_vectorcallcc %struct.HVA4 @"\01hva6@@128"(%struct.HVA4 inreg %a.coerce, ptr dead_on_return inreg noundef %b)
// X64: define dso_local x86_vectorcallcc %struct.HVA4 @"\01hva6@@128"(%struct.HVA4 inreg %a.coerce, ptr dead_on_return noundef %b)

struct HVA5 __vectorcall hva7(void) {struct HVA5 a = {}; return a;}
// X86: define dso_local x86_vectorcallcc void @"\01hva7@@0"(ptr dead_on_unwind noalias writable sret(%struct.HVA5) align 16 %agg.result)
// X64: define dso_local x86_vectorcallcc void @"\01hva7@@0"(ptr dead_on_unwind noalias writable sret(%struct.HVA5) align 16 %agg.result)

v4f32 __vectorcall hva8(v4f32 a, v4f32 b, v4f32 c, v4f32 d, int e, v4f32 f) {return f;}
// X86: define dso_local x86_vectorcallcc <4 x float> @"\01hva8@@84"(<4 x float> inreg noundef %a, <4 x float> inreg noundef %b, <4 x float> inreg noundef %c, <4 x float> inreg noundef %d, i32 inreg noundef %e, <4 x float> inreg noundef %f)
// X64: define dso_local x86_vectorcallcc <4 x float> @"\01hva8@@88"(<4 x float> noundef %a, <4 x float> noundef %b, <4 x float> noundef %c, <4 x float> noundef %d, i32 noundef %e, <4 x float> noundef %f)

typedef float __attribute__((ext_vector_type(3))) v3f32;
struct OddSizeHVA { v3f32 x, y; };

void __vectorcall odd_size_hva(struct OddSizeHVA a) {}
// X86: define dso_local x86_vectorcallcc void @"\01odd_size_hva@@32"(%struct.OddSizeHVA inreg %a.coerce)
// X64: define dso_local x86_vectorcallcc void @"\01odd_size_hva@@32"(%struct.OddSizeHVA inreg %a.coerce)

// The Vectorcall ABI only allows passing the first 6 items in registers in x64, so this shouldn't
// consider 'p7' as a register.  Instead p5 gets put into the register on the second pass.
// x86 should pass p2, p6 and p7 in registers, then p1 in the second pass.
struct HFA2 __vectorcall AddParticles(struct HFA2 p1, float p2, struct HFA4 p3, int p4, struct HFA2 p5, float p6, float p7, int p8){ return p1;}
// X86: define dso_local x86_vectorcallcc %struct.HFA2 @"\01AddParticles@@84"(%struct.HFA2 inreg %p1.coerce, float inreg noundef %p2, ptr dead_on_return inreg noundef %p3, i32 inreg noundef %p4, ptr dead_on_return noundef %p5, float inreg noundef %p6, float inreg noundef %p7, i32 noundef %p8)
// X64: define dso_local x86_vectorcallcc %struct.HFA2 @"\01AddParticles@@104"(%struct.HFA2 inreg %p1.coerce, float noundef %p2, ptr dead_on_return noundef %p3, i32 noundef %p4, %struct.HFA2 inreg %p5.coerce, float noundef %p6, float noundef %p7, i32 noundef %p8)

// Vectorcall in both architectures allows passing of an HVA as long as there is room,
// even if it is not one of the first 6 arguments.  First pass puts p4 into a
// register on both.  p9 ends up in a register in x86 only.  Second pass puts p1
// in a register, does NOT put p7 in a register (since there's no room), then puts
// p8 in a register.
void __vectorcall HVAAnywhere(struct HFA2 p1, int p2, int p3, float p4, int p5, int p6, struct HFA4 p7, struct HFA2 p8, float p9){}
// X86: define dso_local x86_vectorcallcc void @"\01HVAAnywhere@@88"(%struct.HFA2 inreg %p1.coerce, i32 inreg noundef %p2, i32 inreg noundef %p3, float inreg noundef %p4, i32 noundef %p5, i32 noundef %p6, ptr dead_on_return noundef %p7, %struct.HFA2 inreg %p8.coerce, float inreg noundef %p9)
// X64: define dso_local x86_vectorcallcc void @"\01HVAAnywhere@@112"(%struct.HFA2 inreg %p1.coerce, i32 noundef %p2, i32 noundef %p3, float noundef %p4, i32 noundef %p5, i32 noundef %p6, ptr dead_on_return noundef %p7, %struct.HFA2 inreg %p8.coerce, float noundef %p9)

#ifndef __x86_64__
// This covers the three ways XMM values can be passed on 32-bit x86:
// - directly in XMM register (xmm5)
// - indirectly by address, address in GPR (ecx)
// - indirectly by address, address on stack
void __vectorcall vectorcall_indirect_vec(
    double xmm0, double xmm1, double xmm2, double xmm3, double xmm4,
    v4f32 xmm5, v4f32 ecx, int edx, v4f32 mem) {
}

// X86: define dso_local x86_vectorcallcc void @"\01vectorcall_indirect_vec@@{{[0-9]+}}"
// X86-SAME: (double inreg noundef %xmm0,
// X86-SAME: double inreg noundef %xmm1,
// X86-SAME: double inreg noundef %xmm2,
// X86-SAME: double inreg noundef %xmm3,
// X86-SAME: double inreg noundef %xmm4,
// X86-SAME: <4 x float> inreg noundef %xmm5,
// X86-SAME: ptr dead_on_return inreg noundef %0,
// X86-SAME: i32 inreg noundef %edx,
// X86-SAME: ptr dead_on_return noundef %1)

void __vectorcall vectorcall_indirect_fp(
    double xmm0, double xmm1, double xmm2, double xmm3, double xmm4,
    v4f32 xmm5, v4f32 ecx, int edx, double mem) {
}

// X86: define dso_local x86_vectorcallcc void @"\01vectorcall_indirect_fp@@{{[0-9]+}}"
// X86-SAME: (double inreg noundef %xmm0,
// X86-SAME: double inreg noundef %xmm1,
// X86-SAME: double inreg noundef %xmm2,
// X86-SAME: double inreg noundef %xmm3,
// X86-SAME: double inreg noundef %xmm4,
// X86-SAME: <4 x float> inreg noundef %xmm5,
// X86-SAME: ptr dead_on_return inreg noundef %0,
// X86-SAME: i32 inreg noundef %edx,
// X86-SAME: double noundef %mem)
#endif

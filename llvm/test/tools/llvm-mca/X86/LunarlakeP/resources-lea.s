# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=lunarlake -instruction-tables < %s | FileCheck %s

lea 0(), %cx
lea 0(), %ecx
lea 0(), %rcx
lea (%eax), %cx
lea (%eax), %ecx
lea (%eax), %rcx
lea (%rax), %cx
lea (%rax), %ecx
lea (%rax), %rcx
lea (, %ebx), %cx
lea (, %ebx), %ecx
lea (, %ebx), %rcx
lea (, %rbx), %cx
lea (, %rbx), %ecx
lea (, %rbx), %rcx
lea (, %ebx, 1), %cx
lea (, %ebx, 1), %ecx
lea (, %ebx, 1), %rcx
lea (, %rbx, 1), %cx
lea (, %rbx, 1), %ecx
lea (, %rbx, 1), %rcx
lea (, %ebx, 2), %cx
lea (, %ebx, 2), %ecx
lea (, %ebx, 2), %rcx
lea (, %rbx, 2), %cx
lea (, %rbx, 2), %ecx
lea (, %rbx, 2), %rcx
lea (%eax, %ebx), %cx
lea (%eax, %ebx), %ecx
lea (%eax, %ebx), %rcx
lea (%rax, %rbx), %cx
lea (%rax, %rbx), %ecx
lea (%rax, %rbx), %rcx
lea (%eax, %ebx, 1), %cx
lea (%eax, %ebx, 1), %ecx
lea (%eax, %ebx, 1), %rcx
lea (%rax, %rbx, 1), %cx
lea (%rax, %rbx, 1), %ecx
lea (%rax, %rbx, 1), %rcx
lea (%eax, %ebx, 2), %cx
lea (%eax, %ebx, 2), %ecx
lea (%eax, %ebx, 2), %rcx
lea (%rax, %rbx, 2), %cx
lea (%rax, %rbx, 2), %ecx
lea (%rax, %rbx, 2), %rcx

lea -16(), %cx
lea -16(), %ecx
lea -16(), %rcx
lea -16(%eax), %cx
lea -16(%eax), %ecx
lea -16(%eax), %rcx
lea -16(%rax), %cx
lea -16(%rax), %ecx
lea -16(%rax), %rcx
lea -16(, %ebx), %cx
lea -16(, %ebx), %ecx
lea -16(, %ebx), %rcx
lea -16(, %rbx), %cx
lea -16(, %rbx), %ecx
lea -16(, %rbx), %rcx
lea -16(, %ebx, 1), %cx
lea -16(, %ebx, 1), %ecx
lea -16(, %ebx, 1), %rcx
lea -16(, %rbx, 1), %cx
lea -16(, %rbx, 1), %ecx
lea -16(, %rbx, 1), %rcx
lea -16(, %ebx, 2), %cx
lea -16(, %ebx, 2), %ecx
lea -16(, %ebx, 2), %rcx
lea -16(, %rbx, 2), %cx
lea -16(, %rbx, 2), %ecx
lea -16(, %rbx, 2), %rcx
lea -16(%eax, %ebx), %cx
lea -16(%eax, %ebx), %ecx
lea -16(%eax, %ebx), %rcx
lea -16(%rax, %rbx), %cx
lea -16(%rax, %rbx), %ecx
lea -16(%rax, %rbx), %rcx
lea -16(%eax, %ebx, 1), %cx
lea -16(%eax, %ebx, 1), %ecx
lea -16(%eax, %ebx, 1), %rcx
lea -16(%rax, %rbx, 1), %cx
lea -16(%rax, %rbx, 1), %ecx
lea -16(%rax, %rbx, 1), %rcx
lea -16(%eax, %ebx, 2), %cx
lea -16(%eax, %ebx, 2), %ecx
lea -16(%eax, %ebx, 2), %rcx
lea -16(%rax, %rbx, 2), %cx
lea -16(%rax, %rbx, 2), %ecx
lea -16(%rax, %rbx, 2), %rcx

lea 1024(), %cx
lea 1024(), %ecx
lea 1024(), %rcx
lea 1024(%eax), %cx
lea 1024(%eax), %ecx
lea 1024(%eax), %rcx
lea 1024(%rax), %cx
lea 1024(%rax), %ecx
lea 1024(%rax), %rcx
lea 1024(, %ebx), %cx
lea 1024(, %ebx), %ecx
lea 1024(, %ebx), %rcx
lea 1024(, %rbx), %cx
lea 1024(, %rbx), %ecx
lea 1024(, %rbx), %rcx
lea 1024(, %ebx, 1), %cx
lea 1024(, %ebx, 1), %ecx
lea 1024(, %ebx, 1), %rcx
lea 1024(, %rbx, 1), %cx
lea 1024(, %rbx, 1), %ecx
lea 1024(, %rbx, 1), %rcx
lea 1024(, %ebx, 2), %cx
lea 1024(, %ebx, 2), %ecx
lea 1024(, %ebx, 2), %rcx
lea 1024(, %rbx, 2), %cx
lea 1024(, %rbx, 2), %ecx
lea 1024(, %rbx, 2), %rcx
lea 1024(%eax, %ebx), %cx
lea 1024(%eax, %ebx), %ecx
lea 1024(%eax, %ebx), %rcx
lea 1024(%rax, %rbx), %cx
lea 1024(%rax, %rbx), %ecx
lea 1024(%rax, %rbx), %rcx
lea 1024(%eax, %ebx, 1), %cx
lea 1024(%eax, %ebx, 1), %ecx
lea 1024(%eax, %ebx, 1), %rcx
lea 1024(%rax, %rbx, 1), %cx
lea 1024(%rax, %rbx, 1), %ecx
lea 1024(%rax, %rbx, 1), %rcx
lea 1024(%eax, %ebx, 2), %cx
lea 1024(%eax, %ebx, 2), %ecx
lea 1024(%eax, %ebx, 2), %rcx
lea 1024(%rax, %rbx, 2), %cx
lea 1024(%rax, %rbx, 2), %ecx
lea 1024(%rax, %rbx, 2), %rcx

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  2      2     0.33                        leaw	0, %cx
# CHECK-NEXT:  1      1     1.00                        leal	0, %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	0, %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%eax), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%eax), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%eax), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%rax), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%rax), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%rax), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(,%ebx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(,%ebx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(,%ebx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(,%rbx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(,%rbx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(,%rbx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%eax,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%eax,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%eax,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%rax,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%rax,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%rax,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%eax,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%eax,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%eax,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%rax,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%rax,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%rax,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%eax,%ebx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%eax,%ebx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%eax,%ebx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	(%rax,%rbx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	(%rax,%rbx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	(%rax,%rbx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16, %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16, %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16, %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%eax), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%eax), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%eax), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%rax), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%rax), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%rax), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(,%ebx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(,%ebx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(,%ebx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(,%rbx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(,%rbx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(,%rbx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%eax,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%eax,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%eax,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%rax,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%rax,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%rax,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%eax,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%eax,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%eax,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%rax,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%rax,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%rax,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%eax,%ebx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%eax,%ebx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%eax,%ebx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	-16(%rax,%rbx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	-16(%rax,%rbx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	-16(%rax,%rbx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024, %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024, %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024, %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%eax), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%eax), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%eax), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%rax), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%rax), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%rax), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(,%ebx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(,%ebx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(,%ebx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(,%rbx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(,%rbx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(,%rbx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%eax,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%eax,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%eax,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%rax,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%rax,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%rax,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%eax,%ebx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%eax,%ebx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%eax,%ebx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%rax,%rbx), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%rax,%rbx), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%rax,%rbx), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%eax,%ebx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%eax,%ebx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%eax,%ebx,2), %rcx
# CHECK-NEXT:  2      2     0.33                        leaw	1024(%rax,%rbx,2), %cx
# CHECK-NEXT:  1      1     1.00                        leal	1024(%rax,%rbx,2), %ecx
# CHECK-NEXT:  1      1     1.00                        leaq	1024(%rax,%rbx,2), %rcx

# CHECK:      Resources:
# CHECK-NEXT: [0]   - LNLPPort00
# CHECK-NEXT: [1]   - LNLPPort01
# CHECK-NEXT: [2]   - LNLPPort02
# CHECK-NEXT: [3]   - LNLPPort03
# CHECK-NEXT: [4]   - LNLPPort04
# CHECK-NEXT: [5]   - LNLPPort05
# CHECK-NEXT: [6]   - LNLPPort10
# CHECK-NEXT: [7]   - LNLPPort11
# CHECK-NEXT: [8]   - LNLPPort20
# CHECK-NEXT: [9]   - LNLPPort21
# CHECK-NEXT: [10]  - LNLPPort22
# CHECK-NEXT: [11]  - LNLPPort25
# CHECK-NEXT: [12]  - LNLPPort26
# CHECK-NEXT: [13]  - LNLPPort27
# CHECK-NEXT: [14]  - LNLPPortInvalid
# CHECK-NEXT: [15]  - LNLPVPort00
# CHECK-NEXT: [16]  - LNLPVPort01
# CHECK-NEXT: [17]  - LNLPVPort02
# CHECK-NEXT: [18]  - LNLPVPort03

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   [13]   [14]   [15]   [16]   [17]   [18]
# CHECK-NEXT: 7.50   112.50 7.50   22.50  7.50   22.50   -      -      -      -      -      -      -      -      -      -      -      -      -

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   [13]   [14]   [15]   [16]   [17]   [18]   Instructions:
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	0, %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	0, %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	0, %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%eax), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%eax), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%eax), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%rax), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%rax), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%rax), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(,%ebx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(,%ebx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(,%ebx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(,%rbx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(,%rbx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(,%rbx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%eax,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%eax,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%eax,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%rax,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%rax,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%rax,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%eax,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%eax,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%eax,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%rax,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%rax,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%rax,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%eax,%ebx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%eax,%ebx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%eax,%ebx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	(%rax,%rbx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	(%rax,%rbx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	(%rax,%rbx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16, %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16, %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16, %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%eax), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%eax), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%eax), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%rax), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%rax), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%rax), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(,%ebx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(,%ebx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(,%ebx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(,%rbx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(,%rbx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(,%rbx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%eax,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%eax,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%eax,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%rax,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%rax,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%rax,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%eax,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%eax,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%eax,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%rax,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%rax,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%rax,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%eax,%ebx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%eax,%ebx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%eax,%ebx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	-16(%rax,%rbx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	-16(%rax,%rbx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	-16(%rax,%rbx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024, %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024, %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024, %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%eax), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%eax), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%eax), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%rax), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%rax), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%rax), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(,%ebx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(,%ebx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(,%ebx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(,%rbx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(,%rbx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(,%rbx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%eax,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%eax,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%eax,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%rax,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%rax,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%rax,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%eax,%ebx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%eax,%ebx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%eax,%ebx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%rax,%rbx), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%rax,%rbx), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%rax,%rbx), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%eax,%ebx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%eax,%ebx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%eax,%ebx,2), %rcx
# CHECK-NEXT: 0.17   0.50   0.17   0.50   0.17   0.50    -      -      -      -      -      -      -      -      -      -      -      -      -     leaw	1024(%rax,%rbx,2), %cx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leal	1024(%rax,%rbx,2), %ecx
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -      -     leaq	1024(%rax,%rbx,2), %rcx

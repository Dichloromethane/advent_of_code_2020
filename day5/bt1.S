
// B: 0100 0010
// R: 0101 0010
// F: 0100 0110
// L: 0100 1100
//			^

.data
masks:
two_bytes:
  .rep 8
  .byte 0x04
  .endr
tformat:
  .word 0x7FF
  .word 0x7FF
  .word 0x7FF 
  .word 0x0

.text
// calculates TODO entries in one go
// requires at least TODO bytes be writtable at the end of the buffer
// void parse(const char *in, int n)
.globl parse_f
.type parse_f,@function
#define in %rdi
#define n %rsi

#define tmp %rax
#define d0 %rcx
#define d1 %rdx
#define d2 %r8
#define d3 %r9
#define d0_1 %r10
#define d1_1 %r11
#define d2_1 %r12
#define d3_1 %r13
#define mrtwo %r14
#define tf %r15

#define vmask %xmm0
#define vacc %xmm1
#define vcand %xmm2
#define vzero %xmm3

.align 16

parse_f:
  pushq %rbx
  pushq %r12
  pushq %r13
  pushq %r14

  leaq masks(%rip), tmp
  movq two_bytes - masks(tmp), mrtwo
  movq tformat - masks(tmp), tf
  
  vpxor vzero, vzero, vzero
  vpcmpeqd vacc, vacc, vacc
  vpsrlw $1, vacc, vacc

loop:
  movbeq    (in), d0
  movbeq   8(in), d1
  movbeq 2*8(in), d2
  movbeq 3*8(in), d3

  movbeq 4*8+1(in), d0_1
  movbeq 5*8+1(in), d1_1
  movbeq 6*8+1(in), d2_1
  movbeq 7*8+1(in), d3_1

  andq mrtwo, d0
  andq mrtwo, d1
  andq mrtwo, d2
  andq mrtwo, d3

  andq mrtwo, d0_1
  andq mrtwo, d1_1
  andq mrtwo, d2_1
  andq mrtwo, d3_1

  pextq mrtwo, d0, d0
  pextq mrtwo, d1, d1 
  pextq mrtwo, d2, d2 // maybe do some preprocessing before,
  pextq mrtwo, d3, d3 // to avoid some p1 contention

  pextq mrtwo, d0_1, d0_1
  pextq mrtwo, d1_1, d1_1
  pextq mrtwo, d2_1, d2_1 
  pextq mrtwo, d3_1, d3_1

  shlq $24, d0
  shlq $16, d1
  shlq $8,  d2

  shlq $24, d0_1
  shlq $16, d1_1
  shlq $8,  d2_1

  orq d1, d0
  orq d3, d2
  orq d2, d0

  orq d1_1, d0_1
  orq d3_1, d2_1
  orq d2_1, d0_1

  pdepq tf, d0, d0
  pdepq tf, d0_1, d0_1

  vpinsrq $0, d0,   vzero, vcand
  vpinsrq $1, d0_1, vcand, vcand

  // calculates the minimum of the one's complement
  vpcmpgtw vcand, vacc, vmask
  pblendvb vcand, vacc
  
  addq $8*8+2, in
  subq $1, n
  cmpq $0, n
  jg loop

  vmovdqu vacc, %xmm0

  popq %r14
  popq %r13
  popq %r12
  popq %rbx
  ret


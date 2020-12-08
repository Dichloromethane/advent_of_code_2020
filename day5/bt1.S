
// B: 0100 0010
// R: 0101 0010
// F: 0100 0110
// L: 0100 1100
//			^

.data
masks:
two_bytes:
  .rep 32
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
#define t0 %rcx
#define t1 %r10
#define mrtwo %r14
#define tf %r15

#define vmask %xmm0
#define vacc %xmm1
#define vcand %xmm2
#define vzero %xmm3

#define d0 %ymm4
#define d1 %ymm5

.align 16

parse_f:
  pushq %rbx
  pushq %r12
  pushq %r13
  pushq %r14

  leaq masks(%rip), tmp
  vmovdqu two_bytes - masks(tmp), mrtwo
  movq tformat - masks(tmp), tf
  
  vpxor vzero, vzero, vzero
  vpcmpeqd vacc, vacc, vacc
  vpsrlw $1, vacc, vacc

loop:
  vmovdqu (in), d0
  vmovdqu 4*8+1(in), d1

  vpand d0, mrtwo, d0
  vpand d1, mrtwo, d1

  vcmpeqb d0, mrtwo, d0
  vcmpeqb d1, mrtwo, d1

  vpmovmskb d0, t0
  vpmovmskb d1, t1

  pdepq tf, t0, t0
  pdepq tf, t1, t1

  vpinsrq $0, t0,   vzero, vcand
  vpinsrq $1, t1, vcand, vcand

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


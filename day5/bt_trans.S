
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
// void parse(const char *in, int *r, int n)
.globl parse_f
.type parse_f,@function
#define in %rdi
#define res %rsi
#define n %rdx

#define d0 %r8
#define d1 %r9
#define d2 %r10
#define d3 %rax
#define acc %r11
#define mrtwo %r12
#define tf %r13
#define tmp %rcx

.align 16

parse_f:
  pushq %rbx
  pushq %r8
  pushq %r9
  pushq %r10
  pushq %r11
  pushq %r12
  pushq %r13
  pushq %r14

  leaq masks(%rip), tmp
  movq two_bytes - masks(tmp), mrtwo
  movq tformat - masks(tmp), tf

loop:
  movq    (in), d0
  movq   8(in), d1
  movq 2*8(in), d2
  movq 3*8(in), d3

  andq mrtwo, d0
  andq mrtwo, d1
  andq mrtwo, d2
  andq mrtwo, d3

  pextq mrtwo, d0, d0
  pextq mrtwo, d1, d1 // maybe do some preprocessing before?
  pextq mrtwo, d2, d2 // p1 contention
  pextq mrtwo, d3, d3 // p1 contention

  shlq $8, d1
  shlq $16, d2
  shlq $24, d3

  orq d1, d0
  orq d3, d2
  orq d2, d0

  pdepq tf, d0, d0

  movq d0, (res)

  addq $6, res
  addq $4*8+1, in
  subq $1, n
  cmpq $0, n
  jg loop

  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rbx
  ret


.section .text
.global sbrk

sbrk:
  li a7, 10
  ecall
  ret

.section .text
.global _start

_start:
  la sp, stack_top
  la t0, trap_entry
  csrw mtvec, t0

  li a7, 1
  li a0, 'K'
  ecall

  li a7, 93
  ecall

loop: j loop

trap_entry:
  # save the existing registers into memory so we can safely use them for other purposes
  addi sp, sp, -128
  sw ra, 0(sp)
  sw a0, 4(sp)
  sw a1, 8(sp)
  sw a2, 12(sp)
  sw a7, 16(sp)

  # Pass args to the function
  mv a1, a0
  mv a2, a7
  csrr a0, mcause

  jal ra, handle_trap

  #  Restore the registers from memory
  lw ra, 0(sp)
  lw a0, 4(sp)
  lw a1, 8(sp)
  lw a2, 12(sp)
  lw a7, 16(sp)
  addi sp, sp, 128

  # Return to the addr specified in the mepc csr
  csrr t0, mepc
  addi t0, t0, 4
  csrw mepc, t0
  mret

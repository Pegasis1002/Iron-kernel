OUTPUT_ARCH( "riscv" )
ENTRY( _start )

SECTIONS{
  . = 0x80000000;

  .text : { *(.text) }
  .data : { *(.data) }

  . = ALIGN(16);
  heap_start = .;

  stack_top = 0x88000000;
}

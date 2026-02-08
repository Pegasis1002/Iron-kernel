OUTPUT_ARCH( "riscv" )
ENTRY( _start )

SECTIONS{
  . = 0x80000000;

  .text : { *(.text) }
  .data : { *(.data) }

  stack_top = . + 0x100000;
}

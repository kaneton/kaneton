ENTRY("_start")

PHDRS
{
  code PT_LOAD;
  data PT_LOAD;
}

SECTIONS
{
  . = 0x100000;

  .text :
  {
    *(.text*)
  } : code

  .data :
  {
    *(.data*)
  } : data
  .bss :
  {
    *(.bss*)
  } : data
}


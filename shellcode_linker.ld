OUTPUT_FORMAT("binary")
ENTRY(_start)

MEMORY
{
  shellcode (rwx)  : ORIGIN = 0x0000000, LENGTH = 0x1000000
}

SECTIONS
{
	.shellcode :
	{
		*(.text*) *(.data*) *(.bss*)
		/*. = ALIGN(0x1000);*/
	} > shellcode /* = 0xD15AC1D1F1ED*/
	
	/**
	 * These two can be discarded,
	 * not interesting for our shellcode.
	 */
	/DISCARD/ :
	{
		*(.note.gnu.property)
		*(.eh_frame)
	}
}


OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
	.shellcode :
	{
		*(.text*) *(.data*) *(.bss*)
		_GLOBAL_OFFSET_TABLE_ = .;
		/* . = ALIGN(0x1000); */
	} = 0xD15AC1D1F1ED

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


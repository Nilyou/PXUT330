
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(entry)

MEMORY
{
    ddrs (W!RX) : ORIGIN = 0x70080000, LENGTH = 32M
}

SECTIONS
{  
    .fixed :
    {
        . = ALIGN(4);
        _sfixed = .;
        *(.text*)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.data)
        . = ALIGN(4);
        _efixed = .;
    } > ddrs

    .bss : 
    {
        *(.bss)
        . = ALIGN(4);
    } > ddrs

}

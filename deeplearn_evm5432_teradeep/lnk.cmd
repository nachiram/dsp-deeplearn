/****************************************************************************/
/*  lnk.cmd   v4.00                                                         */
/*  Copyright (c) 1996-2000  Texas Instruments Incorporated                 */
/****************************************************************************/
-e _reset


MEMORY
{
    BOOT_MEM:   o = 0x80000000 l = 0x00000400
    PMEM:       o = 0x80000400 l = 0x00FFFC00
    BMEM:       o = 0x81000000 l = 0x10000000
}

/* Added sections .neardata and .rodata to permit ELF testing. Doesn't      */
/* affect COFF, since COFF doesn't have these sections. -Indira, 6/9/08     */

SECTIONS
{
    .boot     >    BOOT_MEM
    big_ro:
    {
       *(.text)
       *(.const)
       *(.far)
       *(.fardata)
    } > PMEM

    .stack      >       BMEM
    .args       >       BMEM

    GROUP
    {
            .neardata   /* Move .bss after .neardata and .rodata. ELF  allows */
            .rodata     /* uninitialized data to follow initialized data in a */
            .bss        /* single segment. This order facilitates a single    */
                        /* segment for the near DP sections.                  */
    }>BMEM

    .cinit      >       BMEM
    .cio        >       BMEM
    .data       >       BMEM
    .switch     >       BMEM
    .sysmem     >       BMEM
    .ppinfo     >       BMEM
    .ppdata     >       BMEM, palign(32) /* Work-around kelvin bug */
}

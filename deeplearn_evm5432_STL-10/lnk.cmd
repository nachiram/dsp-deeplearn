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
       *(.const)
       *(.far)
       *(.fardata)
    } > PMEM
    .text		>       PMEM
    .stack      >       PMEM
    .args       >       PMEM

    GROUP
    {
            .neardata   /* Move .bss after .neardata and .rodata. ELF  allows */
            .rodata     /* uninitialized data to follow initialized data in a */
            .bss        /* single segment. This order facilitates a single    */
                        /* segment for the near DP sections.                  */
    }>PMEM

    .cinit      >       PMEM
    .cio        >       BOOT_MEM
    .data       >       BOOT_MEM
    .switch     >       BOOT_MEM
    .sysmem     >       BMEM
    .ppinfo     >       BOOT_MEM
    .ppdata     >       BOOT_MEM, palign(32) /* Work-around kelvin bug */
}

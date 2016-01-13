/****************************************************************************/
/*  OMAP5430.cmd                                                            */
/*  Copyright (c) 2013  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an OMAP5430.          */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
    OCMC_RAM        o = 0x40300000  l = 0x00020000  /* 128kB L3 Internal memory */
    EMIF_CS0_SDRAM  o = 0x80000000  l = 0x40000000  /* 1GB external SDRAM memory on CS0 */
    EMIF_CS1_SDRAM  o = 0xC0000000  l = 0x40000000  /* 1GB external SDRAM memory on CS1 */
}

SECTIONS
{
    .text          >  OCMC_RAM
    .stack         >  OCMC_RAM
    .bss           >  OCMC_RAM
    .cio           >  OCMC_RAM
    .const         >  OCMC_RAM
    .data          >  OCMC_RAM
    .switch        >  OCMC_RAM
    .sysmem        >  EMIF_CS0_SDRAM
    .far           >  OCMC_RAM
    .args          >  OCMC_RAM
    .ppinfo        >  OCMC_RAM
    .ppdata        >  OCMC_RAM
  
    /* TI-ABI sections */
    .pinit         >  OCMC_RAM
    .cinit         >  OCMC_RAM
  
    /* EABI sections */
    .binit         >  OCMC_RAM
    .init_array    >  OCMC_RAM
    .neardata      >  OCMC_RAM
    .fardata       >  OCMC_RAM
    .rodata        >  OCMC_RAM
    .c6xabi.exidx  >  OCMC_RAM
    .c6xabi.extab  >  OCMC_RAM
}

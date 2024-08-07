ENTRY(0x0)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*       STACK SETTING                                 */
/*;++++++++++++++++++++++++++++++++++++++++++++++++++++*/
SFRORIGIN = 0x8000F000;                 /* SFR Start address                 */
RAMENDNEXT = 0x8000F000;                /* RAM End address+1 & Stack Pointer */
PROVIDE(STACKSIZE = 0x00000400);        /* Stack Size                        */
STACKORIGIN = RAMENDNEXT - STACKSIZE;   /* Stack Start address               */
PROVIDE(_$$SP = RAMENDNEXT);            /* Stack Pointer                     */

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*       DYNAMIC SETTING                               */
/*;++++++++++++++++++++++++++++++++++++++++++++++++++++*/
DYNAMIC = __data_end + 0x2;             /* dynamic start address             */
DYNAMICSIZE = STACKORIGIN - DYNAMIC;    /* dynamic_far or near size          */
PROVIDE(_$$dynamic_far = DYNAMIC);      /* for xgetmem_f                     */
PROVIDE(_$$dynamic_near = DYNAMIC);     /* for xgetmem_n                     */
PROVIDE(_$$dynamic = DYNAMICSIZE);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*       ROMWINDOW SETTING                             */
/*;++++++++++++++++++++++++++++++++++++++++++++++++++++*/
ROMWINDOWSIZE = 0xCFFF;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*       MEMORY RANGE                                  */
/*;++++++++++++++++++++++++++++++++++++++++++++++++++++*/
MEMORY
{
  ISRVEC      : ORIGIN = 0x00000000,  LENGTH = 0x00000100  /* 0000-00FF */
  ROM         : ORIGIN = 0x00000100,  LENGTH = 0x0005E000  /* 0100-1F0FFF */
  DATA        : ORIGIN = 0x80009000,  LENGTH = 0x00002000  /* D000-EFFF */
  STACK       : ORIGIN = STACKORIGIN, LENGTH = STACKSIZE   /* EC00-EFFF */
  SFR         : ORIGIN = SFRORIGIN,   LENGTH = 0x00001000  /* F000-FFFF */

}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*       SECTION Definition                            */
/*;++++++++++++++++++++++++++++++++++++++++++++++++++++*/
SECTIONS {
  .text_vec (0x0):
  {
    KEEP(*(.text.isr_vector))
    . = 0x8;
    KEEP(*(.text0.vector.*))
    PROVIDE_HIDDEN (. = ALIGN(2));
  } > ISRVEC = 0xFFFFFFFF

  .data ALIGN(2):
  {
    KEEP(*(.data0))
    KEEP(*(.data0.*))
    KEEP(*(.data))
    KEEP(*(.data.*))
    PROVIDE_HIDDEN (. = ALIGN(2));
  } > DATA AT > ROM

  .rom ALIGN(2):
  {
    KEEP(*(.rodata0))
    KEEP(*(.rodata0.*))
    __rodata0_end = (.);
    ASSERT(!(__rodata0_end > (ROMWINDOWSIZE)), "Error : .rodata0 exceeds the ROMWINDOW");
    KEEP(*(.text0))
    KEEP(*(.text0.*))
    __text0_end = (.);
    ASSERT(!(__text0_end > (0xFFFF)), "Error : .text0 exceeds 0xFFFF");
    KEEP(*(.rodata))
    KEEP(*(.rodata.*))
    KEEP(*(.text))
    KEEP(*(.text.*))
    PROVIDE_HIDDEN(. = ALIGN(2));
  } > ROM

  .bss ALIGN(2):
  {
    KEEP(*(.bss0))
    KEEP(*(.bss0.*))
    KEEP(*(.bss))
    KEEP(*(.bss.*))
    PROVIDE_HIDDEN (. = ALIGN(2));
    __data_end = .;
  } > DATA

  .vec :
  {
    WDTINT          =  ABSOLUTE(0x0008);
    EXI0INT         =  ABSOLUTE(0x0010);
    EXI1INT         =  ABSOLUTE(0x0012);
    EXI2INT         =  ABSOLUTE(0x0014);
    EXI3INT         =  ABSOLUTE(0x0016);
    EXI4INT         =  ABSOLUTE(0x0018);
    EXI5INT         =  ABSOLUTE(0x001A);
    EXI6INT         =  ABSOLUTE(0x001C);
    EXI7INT         =  ABSOLUTE(0x001E);
    CBUINT          =  ABSOLUTE(0x0020);
    MCSINT          =  ABSOLUTE(0x0024);
    UA00INT         =  ABSOLUTE(0x0026);
    UA01INT         =  ABSOLUTE(0x0028);
    SADINT          =  ABSOLUTE(0x002C);
    SIOF0INT        =  ABSOLUTE(0x002E);
    SIO0INT         =  ABSOLUTE(0x0030);
    I2CM0INT        =  ABSOLUTE(0x0034);
    FTM0INT         =  ABSOLUTE(0x0038);
    FTM1INT         =  ABSOLUTE(0x003A);
    TM0INT          =  ABSOLUTE(0x003C);
    TM1INT          =  ABSOLUTE(0x003E);
    I2CU0INT        =  ABSOLUTE(0x0040);
    UA10INT         =  ABSOLUTE(0x0042);
    UA11INT         =  ABSOLUTE(0x0044);
    TM2INT          =  ABSOLUTE(0x004C);
    TM3INT          =  ABSOLUTE(0x004E);
    UA20INT         =  ABSOLUTE(0x0050);
    UA21INT         =  ABSOLUTE(0x0052);
    TM4INT          =  ABSOLUTE(0x005C);
    TMXINT          =  ABSOLUTE(0x0072);
    LTB0INT         =  ABSOLUTE(0x0074);
    LTB3INT         =  ABSOLUTE(0x0076);
    LTB1INT         =  ABSOLUTE(0x0078);
    LTB2INT         =  ABSOLUTE(0x007A);
  } > ISRVEC

  .sfr :
  {
    DSR             =  ABSOLUTE(0xF000);
  } > SFR

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Variable for  RAM DATA copy from ROM               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++*/
PROVIDE( __START_DATA_lma = LOADADDR(.data));
PROVIDE(__START_DATA = ADDR(.data));
PROVIDE( INITTAB    = __START_DATA_lma & 65535); /*0xFFFF*/
PROVIDE( INITVAR    = __START_DATA     & 65535); /*0xFFFF*/
PROVIDE( INITSIZE   = SIZEOF(.data));
PROVIDE( INITTABSEG = ( (__START_DATA_lma >> 16) & 15) ); /*0xF*/
PROVIDE( INITVARSEG = ( (__START_DATA     >> 16) & 15) ); /*0xF*/

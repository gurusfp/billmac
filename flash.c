#include <stdint.h>
#include <p89v51rd2.h>

#include "flash.h"

#ifdef UNIT_TEST
#error "This file should not be included in Unit tests"
#endif

void
FlashInit()
{
}

/* Write a byte in to the flash using the IAP routines of the p89v51rd2. */
void
FlashWriteByte (uint16_t address, uint8_t value)
{
  assert (address < PROGRAM_CODE_START);

  FCF = 0;
  __asm
    mov R1, #0x02
    ;DPH contains the high part of the sector
    ;DPL contains the low part of the sector
    mov	a,_FlashWriteByte_PARM_2
    lcall 0x1ff0
  __endasm;
  FCF = 1;
}

/* Write a byte in to the flash using the IAP routines of the p89v51rd2. */
uint8_t
FlashReadByte (uint16_t address)
{
  uint8_t value;

  assert (address < PROGRAM_CODE_START);

  FCF = 0;
  __asm
    mov R1, #0x03
    ;DPH contains the high part of the sector
    ;DPL contains the low part of the sector
    lcall 0x1ff0
  __endasm;
  FCF = 1;

  return value;
}

/* Erase one flash sector of 128 bytes using the IAP routines of the p89v51rd2.  Take care
   not to erase the last 64 sectors where the monitor resides.  Also, after erasing
   sector 0, the jumps to the monitor locations are restored. */
void
FlashEraseSector (uint16_t address)
{
  //  assert(address >= FLASH_DATA_START);
  //  assert(address < FLASH_DATA_END);

  FCF = 0;
  __asm
    mov R1, #0x08
    ;DPH contains the high part of the sector
    ;DPL contains the low part of the sector
    lcall 0x1ff0 ;Entry point
  __endasm;
  FCF = 1;
}

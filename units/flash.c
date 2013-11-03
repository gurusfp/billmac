
#include "flash.h"

uint8_t flash[(0x10000/FLASH_SECTOR_SIZE)][FLASH_SECTOR_SIZE];

#ifndef UNIT_TEST
#error  "This file should not be used in non-Unit test mode"
#endif

void
FlashInit()
{
  uint16_t ui1, ui2;

  assert(0 == (0x10000%FLASH_SECTOR_SIZE));

  for (ui1=0; ui1<(0x10000/FLASH_SECTOR_SIZE); ui1++) {
    for (ui2=0; ui2<FLASH_SECTOR_SIZE; ui2++) {
      flash[ui1][ui2] = 0xDC;
    }
  }
}

void
FlashWriteByte (uint16_t address, uint8_t value)
{
  uint16_t ui1 = FLASH_SECTOR_SIZE-1;
  uint16_t ui2 = address & (FLASH_SECTOR_SIZE-1);
  while (ui1) {
    address >>= 1;
    ui1 >>= 1;
  }
  assert(address < (0x10000/FLASH_SECTOR_SIZE));
  flash[address][ui2] = value;
}

uint8_t
FlashReadByte (uint16_t address)
{
  uint16_t ui1 = FLASH_SECTOR_SIZE-1;
  uint16_t ui2 = address & (FLASH_SECTOR_SIZE-1);
  while (ui1) {
    address >>= 1;
    ui1 >>= 1;
  }
  assert(address < (0x10000/FLASH_SECTOR_SIZE));
  return flash[address][ui2];
}

void
FlashEraseSector (uint16_t address)
{
  assert( 0 == (address & (FLASH_SECTOR_SIZE-1)) );

  uint16_t ui1 = FLASH_SECTOR_SIZE-1;
  while (ui1) {
    address >>= 1;
    ui1 >>= 1;
  }
  assert(address < (0x10000/FLASH_SECTOR_SIZE));

  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    flash[address][ui1] = 0;
  }
}

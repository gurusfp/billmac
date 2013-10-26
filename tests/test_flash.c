#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "lcd.c"
#include "flash.c"

uint8_t lfsr8 = 0xAB;

uint8_t
rand8()
{
    uint8_t bit;

    bit   = ((lfsr8 >> 0) ^ (lfsr8 >> 2) ^ (lfsr8 >> 3) ^ (lfsr8 >> 4) ) & 1;
    lfsr8 = (lfsr8 >> 1) | (bit << 7);
    printf("lfsr8:0x%x\n", lfsr8);

    return lfsr8;
}

uint32_t
test_sector(uint16_t sector_id)
{
  uint16_t ui1;
  uint8_t seed, ui2, ui3;
  seed = lfsr8;
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++)
    FlashWriteByte(sector_id|ui1, rand8());

  seed = lfsr8;
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    ui2 = FlashReadByte(sector_id|ui1);
    ui3 = rand8();
    assert(ui2 == ui3);
  }
}

int
main(void)
{
  lfsr8 = time(NULL);
}

#ifndef FLASH_H
#define FLASH_H

void    FlashWriteByte   (uint16_t address, uint8_t value);
uint8_t FlashReadByte    (uint16_t address);
void    FlashEraseSector (uint16_t address);

#define PROGRAM_CODE_START 0xF000
#define FLASH_SECTOR_SIZE  128
#define FLASH_DATA_START   FLASH_SECTOR_SIZE
#define FLASH_DATA_END     PROGRAM_CODE_START

#endif

#ifndef FLASH_H
#define FLASH_H

#if nostore_IN_FLASH
#error "No data stored in Flash, not required of this module"
#endif

void    FlashInit();
void    FlashWriteByte   (uint16_t address, uint8_t value);
uint8_t FlashReadByte    (uint16_t address);
void    FlashEraseSector (uint16_t address);

#define PROGRAM_CODE_START   0xF000
#define FLASH_SECTOR_SIZE    128
#define FLASH_NUM_SECTORS    ((1<<16)/FLASH_SECTOR_SIZE) /* would be 64 * 4 = 256 */

/* Flash Block 0 is used as follows
   sector 0       : Code
   sector 1-41    : 10K for item
   sector 42-223  : Sale data base (45K)
   sector 224-255 : Code (8K Code could be reduced)
 */
#define FLASH_AVAIL_START        FLASH_SECTOR_SIZE
#define FLASH_AVAIL_END          (FLASH_ITEM_START+((1<<10)*10))
#define FLASH_ADDR_INVALID       0xFFFF
#define FLASH_RESET_DATA_VALUE   0x0

#endif

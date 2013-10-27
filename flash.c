#include <stdint.h>
#include <p89v51rd2.h>

#include "flash.h"

void
FlashInit()
{
  flash_item_renew_sector();
}

/* Write a byte in to the flash using the IAP routines of the p89v51rd2. */
void
FlashWriteByte (uint16_t address, uint8_t value)
{
  assert (address < PROGRAM_CODE_START);

  FCF = 0;
  _asm
    mov R1, #0x02
    ;DPH contains the high part of the sector
    ;DPL contains the low part of the sector
    mov	a,_FlashByte_PARM_2
    lcall 0x1ff0
  _endasm;
  FCF = 1;
}

/* Write a byte in to the flash using the IAP routines of the p89v51rd2. */
uint8_t
FlashReadByte (uint16_t address)
{
  uint8_t value;

  assert (address < PROGRAM_CODE_START);

  FCF = 0;
  _asm
    mov R1, #0x03
    ;DPH contains the high part of the sector
    ;DPL contains the low part of the sector
    lcall 0x1ff0
  _endasm;
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
  _asm
    mov R1, #0x08
    ;DPH contains the high part of the sector
    ;DPL contains the low part of the sector
    lcall 0x1ff0 ;Entry point
  _endasm;
  FCF = 1;
}


/* Item type storage : Each item is packed in 20 byte
continous-running-storage.
*/
uint16_t
flash_item_add(uint8_t* byte_arr)
{
  uint16_t item_last_modified = 0xFFFF, ui1;
  uint8_t  ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));

  for (ui1=(uint16_t)(item_last_modified+ITEM_SIZEOF); ui1<FLASH_ITEM_END; ui1+=ITEM_SIZEOF) {
    ui2 = FlashReadByte(ui1);
    if ((0 != (ui2 & ITEM_BYTE_VALID_MASK)) && (0 == (ui2 & ITEM_BYTE_DELETE_MASK))) {
      item_last_modified = ui1;
      break;
    }
  }

  if (0xFFFF == item_last_modified) {
    for (ui1=(uint16_t)(FLASH_ITEM_START); ui1<item_last_modified; ui1+=ITEM_SIZEOF) {
      ui2 = FlashReadByte(ui1);
      if ((0 != (ui2 & ITEM_BYTE_VALID_MASK)) && (0 == (ui2 & ITEM_BYTE_DELETE_MASK))) {
	item_last_modified = ui1;
	break;
      }
    }
  }

  if (0xFFFF == item_last_modified) {
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));

    for (ui2=0; ui2<ITEM_SIZEOF; ui2++) {
      FlashWriteByte(item_last_modified+ui2, byte_arr[0]);
      byte_arr++;
    }
  }

  return item_last_modified;
}

uint16_t
flash_item_find(uint16_t id)
{
  uint16_t l_id = 0, ui1;

  for (ui1=(uint16_t)FLASH_ITEM_START; ui1<FLASH_ITEM_END; ui1+=ITEM_SIZEOF) {
    ui2 = FlashReadByte(ui1);
    if ((0 != (ui2 & ITEM_BYTE_VALID_MASK)) && (0 == (ui2 & ITEM_BYTE_DELETE_MASK))) {
      l_id = ui2 & ITEM_BYTE_ID_H_MASK;
      l_id <<= ITEM_BYTE_ID_H_ARRANGE_SHIFT_NUM;
      l_id |= FlashReadByte(ui1+ITEM_BYTE_ID_BYTE_OFFSET);
      if (l_id == id) {
	return ui1;
      }
    }
  }

  return 0xFFFF;
}

void
flash_item_delete(uint16_t addr)
{
  uint8_t  ui2;

  /* store mod history */

  /* just mark for del */
  ui2 = FlashReadByte(addr);
  FlashWriteByte(addr, ui2 | ITEM_BYTE_DELETE_MASK);
}

/* Renew a sector by removing the deleted items
   1. Find a sector that is having deleted items
   2. Take local copy and remove it
   3. Store local copy back to flash
 */
uint8_t bufSS[FLASH_SECTOR_SIZE];
void
flash_item_renew_sector()
{
  uint16_t sector_scan_p, sector_write_p;
  uint8_t  bufSS_idx, ui2, ui3;

  /* Init */
  sector_scan_p = sector_write_p = FLASH_ITEM_START;
  for (bufSS_idx=0; bufSS_idx<FLASH_SECTOR_SIZE; bufSS_idx++)
    bufSS[bufSS_idx] = 0;
  bufSS_idx = 0;

  /* Scan through and restore */
  for (; sector_scan_p<FLASH_ITEM_END; sector_scan_p+=ITEM_SIZEOF) {
    ui2 = FlashReadByte(sector_scan_p);
    if ((0 != (ui2 & ITEM_BYTE_VALID_MASK)) && (0 == (ui2 & ITEM_BYTE_DELETE_MASK))) {
      for (ui3=0; ui3<ITEM_SIZEOF; ui3++) {
	ui2 = FlashReadByte(sector_scan_p);
	bufSS[bufSS_idx] = ui2;
	bufSS_idx++;
	if (bufSS_idx >= FLASH_SECTOR_SIZE) {
	  FlashEraseSector(sector_write_p);
	  for (bufSS_idx=0; bufSS_idx<FLASH_SECTOR_SIZE; bufSS_idx++) {
	    FlashWriteByte(sector_write_p | bufSS_idx, bufSS[bufSS_idx]);
	    bufSS[bufSS_idx] = 0;
	  }
	  sector_write_p += FLASH_SECTOR_SIZE;
	  bufSS_idx = 0;
	}
      }
    }
  }

  /* End : Save overflow bytes and clear all remaining sectors */
  if (0 != bufSS_idx) {
    FlashEraseSector(sector_write_p);
    for (ui2=0; ui2<bufSS_idx; bufSS_idx++) {
      FlashWriteByte(sector_write_p | ui2, bufSS[bufSS_idx]);
    }
  }
  for (; sector_write_p<FLASH_ITEM_END; sector_write_p += FLASH_SECTOR_SIZE) {
    FlashEraseSector(sector_write_p);
  }
}

void
flash_sale_add(uint8_t *sale)
{
  uint16_t sale_start, sale_end;
  uint16_t sale_start_sector, sale_end_sector, sale_newend_sector;
  uint8_t  nbytes, ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
  sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  sale_end_sector   = sale_end & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  assert(sale_start_sector != sale_end_sector);

  /* compute if there is overflow sector
     Implemented as a circular buffer. Possible combinations are
     start ->         | end   ->
     end   ->         | start ->

     start : points to start of valid data
     end   : points to start of free  space
   */
  nbytes = SALE_INFO_SIZEOF + (((sale[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
  sale_newend_sector = (sale_end+nbytes-1) & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  if (sale_newend_sector >= FLASH_DATA_END) {
    sale_newend_sector = FLASH_DATA_START;
  }
  if (sale_end_sector != sale_newend_sector) {
    /* check if the new sector is free, else make free */
    assert ( (sale_newend_sector <= sale_start_sector) || (sale_newend_sector > sale_end_sector) );
    assert ( (sale_newend_sector >= FLASH_DATA_START)  && (sale_newend_sector < FLASH_DATA_END) );

    if (sale_newend_sector == sale_start_sector) { /* free up */
      for (; sale_start_sector==sale_newend_sector; ) {
	ui2 = FlashReadByte(sale_start);
	sale_start += SALE_INFO_SIZEOF + (((ui2 & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
	sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
      }
      FlashEraseSector(sale_newend_sector);
      EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
    }
  }

  /* Bytes are available ..., just store */
  for (ui2=0; ui2<nbytes; ui2++) {
    FlashWriteByte(sale_end, sale[ui2]);
    sale_end++;
    if (sale_end >= FLASH_DATA_END) {
      sale_end = FLASH_DATA_START;
    }
  }
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
}

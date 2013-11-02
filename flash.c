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

uint8_t bufSS[FLASH_SECTOR_SIZE];

/* Item type storage : Each item is packed in 20 byte
   continous-running-storage.
   return : id 
 */
void
flash_item_add(uint8_t* byte_arr)
{
  uint16_t item_last_modified, ui1;
  uint8_t  ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[item_last_modified/8]), (uint8_t *)&ui2, sizeof(uint8_t));
  for (ui1=item_last_modified+1; ui1!=item_last_modified; ) {
    if (0 == (ui1%8))
      EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[ui1/8]), (uint8_t *)&ui2, sizeof(uint8_t));
    if ( 0 == (ui2 & (1<<(ui1&0x7))) ) {
      break;
    }
    /* incr */
    ui1++;
    if (ui1 >= ITEM_MAX)  ui1 = 0;
  }
  if ( 0 != (ui2 & (1<<(ui1&0x7))) ) {
    assert(0); /* they why? item_count < ITEM_MAX */
    return -1;
  }
  item_last_modified = ui1;

  /* store item */
  ui2 |= 1 << (item_last_modified&0x7);
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[item_last_modified/8]), (uint8_t *)&item_last_modified, sizeof(uint8_t));
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));
  ui1 = ((uint16_t)FLASH_ITEM_START) + (ui1*ITEM_SIZEOF);
  for (ui2=0; ui2<ITEM_SIZEOF; ui2++, ui1++) {
    FlashWriteByte(ui1, byte_arr[0]);
    byte_arr++;
  }

  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
  ui1++;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
}

void
flash_item_delete(uint16_t id)
{
  uint16_t ui1, addr;
  uint8_t item_valid;
  uint8_t *block, *block_end;

  /* do nothing if already invalid */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[id/8]), (uint8_t *)&item_valid, sizeof(uint8_t));
  if (0 == (item_valid & (1<<(addr%0x7))) ) {
    return;
  }
  item_valid &= ~(1 << (addr%0x7));

  /* input is id, compute addr */
  addr = FLASH_ITEM_START + (id * ITEM_SIZEOF);
  block = (uint8_t *) (addr & ~((uint16_t)(FLASH_SECTOR_SIZE-1)));
  block_end = (uint8_t *) ((addr+ITEM_SIZEOF-1) & ~((uint16_t)(FLASH_SECTOR_SIZE-1)));

  /* save start block */
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    if ( ((block+ui1) < addr) || ((block+ui1) > (addr+ITEM_SIZEOF)) )
      bufSS[ui1] = block[ui1];
    else
      bufSS[ui1] = 0;
  }
  FlashEraseSector(block);
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    if (0 != bufSS[ui1])
      FlashWriteByte(block+ui1, bufSS[ui1]);
  }

  /* end block */
  if (block != block_end) {
    for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
      if ( (block_end+ui1) > (addr+ITEM_SIZEOF) )
	bufSS[ui1] = block_end[ui1];
      else
	bufSS[ui1] = 0;
    }
    FlashEraseSector(block_end);
    for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
      if (0 != bufSS[ui1])
	FlashWriteByte(block_end+ui1, bufSS[ui1]);
    }
  }

  /* upate valid */
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_valid[id/8]), (uint8_t *)&item_valid, sizeof(uint8_t));
  uint16_t ui1;
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
  ui1--;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
}

/* Sales are easily accessable from table */
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

  /* compute if there is overflow sector
     Implemented as a circular buffer. Possible combinations are
     start ->         | end   ->
     end   ->         | start ->

     start : pointer to start of valid data
     end   : pointer to start of free  space
   */
  nbytes = SALE_INFO_SIZEOF + (((sale[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * (uint16_t)SALE_SIZEOF);
  sale_newend_sector = (sale_end+nbytes-1) & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  if (sale_newend_sector >= FLASH_DATA_END) {
    sale_newend_sector = FLASH_DATA_START;
  }
  assert ( (sale_newend_sector <= sale_start_sector) || (sale_newend_sector >= sale_end_sector) );
  assert ( (sale_newend_sector >= FLASH_DATA_START)  && (sale_newend_sector < FLASH_DATA_END) );

  /* when new sector is taken, erase it */
  if (sale_end_sector != sale_newend_sector) {
    /* If new sector is already occupied */
    for (; sale_start_sector==sale_newend_sector; ) {
      ui2 = FlashReadByte(sale_start);
      sale_start += SALE_INFO_SIZEOF + (((ui2 & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
      sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
    }
    FlashEraseSector(sale_newend_sector);
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  }

  /* Bytes are available ..., just store */
  sale_index_it(sale, sale_end);
  for (ui2=0; ui2<nbytes; ui2++) {
    FlashWriteByte(sale_end, sale[0]);
    sale_end++, sale++;
    if (sale_end >= FLASH_DATA_END) {
      sale_end = FLASH_DATA_START;
    }
  }
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
}

void
flash_sale_delete_month(uint8_t del_month)
{
  uint16_t sale_start, sale_end;
  uint16_t sale_start_sector, sale_end_sector, sale_newend_sector;
  uint8_t  nbytes, ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
  sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  sale_end_sector   = sale_end & ~((uint16_t) (FLASH_SECTOR_SIZE-1));

  /* Move forward : Not required to reflash the new sector */
  {
    uint8_t ui3[3];
    sale_info *si;
    do {
      si = (void *)ui3;
      ui3[0] = FlashReadByte(sale_start);
      ui3[1] = FlashReadByte(sale_start+1);
      ui3[2] = FlashReadByte(sale_start+2);
      if (del_month == si->date_mm) {
	sale_start += SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
	sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
      }
    } while(del_month == si->date_mm);
  }
}

uint8_t *
flash_sale_find(uint8_t *dmy, uint16_t id)
{
  sale_info  *si, *vptr;
  uint16_t ui1, ui2;
  uint8_t idx;

  idx = (dmy[1] - 1) * 4;
  if (date <= 7) {}
  else if (date <= 14) idx ++;
  else if (date <= 21) idx += 2;
  else idx += 3;

  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_ptr[idx]), (uint8_t *)&si, sizeof(uint16_t));
  if (0 == si) return (void *)si;

  for (ui1=0, vptr=si; ui1<id; ui1++, vptr=si) {
    uint8_t ui3[3];
    sale_info* si_t = ui3;
    ui3[0] = FlashReadByte(si);
    ui3[1] = FlashReadByte(si+1);
    ui3[2] = FlashReadByte(si+2);
    if ( (dmy[0] == si_t->date_dd) && (dmy[1] == si_t->date_mm) ) {
      si = (sale_info *)( ((uint8_t *)si) + SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF) );
    } else { /* less # items for this day */
      return (void *) 0;
    }
  }

  return vptr;
}

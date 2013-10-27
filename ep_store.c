#include <stdint.h>
#include <p89v51rd2.h>

#include "ep_store.h"
#include "crc.h"

void
ep_store_init(void)
{
  uint16_t eeprom_crc, ui1;
  uint8_t  ui2;
  uint8_t eeprom_idx;

  /* check CRC and init everything if screwed */
  CRC16_Init();
  for (ui1=0; ui1<(sizeof(struct ep_store_layout)-2); ui1++) {
    EEPROM_STORE_READ(ui1, &ui2, sizeof(uint8_t));
    CRC16_Update(ui2);
  }
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.eeprom_idx), (uint8_t *)&eeprom_idx, sizeof(uint8_t));
  eeprom_crc = CRC16_High; eeprom_crc <<= 8; eeprom_crc |= CRC16_Low;
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.eeprom_sig[eeprom_idx]), (uint8_t *)&ui1, sizeof(uint16_t));
  if (eeprom_crc == ui1) {
    /* we are good, modify params and commit */
    eeprom_idx++;
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.eeprom_idx), (uint8_t *)&eeprom_idx, sizeof(uint8_t));
    CRC16_Init();
    for (ui1=0; ui1<((uint16_t)&(EEPROM_DATA.eeprom_sig[0])); ui1++) {
      EEPROM_STORE_READ(ui1, &ui2, sizeof(uint8_t));
      CRC16_Update(ui2);
    }
    eeprom_crc = CRC16_High; eeprom_crc <<= 8; eeprom_crc |= CRC16_Low;
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.eeprom_sig[eeprom_idx]), (uint8_t *)&eeprom_crc, sizeof(uint16_t));
    return;
  }

  /* Warn of data loss */
  LCD_CLRSCR;
  LCD_WR_LINE(0, 0, "Data Fail, Loss");
  LCD_WR_LINE(1, 0, "Serv if Repeat");

  /* clear all bytes of flash*/
  ui2 = 0;
  for (ui1=0; ui1<(sizeof(struct ep_store_layout)-2); ui1++) {
    EEPROM_STORE_WRITE(ui1, (uint8_t *)&ui2, sizeof(uint8_t));
  }

  /* */
  ui1 = FLASH_ITEM_START;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&ui1, sizeof(uint16_t));
  ui1 = FLASH_DATA_START;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&ui1, sizeof(uint16_t));
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&ui1, sizeof(uint16_t));

  /* Recompute hash */
  CRC16_Init();
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.eeprom_idx), (uint8_t *)&eeprom_idx, sizeof(uint8_t));
  for (ui1=0; ui1<((uint16_t)&(EEPROM_DATA.eeprom_sig[0])); ui1++) {
    EEPROM_STORE_READ(ui1, &ui2, sizeof(uint8_t));
    CRC16_Update(ui2);
  }
  eeprom_crc = CRC16_High; eeprom_crc <<= 8; eeprom_crc |= CRC16_Low;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.eeprom_sig[eeprom_idx]), (uint8_t *)&eeprom_crc, sizeof(uint16_t));

  /* */
  LCD_CLRSCR;
}

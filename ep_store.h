#ifndef EP_STORE_H
#define EP_STORE_H

/* Available bytes : 8K : 8192 */
struct ep_store_layout {
  item      item_mod_his[35];       /* 20*35  =  700 */

  /* sale constants */
  uint16_t  sale_start[2];          /*             4 */
  uint16_t  sale_end[2];            /*             4 */
  uint16_t  vat[4];                 /*             8 */
  uint16_t  service_tax;            /*             2 */

  /* item constants */
  uint16_t  item_last_modified;     /*             2 */
  uint16_t  item_count;             /*             2 */

  uint8_t   shop_name[12];          /*            12 */
  uint8_t   prn_header[64];         /*            64 */
  uint8_t   prn_footer[24];         /*            24 */
  uint16_t  eeprom_sig;             /*             2 */
};                                  /* Total  =  824 */

#define EEPROM_DATA       (*((struct ep_store_layout *)0))
#define EEPROM_STORE_READ  i2cReadBytes
#define EEPROM_STORE_WRITE(A, B, C) \
  i2cWriteBytes(A, B, C);	    \
  { \
  uint16_t ui1;	uint8_t ui2;					\
  CRC16_Init();							\
  for (ui1=0; ui1<(sizeof(struct ep_store_layout)-2); ui1++) {	\
  EEPROM_STORE_READ(ui1, &ui2, sizeof(uint8_t));		\
  CRC16_Update(ui2);						\
  }								\
  ui1 = CRC16_High; ui1 <<= 8; ui1 |= CRC16_Low;		\
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.eeprom_sig), (uint8_t *)&ui1, sizeof(uint16_t)); \
  }

#endif

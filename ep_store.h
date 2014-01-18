#ifndef EP_STORE_H
#define EP_STORE_H

#define EEPROM_DYNARR_MAX    8
#define HEADER_MAX_SZ       54
#define SHOP_NAME_SZ        16
#define FOOTER_MAX_SZ       24
#define MOD_HIS_SZ          15
#define NUM_TESTING_BYTES    3

/* Available : 8K : 8192 */
struct ep_store_layout {
  uint8_t   item_mod_his_ptr;       /*             1 */
  item      item_mod_his[MOD_HIS_SZ]; /* 20*12=  240 */

  /* sale constants */
  uint16_t  sale_start;             /*             2 */
  uint16_t  sale_end;               /*             2 */
  uint16_t  vat[4];                 /*             8 */
  uint16_t  service_tax;            /*             2 */

  /* item constants
     max items = 64*8 = 512
   */
  uint16_t  item_last_modified;     /*             2 */
  uint16_t  item_count;             /*             2 */ /* 319 */

  /* */
  uint16_t  sale_date_ptr[12*4];    /*            96 */
  uint16_t  sale_date_old_ptr[1*4]; /*             8 */
  uint8_t   sale_date_old_ptr_month;/*             1 */ /* 424 */

  /* User choices */
  uint8_t   print_it;               /*             1 */

  /* Have count of # of bill printed/ day */
  uint16_t  date_month;             /*             2 */
  uint16_t  bill_id[EEPROM_DYNARR_MAX];  /*       16 */ /* 443 */

  /* banners */
  uint8_t   shop_name [SHOP_NAME_SZ];  /*         16 */
  uint8_t   prn_header[HEADER_MAX_SZ]; /*         54 */
  uint8_t   prn_footer[FOOTER_MAX_SZ]; /*         24 */ /* 537 */

  /* */
  uint16_t   passwd;                /*             2 */

  /* integrity : 'corrupted' should be left at 0 and
     written with random value to invalidate the sig
   */
  uint8_t   corrupted;              /*             1 */
  uint8_t   eeprom_idx;             /*             1 */
  uint16_t  eeprom_sig[EEPROM_DYNARR_MAX]; /*     16 */

  /* used for testing purposes */
  uint8_t   testing[NUM_TESTING_BYTES]; /*         3 */ /* 560 */
};                                  /* Total  =  500 */

#define EEPROM_DATA               (*((struct ep_store_layout *)0))
#define EEPROM_STORE_READ         eepromReadBytes
#define EEPROM_STORE_WRITE_NoSig  eepromWriteBytes
#define EEPROM_STORE_WRITE(A, B, C) { 					\
    eepromWriteBytes(A, B, C);						\
    EEPROM_STORE_WRITE_Sig						\
  }

#define EEPROM_STORE_WRITE_Sig {					\
    uint16_t ui1;	uint8_t ui2, eeprom_idx;			\
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.eeprom_idx), (uint8_t *)&eeprom_idx, sizeof(uint8_t)); \
    CRC16_Init();							\
    for (ui1=0; ui1<(uint16_t)&(EEPROM_DATA.eeprom_sig[0]); ui1++) {	\
      EEPROM_STORE_READ(ui1, &ui2, sizeof(uint8_t));			\
      CRC16_Update(ui2);						\
    }									\
    ui1 = CRC16_High; ui1 <<= 8; ui1 |= CRC16_Low;			\
    EEPROM_STORE_WRITE_NoSig((uint16_t)&(EEPROM_DATA.eeprom_sig[eeprom_idx]), (uint8_t *)&ui1, sizeof(uint16_t)); \
}

void ep_store_init(void);
void sale_index_it(sale_info *si, uint16_t ptr);

#endif

#ifndef EP_STORE_H
#define EP_STORE_H

/* Available bytes : 8K : 8192 */
struct ep_store_layout {
  item      items[350];             /* 20*350 = 7000 */
  uint8_t   boot0;                  /*             1 */ /* : will be 0 on first program */
  uint16_t  sale_start;             /*             2 */
  uint16_t  sale_end;               /*             2 */
  uint16_t  vat[4];                 /*             8 */
  uint16_t  service_tax;            /*             2 */
  uint8_t   item_mod_top;           /*             1 */
  item      item_mod_his[35];       /* 20*35  =  700 */
  uint16_t  flash_sector_start;     /*             2 */
  uint16_t  flash_sector_end;       /*             2 */
  uint16_t  n_day_sales;            /*             1 */
  uint8_t   shop_name[12];          /*            12 */
  uint8_t   prn_header[64];         /*            64 */
  uint8_t   prn_footer[24];         /*            24 */
  uint16_t  eeprom_sig;             /*             2 */
  uint16_t  flash_sig;              /*             2 */
} ep_layout;                        /* Total  = 7825 */

#endif

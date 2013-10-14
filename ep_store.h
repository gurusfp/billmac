#ifndef EP_STORE_H
#define EP_STORE_H

struct ep_store_layout {
  item      items[250];             /* 20*250 = 5000 */
  uint16_t  sale_start;             /*             2 */
  uint16_t  sale_end;               /*             2 */
  uint16_t  vat[4];                 /*             8 */
  uint16_t  service_tax;            /*             2 */
  uint8_t   item_mod_top;           /*             1 */
  item      item_mod_his[25];       /* 20*25  =   50 */
  sale_info day_sales;
} ep_layout;                        /* Total  = 5065 */

typedef union {
  uint8_t bytes[1<<13]; /* 8K bytes */
  ep_store_layout ep_layout;
} ep_store;

#endif

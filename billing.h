#ifndef BILLING_H
#define BILLING_H

/* Balanced to 20 bytes */
typedef struct {
  uint16_t  id_h:1;
  uint16_t  unused:2;
  uint16_t  cost:13;
  uint8_t   id;
  uint16_t  has_serv_tax:1;
  uint16_t  vat_sel:2;
  uint16_t  discount:13;
  uint8_t   name[15];
} item;
#define  ITEM_BYTE_VALID_MASK   (1<<6)
#define  ITEM_BYTE_DELETE_MASK  (1<<5)
#define  ITEM_BYTE_ID_H_MASK    (1<<7)
#define  ITEM_BYTE_ID_H_ARRANGE_SHIFT_NUM 1
#define  ITEM_BYTE_ID_BYTE_OFFSET         2

/* 2 bytes */
typedef struct {
  uint16_t   item_id:9;
  uint16_t   unused:3;
  uint16_t   num_sale:4;
} sale_item;

/* 3 bytes */
#define  SALE_INFO_ITEMS_NBITS     4
#define  SALE_INFO_DELETED      0x80
#define  SALE_INFO_MODIFIED     0x40
typedef struct {
  uint16_t  n_items:SALE_INFO_ITEMS_NBITS;

  uint16_t  time_hh:5;
  uint16_t  time_mm:6;

  uint16_t  date_dd:5;
  uint16_t  date_mm:4;
  uint8_t   deleted;
} sale_info;
#define SALE_INFO (*((sale_info *)0))
#define SALE_INFO_BYTE_NITEM_MASK   0xF0
#define SALE_INFO_BYTE_NITEM_SHIFT  4

/* */
#define MAX_ITEMS_IN_BILL  (1<<SALE_INFO_ITEMS_NBITS)
typedef struct {
  sale_info info;
  sale_item items[MAX_ITEMS_IN_BILL];
  uint16_t addrs[MAX_ITEMS_IN_BILL];
} billing;

/* constants */
#define ITEM_SIZEOF       sizeof(item)
#define ITEM_MAX          ((FLASH_ITEM_END-FLASH_ITEM_START)/sizeof(item))
#define SALE_INFO_SIZEOF  sizeof(sale_info)
#define SALE_SIZEOF       sizeof(sale_item)

#endif

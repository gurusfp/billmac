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
  uint8_t   item_id;
  uint8_t   item_id_h:1;
  uint8_t   unused:3;
  uint8_t   num_sale:4;
} sale_item;

/* 3 bytes */
typedef struct {
  uint16_t  n_items:4;

  uint16_t  time_hh:5;
  uint16_t  time_mm:6;

  uint16_t  date_dd:5;
  uint16_t  date_mm:4;
} sale_info;
#define SALE_INFO_BYTE_NITEM_MASK   0xF0
#define SALE_INFO_BYTE_NITEM_SHIFT  4

/* */
typedef struct {
  sale_info info;
  sale_item items[16];
  uint16_t addrs[16];
} billing;

/* checks */
#if 2 != sizeof(sale_item)
#error "sale item not packed into 2 bytes"
#endif
#if 20 != sizeof(item)
#error "item not packed in 20 bytes"
#endif
#if 3 != sizeof(sale_info)
#error "item not packed in 20 bytes"
#endif
#if 0 != ((FLASH_ITEM_END-FLASH_ITEM_START)%sizeof(item))
#error "item type wrongly packed"
#endif

/* constants */
#define ITEM_SIZEOF       sizeof(item)
#define ITEM_MAX          ((FLASH_ITEM_END-FLASH_ITEM_START)/sizeof(item))
#define SALE_INFO_SIZEOF  sizeof(sale_info)
#define SALE_SIZEOF       sizeof(sale_item)

#endif

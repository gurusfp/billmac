#ifndef BILLING_H
#define BILLING_H

typedef struct {
  uint8_t   name[12];
  uint16_t  is_valid:1;
  uint16_t  is_deleted:1;
  uint16_t  na0:1;
  uint16_t  cost:13;
  uint16_t  has_serv_tax:1;
  uint16_t  vat_sel:2;
  uint16_t  discount:13;
  uint8_t   quantity[4];
} item;

typedef sturct {
  uint8_t   item;
  uint8_t   cost_modified:1;
  uint8_t   tax_modified:1;
  uint8_t   other_modification:1;
  uint8_t   num:5;
} sale_item;

typedef struct {
  uint16_t  n_items:4;
  uint16_t  id:12;

  uint16_t  na0:3;
  uint16_t  printed:1;
  uint16_t  sale_time:12;

  uint16_t  date;

  sale_item items[16]; /* max item in 1 bill */
} sale_info;

/* checks */
#if 2 != sizeof(sale_item)
#error "sale item not packed into 2 bytes"
#endif
#if 20 != sizeof(item)
#error "item not packed in 20 bytes"
#endif

#endif

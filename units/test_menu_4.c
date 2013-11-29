#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

#include "billing.h"

#include "assert.h"
#include "crc.h"
#include "lcd.h"
#include "kbd.h"
#include "i2c.h"
#include "flash.h"
#include "ep_store.h"
#include "menu.h"

#include "assert.c"
#include "msg.c"
#include "crc.c"
#include "lcd.c"
#include "kbd.c"
#include "i2c.c"
#include "flash.c"
#include "ep_store.c"
#include "menu.c"

uint8_t inp[NUM_TEST_KEY_ARR][FLASH_SECTOR_SIZE];

/* Convert a int to string */
void
int2str(char *str, uint32_t ui, uint8_t *idx)
{
  if (ui/10) {
    int2str(str, ui, idx);
  } else {
    str[*idx] = '0' + ui;
    (*idx)++;
  }
  idx[0] = 0;
}

void
add_item(item *ri1)
{
  uint32_t ui1, ui3;
  uint8_t ui2;

  /* Construct a random item */
  ui3 = (rand() % (ITEM_NAME_BITL-1)) + 1;
  for (ui1=0; ui1<ui3; ui1++) {
    inp[0][ui1] = 'a' + (rand()%26);
    ri1->name[ui1] = inp[0][ui1];
  }
  inp[0][ui1] = 0;
  ri1->name[ui1] = inp[0][ui1];
  INIT_TEST_KEYS(inp[0]);

  ri1->cost = rand() % ri1->cost();
  ri1->discount = rand() % ri1->cost();
  ui2 = 0; /* cost, discount */
  int2str(inp[1], ri1->cost, &ui2);
  inp[1][ui1] = 0;
  INIT_TEST_KEYS(inp[1]);
  ui2 = 0;
  int2str(inp[2], ri1->discount, &ui2);
  inp[2][ui1] = 0;
  INIT_TEST_KEYS(inp[2]);

  ri1->vat = rand();
  ui2 = 0; /* vat */
  int2str(inp[3], ri1->vat, &ui2);
  inp[3][ui1] = 0;
  INIT_TEST_KEYS(inp[3]);

  ri1->has_serv_tax = rand();
  ui2 = 0; /* vat */
  int2str(inp[4], ri1->vat, &ui2);
  inp[4][ui1] = 0;
  INIT_TEST_KEYS(inp[4]);

  menu_AddItem(MENU_MSUPER);
}

void
compare_item(item *ri, uint16_t flash_item)
{
  item rif;
  char *bufTT = (void *) &(rif);
  for (ui1=0; ui1<ITEM_SIZEOF; ui1++)
    bufTT[ui1] = FlashReadByte(flash_item+ui1);

  if (0 != strncmp(ri->name, rif->name)) {
    fprintf(stderr, "ERROR: Name mismatch Exp:'%s' Obt:'%s'\n", ri->name, rif.name);
  }
  if (ri->cost != rif->cost) {
    fprintf(stderr, "ERROR: cost mismatch Exp:'%s' Obt:'%s'\n", ri->cost, rif.cost);
  }
  if (ri->discount != rif->discount) {
    fprintf(stderr, "ERROR: discount mismatch Exp:'%s' Obt:'%s'\n", ri->discount, rif.discount);
  }
  if (ri->vat_sel != rif->vat_sel) {
    fprintf(stderr, "ERROR: vat_sel mismatch Exp:'%s' Obt:'%s'\n", ri->vat_sel, rif.vat_sel);
  }
  if (ri->has_serv_tax != rif->has_serv_tax) {
    fprintf(stderr, "ERROR: cost mismatch Exp:'%s' Obt:'%s'\n", ri->has_serv_tax, rif.has_serv_tax);
  }
}

void
delete_item(item *ri, uint16_t id)
{
  uint16_t ui1;
  uint8_t  ui2;

  ui2 = 0;
  int2str(id, &ui2);
  INIT_TEST_KEYS(inp[4]);
  menu_DelItem(MENU_MSUPER);

  char *bufTT = (void *)ri;
  for (ui1=0; ui1<ITEM_SIZEOF; ui1++) {
    bufTT[ui1] = 0;
  }
}

int
main(void)
{
  uint16_t ui1, ui2, ui3;
  item all_items[ITEM_MAX];

  srand(time(NULL));

  /* */
  assert_init();
  menu_Init();
  i2cInit();
  ep_store_init();

  KBD_RESET_KEY;

  /* Test to check the number of items that could be stored */
  for (ui1=0; ui1<1; ui1++) {
    add_item(all_items+ui1);
  }
  {
    item ri;
    expect_assert("Items Exceeded");
    add_item(&ri);
  }

  /* Check all items as per expectations */
  for (ui1=0; ui1<1; ui1++) {
    compare_item(all_items+ui1, flash_item_find(ui1));
  }

  /* Now delete an item and check if the item was replaced */
  ui1 = rand() % ITEM_MAX;
  delete_item(all_items+ui1, ui1);
  for (ui1=0; ui1<1; ui1++) {
    compare_item(all_items+ui1, flash_item_find(ui1));
  }

  /* Now add item and check for validness*/
  add_item(all_items+ui1);
  for (ui1=0; ui1<1; ui1++) {
    compare_item(all_items+ui1, flash_item_find(ui1));
  }
}

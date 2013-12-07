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
#include "printer.h"
#include "menu.h"

/* Overrides for tests */
#undef  PRINTER_PRINT
#define PRINTER_PRINT(c) \
  do {			 \
    assert(isprint(c));	 \
    uart_putc(c);	 \
  } while (0)

#undef  PRINTER_COMMAND
#define PRINTER_COMMAND(C)

#undef  PRINTER_CMD_ARG
#define PRINTER_CMD_ARG(N)
/* End of overrides */

#include "assert.c"
#include "msg.c"
#include "crc.c"
#include "lcd.c"
#include "kbd.c"
#include "i2c.c"
#include "flash.c"
#include "ep_store.c"
#include "printer.c"
#include "menu.c"


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

  uint8_t ymd[3];
  for (ui1=0; ui1<1; ui1++) {
    ymd[0] = (rand()%32)+1;
    ymd[1] = (rand()%12)+1;
    printer_prn_date(ymd);
  }

  for (ui1=0; ui1<1; ui1++) {
    ui2 = rand();
    printer_prn_uint16(ui2);
  }

  char s[FLASH_SECTOR_SIZE];
  for (ui1=0; ui1<1; ui1++) {
    ui2 = rand()%FLASH_SECTOR_SIZE;
    for (ui1=0; ui1<ui2; ui1++) {
      if (0 == (rand() % 3))
	inp[ui1] = 'A' + (rand()%26);
      else
	inp[ui1] = 'a' + (rand()%26);
    }
    printer_prn_str(s);
  }

  billing b1 = {
    /* sale_info info; */
    { 16, 23, 59, 31, 12, 0 },
    /* sale_item items[MAX_ITEMS_IN_BILL]; */
    [ {X, 0, 2}, {X, 0, 2}, {X, 0, 2}, {X, 0, 2}, 
      {X, 0, 2}, {X, 0, 2}, {X, 0, 2}, {X, 0, 2},
      {X, 0, 2}, {X, 0, 2}, {X, 0, 2}, {X, 0, 2},
      {X, 0, 2}, {X, 0, 2}, {X, 0, 2}, {X, 0, 2} ],
    /* bill_info bi[MAX_ITEMS_IN_BILL]; */
    [ {0, 1}, {0, 1}, {0, 1}, {0, 1},
      {0, 1}, {0, 1}, {0, 1}, {0, 1},
      {0, 1}, {0, 1}, {0, 1}, {0, 1},
      {0, 1}, {0, 1}, {0, 1}, {0, 1} ],
    /* item temp; */
    { 0, 1234, XXXX, 1, 2, 235, "First bill item" },
  };

  return 0;
}

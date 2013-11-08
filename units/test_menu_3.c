#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

#include "billing.h"

#define ERROR(msg) fprintf(stderr, msg)

#include "crc.h"
#include "lcd.h"
#include "kbd.h"
#include "i2c.h"
#include "flash.h"
#include "ep_store.h"
#include "menu.h"

#include "crc.c"
#include "lcd.c"
#include "kbd.c"
#include "i2c.c"
#include "flash.c"
#include "ep_store.c"
#include "menu.c"

uint8_t inp[FLASH_SECTOR_SIZE];

int
main(void)
{
  uint32_t loop;
  uint8_t ui1;

  srand(time(NULL));

  /* */
  menu_Init();
  i2cInit();
  ep_store_init();

  /* Passwd */
  for (loop=0; loop<1; loop++) {
    for (ui1=0; ui1<LCD_MAX_COL/2; ui1++) {
      if (0 == (rand() % 3))
	inp[ui1] = 'A' + (rand()%26);
      else
	inp[ui1] = 'a' + (rand()%26);
    }
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("Prompt 1", &arg1, MENU_ITEM_STR);
 
    MenuMode = MENU_MNORMAL;
    menu_SetPasswd(MENU_MRESET);
    assert(MENU_MNORMAL == MenuMode);

    /* sometimes corrupt the password */
    uint8_t corrupted = rand() % 2;
    printf("Before corruption inp:'%s'\n", inp);
    if (corrupted) {
      inp[rand()%LCD_MAX_COL]++;
    }
    printf("After corruption inp:'%s'\n", inp);

    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("Prompt 1", &arg1, MENU_ITEM_STR);
    menu_SetPasswd(MENU_MNORMAL|MENU_MVALIDATE);

    printf("Corrupted:%d\n", (uint32_t) corrupted);
    if (corrupted) {
      assert(MENU_MNORMAL == MenuMode);
    } else {
      assert(MENU_MSUPER == MenuMode);
    }
  }

  return 0;
}

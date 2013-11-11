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
  for (loop=0; loop<1000; loop++) {
    uint16_t passwd_size = ( rand() % (LCD_MAX_COL-1) ) + 1;
    for (ui1=0; ui1<passwd_size; ui1++) {
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
    //    printf("Before corruption inp:'%s'\n", inp);
    if (corrupted) {
      inp[rand()%passwd_size]++;
    }
    //    printf("After corruption inp:'%s'\n", inp);

    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("Prompt 1", &arg1, MENU_ITEM_STR);
    menu_SetPasswd(MENU_MNORMAL|MENU_MVALIDATE);

    //    printf("Corrupted:%d\n", (uint32_t) corrupted);
    if (corrupted) {
      //      printf("MenuMode:0x%x\n", (uint32_t)MenuMode);
      assert(MENU_MNORMAL == MenuMode);
    } else {
      assert(MENU_MSUPER == MenuMode);
    }
  }

  /* modvat */
  for (loop=0; loop<1; loop++) {
    uint32_t vat = (rand()%100)*100 + (rand()%100);
    uint32_t vat1 = vat;
    LCD_CLRSCR;
    for (ui1=0; ui1<LCD_MAX_COL; ui1++)
      inp[ui1] = ' ';
    for (ui1=0; ui1<4; ui1++) {
      uint16_t ui2 = vat % 10;
      vat /= 10;
      inp[3-ui1] = '0' + ui2;
    }
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    printf("inp is %s\n", inp);
    menu_getopt("Prompt 1", &arg1, MENU_ITEM_STR);

    /* */
    arg1.valid = MENU_ITEM_NONE;
    LCD_CLRSCR;
    menu_getopt("sdlkfjlaksfklas", &arg1, MENU_ITEM_FLOAT);

    /* select input */
    uint16_t sel = rand() % 4;
    uint16_t ui2;
    inp[0] = 0; inp[1] = 0; inp[2] = 0; inp[3] = 0;
    for (ui1=0; ui1<sel; ui1++)
      inp[ui1] = rand()%2 ? KEY_SC_LEFT : KEY_SC_RIGHT;

    menu_ModVat(MENU_MNORMAL);
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.vat[sel]), (uint8_t *)&ui2, sizeof(uint16_t));
    printf("ui2:%0d vat1:%0d\n", (uint32_t) ui2, (uint32_t) vat1);
    assert(ui2 == vat1);

  }

  return 0;
}

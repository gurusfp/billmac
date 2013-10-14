#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "lcd.c"

int
main(int argc, char *argv[])
{
  LCD_WR(0, 0, "Hello World 7", 13);
  LCD_refresh();
  LCD_WR(1, 0, "Hello World 8", 13);
  LCD_refresh();

  return 0;
}

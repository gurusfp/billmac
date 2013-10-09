#include <stdio.h>
#include <stdtypes.h>
#include <pthread.h>
#include <unistd.h>

uint8_t  KbdHit;

#include "menu.c"

int
main(void)
{
  pthread_t f2_thread, f1_thread; 

  pthread_create(&f1_thread,NULL,f1,NULL);
  pthread_create(&f2_thread,NULL,f2,NULL);

  pthread_join(f1_thread,NULL);
  pthread_join(f2_thread,NULL);

  return 0;
}

void
get_key(void)
{
  uint32_t val;

  printf(" 1   2   3  10\n");
  printf(" 4   5   6  11\n");
  printf(" 7   8   9  12\n");
  printf("13  14  15  16\n");
  sscanf("%d", &val);
  if (0 == KbdHit) {
    printf("Processing Kbd key %d", val);
    KbdHit = val;
  }
}

void *
f1(void *x)
{
  uint8_t key;
  while (1) {
    get_key();
  }
  pthread_exit(0); 
}

void *
f2(void *x)
{
  menu_main();
  pthread_exit(0); 
}

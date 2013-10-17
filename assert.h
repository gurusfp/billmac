#ifndef _ASSERT_H
#define _ASSERT_H

#ifdef  UNIT_TEST

#define assert(X)				\
  if (!(X)) {					\
    printf("ASSERT ERROR " #X " " __FILE__ ":%d\n", __LINE__);	\
  }

#else

#define assert(X)			       \
  if (!(X)) {				       \
    LCD_WR_LINE(1, 0, __FILE__, 12);	       \
    LCD_wrchar('0' + (((__LINE__/10)/10)%10)); \
    LCD_wrchar('0' + ((__LINE__/10)%10));      \
    LCD_wrchar('0' + (__LINE__%10));	       \
  }

#endif
#endif

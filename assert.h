#ifndef ASSERT_H
#define ASSERT_H

#define assert(X)			       \
  if (!(X)) {				       \
    LCD_WR_LINE(1, 0, __FILE__, 12);	       \
    LCD_wrchar('0' + (((__LINE__/10)/10)%10)); \
    LCD_wrchar('0' + ((__LINE__/10)%10));      \
    LCD_wrchar('0' + (__LINE__%10));	       \
  }

#endif

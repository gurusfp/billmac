#ifndef LCD_H
#define LCD_H

#define LCD_MAX_ROW            2
#define LCD_MAX_COL           16

#define LCD_PORT  P1
#define LCD_rs    P2_0
#define LCD_rw    P2_1
#define LCD_en    P2_2

#define LCD_busy {  \
  uint8_t i, j;     \
  for(i=0;i<0x2;i++)	   \
    for(j=0;j<0xFF;j++) {} \
}

#define LCD_PROP_NOECHO_L2  1

#ifdef  UNIT_TEST

uint8_t _lcd_idx = 0;

# define LCD_CMD_DISON_CURON_BLINKON
# define LCD_CMD_CLRSCR        { uint8_t ui1; for (ui1=0; ui1<(LCD_MAX_ROW*LCD_MAX_COL); ui1++) { lcd_buf[0][ui1] = ' '; } _lcd_idx = 0; }
# define LCD_CMD_HOME          _lcd_idx = 0
# define LCD_CMD_DEC_CUR       assert(0)
# define LCD_CMD_INC_CUR
# define LCD_CMD_DISON_CURON
# define LCD_CMD_CUR_10        _lcd_idx = 0
# define LCD_CMD_CUR_20        _lcd_idx = LCD_MAX_COL
# define LCD_CMD_2LINE_5x7
# define LCD_ACT_LINE2         assert(0)

# define LCD_cmd(var)    \
  var

# define LCD_wrchar(var) \
  putchar(var)

#else

# define LCD_CMD_DISON_CURON_BLINKON 0x0F /* LCD ON, Cursor ON, Cursor blinking ON */
# define LCD_CMD_CLRSCR        0x01   /* Clear screen */
# define LCD_CMD_HOME          0x02   /* Return home */
# define LCD_CMD_DEC_CUR       0x04   /* Decrement cursor */
# define LCD_CMD_INC_CUR       0x06   /* Increment cursor */
# define LCD_CMD_DISON_CURON   0x0E   /* Display ON ,Cursor ON */
# define LCD_CMD_CUR_10        0x80   /* Force cursor to the beginning of 1st line */
# define LCD_CMD_CUR_20        0xC0   /* Force cursor to the beginning of 2nd line */
# define LCD_CMD_2LINE_5x7     0x38   /* Use 2 lines and 5×7 matrix */
# define LCD_ACT_LINE2         0x3C   /* Activate second line */

# define LCD_cmd(var)   \
  LCD_PORT = var;      \
  LCD_rs = 0;	       \
  LCD_rw = 0;	       \
  LCD_en = 1;	       \
  LCD_en = 0;	       \
  LCD_busy

# define LCD_idle_drive \
  LCD_PORT = 0;        \
  LCD_rs = 0;	       \
  LCD_rw = 0;	       \
  LCD_en = 0;

# define LCD_wrchar(var) \
  LCD_PORT = var;   \
  LCD_rs = 1; \
  LCD_rw = 0; \
  LCD_en = 1; \
  LCD_en = 0; \
  LCD_busy

#endif

#define LCD_CLRSCR {				\
  uint8_t ui1;				\
  lcd_buf_p = (uint8_t *)lcd_buf;		\
  for (ui1=0; ui1<(LCD_MAX_COL<<1); ui1++) {	\
    lcd_buf_p[0] = ' ';				\
    lcd_buf_p++;				\
  }						\
  lcd_buf_p = (uint8_t *)lcd_buf;		\
  lcd_buf_prop = 0;				\
}

#define LCD_WR_LINE(x, y, str)  {	\
  uint8_t ui1;			        \
  lcd_buf_p = lcd_buf[x]+y;		\
  lcd_buf_p += y;			\
  for (ui1=0; (ui1<LCD_MAX_COL);) {	\
    if (0 == str[ui1]) {		\
      lcd_buf_p[0] = ' ';		\
    } else {				\
      lcd_buf_p[0] = str[ui1];		\
      ui1++;				\
    }					\
    lcd_buf_p++;			\
  }					\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_WR_LINE_N(x, y, str, len)  {	\
  uint8_t ui1;			        \
  lcd_buf_p = lcd_buf[x]+y;		\
  for (ui1=0; (ui1<len); ui1++) {	\
    lcd_buf_p[0] = str[ui1];		\
    lcd_buf_p++;			\
  }					\
  for (; ui1<LCD_MAX_COL; ui1++) {	\
    lcd_buf_p[0] = ' ';			\
    lcd_buf_p++;			\
  }					\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_SCROLL {			\
  uint8_t ui1;				\
  for (ui1=0; ui1<LCD_MAX_COL; ui1++) {	\
    lcd_buf[0][ui1] = lcd_buf[1][ui1];	\
  }					\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_WR_N(str, len) {	 \
  uint8_t ui1;			 \
  for (ui1=0; ui1<len; ui1++) {	 \
    lcd_buf_p[0] = str[ui1];	 \
    lcd_buf_p++;		 \
  }				 \
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_WR(str) {	 \
  uint8_t ui1;			 \
  for (ui1=0; 0 != str[ui1]; ui1++) {	 \
    lcd_buf_p[0] = str[ui1];	 \
    lcd_buf_p++;		 \
  }				 \
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

extern uint8_t lcd_buf_prop;
extern uint8_t *lcd_buf_p;
extern uint8_t lcd_buf[LCD_MAX_ROW][LCD_MAX_COL];
extern void LCD_init(void);
extern void LCD_refresh(void);

#endif

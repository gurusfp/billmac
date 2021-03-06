#ifndef LCD_H
#define LCD_H

#define LCD_MAX_ROW            2
#define LCD_MAX_COL           16

#if 8 == LCD_DPORT_SIZE
# define LCD_PORT  P1
# define LCD_rs    P2_0
# define LCD_en    P2_1
#elif 4 == LCD_DPORT_SIZE
# define LCD_PORT(val)       \
  P2_2 = (val & 0x1);	     \
  P2_3 = ((val>>1) & 0x1);   \
  P2_4 = ((val>>2) & 0x1);   \
  P2_5 = ((val>>3) & 0x1)
# define LCD_en    P2_1
# define LCD_rs    P2_0
#endif

#define LCD_PROP_NOECHO_L2  (1<<0)
#define LCD_PROP_DIRTY      (1<<1)

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

# define LCD_cmd(var)

# define LCD_idle_drive

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
# define LCD_ACT_LINE2         0x3C   /* Activate second line */

#if 8 == LCD_DPORT_SIZE

# define LCD_CMD_2LINE_5x7     0x38   /* Use 2 lines and 5�7 matrix */

# define LCD_cmd(var)   \
  LCD_PORT = var;	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  delayms(2)

# define LCD_idle_drive \
  LCD_PORT = 0;		\
  LCD_rs = 0;		\
  LCD_en = 0

# define LCD_wrchar(var)\
  LCD_PORT = var;	\
  LCD_rs = 1;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  delayms(2)

#elif 4 == LCD_DPORT_SIZE

# define LCD_CMD_2LINE_5x7     0x28   /* Use 2 lines and 5�7 matrix */

# define LCD_wrnib(var)	\
  LCD_PORT(var);	\
  LCD_rs = 1;		\
  LCD_en = 1;		\
  LCD_en = 0

# define LCD_cmd(var)   \
  LCD_PORT(var>>4);	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  LCD_PORT(var);	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  delayms(2)

# define LCD_idle_drive \
  LCD_PORT(0);		\
  LCD_rs = 0;		\
  LCD_en = 0

# define LCD_wrchar(var)\
  LCD_wrnib(var>>4);	\
  LCD_wrnib(var);	\
  delayms(2)

#endif

#endif

#define LCD_CLRSCR {				\
  uint8_t ui1_t;				\
  lcd_buf_p = (uint8_t *)lcd_buf;		\
  for (ui1_t=0; ui1_t<(LCD_MAX_COL*LCD_MAX_ROW); ui1_t++) {	\
    lcd_buf_p[0] = ' ';				\
    lcd_buf_p++;				\
  }						\
  lcd_buf_p = (uint8_t *)lcd_buf;		\
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
}

#define LCD_WR_LINE(x, y, str)  {	\
    uint8_t ui1_t, ui2_t;			\
  lcd_buf_p = lcd_buf[x];		\
  lcd_buf_p += y;			\
  for (ui1_t=0, ui2_t=0; (ui1_t<LCD_MAX_COL); ui1_t++) {	\
    if (0 == ((char *)str)[ui2_t]) {			\
      lcd_buf_p[0] = ' ';		\
    } else {				\
      lcd_buf_p[0] = ((char *)str)[ui2_t];	\
      ui2_t++;					\
    }					\
    lcd_buf_p++;			\
  }					\
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_WR_LINE_N(x, y, str, len)  {	\
  uint8_t ui1_t;			        \
  lcd_buf_p = lcd_buf[x];		\
  lcd_buf_p += y;			\
  for (ui1_t=0; (ui1_t<len); ui1_t++) {	\
    lcd_buf_p[0] = str[ui1_t];		\
    lcd_buf_p++;			\
  }					\
  for (; ui1_t<LCD_MAX_COL; ui1_t++) {	\
    lcd_buf_p[ui1_t] = ' ';		\
  }					\
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_POS(x, y) {			\
  lcd_buf_p = lcd_buf[x];		\
  lcd_buf_p += y;			\
}

#define LCD_SCROLL {			\
  uint8_t ui1_t;				\
  for (ui1_t=0; ui1_t<LCD_MAX_COL; ui1_t++) {	\
    lcd_buf[0][ui1_t] = lcd_buf[1][ui1_t];	\
  }					\
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_WR_N(str, len) {	 \
  uint8_t ui1_t;			 \
  for (ui1_t=0; ui1_t<len; ui1_t++) {	 \
    lcd_buf_p[0] = str[ui1_t];	 \
    lcd_buf_p++;		 \
  }				 \
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_WR(str) {	 \
  uint8_t ui1_t;			 \
  for (ui1_t=0; 0 != str[ui1_t]; ui1_t++) {	 \
    lcd_buf_p[0] = str[ui1_t];	 \
    lcd_buf_p++;		 \
  }				 \
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_PUT_UINT8X(ch) {			\
  uint8_t ui2_a = (ch>>4) & 0xF;			\
  lcd_buf_p[0] = ((ui2_a>9) ? 'A'-10 : '0') + ui2_a;	\
  lcd_buf_p++;					\
  ui2_a = ch & 0xF;				\
  lcd_buf_p[0] = ((ui2_a>9) ? 'A'-10 : '0') + ui2_a;	\
  lcd_buf_p++;					\
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_PUTCH(ch) {				\
  lcd_buf_p[0] = ch;				\
  lcd_buf_p++;					\
  lcd_buf_prop |= LCD_PROP_DIRTY;		\
  assert(lcd_buf_p <= (((uint8_t *)lcd_buf)+32));	\
}

#define LCD_ALERT(str) LCD_WR_LINE(0, 0, str)

extern uint8_t lcd_buf_prop;
extern uint8_t *lcd_buf_p;
extern uint8_t lcd_buf[LCD_MAX_ROW][LCD_MAX_COL];
extern void LCD_init(void);

#endif

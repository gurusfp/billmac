#include <stdint.h>
#include <p89v51rd2.h>

#include "lcd.h"
#include "menu.h"

#define ROW_JOIN
#define COL_JOIN
#define MENU_MODE(A)
#define MENU_NAME(A) A
#define MENU_FUNC(A)
#define ARG1(A, B)
#define ARG2(A, B)
uint8_t *menu_names = MENU_ITEMS;
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

typedef void (*menu_func_t)(uint8_t mode);

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_MODE(A)
#define MENU_NAME(A)
#define MENU_FUNC(A)
#define ARG1(A, B) B,
#define ARG2(A, B) B
uint8_t menu_args[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_MODE(A)
#define MENU_NAME(A)
#define MENU_FUNC(A)
#define ARG1(A, B) A,
#define ARG2(A, B) A
uint8_t menu_prompts[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_MODE(A) A
#define MENU_NAME(A)
#define MENU_FUNC(A)
#define ARG1(A, B)
#define ARG2(A, B)
uint8_t menu_mode[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

#define ROW_JOIN +
#define COL_JOIN
#define MENU_MODE(A)
#define MENU_NAME(A)
#define MENU_FUNC(A) 1
#define ARG1(A, B)
#define ARG2(A, B)
const uint8_t MENU_MAX = MENU_ITEMS;
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

char    menu_prompt_str[] = MENU_PROMPTS;

menu_arg_t arg1, arg2;
uint8_t    menu_error;

#define KCHAR_ROWS        10
#define KCHAR_COLS         9
#define KCHAR_SHIFT_SZ     5
uint8_t keyChars[] = {
  '0', ' ', '.', ',', ')', '+', '?', '_', ':',
  '1', 'a', 'b', 'c', '!', 'A', 'B', 'C', '~',
  '2', 'd', 'e', 'f', '@', 'D', 'E', 'F', '`',
  '3', 'g', 'h', 'i', '#', 'G', 'H', 'I', '{',
  '4', 'j', 'k', 'l', '$', 'J', 'K', 'L', '}',
  '5', 'm', 'n', 'o', '%', 'M', 'N', 'O', '|',
  '6', 'p', 'q', 'r', '^', 'P', 'Q', 'R', '\\',
  '7', 's', 't', 'u', '&', 'S', 'T', 'U', '/',
  '8', 'v', 'w', 'x', '*', 'V', 'W', 'X', '<',
  '9', 'y', 'z', '(', '-', 'Y', 'Z', '=', '>',
};

#define MENU_STR1_IDX_PRICE 0
#define MENU_STR1_IDX_DISCO 1
#define MENU_STR1_IDX_S_TAX 2
#define MENU_STR1_IDX_YesNo 3
#define MENU_STR1_IDX_VAT   5
#define MENU_STR1_IDX_CONFI 6
#define MENU_STR1_IDX_ITEM  7
#define MENU_STR1_IDX_MANY  8
#define MENU_STR1_IDX_FINAL 9
#define MENU_STR1_IDX_PRINT 10
#define MENU_STR1_IDX_SAVE  11
#define MENU_STR1_IDX_DADAT 12
#define MENU_STR1_IDX_NAME  13
#define MENU_STR1_IDX_REPLA 14
uint8_t menu_str1[] = 
  "Price" /* 0 */
  "Disco" /* 1 */
  "S.Tax" /* 2 */
  "Yes  " /* 3 */
  "No   " /* 4 */
  "Vat  " /* 5 */
  "Confi" /* 6 */
  "Item#" /* 7 */
  "Many#" /* 8 */
  "Final" /* 9 */
  "Print" /*10 */
  "Save " /*11 */
  "Delet" /*12 */
  "Name " /*13 */
  "Repla" /*14 */
  ;

/* */
static uint8_t MenuMode = MENU_MNOR;

void
menu_Init(void)
{
  /* FIXME: implement MENU_MRESET */
  MenuMode = MENU_MNOR;
}

void
menu_unimplemented(void)
{
  assert(0);
}

sale_item s_item;

void
menu_Billing(uint8_t mode)
{
  uint8_t ui2;
  uint16_t item_addr, ui1;

  billing *bi = (void *) bufSS;
  for (ui2=0; ui2<sizeof(billing); ui2++) {
    bufSS[ui2] = 0;
  }

  arg2.valid = MENU_ITEM_NONE;
  menu_getopt(menu_str1+(MENU_STR1_IDX_ITEM*MENU_PROMPT_LEN), &arg2, MENU_ITEM_ID);

get_more_items:
  for (; bi->info.n_items < 16; bi->info.n_items++) {
    item_addr = flash_item_find(arg2.value.integer);
    if ((MENU_ITEM_NONE == arg2.valid) || (0xFFFF == item_addr))
      break;

    /* */
    ui2 = bi->info.n_items;
    bi->items[ui2].item_id = arg2.value.integer;
    bi->items[ui2].item_id_h = arg2.value.integer>>8;
    bi->addrs[ui2] = item_addr;

    do {
      /* # items */
      arg2.valid = MENU_ITEM_NONE;
      menu_getopt(menu_str1+(MENU_STR1_IDX_MANY*MENU_PROMPT_LEN), &arg2, MENU_ITEM_ID);
    } while (MENU_ITEM_NONE == arg2.valid);

    /* */
    bi->items[ui2].num_sale  = arg2.value.integer;
  }

  /* */
  if (0 != menu_getchoice(menu_str1+(MENU_STR1_IDX_FINAL*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2))
    goto get_more_items;

  /* */
  if (0 != menu_getchoice(menu_str1+(MENU_STR1_IDX_SAVE*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2))
    return;

  /* time */
  {
    uint8_t buf[3];
    timerDateGet(buf);
    bi->info.date_dd = ((buf[0]>>4)&0x3)*10+(buf[0]&0xF);
    bi->info.date_mm = ((buf[0]>>4)&0x1)*10+(buf[0]&0xF);

    timerTimeGet(buf);
    bi->info.time_hh = ((buf[0]>>4)&0x3)*10+(buf[0]&0xF);
    bi->info.time_mm = ((buf[0]>>4)&0x7)*10+(buf[0]&0xF);
  }

  /* billing ... */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.print_it), (uint8_t *)&ui2, sizeof(uint8_t));
  if (0 == ui2) { /* enabled */
    if (0 == menu_getchoice(menu_str1+(MENU_STR1_IDX_PRINT*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2)) {
      /* print */
      //  uint8_t buf[4];
      //  item*   i_p = (void *)buf;
      //    for (ui2=0; ui2<4; ui2++)
      //      buf[ui2] = FlashReadByte(item_addr+ui2);
    }
  }

  flash_sale_add(bufSS);

  /* */
  if (0 == s_item.n_items)
    return;
}

void
menu_ShowBill(uint8_t mode)
{
  sale_info* si;

  if ((MENU_PR_NONE == arg1.valid) || (MENU_PR_NONE == arg2.valid))
    return;

  si = flash_sale_find(&(arg1.value.date.date), arg2.value.integer.i16);
  if (NULL == si) {
    ERROR("Not Found");
    return;
  }

  if ((mode&(~MENU_MODEMASK)) == MENU_MPRINT) {
    /* FIXME: print */
  } else if ((mode&(~MENU_MODEMASK)) == MENU_MDELETE) {
    FlashWriteByte((uint16_t) &(si->deleted), 1);
  } else {
    /* FIXME: display */
  }
}

/*
  Menu item : Name,     cost, discount, serv-tax, vat-id, id
              15bytes, 13bit,    13bit,     1bit,   2bit, 9bit
 */
void
menu_AddItem(uint8_t mode)
{
  uint8_t choice[MENU_PROMPT_LEN*4], ui2, ui3, ui4, vat;
  uint16_t ui1, item_count;
  menu_arg_t cost;

  /* Check if space is available */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&item_count, sizeof(uint16_t));
  if (item_count >= ITEM_MAX) {
    ERROR("Items Exceeded");
    return;
  }

  /* save name */
  item *it = (void *) bufSS;
  for (ui4=0; ui4<ITEM_SIZEOF; ui4++) {
    bufSS[ui4] = 0;
  }
  for (ui4=0; ui4<15; ui4++) {
    it->name[ui4] = lcd_buf[1][ui4];
  }

  /* Cost, discount */
  cost.valid = MENU_ITEM_NONE;
  menu_getopt(menu_str1+(MENU_STR1_IDX_PRICE*MENU_PROMPT_LEN), &cost, MENU_ITEM_FLOAT);
  arg2.valid = MENU_ITEM_NONE;
  menu_getopt(menu_str1+(MENU_STR1_IDX_DISCO*MENU_PROMPT_LEN), &arg2, MENU_ITEM_FLOAT);

  /* vat */
  for (ui2=0; ui2<4; ui2++) {
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.vat[ui2]), (uint8_t *)&ui1, sizeof(uint16_t));
    for (ui3=0; ui3<4; ui3++) {
      (choice+(ui2*MENU_PROMPT_LEN)+MENU_PROMPT_LEN-ui3)[0] = '0' + ui1%10;
      ui1 /= 10;
    }
  }
  vat = menu_getchoice(menu_str1+(MENU_STR1_IDX_VAT*MENU_PROMPT_LEN), choice, 4);

  /* serv-tax */
  uint8_t s_tax;
  s_tax = menu_getchoice(menu_str1+(MENU_STR1_IDX_S_TAX*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2) ? 0 : 1;

  /* Confirm */
  if ( (MENU_ITEM_NONE == arg1.valid) || (MENU_ITEM_NONE == arg2.valid) ||
       (MENU_ITEM_NONE == cost.valid) ) {
    ERROR("Invalid Option");
    return;
  }
  if (0 != menu_getchoice(menu_str1+(MENU_STR1_IDX_CONFI*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2))
    return;

  /* Pack the value */
  it->vat_sel = vat;
  it->has_serv_tax = s_tax;
  it->id = ui1;
  it->id_h = ui1>>8;
  it->cost = cost.value.integer;
  it->discount = discount.value.integer;

  /* store it at empty space */
  flash_item_add((uint8_t *)it);
}

void
menu_DelItem(uint8_t mode)
{
  if ((MENU_ITEM_NONE == arg1.valid) || (MENU_ITEM_ID != arg1.valid)) {
    ERROR("Invalid Option");
    return;
  }
  if (arg1.value.integer >= ITEM_MAX) {
    ERROR("Too Large Opt");
    return;
  }

  flash_item_delete(arg1.value.integer);
}

void
menu_DayItemBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DayAllBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DayBillPrned(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DayDupBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DayTaxReport(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_MonItemBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_MonAllBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_MonBillPrned(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_MonTaxReport(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_AllItemBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_AllFullBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_TaxReport(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_ModVat(uint8_t mode)
{
  uint16_t ui1;
  uint8_t  ui2, ui3, choice[MENU_PROMPT_LEN*4], vat;

  if (MENU_ITEM_NONE == arg1.valid)
    return;

  for (ui2=0; ui2<4; ui2++) {
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.vat[ui2]), (uint8_t *)&ui1, sizeof(uint16_t));
    for (ui3=0; ui3<4; ui3++) {
      (choice+(ui2*MENU_PROMPT_LEN)+MENU_PROMPT_LEN-ui3)[0] = '0' + ui1%10;
      ui1 /= 10;
    }
  }
  ui3 = menu_getchoice(menu_str1+(MENU_STR1_IDX_REPLA*MENU_PROMPT_LEN), choice, 4);
  assert(ui3 < 4);

  ui1 = arg1.value.integer.i8;
  ui1 <<= 16;
  ui1 |= arg1.value.integer.i16;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.vat[ui3]), (uint8_t *)&ui1, sizeof(uint16_t));
}

void
menu_Header(uint8_t mode)
{
  uint8_t chars = 0, ui2, ui3;
  uint8_t mode_max = ((mode&(~MENU_MODEMASK)) == MENU_MFOOTER) ? FOOTER_MAX_SZ : HEADER_MAX_SZ;

  do {
    if (MENU_ITEM_NONE != arg1.valid) {
      for (ui2=0, ui3=0; (ui2<LCD_MAX_COL) && (0==ui3); ui2++) {
	ui3 = (' ' == lcd_buf[LCD_MAX_COL+ui2]) ? 1 : 0;
      }
      if (ui3) {
	for (ui2=0; (ui2<LCD_MAX_COL) && (chars<mode_max); ui2++) {
	  bufSS[ui2] = lcd_buf[LCD_MAX_COL+ui2];
	  chars++;
	}
      } else break;
    }
    arg1.valid = MENU_ITEM_NONE;
    menu_getopt(menu_str1+(MENU_STR1_IDX_ITEM*MENU_PROMPT_LEN), &arg1, MENU_ITEM_STR);
  } while (chars < mode_max);

  if (chars) {
    if ((mode&(~MENU_MODEMASK)) == MENU_MFOOTER) {
      EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.prn_footer[0]), (uint8_t *)bufSS, sizeof(uint8_t)*chars);
    } else {
      EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.prn_header[0]), (uint8_t *)bufSS, sizeof(uint8_t)*chars);
    }
  }
}

void
menu_DelAllBill(uint8_t mode)
{
  uint16_t ui1 = 0;
  uint8_t  ui2;

  /* Remove all data when date is changed */
  LCD_ALERT("Delete AllData");
  if (0 != menu_getchoice(menu_str1+(MENU_STR1_IDX_DADAT*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2))
    return;

  /* */
  timerDateSet(arg1.value.date.year, arg1.value.date.month, arg1.value.date.date);
  for (ui2=0; ui2<(13*4); ui2++) {
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_date_ptr[ui2]), (uint8_t *)&ui1, sizeof(uint16_t));
  }
  ui1 = FLASH_DATA_START;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&ui1, sizeof(uint16_t));
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&ui1, sizeof(uint16_t));
  FlashEraseSector(ui1);
}

void
menu_SetPasswd(uint8_t mode)
{
  uint16_t ui1, ui4;
  uint8_t ui2, ui3;

  CRC16_Init();
  for (ui1=0; ui1<LCD_MAX_COL; ui1++) {
    ui2 = lcd_buf[LCD_MAX_COL+ui1];
    for (ui3=0; ui3<(KCHAR_COLS*KCHAR_ROWS); ui3++) {
      if (ui2 == keyChars[ui3])
	break;
    }
    if (ui3 < (KCHAR_ROWS*KCHAR_COLS))
      CRC16_Update(ui2);
  }
  ui1 = CRC16_High; ui1 <<= 8; ui1 |= CRC16_Low;

  if ((mode&(~MENU_MODEMASK)) == MENU_MVALIDATE) {
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.passwd), (uint8_t *)&ui4, sizeof(uint16_t));
    if (ui4 != ui1) {
      LCD_ALERT("Passwd InCorre");
    } else {
      LCD_ALERT("Passwd Correct");
      MenuMode = MENU_MSUPER;
    }
  } else {
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.passwd), (uint8_t *)&ui1, sizeof(uint16_t));
  }
}

void
menu_SetServTax(uint8_t mode)
{
  uint16_t ui1;

  if (MENU_ITEM_NONE == arg1.valid)
    return;

  ui1 = arg1.value.integer.i8;
  ui1 <<= 16;
  ui1 |= arg1.value.integer.i16;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.service_tax), (uint8_t *)&ui1, sizeof(uint16_t));
}

void
menu_SetDateTime(uint8_t mode)
{
  uint16_t ui1 = 0;
  uint8_t ymd[3], ui2;

  if ((MENU_ITEM_NONE == arg1.valid) || (MENU_ITEM_NONE == arg2.valid))
    return;

  timerTimeSet(arg1.value.time.hour, arg1.value.time.min);

  /* if no change in date */
  timerDateGet(ymd);
  if ( (ymd[2] == arg1.value.date.year) && (ymd[1] == arg1.value.date.month) && (ymd[0] == arg1.value.date.date) )
    return;

  menu_DelAllBill(mode);
}

void
menu_RunDiag(uint8_t mode)
{
  menu_unimplemented();
}

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_MODE(A)
#define MENU_NAME(A)
#define MENU_FUNC(A) A
#define ARG1(A, B)
#define ARG2(A, B)
menu_func_t menu_handlers[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

void
menu_getopt(uint8_t *prompt, menu_arg_t *arg, uint8_t opt)
{
  uint8_t key, key_n, key_s, col_id;
  uint8_t str[LCD_MAX_COL];

  if (MENU_ITEM_NONE == opt) return;

  /* init */
  LCD_CLRSCR;
  lcd_buf_prop = (opt & MENU_ITEM_PASSWD) ? LCD_PROP_NOECHO_L2 : 0;

  /* Ask a question */
  LCD_WR_LINE_N(0, 0, prompt, MENU_PROMPT_LEN);
  LCD_WR(" ?");

  /* Set the prompt */
  col_id = 0;
  uint8_t *lp = &(lcd_buf[1][0]);

  /* Get a string */
  do {
    while KBD_NOT_HIT {
      sleep(10);
    }
    KBD_GET_KEY;

    /* Don't overflow buffer */
    if (col_id > 15) col_id = 15;

    switch (KbdData) {
    case KEY_SC_LEFT:
      if (col_id == 0) {
	break;
      }
      col_id--; lp--;
      lp[0] = ' ';
      break;
    case KEY_SC_ENTER:
      col_id++;
      break;
    case KEY_SC_RIGHT:
    case KEY_SC_PRINT:
    case KEY_SC_INVALID:
      break;
    default:
      key *= KCHAR_COLS;
      if (key_s) key += KCHAR_SHIFT_SZ;
      key += key_n;
      lp[0] = keyChars[key];
      lp++; col_id++;
    }
    if (KEY_SC_ENTER != key) {
      KbdData = 0;
    }
  } while (key != KEY_SC_ENTER);

  uint8_t item_type = (opt & MENU_ITEM_TYPE_MASK);
  uint8_t *lbp = (uint8_t *) lcd_buf[1];
  uint32_t val = 0;
  uint8_t ui1;

  menu_error = 1;
  if ((MENU_ITEM_ID == item_type) || (MENU_ITEM_FLOAT == item_type)) {
    for (ui1=0; ui1<col_id; ui1++) {
      if ((lbp[0] >= '0') && (lbp[0] <= '9')) {
	val *= 10;
	val += lbp[0] - '0';
	menu_error = 0;
      } else {
	break;
      }
      lbp++;
    }
    arg->value.integer.i8  = val>>16;
    arg->value.integer.i16 = val;
  } else if ((MENU_ITEM_DATE == item_type) || (MENU_ITEM_MONTH == item_type)) {
    /* format DDMMYYYY || format MMYYYY */
    menu_error = 0;
    for (ui1=0; ui1<(item_type+2); ui1++) { /* cleaverly spaced item_type apart */
      if ((lbp[ui1] < '0') || (lbp[ui1] > '9'))
	menu_error++;
    }
    if (0 == menu_error) {
      /* Date */
      if (MENU_ITEM_DATE == item_type) {
	for (ui1=0; ui1<2; ui1++) {
	  val *= 10;
	  val += lbp[0] - '0';
	  lbp++;
	}
	if ((0 == val) || (val > 31)) menu_error++;
	arg->value.date.date = val;
      }
      /* Month */
      val = 0;
      for (ui1=0; ui1<2; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
	lbp++;
      }
      if ((0 == val) || (val > 12)) menu_error++;
      arg->value.date.month = val;
      /* Year */
      val = 0;
      for (ui1=0; ui1<4; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
	lbp++;
      }
      if ((val < 2000) || (val > 2100)) menu_error++;
      arg->value.date.year = val-2000;
    }
  } else if (MENU_ITEM_TIME == item_type) {
    /* format HHMM */
    uint8_t ui1;
    menu_error = 0;
    for (ui1=0; ui1<4; ui1++) {
      if ((lbp[ui1] < '0') || (lbp[ui1] > '9'))
	menu_error++;
    }
    if (0 == menu_error) {
      /* Hour */
      for (ui1=0; ui1<2; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
	lbp++;
      }
      if (val>23) menu_error++;
      arg->value.time.hour = val;
      /* Mins */
      val = 0;
      for (ui1=0; ui1<2; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
	lbp++;
      }
      if (val > 59) menu_error++;
      arg->value.time.min = val;
    }
  } else if (MENU_ITEM_STR == item_type) {
  } else assert(0);

  /* */
  if (0 == menu_error) {
    arg->valid = item_type;
  } else if (opt & MENU_ITEM_OPTIONAL) {
  } else if ((opt & MENU_ITEM_DONTCARE_ON_PREV) && (arg == &arg2)) {
  } else assert(0);
}

uint8_t
menu_getchoice(uint8_t *quest, uint8_t *opt_arr, uint8_t max_idx)
{
  uint8_t ret = 0, key, key_n, key_s;
  do {
    ret %= max_idx;

    LCD_WR_LINE_N(1, 0, quest, MENU_PROMPT_LEN);
    LCD_WR(": ");
    LCD_WR_N(opt_arr+(ret*MENU_PROMPT_LEN), MENU_PROMPT_LEN);

    while KBD_NOT_HIT {
      sleep(10);
    }
    KBD_GET_KEY;

    ret++;
    switch (KbdData) {
    case KEY_SC_LEFT:
    case KEY_SC_RIGHT:
 break;
    case KEY_SC_ENTER:
      return ret;
      break;
    default:
      ret--; /* cancell effect of ++ */
      break;
    }
  } while (1);
  assert (0);
}

void
menu_main(void)
{
  uint8_t menu_selected, key, key_n, key_s;

  /* initialize */
  menu_selected = 0;

menu_main_start:
  /* Wait until get command from user */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.prn_header[0]), bufSS, sizeof(uint8_t)*LCD_MAX_COL);
  LCD_WR_LINE_N(0, 0, bufSS, LCD_MAX_COL);
  LCD_WR_LINE_N(1, 0, (menu_names+(menu_selected*MENU_NAMES_LEN)), MENU_NAMES_LEN);
  while KBD_NOT_HIT {
    DELAY(0xFF);
  }

  KBD_GET_KEY;
  KBD_RESET_KEY;
  if (KEY_SC_ENTER == key) {
    if (MenuMode & (menu_mode[menu_selected-1] & MENU_MODEMASK)) {
      arg1.valid = MENU_ITEM_NONE;
      menu_getopt(menu_prompt_str+((menu_prompts[menu_selected<<1])<<2), &arg1, menu_args[(menu_selected<<1)]);
      arg2.valid = MENU_ITEM_NONE;
      menu_getopt(menu_prompt_str+((menu_prompts[(menu_selected<<1)+1])<<2), &arg2, menu_args[((menu_selected<<1)+1)]);
      (menu_handlers[menu_selected-1])(menu_mode[menu_selected-1]);
    }
    menu_selected = 1;
  } else if (KEY_SC_LEFT == key) {
    menu_selected = (1 <= menu_selected) ? MENU_MAX : menu_selected-1;
  } else if (KEY_SC_RIGHT == key) {
    menu_selected = (MENU_MAX == menu_selected) ? 1 : menu_selected+1;
  } else if (KEY_SC_PRINT == key) {
    /* FIXME */
  } else {
    /* could be hotkey for menu */
    menu_selected *= 10;
    menu_selected += key;
    /* handle out of bounds */
    if (menu_selected >= MENU_MAX)
      menu_selected = key;
  }

  /* recursive call, capture the case when you have unexpected
     exit */
  goto menu_main_start;
}

uint8_t bufSS[FLASH_SECTOR_SIZE];

/* Item type storage : Each item is packed in 20 byte
   continous-running-storage.
   return : id 
 */
void
flash_item_add(uint8_t* byte_arr)
{
  uint16_t item_last_modified, ui1;
  uint8_t  ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[item_last_modified/8]), (uint8_t *)&ui2, sizeof(uint8_t));
  for (ui1=item_last_modified+1; ui1!=item_last_modified; ) {
    if (ui1 >= ITEM_MAX)  ui1 = 0;
    if (0 == (ui1%8))
      EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[ui1/8]), (uint8_t *)&ui2, sizeof(uint8_t));
    if ( 0 == (ui2 & (1<<(ui1&0x7))) ) {
      break;
    }
    /* incr */
    ui1++;
  }
  if ( 0 != (ui2 & (1<<(ui1&0x7))) ) {
    assert(0); /* they why? item_count < ITEM_MAX */
    return -1;
  }
  item_last_modified = ui1;

  /* store item */
  ui2 |= 1 << (item_last_modified&0x7);
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[item_last_modified/8]), (uint8_t *)&item_last_modified, sizeof(uint8_t));
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));
  ui1 = ((uint16_t)FLASH_ITEM_START) + (ui1*ITEM_SIZEOF);
  for (ui2=0; ui2<ITEM_SIZEOF; ui2++, ui1++) {
    FlashWriteByte(ui1, byte_arr[0]);
    byte_arr++;
  }

  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
  ui1++;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
}

void
flash_item_delete(uint16_t id)
{
  uint16_t ui1, addr;
  uint8_t item_valid;
  uint8_t *block, *block_end;

  /* do nothing if already invalid */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[id/8]), (uint8_t *)&item_valid, sizeof(uint8_t));
  if (0 == (item_valid & (1<<(addr%0x7))) ) {
    return;
  }
  item_valid &= ~(1 << (addr%0x7));

  /* input is id, compute addr */
  addr = FLASH_ITEM_START + (id * ITEM_SIZEOF);
  block = (uint8_t *) (addr & ~((uint16_t)(FLASH_SECTOR_SIZE-1)));
  block_end = (uint8_t *) ((addr+ITEM_SIZEOF-1) & ~((uint16_t)(FLASH_SECTOR_SIZE-1)));

  /* save start block */
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    if ( ((block+ui1) < addr) || ((block+ui1) > (addr+ITEM_SIZEOF)) )
      bufSS[ui1] = block[ui1];
    else
      bufSS[ui1] = 0;
  }
  FlashEraseSector(block);
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    if (0 != bufSS[ui1])
      FlashWriteByte(block+ui1, bufSS[ui1]);
  }

  /* end block */
  if (block != block_end) {
    for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
      if ( (block_end+ui1) > (addr+ITEM_SIZEOF) )
	bufSS[ui1] = block_end[ui1];
      else
	bufSS[ui1] = 0;
    }
    FlashEraseSector(block_end);
    for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
      if (0 != bufSS[ui1])
	FlashWriteByte(block_end+ui1, bufSS[ui1]);
    }
  }

  /* upate valid */
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_valid[id/8]), (uint8_t *)&item_valid, sizeof(uint8_t));
  uint16_t ui1;
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
  ui1--;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
}

/* Sales are easily accessable from table */
void
flash_sale_add(uint8_t *sale)
{
  uint16_t sale_start, sale_end;
  uint16_t sale_start_sector, sale_end_sector, sale_newend_sector;
  uint8_t  nbytes, ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
  sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  sale_end_sector   = sale_end & ~((uint16_t) (FLASH_SECTOR_SIZE-1));

  /* compute if there is overflow sector
     Implemented as a circular buffer. Possible combinations are
     start ->         | end   ->
     end   ->         | start ->

     start : pointer to start of valid data
     end   : pointer to start of free  space
   */
  nbytes = SALE_INFO_SIZEOF + (((sale[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * (uint16_t)SALE_SIZEOF);
  sale_newend_sector = (sale_end+nbytes-1) & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  if (sale_newend_sector >= FLASH_DATA_END) {
    sale_newend_sector = FLASH_DATA_START;
  }
  assert ( (sale_newend_sector <= sale_start_sector) || (sale_newend_sector >= sale_end_sector) );
  assert ( (sale_newend_sector >= FLASH_DATA_START)  && (sale_newend_sector < FLASH_DATA_END) );

  /* when new sector is taken, erase it */
  if (sale_end_sector != sale_newend_sector) {
    /* If new sector is already occupied */
    for (; sale_start_sector==sale_newend_sector; ) {
      ui2 = FlashReadByte(sale_start);
      sale_start += SALE_INFO_SIZEOF + (((ui2 & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
      sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
    }
    FlashEraseSector(sale_newend_sector);
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  }

  /* Bytes are available ..., just store */
  sale_index_it(sale, sale_end);
  for (ui2=0; ui2<nbytes; ui2++) {
    FlashWriteByte(sale_end, sale[0]);
    sale_end++, sale++;
    if (sale_end >= FLASH_DATA_END) {
      sale_end = FLASH_DATA_START;
    }
  }
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
}

void
flash_sale_delete_month(uint8_t del_month)
{
  uint16_t sale_start, sale_end;
  uint16_t sale_start_sector, sale_end_sector, sale_newend_sector;
  uint8_t  nbytes, ui2;

  /* init */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
  sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  sale_end_sector   = sale_end & ~((uint16_t) (FLASH_SECTOR_SIZE-1));

  /* Move forward : Not required to reflash the new sector */
  {
    uint8_t ui3[3];
    sale_info *si;
    do {
      si = (void *)ui3;
      ui3[0] = FlashReadByte(sale_start);
      ui3[1] = FlashReadByte(sale_start+1);
      ui3[2] = FlashReadByte(sale_start+2);
      if (del_month == si->date_mm) {
	sale_start += SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
	sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
      }
    } while(del_month == si->date_mm);
  }
}

uint8_t *
flash_sale_find(uint8_t *dmy, uint16_t id)
{
  sale_info  *si, *vptr;
  uint16_t ui1, ui2;
  uint8_t idx;

  idx = (dmy[1] - 1) * 4;
  if (date <= 7) {}
  else if (date <= 14) idx ++;
  else if (date <= 21) idx += 2;
  else idx += 3;

  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_ptr[idx]), (uint8_t *)&si, sizeof(uint16_t));
  if (0 == si) return (void *)si;

  for (ui1=0, vptr=si; ui1<id; ui1++, vptr=si) {
    uint8_t ui3[3];
    sale_info* si_t = ui3;
    ui3[0] = FlashReadByte(si);
    ui3[1] = FlashReadByte(si+1);
    ui3[2] = FlashReadByte(si+2);
    if ( (dmy[0] == si_t->date_dd) && (dmy[1] == si_t->date_mm) ) {
      si = (sale_info *)( ((uint8_t *)si) + SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF) );
    } else { /* less # items for this day */
      return (void *) 0;
    }
  }

  return vptr;
}

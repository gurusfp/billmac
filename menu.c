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
  ;

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
  uint16_t item_addr;

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

  /* */
  if (0 == menu_getchoice(menu_str1+(MENU_STR1_IDX_PRINT*MENU_PROMPT_LEN), menu_str1+(MENU_STR1_IDX_YesNo*MENU_PROMPT_LEN), 2)) {
    /* print */
    //  uint8_t buf[4];
    //  item*   i_p = (void *)buf;
    //    for (ui2=0; ui2<4; ui2++)
    //      buf[ui2] = FlashReadByte(item_addr+ui2);
  }

  flash_sale_add(bufSS);

  /* */
  if (0 == s_item.n_items)
    return;

}

void
menu_ShowBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DupBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DeleteBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_ValidatePaswd(uint8_t mode)
{
  menu_unimplemented();
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

  /* */
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

  /* store it */
  uint16_t item_addr = flash_item_add((uint8_t *)it);
  if (0xFFFF == item_addr) {
    ERROR("Can't add item");
  } else { /* update database */
    item_count++;
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&item_count, sizeof(uint16_t));
  }
}

void
menu_DelItem(uint8_t mode)
{
  if (MENU_ITEM_NONE == arg1.valid) {
    ERROR("Invalid Option");
    return;
  }

  uint16_t item_addr = flash_item_find(arg1.value.integer);
  if (0xFFFF == item_addr) {
    ERROR("Not Found");
    return;
  }
  uint8_t  ui2 = FlashReadByte(item_addr);
  ui2 |= ITEM_BYTE_DELETE_MASK;
  FlashWriteByte(item_addr, ui2);

  uint16_t item_count;
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&item_count, sizeof(uint16_t));
  item_count--;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&item_count, sizeof(uint16_t));
}

#if 0
void
menu_ModItem(uint8_t mode)
{
  uint8_t ui2, ui3;

  if (MENU_ITEM_NONE == arg1.valid) {
    ERROR("Invalid Option");
    return;
  }

  uint16_t item_addr = flash_item_find(arg1.value.integer);
  if (0xFFFF == item_addr) {
    ERROR("Not Found");
    return;
  }

  /* Obtain the full item */
  item *it = (void *) bufSS;
  for (ui2=0; ui2<ITEM_SIZEOF; ui2++) {
    bufSS[ui2] = FlashReadByte(item_addr+ui2);
  }

  /* insert the new one */
  uint16_t item_addr1 = flash_item_add((uint8_t *)it);
  if (0xFFFF == item_addr1) {
    ERROR("Can't add item");
  } else { /* update database */
    /* delete the old one */
    ui2 = bufSS[0] | ITEM_BYTE_DELETE_MASK;
    FlashWriteByte(item_addr, ui2);
  }
}
#endif

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
  menu_unimplemented();
}

void
menu_Header(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_Footer(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_DelAllBill(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_SetPasswd(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_SetServTax(uint8_t mode)
{
  menu_unimplemented();
}

void
menu_SetDateTime(uint8_t mode)
{
  menu_unimplemented();
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
  LCD_WR_LINE(0, 0, "Demo App FIXME");  /* FIXME: Need to display shop name in 0,0 */
  LCD_WR_LINE_N(1, 0, (menu_names+(menu_selected*MENU_NAMES_LEN)), MENU_NAMES_LEN);
  while KBD_NOT_HIT {
    DELAY(0xFF);
  }

  KBD_GET_KEY;
  KBD_RESET_KEY;
  if (KEY_SC_ENTER == key) {
    arg1.valid = MENU_ITEM_NONE;
    menu_getopt(menu_prompt_str+((menu_prompts[menu_selected<<1])<<2), &arg1, menu_args[(menu_selected<<1)]);
    arg2.valid = MENU_ITEM_NONE;
    menu_getopt(menu_prompt_str+((menu_prompts[(menu_selected<<1)+1])<<2), &arg2, menu_args[((menu_selected<<1)+1)]);
    (*menu_handlers)(menu_args[((menu_selected<<1)+1)]);
  } else if (KEY_SC_LEFT == key) {
    menu_selected--;
    key = MENU_MAX - 1; /* out of bound value */
  } else if (KEY_SC_RIGHT == key) {
    menu_selected++;
    key = 0; /* out of bound value */
  } else if (KEY_SC_PRINT == key) {
  } else {
    /* could be hotkey for menu */
    menu_selected *= 10;
    menu_selected += key;
  }

  /* handle out of bounds */
  if (menu_selected >= MENU_MAX)
    menu_selected = key;

  /* recursive call, capture the case when you have unexpected
     exit */
  goto menu_main_start;
}

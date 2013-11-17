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

uint8_t keyChars[] = {
  /* KCHAR_ROWS x KCHAR_COLS */
  '0', ' ', '.', ',', ')', '+', '?', '_', ':',
  '1', 'a', 'b', 'c', '!', 'A', 'B', 'C', '~',
  '2', 'd', 'e', 'f', '@', 'D', 'E', 'F', '{',
  '3', 'g', 'h', 'i', '#', 'G', 'H', 'I', '}',
  '4', 'j', 'k', 'l', '$', 'J', 'K', 'L', '[',
  '5', 'm', 'n', 'o', '%', 'M', 'N', 'O', ']',
  '6', 'p', 'q', 'r', '^', 'P', 'Q', 'R', '|',
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
#define MENU_STR1_IDX_NUM_ITEMS 15
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
static uint8_t MenuMode = MENU_MNORMAL;

void
menu_Init(void)
{
  MenuMode = KbdIsShiftPressed() ? MENU_MSUPER : MENU_MNORMAL;

  if (3 == sizeof(sale_item))
    ERROR("sale item not packed into 3 bytes");
  if (20 != sizeof(item))
    ERROR("item not packed in 20 bytes");
  if (4 != sizeof(sale_info))
    ERROR("item not packed in 20 bytes");
  if (0 != ((FLASH_ITEM_END-FLASH_ITEM_START)%sizeof(item)))
    ERROR("item type wrongly packed");
  
  assert(sizeof(billing) <= FLASH_SECTOR_SIZE);
  assert(ITEM_SIZEOF < (1<<8));
}

void
menu_unimplemented(void)
{
  assert(0);
}

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
  for (; bi->info.n_items < MAX_ITEMS_IN_BILL; bi->info.n_items++) {
    item_addr = flash_item_find(arg2.value.integer.i16);
    if ((MENU_ITEM_NONE == arg2.valid) || (FLASH_ADDR_INVALID == item_addr))
      break;

    /* */
    ui2 = bi->info.n_items;
    bi->items[ui2].item_id = arg2.value.integer.i16;
    bi->addrs[ui2] = item_addr;

    do {
      /* # items */
      arg2.valid = MENU_ITEM_NONE;
      menu_getopt(menu_str1+(MENU_STR1_IDX_MANY*MENU_PROMPT_LEN), &arg2, MENU_ITEM_ID);
    } while (MENU_ITEM_NONE == arg2.valid);

    /* */
    bi->items[ui2].num_sale  = (uint8_t) arg2.value.integer.i16;
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
}

void
menu_ShowBill(uint8_t mode)
{
  uint16_t sale_info;

  if ((MENU_PR_NONE == arg1.valid) || (MENU_PR_NONE == arg2.valid))
    return;

  sale_info = flash_sale_find(&(arg1.value.date.date), arg2.value.integer.i16);
  if (FLASH_ADDR_INVALID == sale_info) {
    ERROR("Not Found");
    return;
  }

  /* FIXME: First display before any operation */

  if ((mode&(~MENU_MODEMASK)) == MENU_MPRINT) {
    /* FIXME: print */
  } else if ((mode&(~MENU_MODEMASK)) == MENU_MDELETE) {
    //    FlashWriteByte(sale_info+(uint16_t)(&(SALE_INFO.deleted)), SALE_INFO_DELETED);
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
  it->cost = cost.value.integer.i16;
  it->discount = arg2.value.integer.i16;

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
  if (arg1.value.integer.i16 >= ITEM_MAX) {
    ERROR("Too Large Opt");
    return;
  }

  flash_item_delete(arg1.value.integer.i16);
}

static uint8_t
poppulate_billing(uint16_t recordp, uint8_t *buf)
{
  billing *bp = (void *)buf;
  uint8_t ui2, ui3, ui4;

  for (ui2=0; ui2<SALE_INFO_SIZEOF; ui2++) {
    bufSS[ui2] = FlashReadByte(recordp+ui2);
  }
  /* retrieve all data */
  for (ui3=0; ui3<(ITEM_SIZEOF*(bp->info.n_items)); ui3++) {
    bufSS[ui2] = FlashReadByte(recordp+ui2);
  }
  /* populate pointers */
  for (ui3=0; ui3<(bp->info.n_items); ui3++) {
    bp->addrs[ui3] = flash_item_find((uint16_t)(bp->items[ui3].item_id));
  }
}


void
menu_DayItemBill(uint8_t mode)
{
  uint8_t ui2, ui3;
  billing *bp = (void *)bufSS;

  uint16_t start_record = flash_sale_find((uint8_t *)&(arg1.value.date), 1);
  if (FLASH_ADDR_INVALID == start_record)
    return;

  uint16_t next_record = start_record;

  while (1) {
    ui3 = poppulate_billing(next_record, bufSS);

    if (bp->date_mm != arg1.value.date.month) {
      assert(start_record != next_record);
      return;
    }
    if (bp->date_dd == arg1.value.date.date) {
      /* print : could contain deleted record */
    } else {
      ui3 -= SALE_INFO_SIZEOF;
    }

    /* atleast one record, move past this */
    assert(start_record != next_record);
    next_record += ui3;
  }
}

void
menu_DayAllBill(uint8_t mode)
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
menu_ModVat(uint8_t mode)
{
  uint16_t ui1;
  uint8_t  ui2, ui3, choice[MENU_PROMPT_LEN*4], vat;

  if (MENU_ITEM_NONE == arg1.valid)
    return;

  for (ui2=0; ui2<4; ui2++) {
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.vat[ui2]), (uint8_t *)&ui1, sizeof(uint16_t));
    printf("choice ui1:%0d\n", (uint32_t) ui1);
    for (ui3=0; ui3<MENU_PROMPT_LEN; ui3++) {
      *(choice+(ui2*MENU_PROMPT_LEN)+MENU_PROMPT_LEN-1-ui3) = '0' + ui1%10;
      ui1 /= 10;
    }
  }
  ui3 = menu_getchoice(menu_str1+(MENU_STR1_IDX_REPLA*MENU_PROMPT_LEN), choice, 4);
  printf("Choice:%s\n", choice);
  assert(ui3 < 4);

  ui1 = arg1.value.integer.i8;
  ui1 <<= 16;
  ui1 |= arg1.value.integer.i16;
  printf("Obtained integer:%0d\n", (uint32_t)ui1);
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
	ui3 = (' ' == lcd_buf[1][ui2]) ? 1 : 0;
      }
      if (ui3) {
	for (ui2=0; (ui2<LCD_MAX_COL) && (chars<mode_max); ui2++) {
	  bufSS[ui2] = lcd_buf[1][ui2];
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
    ui2 = lcd_buf[1][ui1];
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
  uint16_t ui1;
  uint8_t  ui2, ui3, ui4;

  /* Run LCD
     Run slowly and make user see if there is any error
   */
  /* Run Printer : Print test page
   */
  /* Verify unused EEPROM spacesd : Write/readback
   */
  /* FIXME: randomize ui3 */
  for (ui2=0; ui2<NUM_TESTING_BYTES; ui2++) {
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.testing[ui2]), &ui3, sizeof(uint8_t)*LCD_MAX_COL);
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.testing[ui2]), &ui3, sizeof(uint8_t)*LCD_MAX_COL);
  }

  /* Verify Keypad : Ask user to press a key and display it
   */
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
      KBD_RESET_KEY;
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
    menu_error = 0;
  } else assert(0);

  /* */
  if (0 == menu_error) {
    arg->valid = item_type;
  } else if (opt & MENU_ITEM_OPTIONAL) {
  } else if ((opt & MENU_ITEM_DONTCARE_ON_PREV) && (arg == &arg2)) {
  } else
    assert(0);
}

uint8_t
menu_getchoice(uint8_t *quest, uint8_t *opt_arr, uint8_t max_idx)
{
  uint8_t ret = 0, key, key_n, key_s;

  do {
    assert(ret < max_idx);

    LCD_WR_LINE_N(1, 0, quest, MENU_PROMPT_LEN);
    LCD_WR(": ");
    LCD_WR_N((opt_arr+(ret*MENU_PROMPT_LEN)), MENU_PROMPT_LEN);

    while KBD_NOT_HIT {
      sleep(10);
    }
    KBD_GET_KEY;

    if (KEY_SC_LEFT == KbdData) {
      ret = (0==ret) ? max_idx-1 : ret-1;
    } else if (KEY_SC_RIGHT == KbdData) {
      ret = ((max_idx-1)==ret) ? 0 : ret+1;
    }

    if (KEY_SC_ENTER == KbdData) {
      KBD_RESET_KEY;
      return ret;
    }
    KBD_RESET_KEY;
  } while (1);
  assert (0);
}

void
menu_main(void)
{
  uint8_t menu_selected, key, key_n, key_s;

  /* initialize */
  menu_selected = 0;
  key = 0;

menu_main_start:
  assert(KBD_NOT_HIT); /* ensures kbd for user-inputs */

  /* Check if the new assignment is mode appropriate */
  if ( (0 == (MenuMode & (menu_mode[menu_selected] & MENU_MODEMASK))) ||
       (menu_selected >= MENU_MAX) ) {
    menu_selected = 0;
  }

  /* Wait until get command from user */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.prn_header[0]), bufSS, sizeof(uint8_t)*LCD_MAX_COL);
  LCD_WR_LINE_N(0, 0, bufSS, LCD_MAX_COL);
  LCD_WR_LINE_N(1, 0, (menu_names+(menu_selected*MENU_NAMES_LEN)), MENU_NAMES_LEN);
  while KBD_NOT_HIT {
    DELAY(0xFF);
  }

  KBD_GET_KEY;

  if (KEY_SC_ENTER == key) {
    KBD_RESET_KEY;
    arg1.valid = MENU_ITEM_NONE;
    LCD_CLRSCR;
    menu_getopt(menu_prompt_str+((menu_prompts[menu_selected<<1])<<2), &arg1, menu_args[(menu_selected<<1)]);
    assert (KBD_HIT);
    KBD_RESET_KEY;
    arg2.valid = MENU_ITEM_NONE;
    LCD_CLRSCR;
    menu_getopt(menu_prompt_str+((menu_prompts[(menu_selected<<1)+1])<<2), &arg2, menu_args[((menu_selected<<1)+1)]);
    (menu_handlers[menu_selected])(menu_mode[menu_selected]);
  } else if (KEY_SC_LEFT == key) {
    menu_selected = (0 == menu_selected) ? MENU_MAX : menu_selected-1;
  } else if (KEY_SC_RIGHT == key) {
    menu_selected = (menu_selected >= (MENU_MAX-1)) ? 0 : menu_selected+1;
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

  /* Clear Key press */
  assert (KBD_HIT);
  KBD_RESET_KEY;

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

  /* store that we've taken the spot */
  if ( 0 != (ui2 & (1<<(ui1&0x7))) ) {
    assert(0); /* should not be called */
    return;
  }
  item_last_modified = ui1;
  ui2 |= 1 << (item_last_modified&0x7);
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_valid[item_last_modified/8]), (uint8_t *)&ui2, sizeof(uint8_t));
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_last_modified), (uint8_t *)&item_last_modified, sizeof(uint16_t));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));
  ui1++;
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_count), (uint8_t *)&ui1, sizeof(uint16_t));

#if DEBUG
  /* Check if the space is empty */
  assert(ui1<ITEM_MAX);
  ui1 = flash_item_find(ui1);
  for (ui2=0; ui2<ITEM_SIZEOF; ui2++) {
    if (0 != ui1) break;
  }
  assert (ui2 == ITEM_SIZEOF);
#endif

  /* */
  for (ui2=0; ui2<ITEM_SIZEOF; ui2++, ui1++) {
    if (0 != byte_arr[0])
      FlashWriteByte(ui1, byte_arr[0]);
    byte_arr++;
  }
}

void
flash_item_delete(uint16_t id)
{
  uint16_t ui1, addr;
  uint8_t item_valid, ui2, ui3;
  uint8_t *block;

  /* do nothing if already invalid */
  assert(id<ITEM_MAX);
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_valid[id/8]), (uint8_t *)&item_valid, sizeof(uint8_t));
  if (0 == (item_valid & (1<<(addr%0x7))) ) {
    return;
  }

  /* input is id, compute addr */
  addr = flash_item_find(id);
  block = (uint8_t *) (addr & ~((uint16_t)(FLASH_SECTOR_SIZE-1)));

  /* Save the item to EEPROM */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.item_mod_his_ptr), (uint8_t *)&item_valid, sizeof(uint8_t));
  ui1 = (uint16_t)&(EEPROM_DATA.item_mod_his[item_valid]);
  for (ui2=0; ui2<ITEM_SIZEOF; ui1++, ui2++) {
    ui3 = FlashReadByte(addr+ui2);
    EEPROM_STORE_WRITE_NoSig(ui1, (uint8_t *)&ui3, sizeof(uint8_t));
  }
  item_valid++;
  EEPROM_STORE_WRITE_NoSig((uint16_t)&(EEPROM_DATA.item_mod_his_ptr), (uint8_t *)&item_valid, sizeof(uint8_t));
  EEPROM_STORE_WRITE_Sig;

  /* Modify lookups accordingly */
  {
    uint16_t sale_start, sale_end;
    uint8_t  n_bytes;
    billing *bi = (void *)bufSS;
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
    for (; sale_start!=sale_end;) {
      for (ui2=0; ui2<SALE_INFO_SIZEOF; ui2++)
	bufSS[ui2] = FlashReadByte(sale_start+ui2);
      n_bytes = SALE_INFO_SIZEOF + (bi->info.n_items * (uint16_t)SALE_SIZEOF);
      if (0 == (bi->info.deleted & SALE_INFO_DELETED)) {
	for (ui2=SALE_INFO_SIZEOF; ui2<n_bytes; ui2++)
	  bufSS[ui2] = FlashReadByte(sale_start+ui2);
	for (ui2=0; ui2<bi->info.n_items; ui2++) {
	  if (bi->items[ui2].item_id == id) {
	    /* it has modified items */
	    FlashWriteByte(sale_start+(uint16_t)(&(SALE_INFO.deleted)), SALE_INFO_MODIFIED);
	    break;
	  }
	}
      }
      sale_start += n_bytes;
      if (sale_start >= FLASH_DATA_END) {
	sale_start = FLASH_DATA_START + (sale_start % FLASH_SECTOR_SIZE);
      }
    }
  }

  /* save start block */
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    if ( ((block+ui1) < (uint8_t *)addr) || ((block+ui1) >= (uint8_t *)(addr+ITEM_SIZEOF)) )
      bufSS[ui1] = block[ui1];
    else
      bufSS[ui1] = 0;
  }
  FlashEraseSector((uint16_t)block);
  for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
    if (0 != bufSS[ui1])
      FlashWriteByte(block+ui1, bufSS[ui1]);
  }

  /* end block */
  ui1 = (uint16_t) block;
  block = (uint8_t *) ((addr+ITEM_SIZEOF-1) & ~((uint16_t)(FLASH_SECTOR_SIZE-1)));
  if ((uint8_t*)ui1 != block) {
    for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
      if ( (block+ui1) >= (uint8_t *)(addr+ITEM_SIZEOF) )
	bufSS[ui1] = block[ui1];
      else
	bufSS[ui1] = 0;
    }
    FlashEraseSector((uint16_t)block);
    for (ui1=0; ui1<FLASH_SECTOR_SIZE; ui1++) {
      if (0 != bufSS[ui1])
	FlashWriteByte(block+ui1, bufSS[ui1]);
    }
  }

  /* upate valid */
  item_valid &= ~(1 << (addr%0x7));
  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.item_valid[id/8]), (uint8_t *)&item_valid, sizeof(uint8_t));
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
  assert(sale_start_sector != sale_end_sector);

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
      /* Need to delete few sectors... on ToT */
      flash_sale_free_old_sector();
      /* */
      EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
      sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
    }
    FlashEraseSector(sale_newend_sector);
  }

  /* Bytes are available ..., just store */
  sale_index_it((sale_info *)sale, sale_end);
  for (ui2=0; ui2<nbytes; ui2++) {
    if (0 != sale[0])
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
      if ((del_month == si->date_mm) && (sale_start!=sale_end)) {
	sale_start += SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
	if (sale_start >= FLASH_DATA_END) {
	  sale_start = FLASH_DATA_START + (sale_start % FLASH_SECTOR_SIZE);
	}
	sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
      }
    } while(del_month == si->date_mm);
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  }
}

uint16_t
flash_sale_find(uint8_t *dmy, uint16_t id)
{
  sale_info  *si;
  uint16_t ui1, ui2, vptr;
  uint8_t idx;

  idx = (dmy[1] - 1) * 4;
  if (dmy[0] <= 7) {}
  else if (dmy[0] <= 14) idx ++;
  else if (dmy[0] <= 21) idx += 2;
  else idx += 3;

  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_ptr[idx]), (uint8_t *)&vptr, sizeof(uint16_t));
  si = (sale_info *)vptr;
  if (0 == si) return (void *)FLASH_ADDR_INVALID;

  for (ui1=0; ui1<id; vptr=(uint16_t)si) {
    uint8_t ui3[3];
    sale_info* si_t = ui3;
    ui3[0] = FlashReadByte((uint16_t)si);
    ui3[1] = FlashReadByte((uint16_t)(si+1));
    ui3[2] = FlashReadByte((uint16_t)(si+2));
    if (dmy[1] != si_t->date_mm)
      return (void *) FLASH_ADDR_INVALID;
    /* move past this record */
    si = (sale_info *)( ((uint8_t *)si) + SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF) );
    if (dmy[0] < si_t->date_dd) {
    } else if (dmy[0] == si_t->date_dd) {
      ui1++;
    } else { /* less # items for this day */
      return (void *) FLASH_ADDR_INVALID;
    }
  }

  /* */
  si = (sale_info *)vptr;
  if ((si->deleted) & SALE_INFO_DELETED)
    return (void *) FLASH_ADDR_INVALID;
  return vptr;
}

void
flash_sale_free_old_sector()
{
  uint16_t ui1, ui6;
  uint8_t ui2, ui4, ui5;
  uint16_t sale_start, sale_end;
  uint16_t sale_start_sector, sale_end_sector, sale_newstart_sector;

  /* Find the first reference to sale data */
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_old_ptr_month), (uint8_t *)&ui2, sizeof(uint8_t));
  ui4 = ui2 = (ui2-1)*4;
  do {
    ui4 %= 12*4;
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_ptr[ui4]), (uint8_t *)&ui1, sizeof(uint16_t));
    if (0 != ui1) break;
    ui4++;
  } while (ui2 != ui4);
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));
  sale_start_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_end), (uint8_t *)&sale_end, sizeof(uint16_t));
  sale_end_sector = sale_end & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
  assert(ui1 == sale_start);

  /* We just need to delete one sector of data */
  {
    uint8_t ui3[3];
    sale_info *si = (void *)ui3;
    for (sale_newstart_sector=sale_start_sector; sale_newstart_sector==sale_start_sector;) {
      ui3[0] = FlashReadByte(sale_start);
      if (sale_start!=sale_end) {
	sale_start += SALE_INFO_SIZEOF + (((ui3[0] & SALE_INFO_BYTE_NITEM_MASK) >> SALE_INFO_BYTE_NITEM_SHIFT) * SALE_SIZEOF);
	if (sale_start >= FLASH_DATA_END) {
	  sale_start = FLASH_DATA_START + (sale_start % FLASH_SECTOR_SIZE);
	}
	sale_newstart_sector = sale_start & ~((uint16_t) (FLASH_SECTOR_SIZE-1));
      }
    }
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_start), (uint8_t *)&sale_start, sizeof(uint16_t));

    /* Clear all month references before this month */
    ui3[0] = FlashReadByte(sale_start);
    ui3[1] = FlashReadByte(sale_start+1);
    ui3[2] = FlashReadByte(sale_start+2);
    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_old_ptr_month), (uint8_t *)&ui2, sizeof(uint8_t));
    if (ui2 != si->date_mm) { /* we have deleted few months of data */
      ui1 = 0;
      for (ui2=INCR_MONTH(ui2); ui2!=si->date_mm; ui2=INCR_MONTH(ui2)) {
	ui5 = (ui2-1)*4;
	for (ui4=0; ui4<4; ui4++) {
	  EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_date_ptr[ui5+ui4]), (uint8_t *)&ui1, sizeof(uint16_t));
	}
      }
      /* move pointers */
      ui2 = si->date_mm;
      ui2 = (ui2-1)*4;
      for (ui1=0, ui6=0, ui4=0, ui5=8; ui4<4; ui4++, ui5+=7) {
	if (si->date_dd < ui5) {
	  if (0 == ui1)
	    ui1 = sale_start;
	  else
	    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_ptr[ui2+ui4]), (uint8_t *)&ui1, sizeof(uint16_t));
	}
	EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_date_old_ptr[ui4]), (uint8_t *)&ui1, sizeof(uint16_t));
	EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_date_ptr[ui2+ui4]), (uint8_t *)&ui6, sizeof(uint16_t));
      }
      assert(0 != ui1);
    } else { /* we have deleted < 1 month of data */
      for (ui1=0, ui6=0, ui4=0, ui5=8; ui4<4; ui4++, ui5+=7) {
	if (si->date_dd < ui5) {
	  if (0 == ui1)
	    ui1 = sale_start;
	  else
	    EEPROM_STORE_READ((uint16_t)&(EEPROM_DATA.sale_date_old_ptr[ui4]), (uint8_t *)&ui1, sizeof(uint16_t));
	}
	EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_date_old_ptr[ui4]), (uint8_t *)&ui1, sizeof(uint16_t));
      }
      assert(0 != ui1);
    }
    ui2 = si->date_mm;
    EEPROM_STORE_WRITE((uint16_t)&(EEPROM_DATA.sale_date_old_ptr_month), (uint8_t *)&ui2, sizeof(uint8_t));
  }
}

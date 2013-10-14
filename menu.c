#include <stdint.h>
#include <p89v51rd2.h>

#include "menu.h"

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_MODE(A)
#define MENU_NAME(A) A
#define MENU_FUNC(A)
#define ARG1(A, B)
#define ARG2(A, B)
char *menu_names = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  MENU_MODE
#undef  ROW_JOIN
#undef  COL_JOIN

typedef void menu_func_t(uint8_t mode);

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

char    menu_prompt_str[] = MENU_PROMPTS;

menu_arg_t arg1, arg2;
uint8_t    menu_error;

void
menu_getopt(uint8_t *prompt, menu_arg_t *arg, uint8_t opt)
{
  uint8_t key, key_n, key_s, col_id;
  if (MENU_ITEM_NONE == opt) return;

  /* init */
  for (col_id=0; col_id<LCD_MAX_COL; col_id++)
    arg->str[col_id] = 0;
  
  /* Ask a question */
  LCD_WR_LINE(0, 0, prompt, 4);
  LCD_WR(0, 5, '?', 1);

  /* Set the prompt */
  LCD_cmd(LCD_CMD_CUR_20);
  LCD_WR_LINE(0, 0, prompt, 0);
  col_id = 0;
  uint8_t *lp = &(lcd_buf[1][0]);

  /* Get a string */
  do {
    while KBD_NOT_HIT {
      sleep(10);
    }
    KBD_GET_KEY;

    /* abc def ghi
       jkl mno pqr
       stu vwx yz */
    switch (KbdHit) {
    case KEY_SC_LEFT:
      if (col_id == 0) {
	break;
      }
      col_id--; lp--;
      lp[0] = ' ';
      LCD_cmd(LCD_CMD_CUR_20|col_id);
      break;
    case KEY_SC_ENTER:
      lp[0] = 0;
      col_id++;
      break;
    default:
      if ((0 == key_s) && (0 == key_n))
	lp[0] = '0' + key;
      else if (0 == key_s)
	lp[0] = 'a' + key + key_n - 1;
      else
	lp[0] = 'A' + key + key_n - 1;
      lp++; col_id++;
    }
  } while ((col_id < LCD_MAX_COL) &&  (key != KEY_SC_ENTER));

  uint8_t item_type = (opt&MENU_ITEM_TYPE_MASK);
  uint8_t *lbp = &(lcd_buf[1][0]);
  uint32_t val = 0;

  menu_error = 0;
  if ((MENU_ITEM_NUM == item_type) || (MENU_ITEM_FLOAT == item_type)) {
    while (lbp[0]) {
      if ((lbp[0] >= '0') && (lbp[0] <= '9')) {
	val *= 10;
	val += lbp[0] - '0';
      } else {
	menu_error++;
      }
      lbp++;
    }
    arg->integer = val;
  } else if ((MENU_ITEM_DATE == item_type) || (MENU_ITEM_MONTH == item_type)) {
    /* format DDMMYYYY || format MMYYYY */
    uint8_t ui1;
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
	}
	if ((0==val) || (val > 31)) menu_error++;
	arg->date.day = val;
      }
      /* Month */
      val = 0;
      for (ui1=0; ui1<2; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
      }
      if ((0 == val) || (val > 12)) menu_error++;
      arg->date.month = val;
      /* Year */
      val = 0;
      for (ui1=0; ui1<4; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
      }
      if ((val < 2000) || (val > 2100)) menu_error++;
      arg->date.year = val-2000;
    }
  } else if (MENU_ITEM_TIME == item_type) {
    /* format HHMM */
    uint8_t ui1;
    for (ui1=0; ui1<4; ui1++) {
      if ((lbp[ui1] < '0') || (lbp[ui1] > '9'))
	menu_error++;
    }
    if (0 == menu_error) {
      /* Hour */
      for (ui1=0; ui1<2; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
      }
      if (val>23) menu_error++;
      arg->time.hour = val;
      /* Mins */
      val = 0;
      for (ui1=0; ui1<2; ui1++) {
	val *= 10;
	val += lbp[0] - '0';
      }
      if (val > 59) menu_error++;
      arg->time.min = val;
    }
  } else if (MENU_ITEM_STR == item_type) {
  } else ASSERT(0);

  /* */
  if (menu_error) {
    ASSERT(0 == menu_error);
  }
}

void
menu_main(void)
{
  uint8_t menu_selected, key, key_n, key_s;

  /* initialize */
  menu_selected = 0;

menu_main_start:
  /* Wait until get command from user */
  LCD_WR_LINE(0, 0, "Demo App FIXME");  /* FIXME: Need to shop company name in 0,0 */
  LCD_WR_LINE(1, 0, menu_names[menu_selected], MENU_NAMES_LEN);
  while KBD_NOT_HIT {
    DELAY(0xFF);
  }

  KBD_GET_KEY;
  KBD_RESET_KEY;
  if (KEY_SC_ENTER == key) {
    menu_getopt(menu_prompt_str+((menu_prompts[menu_selected<<1])<<2), &arg1, menu_args[(menu_selected<<1)]);
    menu_getopt(menu_prompt_str+((menu_prompts[(menu_selected<<1)+1])<<2), &arg2, menu_args[((menu_selected<<1)+1)]);
    (*menu_handlers)();
  } else if (KEY_SC_LEFT == key) {
    menu_selected--;
    key = MENU_MAX - 1;
  } else if (KEY_SC_RIGHT == key) {
    menu_selected++;
    key = 0;
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

void
menu_Billing(uint8_t mode)
{
}

void
menu_ShowBill(uint8_t mode)
{
}

void
menu_DupBill(uint8_t mode)
{
}

void
menu_DeleteBill(uint8_t mode)
{
}

void
menu_ValidatePaswd(uint8_t mode)
{
}

void
menu_AddItem(uint8_t mode)
{
}

void
menu_ModItem(uint8_t mode)
{
}

void
menu_DayItemBill(uint8_t mode)
{
}

void
menu_DayAllBill(uint8_t mode)
{
}

void
menu_DayBillPrned(uint8_t mode)
{
}

void
menu_DayDupBill(uint8_t mode)
{
}

void
menu_DayTaxReport(uint8_t mode)
{
}

void
menu_MonItemBill(uint8_t mode)
{
}

void
menu_MonAllBill(uint8_t mode)
{
}

void
menu_MonBillPrned(uint8_t mode)
{
}

void
menu_MonTaxReport(uint8_t mode)
{
}

void
menu_AllItemBill(uint8_t mode)
{
}

void
menu_AllFullBill(uint8_t mode)
{
}

void
menu_TaxReport(uint8_t mode)
{
}

void
menu_ModVat(uint8_t mode)
{
}

void
menu_Header(uint8_t mode)
{
}

void
menu_Footer(uint8_t mode)
{
}

void
menu_DelAllBill(uint8_t mode)
{
}

void
menu_SetPasswd(uint8_t mode)
{
}

void
menu_SetServTax(uint8_t mode)
{
}

void
menu_SetDateTime(uint8_t mode)
{
}

void
menu_RunDiag(uint8_t mode)
{
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

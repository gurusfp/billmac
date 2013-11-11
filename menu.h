#ifndef MENU_H
#define MENU_H

/* Type of Menu argument */
#define MENU_ITEM_NONE                 0
#define MENU_ITEM_ID                   1
#define MENU_ITEM_DATE                 2
#define MENU_ITEM_STR                  3
#define MENU_ITEM_MONTH                4
#define MENU_ITEM_FLOAT                5
#define MENU_ITEM_TIME                 6
#define MENU_ITEM_TYPE_MASK         0x1F
#define MENU_ITEM_OPTIONAL         (1<<7)
#define MENU_ITEM_DONTCARE_ON_PREV (1<<6)
#define MENU_ITEM_PASSWD           (1<<5)

/* Prompt for Menu argument */
#define MENU_PROMPT_LEN 5
#define MENU_PROMPTS \
  "Id   " /* 0 */    \
  "Name " /* 1 */    \
  "Passw" /* 2 */    \
  "Month" /* 3 */    \
  "Time " /* 4 */    \
  "Date " /* 5 */    \
  "Decim" /* 6 */

#define MENU_PR_NONE   0
#define MENU_PR_ID     1
#define MENU_PR_NAME   2
#define MENU_PR_PASS   3
#define MENU_PR_MONTH  4
#define MENU_PR_TIME   5
#define MENU_PR_DATE   6
#define MENU_PR_FLOAT  7

typedef struct {
  union {
    struct {
      /* Total 24 bit max */
      uint8_t  i8;  /* msb */
      uint16_t i16;
    } integer;
    struct {
      uint8_t date;
      uint8_t month;
      uint8_t year;
    } date;
    struct {
      uint8_t min;
      uint8_t hour;
    } time;
  } value;
  uint8_t   valid;
} menu_arg_t;

#define MENU_NAMES_LEN 12

#define MENU_MSUPER    0xC0
#define MENU_MNORMAL   0x40
#define MENU_MRESET    0x80
#define MENU_MPRINT    0x01
#define MENU_MDELETE   0x02
#define MENU_MVALIDATE 0x03
#define MENU_MFOOTER   0x04
#define MENU_MODEMASK  0xE0

#define MENU_ITEMS \
  MENU_MODE(MENU_MNORMAL)  MENU_NAME("Billing     ") COL_JOIN MENU_FUNC(menu_Billing) COL_JOIN \
    ARG1(MENU_PR_ID, MENU_ITEM_NONE) COL_JOIN ARG2(MENU_PR_NAME, MENU_ITEM_STR|MENU_ITEM_OPTIONAL|MENU_ITEM_DONTCARE_ON_PREV) ROW_JOIN \
  MENU_MODE(MENU_MNORMAL)  MENU_NAME("Show Bills  ") COL_JOIN MENU_FUNC(menu_ShowBill) COL_JOIN \
    ARG1(MENU_PR_DATE,  MENU_ITEM_DATE|MENU_ITEM_OPTIONAL) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_ID|MENU_ITEM_OPTIONAL) ROW_JOIN \
  MENU_MODE(MENU_MNORMAL|MENU_MPRINT)  MENU_NAME("Dup Bill    ") COL_JOIN MENU_FUNC(menu_ShowBill) COL_JOIN \
    ARG1(MENU_PR_DATE,  MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_ID) ROW_JOIN \
  MENU_MODE(MENU_MNORMAL|MENU_MDELETE)  MENU_NAME("Delete Bill ") COL_JOIN MENU_FUNC(menu_ShowBill) COL_JOIN \
    ARG1(MENU_PR_DATE,  MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_ID) ROW_JOIN \
  MENU_MODE(MENU_MNORMAL|MENU_MVALIDATE)  MENU_NAME("ValidatePass") COL_JOIN MENU_FUNC(menu_SetPasswd) COL_JOIN \
    ARG1(MENU_PR_PASS,  MENU_ITEM_STR|MENU_ITEM_PASSWD) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Add Item    ") COL_JOIN MENU_FUNC(menu_AddItem) COL_JOIN \
    ARG1(MENU_PR_NAME, MENU_ITEM_STR) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Delete Item ") COL_JOIN MENU_FUNC(menu_DelItem) COL_JOIN \
    ARG1(MENU_PR_ID, MENU_ITEM_ID) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  /* MENU_MODE(MENU_MSUPER) MENU_NAME("Modify Item ") COL_JOIN MENU_FUNC(menu_ModItem) COL_JOIN */ \
  /*   ARG1(MENU_PR_ID, MENU_ITEM_ID) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN */ \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Day ItemBill") COL_JOIN MENU_FUNC(menu_DayItemBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Day FullBill") COL_JOIN MENU_FUNC(menu_DayAllBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  /* MENU_MODE(MENU_MSUPER) MENU_NAME("DayBillPrned") COL_JOIN MENU_FUNC(menu_DayBillPrned) COL_JOIN */ \
  /*  ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN */ \
  /* MENU_MODE(MENU_MSUPER) MENU_NAME("Day Dup Bill") COL_JOIN MENU_FUNC(menu_DayDupBill) COL_JOIN */ \
  /*  ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN */ \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Day Tax Repo") COL_JOIN MENU_FUNC(menu_DayTaxReport) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Mon ItemBill") COL_JOIN MENU_FUNC(menu_MonItemBill) COL_JOIN \
    ARG1(MENU_PR_MONTH , MENU_ITEM_MONTH) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Mon FullBill") COL_JOIN MENU_FUNC(menu_MonAllBill) COL_JOIN \
    ARG1(MENU_PR_MONTH, MENU_ITEM_MONTH) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  /* MENU_MODE(MENU_MSUPER) MENU_NAME("MonBillPrned") COL_JOIN MENU_FUNC(menu_MonBillPrned) COL_JOIN */ \
  /*  ARG1(MENU_PR_MONTH, MENU_ITEM_MONTH) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN */ \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Mon Tax Repo") COL_JOIN MENU_FUNC(menu_MonTaxReport) COL_JOIN \
    ARG1(MENU_PR_MONTH, MENU_ITEM_MONTH) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("All ItemBill") COL_JOIN MENU_FUNC(menu_AllItemBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_DATE, MENU_ITEM_DATE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("All FullBill") COL_JOIN MENU_FUNC(menu_AllFullBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_DATE, MENU_ITEM_DATE) ROW_JOIN \
  /* MENU_MODE(MENU_MSUPER) MENU_NAME("All Tax Repo") COL_JOIN MENU_FUNC(menu_TaxReport) COL_JOIN */ \
  /*  ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_DATE, MENU_ITEM_DATE) ROW_JOIN */ \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Modify VAT  ") COL_JOIN MENU_FUNC(menu_ModVat) COL_JOIN \
    ARG1(MENU_PR_FLOAT, MENU_ITEM_FLOAT) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Set Header  ") COL_JOIN MENU_FUNC(menu_Header) COL_JOIN \
    ARG1(MENU_PR_NAME, MENU_ITEM_STR) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER|MENU_MFOOTER) MENU_NAME("Set Footer  ") COL_JOIN MENU_FUNC(menu_Header) COL_JOIN \
    ARG1(MENU_PR_NAME, MENU_ITEM_STR) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Del All Bill") COL_JOIN MENU_FUNC(menu_DelAllBill) COL_JOIN \
    ARG1(MENU_PR_ID, MENU_ITEM_NONE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MRESET) MENU_NAME("Set Password") COL_JOIN MENU_FUNC(menu_SetPasswd) COL_JOIN \
    ARG1(MENU_PR_PASS, MENU_ITEM_STR) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Set Serv Tax") COL_JOIN MENU_FUNC(menu_SetServTax) COL_JOIN \
    ARG1(MENU_PR_FLOAT, MENU_ITEM_FLOAT) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Set DateTime") COL_JOIN MENU_FUNC(menu_SetDateTime) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_TIME, MENU_ITEM_TIME) ROW_JOIN \
  MENU_MODE(MENU_MRESET) MENU_NAME("Run Diagnost") COL_JOIN MENU_FUNC(menu_RunDiag) COL_JOIN \
    ARG1(MENU_PR_ID, MENU_ITEM_NONE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE)

#define KCHAR_ROWS        10
#define KCHAR_COLS         9
#define KCHAR_SHIFT_SZ     5
extern uint8_t keyChars[];
extern uint8_t menu_error;
extern uint8_t bufSS[];

void menu_getopt(uint8_t *prompt, menu_arg_t *arg, uint8_t opt);
void menu_Billing(uint8_t mode);
void menu_ShowBill(uint8_t mode);
void menu_AddItem(uint8_t mode);
void menu_DelItem(uint8_t mode);
void menu_DayItemBill(uint8_t mode);
void menu_DayAllBill(uint8_t mode);
void menu_DayBillPrned(uint8_t mode);
void menu_DayDupBill(uint8_t mode);
void menu_DayTaxReport(uint8_t mode);
void menu_MonItemBill(uint8_t mode);
void menu_MonAllBill(uint8_t mode);
void menu_MonTaxReport(uint8_t mode);
void menu_AllItemBill(uint8_t mode);
void menu_AllFullBill(uint8_t mode);
void menu_ModVat(uint8_t mode);
void menu_Header(uint8_t mode);
void menu_DelAllBill(uint8_t mode);
void menu_SetPasswd(uint8_t mode);
void menu_SetServTax(uint8_t mode);
void menu_SetDateTime(uint8_t mode);
void menu_RunDiag(uint8_t mode);
void menu_getopt(uint8_t *prompt, menu_arg_t *arg, uint8_t opt);
uint8_t menu_getchoice(uint8_t *quest, uint8_t *opt_arr, uint8_t max_idx);
void menu_main(void);

void flash_item_add(uint8_t* byte_arr);
void flash_item_delete(uint16_t id);

void flash_sale_add(uint8_t *sale);
void flash_sale_delete_month(uint8_t del_month);
uint16_t flash_sale_find(uint8_t *dmy, uint16_t id);

#define flash_item_find(id) ((id<ITEM_MAX) ? (FLASH_ITEM_START + (id * ITEM_SIZEOF)) : FLASH_ADDR_INVALID)

#endif

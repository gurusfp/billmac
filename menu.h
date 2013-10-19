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
#define MENU_PROMPTS \
  "Id  " /* 0 */    \
  "Name" /* 1 */    \
  "Pass" /* 2 */    \
  "Mont" /* 3 */    \
  "Time" /* 4 */    \
  "Date" /* 5 */    \
  "Deci" /* 6 */

#define MENU_PR_ID     0
#define MENU_PR_NAME   1
#define MENU_PR_PASS   2
#define MENU_PR_MONTH  3
#define MENU_PR_TIME   4
#define MENU_PR_DATE   5
#define MENU_PR_FLOAT  6

typedef union {
  uint16_t integer;
  struct {
    uint8_t day;
    uint8_t month;
    uint8_t year;
  } date;
  struct {
    uint8_t hour;
    uint8_t min;
  } time;
} menu_arg_t;

#define MENU_NAMES_LEN 12

#define MENU_MSUPER   0xC0
#define MENU_MNOR     0x40
#define MENU_MRESET   0x20
#define MENU_MODEMASK 0xE0

#define MENU_ITEMS \
  MENU_MODE(MENU_MNOR)  MENU_NAME("Billing     ") COL_JOIN MENU_FUNC(menu_Billing) COL_JOIN \
    ARG1(MENU_PR_ID, MENU_ITEM_ID) COL_JOIN ARG2(MENU_PR_NAME, MENU_ITEM_STR|MENU_ITEM_OPTIONAL|MENU_ITEM_DONTCARE_ON_PREV) ROW_JOIN \
  MENU_MODE(MENU_MNOR)  MENU_NAME("Show Bills  ") COL_JOIN MENU_FUNC(menu_ShowBill) COL_JOIN \
    ARG1(MENU_PR_DATE,  MENU_ITEM_DATE|MENU_ITEM_OPTIONAL) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_ID|MENU_ITEM_OPTIONAL) ROW_JOIN \
  MENU_MODE(MENU_MNOR)  MENU_NAME("Dup Bill    ") COL_JOIN MENU_FUNC(menu_DupBill) COL_JOIN \
    ARG1(MENU_PR_DATE,  MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_ID) ROW_JOIN \
  MENU_MODE(MENU_MNOR)  MENU_NAME("Delete Bill ") COL_JOIN MENU_FUNC(menu_DeleteBill) COL_JOIN \
    ARG1(MENU_PR_DATE,  MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_ID) ROW_JOIN \
  MENU_MODE(MENU_MNOR)  MENU_NAME("ValidatePass") COL_JOIN MENU_FUNC(menu_ValidatePaswd) COL_JOIN \
    ARG1(MENU_PR_PASS,  MENU_ITEM_STR|MENU_ITEM_PASSWD) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Add Item    ") COL_JOIN MENU_FUNC(menu_AddItem) COL_JOIN \
    ARG1(MENU_PR_NAME, MENU_ITEM_STR) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Modify Item ") COL_JOIN MENU_FUNC(menu_ModItem) COL_JOIN \
    ARG1(MENU_PR_ID, MENU_ITEM_ID) COL_JOIN ARG2(MENU_PR_NAME, MENU_ITEM_STR|MENU_ITEM_OPTIONAL|MENU_ITEM_DONTCARE_ON_PREV) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Day ItemBill") COL_JOIN MENU_FUNC(menu_DayItemBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Day FullBill") COL_JOIN MENU_FUNC(menu_DayAllBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
										 /* MENU_MODE(MENU_MSUPER) MENU_NAME("DayBillPrned") COL_JOIN MENU_FUNC(menu_DayBillPrned) COL_JOIN */ \
										 /*  ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN */ \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Day Dup Bill") COL_JOIN MENU_FUNC(menu_DayDupBill) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
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
  MENU_MODE(MENU_MSUPER) MENU_NAME("All Tax Repo") COL_JOIN MENU_FUNC(menu_TaxReport) COL_JOIN \
    ARG1(MENU_PR_DATE, MENU_ITEM_DATE) COL_JOIN ARG2(MENU_PR_DATE, MENU_ITEM_DATE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Modify VAT  ") COL_JOIN MENU_FUNC(menu_ModVat) COL_JOIN \
    ARG1(MENU_PR_FLOAT, MENU_ITEM_FLOAT) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Set Header  ") COL_JOIN MENU_FUNC(menu_Header) COL_JOIN \
    ARG1(MENU_PR_NAME, MENU_ITEM_STR) COL_JOIN ARG2(MENU_PR_ID, MENU_ITEM_NONE) ROW_JOIN \
  MENU_MODE(MENU_MSUPER) MENU_NAME("Set Footer  ") COL_JOIN MENU_FUNC(menu_Footer) COL_JOIN \
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

extern uint8_t keyChars[];
extern uint8_t menu_error;

#endif

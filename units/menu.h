#ifndef MENU_H
#define MENU_H

#define MENU_ITEM_NONE 0
#define MENU_ITEM_NUM  1
#define MENU_ITEM_DATE 2
#define MENU_ITEM_STR  3

#define MENU_ITEMS \
  MENU_NAME("Billing     ") COL_JOIN MENU_FUNC(menu_Billing) COL_JOIN \
    ARG1("Id", MENU_ITEM_NUM) COL_JOIN ARG2("Name", MENU_ITEM_STR) ROW_JOIN \
  MENU_NAME("Dup Bill    ") COL_JOIN MENU_FUNC(menu_DupBill) COL_JOIN \
    ARG1("Date", MENU_ITEM_DATE) COL_JOIN ARG2("Id", MENU_ITEM_ID) ROW_JOIN \
  MENU_NAME("ValidatePass") COL_JOIN MENU_FUNC(menu_ValidatePaswd) COL_JOIN \
    ARG1("Pass", MENU_ITEM_STR) COL_JOIN ARG2("", MENU_ITEM_NONE) ROW_JOIN \
  MENU_NAME("Add Item    ") COL_JOIN MENU_FUNC(menu_AddItem) COL_JOIN \
    ARG1("Name", MENU_ITEM_STR) COL_JOIN ARG2("", MENU_ITEM_NONE) ROW_JOIN \
  MENU_NAME("Modify Item ") COL_JOIN MENU_FUNC(menu_ModItem) COL_JOIN \
    ARG1(, ) COL_JOIN ARG2(, MENU_ITEM_NONE) ROW_JOIN \
  MENU_NAME("Day ItemBill") COL_JOIN MENU_FUNC(menu_DayItemBill) COL_JOIN \
    ARG1("Date", ) COL_JOIN ARG2(, ) ROW_JOIN \
  MENU_NAME("Day All Bill") COL_JOIN MENU_FUNC(menu_DayAllBill) COL_JOIN \
    ARG1(, ) COL_JOIN ARG2(, ) ROW_JOIN \
  MENU_NAME("DayBillPrned") COL_JOIN MENU_FUNC(menu_DayBillPrned) COL_JOIN \
    ARG1(, ) COL_JOIN ARG2(, ) ROW_JOIN \
  MENU_NAME("Day Dup Bill") COL_JOIN MENU_FUNC(menu_DayDupBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Day Tax Rep ") COL_JOIN MENU_FUNC(menu_DayTaxReport) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Mon ItemBill") COL_JOIN MENU_FUNC(menu_MonItemBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Mon All Bill") COL_JOIN MENU_FUNC(menu_MonAllBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("MonBillPrned") COL_JOIN MENU_FUNC(menu_MonBillPrned) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Mon Tax Rep ") COL_JOIN MENU_FUNC(menu_MonTaxReport) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("All ItemBill") COL_JOIN MENU_FUNC(menu_AllItemBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("All FullBill") COL_JOIN MENU_FUNC(menu_AllFullBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("All Tax Rep ") COL_JOIN MENU_FUNC(menu_TaxReport) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Add VAT     ") COL_JOIN MENU_FUNC(menu_AddVat) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Modify VAT  ") COL_JOIN MENU_FUNC(menu_ModVat) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Set Header  ") COL_JOIN MENU_FUNC(menu_Header) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Set Footer  ") COL_JOIN MENU_FUNC(menu_Footer) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Delete Bill ") COL_JOIN MENU_FUNC(menu_DelBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Del All Bill") COL_JOIN MENU_FUNC(menu_DelAllBill) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Set Password") COL_JOIN MENU_FUNC(menu_SetPasswd) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Set Serv Tax") COL_JOIN MENU_FUNC(menu_SetServTax) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0) ROW_JOIN \
  MENU_NAME("Set DateTime") COL_JOIN MENU_FUNC(menu_SetDateTime) COL_JOIN \
    ARG1(, 1) COL_JOIN ARG2(, 0)

#define ROW_JOIN +
#define COL_JOIN
#define MENU_NAME(A)
#define MENU_FUNC(A) 1
#define ARG1(A, B)
#define ARG2(A, B)
#define MENU_MAX ( MENU_ITEMS )
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  ROW_JOIN
#undef  COL_JOIN

#endif

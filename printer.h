#ifndef PRINTER_H
#define PRINTER_H

#define PRINTER_PAPER_STATUS        0x1B76
#define PRINTER_PRINT_FEED          0x1B4A
#define PRINTER_EMPHASIZE_ONOFF     0x1B45
#define PRINTER_INITIALIZE          0x1B40 /* -n */
#define PRINTER_UNDERLINE_ONOFF     0x1B2D
#define PRINTER_SLEEP_TIME          0x1B38 /* n1+n2*256 */
#define PRINTER_WAKE                0xFF
#define PRINTER_PRINT_TEST_PAGE     0x1254
#define PRINTER_PAPER_CUT           0x1B69
#define PRINTER_PAPER_PARTIAL_CUT   0x1B6D

#define PRINTER_PRINT(c) \
  uart_write(c)

extern void printer_init(void);

#endif

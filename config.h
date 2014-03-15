#ifndef CONFIG_H
#define CONFIG_H

/* Toplevel features those are configurable ...
   go into this file... Choose the options before
   compilation.
   Detailed configurations should go into the feature
   specific file.
 */

/* LCD could be used in 8-bit or 4-bit mode */
//#define LCD_DPORT_SIZE            8
#define LCD_DPORT_SIZE            4

/* FLASH configurations : only one of the below defines
   should be 1.
 */
#define SALEandITEM_IN_FLASH      0
#define ITEMonly_IN_FLASH         0
#define nostore_IN_FLASH          1

/* EEPROM configurations for 24c512 */
#define EEPROM_HOWMANY            2 /* upto 4 * 512bit */
#define EEPROM_SIZE               (uint32_t)(((uint32_t)512)<<(10-3))
#define EEPROM_ADDR_LEN           16 /* for 24C512 */
#define EEPROM_ADDR_MAX           ((uint32_t)(EEPROM_HOWMANY*EEPROM_SIZE))

#define  ITEM_MAX                 2000

#endif

#ifndef I2C_H
#define I2C_H

void i2cInit(void);
void i2cWriteBytes(uint16_t addr, uint8_t *data, uint8_t n_bytes);
void i2cReadBytes(uint16_t addr, uint8_t *data, uint8_t n_bytes);

#define ACK      0
#define NACK     1

#define SCL P2_0
#define SDA P2_1

#define EEPROM_CTRL_WRITE 0xA0
#define EEPROM_CTRL_READ  0xA1
#define TIMER_CTRL_WRITE  0xD0
#define TIMER_CTRL_READ   0xD1

#define I2C_ADDR_IS_1BYTE 0x8000

#define EEPROM_SIZE (1<<13)

#define TIMER_ADDR_MIN    1
#define TIMER_ADDR_HOUR   2
#define TIMER_ADDR_DATE   4
#define TIMER_ADDR_MONTH  5
#define TIMER_ADDR_YEAR   6

#define NOP2 \
  __asm	     \
  NOP	     \
  NOP	     \
  __endasm

#define NOP  \
  __asm	     \
  NOP	     \
  __endasm

#define I2C_START_BIT	 \
  SDA = 0;		 \
  NOP;			 \
  SCL = 0;		 \
  NOP

#define I2C_STOP_BIT	 \
  SDA = 0;		 \
  NOP;			 \
  SCL = 1;		 \
  NOP;			 \
  SDA = 1

#define I2C_GET_ACK(a)	   \
  SDA = 1;		   \
  SCL = 1;		   \
  NOP;			   \
  a = SDA;		   \
  SCL = 0

#define I2C_SEND_ACK(a)	   \
  SDA = a;		   \
  SCL = 1;		   \
  NOP;			   \
  SCL = 0

#define I2C_SEND_BYTE(b)    \
  ui1=8;		    \
  do {			    \
    ui1--;		    \
    SDA = b >> ui1;	    \
    SCL=1;		    \
    NOP;		    \
    SCL=0;		    \
  } while (ui1>0)

#define I2C_GET_BYTE(b)     \
  b = 0;		    \
  ui1=8;		    \
  do {			    \
    SCL=1;		    \
    ui1--;		    \
    b <<= 1;		    \
    NOP;		    \
    b |= SDA;		    \
    SCL=0;		    \
  } while (ui1>0)

#endif

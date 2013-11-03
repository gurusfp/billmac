#include "i2c.h"

#ifndef UNIT_TEST
#error  "This file should not be used in non-Unit test mode"
#endif

uint8_t i2c_ymd[3], i2c_hm[2];

#define NIBBLE_PACK(A, B) ((A<<4)|B)
#define I2C_SIZE (1<<10)
uint8_t i2c_bytes[I2C_SIZE];

void
i2cInit(void)
{
  i2c_ymd[0] = NIBBLE_PACK(0, 1);
  i2c_ymd[1] = NIBBLE_PACK(0, 1);
  i2c_ymd[2] = NIBBLE_PACK(1, 4);

  uint16_t ui1;
  for (ui1=0; ui1<I2C_SIZE; ui1++)
    i2c_bytes[ui1] = 0xBD;
}

void
i2cWriteBytes(uint16_t addr, uint8_t *data, uint8_t n_bytes)
{
  assert (addr < I2C_SIZE);
  for (; n_bytes>0; n_bytes--) {
    i2c_bytes[addr] = data[0];
    addr++, data++;
  }
}

void
i2cReadBytes(uint16_t addr, uint8_t *data, uint8_t n_bytes)
{
  assert (addr < I2C_SIZE);
  for (; n_bytes>0; n_bytes--) {
    data[0] = i2c_bytes[addr];
    addr++, data++;
  }
}

void
timerDateSet(uint8_t year, uint8_t month, uint8_t date)
{
  i2c_ymd[0] = date;
  i2c_ymd[1] = month;
  i2c_ymd[2] = year;
}

void
timerDateGet(uint8_t *ymd)
{
  ymd[0] = i2c_ymd[0];
  ymd[1] = i2c_ymd[1];
  ymd[2] = i2c_ymd[2];
}

void
timerTimeSet(uint8_t hour, uint8_t min)
{
  i2c_hm[0] = min;
  i2c_hm[1] = hour;
}

void
timerTimeGet(uint8_t *hm)
{
  hm[0] = i2c_hm[0];
  hm[1] = i2c_hm[1];
}

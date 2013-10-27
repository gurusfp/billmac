#include <stdint.h>
#include <p89v51rd2.h>

#include "i2c.h"

void
i2cInit(void)
{
  SDA = 1;
  SCL = 1;
}

/* (addr & 0x8000) : Addr is 8-bits */
void
i2cWriteBytes(uint16_t addr, uint8_t *data, uint8_t n_bytes)
{
  uint8_t ui1;
  bit     ack;

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(EEPROM_CTRL_WRITE);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* send addr */
  if ((uint16_t)0 == (addr & I2C_ADDR_IS_1BYTE)) {
    ui1 = addr>>8;
    I2C_SEND_BYTE(ui1);
    I2C_GET_ACK(ack);
    assert(ACK == ack);
  }
  ui1 = addr;
  I2C_SEND_BYTE(ui1);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  /* */
  for (; n_bytes>0; n_bytes--) {
    I2C_SEND_BYTE(data[0]);
    data ++;
    I2C_GET_ACK(ack);
    assert(ACK == ack);
  }

  I2C_STOP_BIT;
}

void
i2cReadBytes(uint16_t addr, uint8_t *data, uint8_t n_bytes)
{
  uint8_t ui1, ui2;
  bit     ack;

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(EEPROM_CTRL_WRITE);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* send addr */
  I2C_SEND_BYTE((uint8_t)addr);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(EEPROM_CTRL_READ);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* */
  for (; n_bytes>1; n_bytes--) {
    I2C_GET_BYTE(ui2);
    I2C_SEND_ACK(ACK);
    data[0] = ui2;
    data++;
  }
  I2C_GET_BYTE(data);
  I2C_SEND_ACK(NACK);
  data[0] = ui2;
  data++;

  I2C_STOP_BIT;
}

void
timerDateSet(uint8_t year, uint8_t month, uint8_t date)
{
  uint8_t ui1;

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(TIMER_CTRL_WRITE);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* send addr */
  I2C_SEND_BYTE((uint8_t)TIMER_ADDR_DATE);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_SEND_BYTE(date);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_SEND_BYTE(month);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_SEND_BYTE(year);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_STOP_BIT;
}

void
timerDateGet(uint8_t *ymd)
{
  uint8_t ui1, ui2;

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(TIMER_CTRL_WRITE);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* send addr */
  I2C_SEND_BYTE((uint8_t)TIMER_ADDR_DATE);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(TIMER_CTRL_READ);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  I2C_GET_BYTE(ui2);
  I2C_SEND_ACK(ACK);
  ymd[0] = ui2;

  I2C_GET_BYTE(ui2);
  I2C_SEND_ACK(ACK);
  ymd[1] = ui2;

  I2C_GET_BYTE(ui2);
  I2C_SEND_ACK(NACK);
  ymd[2] = ui2;

  I2C_STOP_BIT;
}

void
timerTimeSet(uint8_t hour, uint8_t min)
{
  uint8_t ui1;

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(TIMER_CTRL_WRITE);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* send addr */
  I2C_SEND_BYTE((uint8_t)TIMER_ADDR_MIN);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_SEND_BYTE(min);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_SEND_BYTE(hour);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  I2C_STOP_BIT;
}

void
timerTimeGet(uint8_t *hm)
{
  uint8_t ui1, ui2;

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(TIMER_CTRL_WRITE);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  /* send addr */
  I2C_SEND_BYTE((uint8_t)TIMER_ADDR_MIN);
  I2C_GET_ACK(ack);
  assert(ACK == ack);

  do {
    I2C_START_BIT;
    I2C_SEND_BYTE(TIMER_CTRL_READ);
    I2C_GET_ACK(ack);
  } while (NACK == ack);

  I2C_GET_BYTE(ui2);
  I2C_SEND_ACK(ACK);
  hm[1] = ui2;

  I2C_GET_BYTE(ui2);
  I2C_SEND_ACK(NACK);
  hm[0] = ui2;

  I2C_STOP_BIT;
}

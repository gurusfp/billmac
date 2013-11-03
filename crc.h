#ifndef CRC_H
#define CRC_H

extern uint8_t CRC16_High, CRC16_Low;

void CRC16_Init(void);
void CRC16_Update4Bits(uint8_t val);

#define CRC16_Update(val) do {	     \
  CRC16_Update4Bits( val >> 4 );     \
  CRC16_Update4Bits( val & 0x0F );   \
} while (0)

#endif

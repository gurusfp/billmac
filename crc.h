#ifndef CRC_H
#define CRC_H

extern uint8_t CRC16_High, CRC16_Low;

void CRC16_Init(void);
void CRC16_Update4Bits(uint8_t val);

#define CRC16_Update(val) do {	     \
  CRC16_Update4Bits( val >> 4 );     \
  CRC16_Update4Bits( val & 0x0F );   \
} while (0)

#define rand8(val)				\
  /* taps: 8 6 5 4; feedback polynomial: x^8 + x^6 + x^5 + x^4 + 1 */	\
  (val >> 1) | (((val << 7) ^ (val << 5) ^ (val << 4) ^ (val << 2)) & 0x80);

#endif

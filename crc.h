#ifndef CRC_H
#define CRC_H

extern uint8_t CRC16_High, CRC16_Low;

void CRC16_Init(void);
void CRC16_Update4Bits(uint8_t val);
void CRC16_Update(uint8_t val);

#endif

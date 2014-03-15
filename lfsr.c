#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t lfsr = 0xE1;
    uint8_t bit;
    uint32_t period = 0;

    do
    {
        /* taps: 8 6 5 4; feedback polynomial: x^8 + x^6 + x^5 + x^4 + 1 */
        bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
        lfsr =  (lfsr >> 1) | (bit << 15);
        ++period;
    } while(lfsr != 0xE1u);

    printf("period : %d\n", period);
 
    return 0;
}

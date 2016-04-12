#include "utils.h"

void delay_ms(uint16_t t) {
    uint16_t cycles = t/10;
    for (uint16_t x = 0; x < cycles; ++x) {
        __delay_ms(10);
    }
    for (uint16_t x = 0; x < t; ++x) {
        __delay_ms(1);
    }
}

void delay_us(uint16_t t) {
// manually calculated looking at asm code
#define overhead_us ((7 + 17 * 5) / 8 + 1)
// balance the main & else branches, so that the overhead is the same
#define else_cycles (1)
    if (t >= overhead_us) t -= overhead_us;
    if (t & (1 << 0)) { __delay_us(1 << 0); } else { _delay(else_cycles); }
    if (t & (1 << 1)) { __delay_us(1 << 1); } else { _delay(else_cycles); }
    if (t & (1 << 2)) { __delay_us(1 << 2); } else { _delay(else_cycles); }
    if (t & (1 << 3)) { __delay_us(1 << 3); } else { _delay(else_cycles); }
    if (t & (1 << 4)) { __delay_us(1 << 4); } else { _delay(else_cycles); }
    if (t & (1 << 5)) { __delay_us(1 << 5); } else { _delay(else_cycles); }
    if (t & (1 << 6)) { __delay_us(1 << 6); } else { _delay(else_cycles); }
    if (t & (1 << 7)) { __delay_us(1 << 7); } else { _delay(else_cycles); }
    if (t & (1 << 8)) { __delay_us(1 << 8); } else { _delay(else_cycles); }
    if (t & (1 << 9)) { __delay_us(1 << 9); } else { _delay(else_cycles); }
    if (t & (1 << 10)) { __delay_us(1 << 10); } else { _delay(else_cycles); }
    if (t & (1 << 11)) { __delay_us(1 << 11); } else { _delay(else_cycles); }
    if (t & (1 << 12)) { __delay_us(1 << 12); } else { _delay(else_cycles); }
    if (t & (1 << 13)) { __delay_us(1 << 13); } else { _delay(else_cycles); }
    if (t & (1 << 14)) { __delay_us(1 << 14); } else { _delay(else_cycles); }
    // x2
    if (t & (1 << 15)) { __delay_us(1 << 14); } else { _delay(else_cycles); }
    if (t & (1 << 15)) { __delay_us(1 << 14); } else { _delay(else_cycles); }
#undef overhead_us
#undef else_cycles
}

uint8_t parity(uint8_t data) {
    uint8_t ones = 0;

    while (data != 0) {  // until all bits tested
        if (data & 0x01) {  // if bit is 1
            ones++;
        }
        data = data << 1;  // go to next bit
    }

    // if 'ones' is odd, least significant bit will be 1
    return (ones & 0x01);
}

uint16_t rand(void) {
    static uint16_t lfsr = 0xACE1;
    static bool flag = 0;

    flag = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr = (lfsr >> 1) | (flag << 15);
}

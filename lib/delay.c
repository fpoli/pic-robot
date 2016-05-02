#include "delay.h"

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

#include "utils.h"

void inline delay_ms(uint16_t t) {
    uint16_t cycles = t/10;
    for (uint16_t x = 0; x < cycles; ++x) {
        __delay_ms(10);
    }
    for (uint16_t x = 0; x < t; ++x) {
        __delay_ms(1);
    }
}

void inline delay_us(uint16_t t) {
#define sistematic_error_us 42  // somehow measured
#define else_clocks (4 * 5)     // somehow measured
    if (t >= sistematic_error_us) t -= sistematic_error_us;
    if (t & (1 << 0)) __delay_us(1 << 0); else _delay(else_clocks);
    if (t & (1 << 1)) __delay_us(1 << 1); else _delay(else_clocks);
    if (t & (1 << 2)) __delay_us(1 << 2); else _delay(else_clocks);
    if (t & (1 << 3)) __delay_us(1 << 3); else _delay(else_clocks);
    if (t & (1 << 4)) __delay_us(1 << 4); else _delay(else_clocks);
    if (t & (1 << 5)) __delay_us(1 << 5); else _delay(else_clocks);
    if (t & (1 << 6)) __delay_us(1 << 6); else _delay(else_clocks);
    if (t & (1 << 7)) __delay_us(1 << 7); else _delay(else_clocks);
    if (t & (1 << 8)) __delay_us(1 << 8); else _delay(else_clocks);
    if (t & (1 << 9)) __delay_us(1 << 9); else _delay(else_clocks);
    if (t & (1 << 10)) __delay_us(1 << 10); else _delay(else_clocks);
    if (t & (1 << 11)) __delay_us(1 << 11); else _delay(else_clocks);
    if (t & (1 << 12)) __delay_us(1 << 12); else _delay(else_clocks);
    if (t & (1 << 13)) __delay_us(1 << 13); else _delay(else_clocks);
    if (t & (1 << 14)) __delay_us(1 << 14); else _delay(else_clocks);
    // x 2
    if (t & (1 << 15)) __delay_us(1 << 14); else _delay(else_clocks);
    if (t & (1 << 15)) __delay_us(1 << 14); else _delay(else_clocks);
#undef sistematic_error_us
#undef else_clocks
}

uint8_t parity(uint8_t data) {
	uint8_t ones = 0;

	while (data != 0) { // until all bits tested
		if(data & 0x01) { // if bit is 1
			ones++;
		}
		data = data << 1; // go to next bit
	}

	// if num_of_ones is odd, least significant bit will be 1

	return (ones & 0x01); 
}

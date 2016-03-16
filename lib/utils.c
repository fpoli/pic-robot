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
    uint16_t cycles = t/20;
    for (uint16_t x = 0; x < cycles; ++x) {
        __delay_us(20);
    }
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

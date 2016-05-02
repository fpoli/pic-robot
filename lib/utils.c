#include "utils.h"

bool parity(uint8_t data) {
    uint8_t ones = 0;

    while (data != 0) {  // until all bits tested
        if (data & 0x01) {  // if bit is 1
            ones++;
        }
        data = data << 1;  // go to next bit
    }

    // if 'ones' is odd, least significant bit will be 1
    return ones & 0x01;
}

uint16_t rand(void) {
    static uint16_t lfsr = 0xACE1;
    static bool flag = 0;

    flag = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr = (lfsr >> 1) | (flag << 15);
}

float constrainf(float value, float min, float max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

uint16_t constrain(uint16_t value, uint16_t min, uint16_t max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

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

// Return random float x: min < x <= max
float rand_float(float min, float max) {
    return (1.0 * rand() / RAND_MAX * (max - min)) + min;
}

// Return random uint16_t x: 0 <= x < max
uint16_t rand_int(uint16_t max) {
    return (uint16_t)(rand_float(0, max));
}

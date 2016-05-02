#ifndef LIB_UTILS_H_
#define LIB_UTILS_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

bool parity(uint8_t);

float constrainf(float value, float min, float max);

uint16_t constrain(uint16_t value, uint16_t min, uint16_t max);

// Return random float x: min < x <= max
float rand_float(float min, float max);

// Return random uint16_t x: 0 <= x < max
uint16_t rand_int(uint16_t max);

#define RAD_TO_DEG (1.0 * 180 / M_PI)
#define DEG_TO_RAD (1.0 / 180 * M_PI)

#endif  // LIB_UTILS_H_

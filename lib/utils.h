#ifndef LIB_UTILS_H_
#define LIB_UTILS_H_

#include <stdint.h>
#include <stdbool.h>

bool parity(uint8_t);

uint16_t rand(void);

#define RAND_MAX 65535

float constrainf(float value, float min, float max);

uint16_t constrain(uint16_t value, uint16_t min, uint16_t max);

#endif  // LIB_UTILS_H_

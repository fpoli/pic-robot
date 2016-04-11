#ifndef LIB_UTILS_H_
#define LIB_UTILS_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"

void delay_ms(uint16_t);

void delay_us(uint16_t);

uint8_t parity(uint8_t);

uint16_t rand(void);

#define RAND_MAX 65535

#endif  // LIB_UTILS_H_

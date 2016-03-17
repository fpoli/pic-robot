#ifndef UTILS_H
#define	UTILS_H

#include <xc.h>
#include <stdint.h>
#include "config.h"

void inline delay_ms(uint16_t);

void inline delay_us(uint16_t);

uint8_t parity(uint8_t);

#endif	/* UTILS_H */

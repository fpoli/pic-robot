#ifndef UART_H
#define	UART_H

#include <stdint.h>
#include <stdio.h>
#include <usart.h>
#include <delays.h>
#include "config.h"

#ifndef UART_BAUD
#error "uart.h library requires the definition of UART_BAUD"
#endif

void uart_init(void);

void uart_write_byte(uint8_t data);

void putch(uint8_t data);

#endif	/* UART_H */

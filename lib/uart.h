#ifndef USART_IO_H
#define	USART_IO_H

#include <stdint.h>
#include <stdio.h>
#include <usart.h>
#include <delays.h>
#include "config.h"

void uart_init(void);

void uart_write_byte(uint8_t data);

void putch(uint8_t data);

#endif	/* USART_IO_H */

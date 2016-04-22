#ifndef LIB_CONFIG_H_
#define LIB_CONFIG_H_

// Frequency of the clock used by __delay_ms()
#define _XTAL_FREQ 32000000

// baud rate used by uart_io.h
#define UART_BAUD 115200

// Pin used by sound.h
#define SPEAKER_PIN LATBbits.LATB3
#define SPEAKER_TRIS TRISBbits.TRISB3

#define LED_PIN LATBbits.LATB3
#define LED_TRIS LATBbits.LATB3

//#define DEBUG

// NOTE: Configuration bits are not writable in order to protect the bootloader.
// So, it is useless to set them

#endif  // LIB_CONFIG_H_

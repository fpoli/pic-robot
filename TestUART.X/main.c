#include <stdio.h>
#include <stdint.h>
#include <xc.h>
#include <usart.h>
#include "../lib/config.h"
#include "../lib/utils.h"

void init(void) {
    // Initialize data direction of PORTB registers
    // (pic18f45k22 data sheet, section 10.2, page 140)
    TRISBbits.TRISB3 = 0;  // output

    // Initialize UART module
    TRISCbits.RC6 = 1;  // TX pin set as output
    TRISCbits.RC7 = 1;  // RX pin set as input
    ANSELCbits.ANSC6 = 0;  // Disable analog input
    ANSELCbits.ANSC7 = 0;  // Disable analog input

    Close1USART();
    baud1USART(
        BAUD_8_BIT_RATE &
        BAUD_AUTO_OFF &
        BAUD_IDLE_CLK_LOW
    );
    Open1USART(
        USART_TX_INT_OFF &
        USART_RX_INT_OFF &
        USART_ASYNCH_MODE &
        USART_EIGHT_BIT &
        USART_CONT_RX &
        USART_BRGH_HIGH,
        16
    );
}

void main(void) {
    init();

    LATBbits.LATB3 = 0b1;

    // It is recommended that the main() function does not end
    // (XC8 manual, section 5.10, page 209)
    while (1) {
        putch('H');
        putch('e');
        putch('l');
        putch('l');
        putch('o');
        putch(' ');
        putch('W');
        putch('o');
        putch('r');
        putch('l');
        putch('d');
        putch('!');
        putch('\n');

        delay_ms(200);
        LATBbits.LATB3 = !LATBbits.LATB3;
    }
}

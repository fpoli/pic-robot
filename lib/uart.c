#include "uart.h"

void uart_init(void) {
    // Initialize UART module
    TRISCbits.RC6 = 1; // TX pin set as output
    TRISCbits.RC7 = 1; // RX pin set as input
    ANSELCbits.ANSC6 = 0; // Disable analog input
    ANSELCbits.ANSC7 = 0; // Disable analog input

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
        // 32 MHz, 115200 baud/s --> value 16
        (_XTAL_FREQ / _UART_BAUD / 16 - 1)
    );
}

void uart_write_byte(uint8_t data) {
    while(Busy1USART());
    putc1USART(data);
}

void putch(uint8_t data) {
    uart_write_byte(data);
}

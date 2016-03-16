#include <stdint.h>
#include <xc.h>
#include <usart.h>
#include "../lib/config.h"
#include "../lib/utils.h"
#include "../lib/sound.h"

void init(void) {
    // Initialize data direction of PORTB registers
    // (pic18f45k22 data sheet, section 10.2, page 140)
    TRISBbits.TRISB3 = 0;  // output
}

void main(void) {
    init();

    LATBbits.LATB3 = 0b1; // LED on

    Note imperial_march[] = {
        {NOTE_A4, 1./4},
        {NOTE_A4, 1./4},
        {NOTE_A4, 1./4},
        {NOTE_F4, 3./16},
        {NOTE_C5, 1./16},

        {NOTE_A4, 1./4},
        {NOTE_F4, 3./16},
        {NOTE_C5, 1./16},
        {NOTE_A4, 1./2},

        {NOTE_E5, 1./4},
        {NOTE_E5, 1./4},
        {NOTE_E5, 1./4},
        {NOTE_F5, 3./16},
        {NOTE_C5, 1./16},

        {NOTE_GS4, 1./4},
        {NOTE_F4, 3./16},
        {NOTE_C5, 1./16},
        {NOTE_A4, 1./2}
    };

    // It is recommended that the main() function does not end
    // (XC8 manual, section 5.10, page 209)
    while(1) {
        play_melody(imperial_march, sizeof(imperial_march)/sizeof(Note), 2000);

        delay_ms(2000);
        play_sound(880, 100);
        delay_ms(100);
        play_sound(880, 100);
        delay_ms(2000);
    }
}

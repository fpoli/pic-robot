#include <stdint.h>
#include <xc.h>
#include <usart.h>
#include "../lib/config.h"
#include "../lib/utils.h"
#include "../lib/sound.h"
#include "../lib/melodies.h"

void init(void) {
    // Initialize data direction of PORTB registers
    // (pic18f45k22 data sheet, section 10.2, page 140)
    TRISBbits.TRISB3 = 0;  // output
}

void main(void) {
    init();

    LATBbits.LATB3 = 0b1; // LED on

    play_sound(220,  1000);
    play_sound(440,  1000);
    play_sound(880,  1000);
    play_sound(1760, 1000);
    delay_ms(1000);

    Melody melody = get_imperial_march_melody();
    play_melody(melody.notes, melody.length, melody.period);

    //melody = get_birthday_melody();
    play_melody(melody.notes, melody.length, melody.period);

    // It is recommended that the main() function does not end
    // (XC8 manual, section 5.10, page 209)
    while(1) {
        play_sound(400 + (rand() / 65), 50);
    }
}

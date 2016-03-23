#include "melodies.h"

Melody get_imperial_march_melody() {
    // From http://www.tocapartituras.com/2011/07/la-marcha-imperial-de-star-wars.html
    static Note imperial_march[] = {
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
        {NOTE_A4, 1./2},

        {NOTE_A5, 1./4},
        {NOTE_A4, 3./16},
        {NOTE_A4, 1./16},
        {NOTE_A5, 1./4},
        {NOTE_GS5, 3./16},
        {NOTE_G5, 1./16},

        {NOTE_FS5, 1./16},
        {NOTE_F5, 1./16},
        {NOTE_FS5, 1./8},
        {SILENCE, 1./8},
        {NOTE_AS4, 1./8},
        {NOTE_DS5, 1./4},
        {NOTE_D5, 3./16},
        {NOTE_CS5, 1./16},

        {NOTE_C5, 1./16},
        {NOTE_B4, 1./16},
        {NOTE_C5, 1./8},
        {SILENCE, 1./8},
        {NOTE_F4, 1./8},
        {NOTE_GS4, 1./4},
        {NOTE_F4, 3./16},
        {NOTE_GS4, 1./16},

        {NOTE_C5, 1./4},
        {NOTE_A4, 3./16},
        {NOTE_C5, 1./16},
        {NOTE_E5, 1./2}
    };
    Melody melody = {
        imperial_march,
        sizeof(imperial_march)/sizeof(Note),
        2000
    };
    return melody;
}

Melody get_birthday_melody() {
    static Note birthday[] = {
        {NOTE_D4, 1./4},
        {NOTE_D4, 1./8},
        {NOTE_E4, 1./8},
        {NOTE_D4, 1./4},
        {NOTE_G4, 1./4},
        {NOTE_FS4, 1./4},
        {SILENCE, 1./4},
        {NOTE_D4, 1./4},
        {NOTE_D4, 1./8},
        {NOTE_E4, 1./8},
        {NOTE_D4, 1./4},
        {NOTE_A4, 1./4},
        {NOTE_G4, 1./4},
        {SILENCE, 1./4}
    };
    Melody melody = {
        birthday,
        sizeof(birthday)/sizeof(Note),
        2000
    };
    return melody;
}

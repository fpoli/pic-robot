#include "sound.h"

void play_sound(uint16_t frequency, uint16_t duration_ms) {
    uint16_t cycles = (uint16_t)(1.0 * duration_ms * frequency / 1000);
    uint16_t half_period = (uint16_t)(500000.0 / frequency);
    for (uint16_t x = 0; x < cycles; ++x) {
        delay_us(half_period);
        SPEAKER_PIN = 1;
        delay_us(half_period);
        SPEAKER_PIN = 0;
    }
}

void play_note(Note n, uint16_t period_duration_ms) {
    const float silence = 0.3;
    if (n.frequency == 0) {
        delay_ms((uint16_t)(period_duration_ms * n.duration));
    } else {
        play_sound(n.frequency, (uint16_t)(period_duration_ms * n.duration * (1 - silence)));
        delay_ms((uint16_t)(period_duration_ms * n.duration * silence));
    }
}


void play_melody(const Note *notes, uint16_t length, uint16_t period_duration_ms) {
    for (uint16_t i = 0; i < length; ++i) {
        play_note(notes[i], period_duration_ms);
    }
}

void play_dot_dash_data(uint16_t data, uint16_t duration_ms) {
    uint16_t dot_freq = 880;
    uint16_t dash_freq = 440;

    for (uint16_t x = 0; x < 16; ++x) {
        // bit 1 --> dot, bit 0 --> dash
        if (data & 0x8000) {
            play_sound(dot_freq, duration_ms);
        } else {
            play_sound(dash_freq, 2 * duration_ms);
        }
        delay_ms(duration_ms);
        data = data << 1;  // go to next bit
    }
}

void play_ok(void) {
    play_sound(440 * 4, 250);
    delay_ms(250);
}

void play_error(void) {
    play_sound(440, 500);
    delay_ms(250);
    play_sound(440, 500);
    delay_ms(250);
}

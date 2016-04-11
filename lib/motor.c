#include "motor.h"

void motor_init(void) {
    // Using port D, RD1 pin for PWM
    TRISD = 0;
    // Timer initialization
    T2CON = 0b00000100;
    CCPTMRS0 = 0b00000000;
    // PWM period (32 us - 31250 Hz with 32 MHz oscillator)
    PR2 = 255;
    // Set CCP1 module in PWM mode
    CCP4CON = 0b00001100;
    // Set 10-bit resolution PWM pulse width to max
    CCPR4L = 0b11111111;
    CCP4CONbits.DC4B = 0b11;
    // Enable PWM
    TRISDbits.RD1 = 0;

    // Left for digital pins setup (if needed)

    // Set pins RD2, RD3 as digital
    // ANSELDbits.ANSD2 = 0; // read p. 154
}

void motor_set_pwm(uint16_t power) {
    // Set PWM pulse width bits (10 bit resolution)
    CCPR4L = power >> 2;  // 8 bit here
    CCP4CONbits.DC4B = power % 4;  // 2 bit here

    // Enable
    TRISD = 0;
}

void motor_set_dir(uint8_t dir) {
    // Using port D, RD2, RD3 pin for PWM
    // Low  dir bit sets RD2
    // High dir bit sets RD3
    LATDbits.LATD3 = dir % 2;
    LATDbits.LATD2 = (dir >> 1) % 2;
    PORTDbits.RD3 = 0;
    PORTDbits.RD2 = 0;
}

#include "motor.h"

void motor_init(void) {
    // Use port D as output
    // RD0 pin for standby
    // RD1 pin for PWM
    // RD2 and RD3 pin for direction
    TRISD = 0;
    // Clear port D
    PORTD = 0;

    // Timer initialization
    T2CON = 0b00000100;
    CCPTMRS0 = 0b00000000;
    // PWM period (32 us - 31250 Hz with 32 MHz oscillator)
    PR2 = 255;
    // Set CCP1 module in PWM mode
    CCP4CON = 0b00001100;

    // Set 10-bit resolution PWM pulse width to zero
    CCPR4L = 0b00000000;
    CCP4CONbits.DC4B = 0b00;

    // Set standby pin up
    LATDbits.LATD0 = 1;
}

void motor_set_pwm(uint16_t power) {
    // Set PWM pulse width bits (10 bit resolution)
    CCPR4L = power >> 2;  // 8 bit here
    CCP4CONbits.DC4B = power & 0x0003;  // 2 bit here
}

void motor_set_dir(uint8_t dir) {
    // Using port D, RD2, RD3 pin for PWM
    // Low  dir bit sets RD2
    // High dir bit sets RD3
    LATDbits.LATD2 = dir & 0x01;
    LATDbits.LATD3 = (dir >> 1) & 0x01;
}

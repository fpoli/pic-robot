#include <stdint.h>
#include <xc.h>
#include "../lib/config.h"
#include "../lib/utils.h"
#include "../lib/mpu6050.h"
#include "../lib/uart.h"
#include "../lib/sound.h"

void init(void) {
    // Initialize LED/SOUND pin as output
    TRISBbits.TRISB3 = 0;
    play_ok();

    // Initialize UART module
    uart_init();
    printf("\n");
    printf("[UART] module initialized\n");

    // Initialize I2C
    printf("[MPU6050] Initializing I2C...\n");
    MPU6050_init_I2C();
    printf("[MPU6050] Testing I2C...\n");
    printf("[MPU6050] If it hangs here remove the power supply and retry.\n");
    MPU6050_test_I2C();
    printf("[MPU6050] I2C is ok\n");

    // Configure sensor
    printf("[MPU6050] Configuring sensor...\n");
    MPU6050_configure();
    printf("[MPU6050] Done.\n");
}

void main(void) {
    init();

    Accel_regs accel, old_accel;
    int16_t old_accel_x = 0, old_accel_y = 0, old_accel_z = 0;
    int16_t accel_x, accel_y, accel_z;
    float theta;

    printf("[MAIN] Start main loop\n");
    while (1) {
        // Read registers from sensor
        MPU6050_read(ACCEL_REGS, (uint8_t*) &accel, ACCEL_REGS_SIZE);

        // Build 16 bit little endian values
        accel_x = (accel.xout_h << 8) + accel.xout_l;
        accel_y = (accel.yout_h << 8) + accel.yout_l;
        accel_z = (accel.zout_h << 8) + accel.zout_l;

        theta = ((float)accel_y) / ((float)accel_z);

        printf("accel: %+6d, %+6d, %+6d  tetha: %+6d\n",
            accel_x, accel_y, accel_z,
            (int16_t)(theta * 1000)
        );

        // Notify new data
        if (accel_x != old_accel_x ||
            accel_y != old_accel_y ||
            accel_z != old_accel_z) {
            LATBbits.LATB3 = !LATBbits.LATB3;
            old_accel_x = accel_x;
            old_accel_y = accel_y;
            old_accel_z = accel_z;
        }
    }

    // It is recommended that the main() function does not end
    while (1) play_error();
}

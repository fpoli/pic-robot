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
    static bit data_changed = 0;
    int16_t accel_x, accel_y, accel_z;
    float theta;

    printf("[MAIN] Start main loop\n");
    while (1) {
        // Read registers from sensor
        MPU6050_read(ACCEL_REGS, (uint8_t*) &accel, ACCEL_REGS_SIZE);

        data_changed =
                accel.xout_h != old_accel.xout_h ||
                accel.xout_l != old_accel.xout_l ||
                accel.yout_h != old_accel.yout_h ||
                accel.yout_l != old_accel.yout_l ||
                accel.zout_h != old_accel.zout_h ||
                accel.zout_l != old_accel.zout_l;

        // If the data did not change, read again
        if (!data_changed) continue;
        old_accel = accel;

        // Build 16 bit little endian values
        accel_x = (accel.xout_h << 8) + accel.xout_l;
        accel_y = (accel.yout_h << 8) + accel.yout_l;
        accel_z = (accel.zout_h << 8) + accel.zout_l;

        theta = ((float)accel_y) / ((float)accel_z);

        // Report new data
        printf("accel: %+6d, %+6d, %+6d  tetha: %+6d\n",
            accel_x, accel_y, accel_z,
            (int16_t)(theta * 1000)
        );

        // Notify end of loop cycle
        LATBbits.LATB3 = !LATBbits.LATB3;
    }

    // It is recommended that the main() function does not end
    while (1) play_error();
}

/*******************************************************************************
 * Copyright (C) 2023 by Jithendra H S
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Jithendra H S and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/
/**
 * @file mpu6050.c
 * @brief Source file for MPU6050 sensor communication and data processing.
 * @author Jithendra H S
 * @date December 15, 2023
*/
#include "mpu6050.h"
#include "stdint.h"
#include "math.h"
#include <stdio.h>

// MPU6050 I2C address
#define MPU6050_ADDRESS (0x68)

/**
 * @brief Resets the MPU6050 device.
 *
 * This function sends the necessary I2C command to reset the MPU6050 device.
 *
 * @param i2c The I2C instance to use for communication.
 */
void resetMPU6050(i2c_inst_t *i2c) {
    // Data to send for the reset
    uint8_t resetData[] = {0x6B, 0x00};
    
    // Write the reset data to the MPU6050 device
    i2c_write_blocking(i2c, MPU6050_ADDRESS, resetData, sizeof(resetData), false);
}


/**
 * @brief Reads accelerometer data from the MPU6050 device.
 *
 * This function reads accelerometer data from the MPU6050 device over I2C.
 *
 * @param i2c The I2C instance to use for communication.
 * @param accel An array to store the accelerometer data [X, Y, Z].
 */
void readAccelData(i2c_inst_t *i2c, int16_t accel[3]) {
    // Buffer to store raw accelerometer data
    uint8_t buffer[6];
    
    // Register address for accelerometer data
    uint8_t regAddress = 0x3B;

    // Write the register address to initiate reading
    i2c_write_blocking(i2c, MPU6050_ADDRESS, &regAddress, 1, true);

    // Read accelerometer data from the MPU6050 device
    i2c_read_blocking(i2c, MPU6050_ADDRESS, buffer, sizeof(buffer), false);

    // Combine high and low bytes for each axis and store in the accel array
    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }
}

/**
 * @brief Calculates the tilt angle in degrees based on accelerometer values.
 *
 * This function calculates the tilt angle in degrees using the arctangent function.
 *
 * @param accel_value The accelerometer value to calculate the tilt angle for.
 * @return The calculated tilt angle in degrees.
 */
float calculate_tilt_angle(int16_t accel_value) {
    // Calculate the tilt angle in degrees using arctangent
    return atan2(accel_value, 16384.0) * (180.0 / M_PI);
}

/**
 * @brief Reads accelerometer data and calculates tilt angles.
 *
 * This function reads accelerometer data from the MPU6050 device, calculates tilt
 * angles for each axis, and prints the results.
 *
 * @param acceleration An array to store the accelerometer data [X, Y, Z].
 */
void read_tilt_angle(int16_t acceleration[3]){
    // Read accelerometer data from the MPU6050 device
    readAccelData(i2c1, acceleration);

    // Calculate tilt angles for each axis and store the results in the array
    acceleration[0] = (int16_t)calculate_tilt_angle(acceleration[0]) * 2;
    acceleration[1] = (int16_t)calculate_tilt_angle(acceleration[1]) * 2;
    acceleration[2] = (int16_t)calculate_tilt_angle(acceleration[2]) * 2;

    // Print the calculated tilt angles for X and Y axes
    printf("Acc. X = %d, Y = %d\n", acceleration[0], acceleration[1]);
}

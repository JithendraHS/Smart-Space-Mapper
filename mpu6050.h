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
 * @file mpu6050.h
 * @brief Function declaration for MPU6050 sensor communication and data processing.
 * @author Jithendra H S
 * @date December 15, 2023
*/
#include "hardware/i2c.h"

/**
 * @brief Resets the MPU6050 device.
 *
 * This function sends the necessary I2C command to reset the MPU6050 device.
 *
 * @param i2c The I2C instance to use for communication.
 */
void resetMPU6050(i2c_inst_t *i2c);

/**
 * @brief Reads accelerometer data from the MPU6050.
 *
 * This function reads raw accelerometer data from the MPU6050 and stores it in the provided array.
 *
 * @param i2c The I2C instance to use for communication.
 * @param accel An array to store the X, Y, and Z-axis accelerometer values.
 */
void readAccelData(i2c_inst_t *i2c, int16_t accel[]);

/**
 * @brief Calculates the tilt angle from accelerometer value.
 *
 * This function calculates the tilt angle in degrees from the given accelerometer value.
 *
 * @param accel_value The raw accelerometer value.
 * @return The tilt angle in degrees.
 */
float calculate_tilt_angle(int16_t accel_value);

/**
 * @brief Reads the tilt angle from the MPU6050 accelerometer.
 *
 * This function reads the accelerometer data, calculates the tilt angles, and stores them in the provided array.
 *
 * @param acceleration An array to store the X, Y, and Z-axis tilt angles.
 */
void read_tilt_angle(int16_t acceleration[]);

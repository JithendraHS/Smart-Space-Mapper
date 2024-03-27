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
 * @file lidar.c
 * @brief Source file for LIDAR interface functions.
 * @author Jithendra H S
 * @date 2023-12-15
*/
#include "lidar.h"
#include "i2c_code.h"
#include "stdio.h"

// I2C address of the LIDAR device
static const uint8_t LIDAR = 0x10;

// Registers
static const uint8_t DIST_LOW = 0x00;
static const uint8_t DIST_HIGH = 0x01;

/**
 * @brief Read distance from the LIDAR device.
 *
 * This function communicates with the LIDAR device over I2C to read the distance.
 * It reads the low and high bytes from the specified registers and combines them
 * to obtain the final distance value.
 *
 * @return The distance measured by the LIDAR device.
 */
uint16_t read_lidar() {
    // I2C instance to use (in this case, i2c0)
    i2c_inst_t *i2c = i2c0;

    // Buffer to store raw reads
    uint8_t data[2];

    // Read distance low and high bytes
    reg_read(i2c, LIDAR, DIST_LOW, data, 2);

    // Combine low and high bytes to get the distance
    uint16_t distance = (data[1] << 8) | data[0];

    // Print the results (you can remove this if not needed)
    printf("Distance: %d\r\n", distance);

    return distance;
}

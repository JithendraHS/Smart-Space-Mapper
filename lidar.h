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
 * @file lidar.h
 * @brief Header file for LIDAR interface functions.
 * @author Jithendra H S
 * @date 2023-12-15
*/
#include "stdint.h"

/**
 * @brief Read distance from the LIDAR device.
 *
 * This function communicates with the LIDAR device over I2C to read the distance.
 * It reads the low and high bytes from the specified registers and combines them
 * to obtain the final distance value.
 *
 * @return The distance measured by the LIDAR device.
 */
uint16_t read_lidar();

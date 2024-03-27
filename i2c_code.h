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
 * @file i2c_code.h
 * @brief Header file for I2C communication functions.
 * @author Jithendra H S
 * @date December 15, 2023
*/
#include "stdint.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define I2C_SDA (16)
#define I2C_SCK (17)

#define I2C1_SDA_PIN (6)
#define I2C1_SCL_PIN (7)

/**
 * @brief Write data to the specified register over I2C.
 *
 * @param i2c       I2C instance.
 * @param addr      I2C device address.
 * @param reg       Register address to write to.
 * @param buf       Pointer to the data buffer.
 * @param nbytes    Number of bytes to write.
 * @return int      Number of bytes read.
 */
int reg_write(i2c_inst_t *i2c,
              const uint addr,
              const uint8_t reg,
              uint8_t *buf,
              const uint8_t nbytes);

/**
 * @brief Read data from the specified register over I2C.
 *
 * @param i2c       I2C instance.
 * @param addr      I2C device address.
 * @param reg       Register address to read from.
 * @param buf       Pointer to the data buffer.
 * @param nbytes    Number of bytes to read.
 * @return int      Number of bytes read.
 */
int reg_read(i2c_inst_t *i2c,
             const uint addr,
             const uint8_t reg,
             uint8_t *buf,
             const uint8_t nbytes);

/**
 * @brief Initialize I2C modules and pins.
 */
void I2C_Module_Init();
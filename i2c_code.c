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
 * @file i2c_code.c
 * @brief Source file for I2C communication functions.
 * @author Jithendra H S
 * @date December 15, 2023
*/
#include "stdio.h"
#include "i2c_code.h"

/**
 * @brief Write data to the specified register over I2C.
 *
 * This function writes data to a specific register of an I2C device.
 *
 * @param i2c       I2C instance.
 * @param addr      I2C device address.
 * @param reg       Register address to write to.
 * @param buf       Pointer to the data buffer.
 * @param nbytes    Number of bytes to write.
 * @return int      Number of bytes written.
 */
int reg_write(i2c_inst_t *i2c,
              const uint addr,
              const uint8_t reg,
              uint8_t *buf,
              const uint8_t nbytes) {
    // Initialize the number of bytes written
    int num_bytes_written = 0;

    // Create a message buffer including the register address
    uint8_t msg[nbytes + 1];

    // Check if the number of bytes is valid
    if (nbytes < 1) {
        return 0; // Return 0 if an invalid number of bytes is provided
    }

    // Append the register address to the front of the data packet
    msg[0] = reg;
    for (int i = 0; i < nbytes; i++) {
        msg[i + 1] = buf[i];
    }

    // Write data to register(s) over I2C
    num_bytes_written = i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);

    return num_bytes_written; // Return the number of bytes written
}


/**
 * @brief Read data from the specified register(s) over I2C.
 *
 * This function reads data from a specific register of an I2C device.
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
             const uint8_t nbytes) {
    // Initialize the number of bytes read
    int num_bytes_read = 0;

    // Check if the number of bytes is valid
    if (nbytes < 1) {
        return 0; // Return 0 if an invalid number of bytes is provided
    }

    // Read data from register(s) over I2C
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

    return num_bytes_read; // Return the number of bytes read
}


/**
 * @brief Initialize the I2C modules and pins.
 *
 * This function initializes the I2C modules and pins for communication.
 * It sets the I2C0 port at 200 kHz and I2C1 port at 100 kHz.
 * It configures the corresponding SDA and SCK pins for I2C functionality.
 */
void I2C_Module_Init() {
    // Initialize I2C0 port at 200 kHz
    i2c_init(i2c0, 200 * 1000);

    // Initialize I2C0 pins
    iobank0_hw->io[I2C_SDA].ctrl = GPIO_FUNC_I2C << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[I2C_SCK].ctrl = GPIO_FUNC_I2C << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;

    // Initialize I2C1 port at 100 kHz
    i2c_init(i2c1, 100 * 1000);

    // Initialize I2C1 pins
    iobank0_hw->io[I2C1_SDA_PIN].ctrl = GPIO_FUNC_I2C << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[I2C1_SCL_PIN].ctrl = GPIO_FUNC_I2C << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
}

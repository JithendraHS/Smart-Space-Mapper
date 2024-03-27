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
 * @file main.c
 * @brief Main file for the SS Mapper application.
 * @author Jithendra H S
 * @date 2023-12-15
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "spi_code.h"
#include "i2c_code.h"
#include "lidar.h"
#include "area.h"
#include "button.h"
#include "user_interface.h"
#include "mpu6050.h"

/**
 * @brief Main function for the SS Mapper application.
 *
 * This function initializes the serial port, I2C module, SPI module, OLED display,
 * MPU6050 sensor, buttons, and starts the user interface for the SS Mapper application.
 * The application then enters the main loop where it continuously checks for button
 * presses and updates the user interface accordingly.
 *
 * @return 0 upon successful execution.
 */
int main() {
    // Initialize chosen serial port
    stdio_init_all();
    printf(" !!!!!!!!!!!!!!!!!! SS Mapper started !!!!!!!!!!!!!!!!!!!\n");

    // Initialize I2C module
    I2C_Module_Init();

    // Initialize SPI module using bit-banging
    SPI_Module_Init_BIT_BANGING();

    // Initialize OLED display
    OLED_init();
    OLED_Clear();

    // Reset MPU6050 sensor
    resetMPU6050(i2c1);

    // Initialize buttons
    Button_Init();

    // Start the user interface
    user_interface();

    return 0;
}
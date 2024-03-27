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
 * @file spi_code.h
 * @brief Header file for SPI communication functions.
 * @author Jithendra H S
 * @date December 15, 2023
*/
#include "stdint.h"
#include "hardware/structs/iobank0.h"
#include "hardware/structs/spi.h"

// Define pin constants for SPI
#define SPI_PORT (spi0_hw)
#define SPI_SCK_PIN (2)
#define SPI_TX_PIN (3)
#define SPI_CS_PIN (5)
#define SPI_DC_PIN (20)
#define SPI_RESET_PIN (21)

#define SET_SPI_DC_PIN (1ul << SPI_DC_PIN)
#define SET_SPI_CS_PIN (1ul << SPI_CS_PIN);

/**
 * @brief Initializes the SPI module for normal operation.
 * 
 * This function configures the SPI module for normal operation with the specified
 * parameters, including baud rate, data format, and other settings.
 */
void SPI_Module_Init();

/**
 * @brief Writes a sequence of data to the SPI module.
 * 
 * This function writes a sequence of data to the SPI module. It waits for the
 * transmit FIFO not full (TNF) condition and then writes data to the transmit FIFO.
 * After completing the data transfer, it waits for the SPI module to finish and
 * clears any overrun interrupt.
 * 
 * @param spi The SPI hardware instance.
 * @param data A pointer to the data to be written.
 * @param len The length of the data to be written.
 */
void SPI_write(spi_hw_t *spi, const uint8_t *data, size_t len);

/**
 * @brief Writes a command to the SPI display.
 * 
 * This function writes a command to the SPI display. It sets the DC pin low to
 * indicate command mode, sets the CS pin low to enable SPI communication, writes
 * the command data to SPI using bit-banging, and finally sets the CS pin high to
 * disable SPI communication.
 * 
 * @param data The command data to be written.
 */
void SPI_WriteCommand(const uint8_t data);

/**
 * @brief Writes data to the SPI display.
 * 
 * This function writes data to the SPI display. It sets the DC pin high to
 * indicate data mode, sets the CS pin low to enable SPI communication, writes
 * the data to SPI using bit-banging, and finally sets the CS pin high to
 * disable SPI communication.
 * 
 * @param data The data to be written.
 */
void SPI_WriteData(const uint8_t data);

/**
 * @brief Initializes the SPI module using bit-banging.
 * 
 * This function configures the GPIO pins used for SPI communication and sets
 * up the SPI module using bit-banging.
 */
void SPI_Module_Init_BIT_BANGING();

/**
 * @brief Sends a byte of data over SPI using bit-banging.
 * 
 * This function sends each bit of the data byte sequentially over SPI using
 * bit-banging. It assumes that the SPI pins and other configurations have been
 * properly set up before calling this function.
 * 
 * @param data The byte of data to be sent.
 */
void SPI_send_byte(uint8_t data);


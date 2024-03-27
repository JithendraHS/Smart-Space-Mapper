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
 * @file spi_code.c
 * @brief SPI communication functions for OLED display.
 * @author Jithendra H S
 * @date December 15, 2023
 */
#include "spi_code.h"
#include "pico/stdlib.h"
#include "oled.h"

// Constant for byte size
#define BYTE_SIZE (8)

/**
 * @brief Writes data to the SPI module.
 *
 * @param spi   Pointer to the SPI hardware structure.
 * @param data  Pointer to the data buffer.
 * @param len   Number of bytes to write.
 */
void SPI_write(spi_hw_t *spi, const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        // Wait for transmit FIFO not full (TNF)
        while (!(spi->sr & SPI_SSPSR_TNF_BITS));
        
        // Write data to transmit FIFO
        spi->dr = (uint32_t)data[i];
    }
    // Wait for SPI to finish
    while (spi->sr & SPI_SSPSR_BSY_BITS);

    // Clear overrun interrupt
    spi->icr = SPI_SSPICR_RORIC_BITS;
}

/**
 * @brief Writes a command to the SPI module.
 *
 * @param data  The command to write.
 */
void SPI_WriteCommand(const uint8_t data) {
    // Set DC pin low to indicate command mode
    sio_hw->gpio_clr = SET_SPI_DC_PIN;

    // Set CS pin low to enable SPI communication
    sio_hw->gpio_clr = SET_SPI_CS_PIN;

    // Write command data to SPI
    SPI_send_byte(data);

    // Set CS pin high to disable SPI communication
    sio_hw->gpio_set = SET_SPI_CS_PIN;
}

/**
 * @brief Writes data to the SPI module in data mode.
 *
 * @param data  The data to write.
 */
void SPI_WriteData(const uint8_t data) {
    // Set DC pin high to indicate data mode
    sio_hw->gpio_set = SET_SPI_DC_PIN;

    // Set CS pin low to enable SPI communication
    sio_hw->gpio_clr = SET_SPI_CS_PIN;

    // Write data to SPI
    SPI_send_byte(data);

    // Set CS pin high to disable SPI communication
    sio_hw->gpio_set = SET_SPI_CS_PIN;
}


/**
 * @brief Initializes the SPI module with the specified configuration.
 */
void SPI_Module_Init() {
    // Get a pointer to the SPI hardware
    spi_hw_t *spi = spi0_hw;

    // Disable the SPI
    spi->cr1 &= ~SPI_SSPCR1_SSE_BITS;

    // Set prescalar and postdiv for baud rate
    uint8_t prescalar = 2;
    uint16_t postdiv = 63;
    spi->cpsr = prescalar;
    spi->cr0 |= (postdiv - 1) << 8;

    // Configure data format (8 data bits, cpol, cpha)
    uint8_t data_bits = 8;
    uint8_t cpol = 0;
    uint8_t cpha = 0;
    spi->cr0 |= ((uint8_t)(data_bits - 1)) << SPI_SSPCR0_DSS_LSB |
                ((uint8_t)cpol) << SPI_SSPCR0_SPO_LSB |
                ((uint8_t)cpha) << SPI_SSPCR0_SPH_LSB;

    // Enable the SPI
    spi->cr1 |= SPI_SSPCR1_SSE_BITS;

    // Configure GPIO pins for SPI
    iobank0_hw->io[SPI_SCK_PIN].ctrl = GPIO_FUNC_SPI << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[SPI_TX_PIN].ctrl = GPIO_FUNC_SPI << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
}


/**
 * @brief Initializes the SPI module for bit-banging mode.
 */
void SPI_Module_Init_BIT_BANGING() {
    // Set GPIO functions for SPI pins
    iobank0_hw->io[SPI_SCK_PIN].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[SPI_TX_PIN].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[SPI_CS_PIN].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[SPI_DC_PIN].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    iobank0_hw->io[SPI_RESET_PIN].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;

    // Set output enables for SPI pins
    sio_hw->gpio_oe_set = (1ul << SPI_SCK_PIN);
    sio_hw->gpio_oe_set = (1ul << SPI_TX_PIN);
    sio_hw->gpio_oe_set = (1ul << SPI_CS_PIN);
    sio_hw->gpio_oe_set = (1ul << SPI_DC_PIN);
    sio_hw->gpio_oe_set = (1ul << SPI_RESET_PIN);

    // Clear SPI pins
    sio_hw->gpio_clr = (1ul << SPI_SCK_PIN);
    sio_hw->gpio_clr = (1ul << SPI_TX_PIN);
    sio_hw->gpio_clr = (1ul << SPI_CS_PIN);
}


/**
 * @brief Sends a byte of data over SPI using bit-banging.
 * 
 * This function sends each bit of the data byte sequentially over SPI using
 * bit-banging. It assumes that the SPI pins and other configurations have been
 * properly set up before calling this function.
 * 
 * @param data The byte of data to be sent.
 */
void SPI_send_byte(uint8_t data) {
    // Reverse the bits of the data byte
    data = reverse(data);

    for(int i = 0; i < BYTE_SIZE; i++) {
        // Extract the least significant bit
        uint8_t bit = data & 0x01;

        // Set or clear the SPI_TX_PIN based on the bit value
        gpio_put(SPI_TX_PIN, bit);

        // Set or clear the GPIO corresponding to SPI_TX_PIN
        if(bit) {
            sio_hw->gpio_set = (1ul << SPI_TX_PIN);
        } else {
            sio_hw->gpio_clr = (1ul << SPI_TX_PIN);
        }

        // Wait for a short duration
        sleep_us(5);

        // Set the SPI clock signal high
        sio_hw->gpio_set = (1ul << SPI_SCK_PIN);

        // Wait for a short duration
        sleep_us(10);

        // Clear the SPI clock signal
        sio_hw->gpio_clr = (1ul << SPI_SCK_PIN);

        // Wait for a short duration
        sleep_us(5);

        // Shift to the next bit
        data = data >> 1;

        // Wait for a short duration
        sleep_us(2);
    }
}



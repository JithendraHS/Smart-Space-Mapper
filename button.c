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
 * @file button.c
 * @brief Implementation of button initialization functions.
 * @author Jithendra H S
 * @date 2023-12-15
*/
#include "button.h"
#include "pico/stdlib.h"

#define bool_to_bit(x) ((uint)!!(x))

void Button_Init(){
    // Set GPIO10 as input
    sio_hw->gpio_oe_clr = (1ul << GPIO10);
    sio_hw->gpio_clr = (1ul << GPIO10);
    iobank0_hw->io[GPIO10].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    // Enable pull-up resistor
    *(io_rw_32 *) ((void *)(REG_ALIAS_XOR_BITS | (uintptr_t)((volatile void *) &padsbank0_hw->io[GPIO10])))
     = (padsbank0_hw->io[GPIO10] ^ ((bool_to_bit(true) << PADS_BANK0_GPIO0_PUE_LSB)
      | (bool_to_bit(false) << PADS_BANK0_GPIO0_PDE_LSB)))
      & (PADS_BANK0_GPIO0_PUE_BITS | PADS_BANK0_GPIO0_PDE_BITS);

	// Set GPIO11 as input
    sio_hw->gpio_oe_clr = (1ul << GPIO11);
    sio_hw->gpio_clr = (1ul << GPIO11);
    iobank0_hw->io[GPIO11].ctrl = GPIO_FUNC_SIO << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    // Enable pull-up resistor
    *(io_rw_32 *) ((void *)(REG_ALIAS_XOR_BITS | (uintptr_t)((volatile void *) &padsbank0_hw->io[GPIO11]))) =
     (padsbank0_hw->io[GPIO11] ^ ((bool_to_bit(true) << PADS_BANK0_GPIO0_PUE_LSB)
     | (bool_to_bit(false) << PADS_BANK0_GPIO0_PDE_LSB))) 
     & (PADS_BANK0_GPIO0_PUE_BITS | PADS_BANK0_GPIO0_PDE_BITS);

}
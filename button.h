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
 * @file button.h
 * @brief Header file for button initialization functions.
 * @author Jithendra H S
 * @date 2023-12-15
*/
// Define constants for GPIO pins
#define GPIO10 (10)
#define GPIO11 (11)

// Define constants for GPIO direction
#define GPIO_OUT 1
#define GPIO_IN  0

// Function prototype for initializing buttons
void Button_Init();

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
 * @file user_interface.c
 *
 * @brief Main user interface function for handling user interactions and
 *        displaying information on an OLED screen.
 *
 * @author Jithendra H S
 *
 * @date December 15, 2023
*/
#include "user_interface.h"
#include "string.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "button.h"
#include "area.h"

/**
 * @brief Main user interface function
 * 
 * This function handles user interactions and displays information on the OLED screen.
 * It continuously checks the states of GPIO10 and GPIO11 buttons to perform corresponding actions.
 */
void user_interface() {
    // Create a new image cache
    UBYTE *BlackImage;
    BlackImage = (UBYTE *)malloc(OLED_IMAGE_SIZE);
    if (BlackImage == NULL) { 
        // No enough memory
        printf(" No enough memory\n");
    }
    
    // Display the main menu
    menu(BlackImage);

    // Initial shape selection
    char *shape = shapes[0];

    bool gpio10_state;
    bool gpio11_state;

    while (true) {
        // Read the state of GPIO10
        gpio10_state = !!((1ul << GPIO10) & sio_hw->gpio_in);
        // Read the state of GPIO11
        gpio11_state = !!((1ul << GPIO11) & sio_hw->gpio_in);

        // Perform actions based on the GPIO10 state
        if (!gpio10_state) {
            sleep_ms(200);
            // Move cursor to select a shape
            shape = move_cursor(BlackImage);
            printf("GPIO10 is high (pressed)! and shape %s\n\r", shape);
        }
        
        // Perform actions based on the GPIO11 state
        if (!gpio11_state) {
            sleep_ms(200);
            OLED_Clear();
            // Use memset to set all values to 0x00
            memset(BlackImage, 0x00, OLED_IMAGE_SIZE);
            OLED_Display(BlackImage);
            
            // Calculate area based on the selected shape
            double_array area = calculate_area(shape, BlackImage);
            
            // Display the calculated area
            OLED_Clear();
            // Use memset to set all values to 0x00
            memset(BlackImage, 0x00, OLED_IMAGE_SIZE);
            printf("GPIO11 is high (pressed)! , shape : %s and area %f\n\r", shape, area.result[1]);
            Paint_DrawString_EN(0, 12, "Final value :", &Font12, WHITE, BLACK);
            Paint_DrawString_EN(93, 24, "     ", &Font12, WHITE, BLACK);
            Paint_DrawString_EN(79, 36, "        ", &Font12, WHITE, BLACK);

            if (strcmp(shape, "Distance") == 0) {
                Paint_DrawNum(0, 24, area.result[0], &Font12, 2, WHITE, BLACK);
                Paint_DrawString_EN(114, 24, "cm", &Font12, WHITE, BLACK);
                Paint_DrawNum(0, 36, area.result[1], &Font12, 2, WHITE, BLACK);
                Paint_DrawString_EN(100, 36, "foot", &Font12, WHITE, BLACK);
            } else {
                Paint_DrawNum(0, 24, area.result[0], &Font12, 2, WHITE, BLACK);
                Paint_DrawString_EN(93, 24, "sq.cm", &Font12, WHITE, BLACK);
                Paint_DrawNum(0, 36, area.result[1], &Font12, 2, WHITE, BLACK);
                Paint_DrawString_EN(79, 36, "sq.foot", &Font12, WHITE, BLACK);
            }

            Paint_DrawString_EN(0, 120, "*Press Red to exit", &Font8, WHITE, BLACK);
            OLED_Display(BlackImage);

            // Wait for GPIO11 to be released before going back to the menu
            while (true) {
                // Read the state of GPIO11
                bool gpio11_state = !!((1ul << GPIO11) & sio_hw->gpio_in);
                // Perform actions based on the GPIO11 state
                if (!gpio11_state) {
                    printf("Exiting after area is displayed\n\r");
                    sleep_ms(500);
                    break;
                }
            }
            
            // Display the main menu
            menu(BlackImage);
        }
    }
}

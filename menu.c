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
 * @file menu.c
 * @brief Menu functions for SS Mapper application.
 * @author Jithendra H S
 * @date 2023-12-15
*/
#include "menu.h"
#include "pico/stdlib.h"
#include <string.h>

/**
 * @brief X-coordinate for the start position of shapes in the menu.
 */
#define SHAPES_START_X (24)

/**
 * @brief Y-coordinate for the start position of shapes in the menu.
 */
#define SHAPES_START_Y (35)

/**
 * @brief X-coordinate for the start position of the cursor in the menu.
 */
#define CURSOR_START_X (0)

/**
 * @brief Array of strings representing different shapes.
 */
char shapes[][16] = {"Distance", "Circle", "Rectangle", "Triangle", "Irregular menu"};

/**
 * @brief Array of strings representing shapes in the irregular menu.
 */
char irr_shapes[][16] = {"shape1", "shape2", "shape3", "shape4", "shape5"};

/**
 * @brief Cursor position in the main menu.
 */
static volatile int8_t cursor_pos = 0;

/**
 * @brief Cursor position in the irregular menu.
 */
static volatile int8_t cursor_pos_irr_menu = 0;

/**
 * @brief Displays the main menu on the OLED screen.
 *
 * This function initializes the display with shapes, including the cursor for shape selection.
 * It also provides instructions for navigation and selection using different button presses.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 */
void menu(UBYTE *BlackImage) {
    // Initialize the OLED display
    Paint_NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, 0, BLACK);
    Paint_SetScale(2);

    Paint_SelectImage(BlackImage);
    Paint_Clear(BLACK);

    // Display the application name
    Paint_DrawString_EN(10, 0, "SS Mapper", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(0, 18, "-------------------", &Font12, WHITE, BLACK);

    // Display shapes with checkboxes
    for (int i = 0; i < sizeof(shapes) / sizeof(shapes[0]); i++) {
        Paint_DrawString_EN(CURSOR_START_X, SHAPES_START_Y + (i * Font12.Height), "[ ]", &Font12, WHITE, BLACK);
        Paint_DrawString_EN(SHAPES_START_X, SHAPES_START_Y + (i * Font12.Height), shapes[i], &Font12, WHITE, BLACK);
    }

    // Display cursor for shape selection
    Paint_DrawString_EN(CURSOR_START_X + 8, SHAPES_START_Y + (cursor_pos * Font12.Height), "+", &Font12, WHITE, BLACK);

    // Display navigation instructions
    Paint_DrawString_EN(0, 112, "*Press Yellow to Navigate", &Font8, WHITE, BLACK);
    Paint_DrawString_EN(0, 120, "*Press Red to select", &Font8, WHITE, BLACK);

    // Update the OLED display
    OLED_Display(BlackImage);
}

/**
 * @brief Moves the cursor to the next shape in the main menu.
 *
 * This function updates the display to move the cursor to the next shape in the main menu.
 * It ensures that the cursor wraps around to the beginning if it reaches the end.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 * @return A pointer to the string representing the currently selected shape.
 */
char *move_cursor(UBYTE *BlackImage) {
    // Clear the previous cursor position
    Paint_DrawString_EN(CURSOR_START_X + 8, SHAPES_START_Y + (cursor_pos * Font12.Height), " ", &Font12, WHITE, BLACK);

    // Move to the next shape in the main menu
    cursor_pos++;
    cursor_pos = cursor_pos % (sizeof(shapes) / sizeof(shapes[0]));

    // Display the cursor at the new position
    Paint_DrawString_EN(CURSOR_START_X + 8, SHAPES_START_Y + (cursor_pos * Font12.Height), "+", &Font12, WHITE, BLACK);

    // Update the OLED display
    OLED_Display(BlackImage);

    // Return the name of the currently selected shape
    return shapes[cursor_pos];
}

/**
 * @brief Moves the cursor to the next shape in the irregular menu.
 *
 * This function updates the display to move the cursor to the next shape in the irregular menu.
 * It ensures that the cursor wraps around to the beginning if it reaches the end.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 * @return A pointer to the string representing the currently selected shape in the irregular menu.
 */
char *move_cursor_irr_menu(UBYTE *BlackImage) {
    // Clear the previous cursor position
    Paint_DrawString_EN(CURSOR_START_X + 8, SHAPES_START_Y + (cursor_pos_irr_menu * Font12.Height), " ", &Font12, WHITE, BLACK);

    // Move to the next shape in the irregular menu
    cursor_pos_irr_menu++;
    cursor_pos_irr_menu = cursor_pos_irr_menu % (sizeof(irr_shapes) / sizeof(irr_shapes[0]));

    // Display the cursor at the new position
    Paint_DrawString_EN(CURSOR_START_X + 8, SHAPES_START_Y + (cursor_pos_irr_menu * Font12.Height), "+", &Font12, WHITE, BLACK);

    // Update the OLED display
    OLED_Display(BlackImage);

    // Return the name of the currently selected shape in the irregular menu
    return irr_shapes[cursor_pos_irr_menu];
}


/**
 * @brief Retrieves the current cursor position in the main menu.
 *
 * This function returns the current cursor position in the main menu, allowing other
 * functions to query and use the cursor position information.
 *
 * @return The current cursor position in the main menu.
 */
int8_t get_cursor_pos() {
    return cursor_pos;
}

/**
 * @brief Sets the cursor position in the main menu.
 *
 * This function sets the cursor position in the main menu to the specified value.
 *
 * @param pos The new cursor position to set.
 */
void put_cursor_pos(int8_t pos) {
    cursor_pos = pos;
}

/**
 * @brief Displays the irregular menu on the OLED screen.
 *
 * This function displays the irregular menu on the OLED screen, including the shapes and
 * the cursor indicating the current selection. It also provides navigation instructions.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 */
void irr_menu(UBYTE *BlackImage) {
    // Reset cursor position in the irregular menu
    cursor_pos_irr_menu = 0;

    // Clear OLED display
    OLED_Clear();
    // Use memset to set all values to 0x00
    memset(BlackImage, 0x00, OLED_IMAGE_SIZE);

    // Display the title
    Paint_DrawString_EN(10, 0, "SS Mapper", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(0, 18, "-------------------", &Font12, WHITE, BLACK);

    // Display shapes and cursor in the irregular menu
    for (int i = 0; i < sizeof(irr_shapes) / sizeof(irr_shapes[0]); i++) {
        Paint_DrawString_EN(CURSOR_START_X, SHAPES_START_Y + (i * Font12.Height), "[ ]", &Font12, WHITE, BLACK);
        Paint_DrawString_EN(SHAPES_START_X, SHAPES_START_Y + (i * Font12.Height), irr_shapes[i], &Font12, WHITE, BLACK);
    }

    // Display the cursor at the initial position
    Paint_DrawString_EN(CURSOR_START_X + 8, SHAPES_START_Y + (cursor_pos_irr_menu * Font12.Height), "+", &Font12, WHITE, BLACK);

    // Display navigation instructions
    Paint_DrawString_EN(0, 112, "*Press Yellow to Navigate", &Font8, WHITE, BLACK);
    Paint_DrawString_EN(0, 120, "*Press Red to select", &Font8, WHITE, BLACK);

    // Update the OLED display
    OLED_Display(BlackImage);
}



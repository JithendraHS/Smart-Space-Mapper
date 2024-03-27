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
 * @file menu.h
 * @brief Header file for SS Mapper menu functions.
 * @author Jithendra H S
 * @date 2023-12-15
*/
#include "oled.h"

// Maximum length for shape names
#define SHAPE_NAME_MAX_LENGTH 16

// External declarations for shape arrays
extern char shapes[][SHAPE_NAME_MAX_LENGTH];
extern char irr_shapes[][SHAPE_NAME_MAX_LENGTH];

// Function declarations

/**
 * @brief Displays the main menu on the OLED screen.
 *
 * This function displays the main menu on the OLED screen, including shape options and
 * the cursor indicating the current selection. It also provides navigation instructions.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 */
void menu(UBYTE *BlackImage);

/**
 * @brief Moves the cursor in the main menu and returns the selected shape.
 *
 * This function updates the cursor position in the main menu and returns the selected shape
 * based on the new cursor position.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 * @return A pointer to the selected shape name.
 */
char *move_cursor(UBYTE *BlackImage);

/**
 * @brief Moves the cursor in the irregular menu and returns the selected shape.
 *
 * This function updates the cursor position in the irregular menu and returns the selected shape
 * based on the new cursor position.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 * @return A pointer to the selected irregular shape name.
 */
char *move_cursor_irr_menu(UBYTE *BlackImage);

/**
 * @brief Retrieves the current cursor position in the main menu.
 *
 * This function returns the current cursor position in the main menu, allowing other
 * functions to query and use the cursor position information.
 *
 * @return The current cursor position in the main menu.
 */
int8_t get_cursor_pos();

/**
 * @brief Sets the cursor position in the main menu.
 *
 * This function sets the cursor position in the main menu to the specified value.
 *
 * @param pos The new cursor position to set.
 */
void put_cursor_pos(int8_t pos);

/**
 * @brief Displays the irregular menu on the OLED screen.
 *
 * This function displays the irregular menu on the OLED screen, including the shapes and
 * the cursor indicating the current selection. It also provides navigation instructions.
 *
 * @param BlackImage Pointer to the image buffer for the OLED display.
 */
void irr_menu(UBYTE *BlackImage);
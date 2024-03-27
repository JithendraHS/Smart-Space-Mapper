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
 * @file area.h
 * @brief Functions for calculating areas of different shapes and distances.
 * @author Jithendra H S
 * @date December 15, 2023
 */
#include "stdint.h"
#include "menu.h"
#include "string.h"

// Structure to store the result of area calculations
typedef struct double_array double_array;

struct double_array{
    double result[2];
};

/**
 * @brief Calculate the area based on the specified shape.
 *
 * @param shape         A string identifier for the shape to be calculated.
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area(char *shape, UBYTE *BlackImage);

/**
 * @brief Calculate the area based on the specified irregular shape.
 *
 * @param shape         A string identifier for the irregular shape to be calculated.
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_irr_shape(char *shape, UBYTE *BlackImage);

/**
 * @brief Capture the distance using LIDAR sensor.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return uint16_t     The captured distance in centimeters.
 */
uint16_t capture_distance(UBYTE *BlackImage);

/**
 * @brief Calculate the distance and convert it to both centimeters and feet.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated distance in centimeters
 *                      (result[0]) and feet (result[1]).
 */
double_array calculate_distance(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a circle based on the captured diameter.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_circle(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a rectangle based on the measured width and length.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_rectangle(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a triangle based on the measured distances at three corners.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_triangle(UBYTE *BlackImage);

/**
 * @brief Calculate the area of an irregular shape based on the user's selection.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_Irregular(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a specific irregular shape (Shape 1) based on the measured
 *        distances at six corners.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_shape1(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a specific irregular shape (Shape 2) based on the measured
 *        distances at eight corners.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_shape2(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a specific irregular shape (Shape 3) based on the measured
 *        distances at eight corners.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_shape3(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a specific irregular shape (Shape 4) based on the measured
 *        distances at twelve corners.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_shape4(UBYTE *BlackImage);

/**
 * @brief Calculate the area of a specific irregular shape (Shape 5) based on the measured
 *        distances at three corners.
 *
 * @param BlackImage    Pointer to the image data.
 * 
 * @return double_array A structure containing the calculated area in square centimeters
 *                      (result[0]) and square feet (result[1]).
 */
double_array calculate_area_shape5(UBYTE *BlackImage);

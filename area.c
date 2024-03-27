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
 * @file area.c
 * @brief Functions for calculating and displaying area based on various shapes.
 * @author Jithendra H S
 * @date 2023-12-15
 */
#include "area.h"
#include "string.h"
#include "stdio.h"
#include "pico/stdlib.h"
#include <math.h>
#include "lidar.h"
#include "mpu6050.h"

#define FOOT_IN_CM (30.48)
#define SQUARE_FEET (0.0010764)

/**
 * @brief Calculate area based on the selected shape
 * 
 * This function determines the selected shape and calls the corresponding area calculation function.
 * The result is returned as a double_array structure.
 * 
 * @param shape A string indicating the selected shape
 * @param BlackImage A pointer to the image cache
 * 
 * @return A double_array structure containing the calculated area values
 */
double_array calculate_area(char * shape, UBYTE *BlackImage){
    // Check the selected shape and call the corresponding area calculation function
    if(strcmp(shape, "Distance") == 0){
        return calculate_distance(BlackImage);
    }else if(strcmp(shape, "Circle") == 0){
        return calculate_area_circle(BlackImage);
    }else if(strcmp(shape, "Rectangle") == 0){
        return calculate_area_rectangle(BlackImage);
    }else if(strcmp(shape, "Triangle") == 0){
        return calculate_area_triangle(BlackImage);
    }else if(strcmp(shape, "Irregular menu") == 0){
        return calculate_area_Irregular(BlackImage);
    }else{
        printf("Unknown command\n\r");
    }
}


/**
 * @brief Calculate area for irregular shapes based on the selected shape
 * 
 * This function determines the selected irregular shape and calls the corresponding area calculation function.
 * The result is returned as a double_array structure.
 * 
 * @param shape A string indicating the selected irregular shape
 * @param BlackImage A pointer to the image cache
 * 
 * @return A double_array structure containing the calculated area values for the irregular shape
 */
double_array calculate_area_irr_shape(char * shape, UBYTE *BlackImage){
    // Check the selected irregular shape and call the corresponding area calculation function
    if(strcmp(shape, "shape1") == 0){
        return calculate_area_shape1(BlackImage);
    }else if(strcmp(shape, "shape2") == 0){
        return calculate_area_shape2(BlackImage);
    }else if(strcmp(shape, "shape3") == 0){
        return calculate_area_shape3(BlackImage);
    }else if(strcmp(shape, "shape4") == 0){
        return calculate_area_shape4(BlackImage);
    }else if(strcmp(shape, "shape5") == 0){
        return calculate_area_shape5(BlackImage);
    }else{
        printf("Unknown command\n\r");
    }
}

/**
 * @brief Capture and display distance from the LiDAR sensor along with device orientation
 * 
 * This function continuously captures distance data from the LiDAR sensor and displays it on the OLED screen.
 * Additionally, it reads and displays the device's X and Y orientation angles.
 * The function returns the captured distance when the user presses the red button (GPIO11).
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return The captured distance from the LiDAR sensor
 */
uint16_t capture_distance(UBYTE *BlackImage){
    uint16_t distance = 0;
    int16_t co_ordinate[3] = {0};

    // Display distance-related information on the OLED screen
    Paint_DrawString_EN(0, 12, "Distance:", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(24, 24, "cm", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(76, 24, "Feet", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(0, 60, "Device orientation:", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(0, 72, "X :", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(0, 84, "Y :", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(0, 120, "*Press Red to Read", &Font8, WHITE, BLACK);

    // Continuously capture and display distance data
    while(true){
        distance = read_lidar();
        read_tilt_angle(co_ordinate);

        // Display captured distance in both centimeters and feet
        Paint_DrawString_EN(24, 36, "          ", &Font12, WHITE, BLACK);
        Paint_DrawNum(24, 36, distance, &Font12, 0, WHITE, BLACK);
        Paint_DrawString_EN(76, 36, "          ", &Font12, WHITE, BLACK);
        Paint_DrawNum(76, 36, (double)distance/FOOT_IN_CM, &Font12, 2, WHITE, BLACK);

        // Display device orientation angles
        Paint_DrawString_EN(21, 72, "          ", &Font12, WHITE, BLACK);
        Paint_DrawNum(21, 72, co_ordinate[0], &Font12, 0, WHITE, BLACK);
        Paint_DrawString_EN(21, 84, "          ", &Font12, WHITE, BLACK);
        Paint_DrawNum(21, 84, co_ordinate[1], &Font12, 0, WHITE, BLACK);

        // Update the OLED display
        OLED_Display(BlackImage);

        // Read the state of GPIO11
        bool gpio11_state = gpio_get(11);

        // Return the captured distance if the user presses the red button
        if (!gpio11_state) {
            printf("Captured distance\n\r");
            sleep_ms(200);
            return distance;
        }

        // Wait for a short duration before the next iteration
        sleep_ms(100);
    }
}
/**
 * @brief Calculate and display distance and converted feet value
 * 
 * This function calculates the distance using the `capture_distance` function and converts it to feet.
 * It then displays the measured distance and converted feet value on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated distance in centimeters and converted feet value
 */
double_array calculate_distance(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Display a message for distance measurement
    Paint_DrawString_EN(0, 0, "Measuring Distance", &Font8, WHITE, BLACK);

    // Capture distance using the capture_distance function
    double output = (double)capture_distance(BlackImage);

    // Store the calculated distance and converted feet value in the result structure
    result.result[0] = output;
    result.result[1] = output / FOOT_IN_CM;

    // Return the result structure
    return result;
}

/**
 * @brief Calculate and display area of a circle
 * 
 * This function calculates the area of a circle using the `capture_distance` function to measure the diameter.
 * It then displays the measured diameter, calculated area, and converted area value on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_circle(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Display a message for diameter measurement
    Paint_DrawString_EN(0, 0, "Measuring Diameter", &Font8, WHITE, BLACK);

    // Capture diameter using the capture_distance function
    uint16_t diameter = capture_distance(BlackImage);

    // Calculate the area of the circle
    double output = (double)(M_PI * diameter * diameter / 4);

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}

/**
 * @brief Calculate and display area of a rectangle
 * 
 * This function measures the width and length using the `capture_distance` function,
 * calculates the area of the rectangle, and displays the measured values and the area
 * on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_rectangle(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Display a message for width measurement
    Paint_DrawString_EN(0, 0, "Measuring Width", &Font8, WHITE, BLACK);

    // Measure the width using the capture_distance function
    uint16_t width = capture_distance(BlackImage);

    // Sleep for 500 ms to allow the user to reposition the sensor
    sleep_ms(500);

    // Display a message for length measurement
    Paint_DrawString_EN(0, 0, "Measuring Length", &Font8, WHITE, BLACK);

    // Measure the length using the capture_distance function
    uint16_t length = capture_distance(BlackImage);

    // Calculate the area of the rectangle
    double output = (double)(length * width);

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}

/**
 * @brief Calculate and display area of a triangle
 * 
 * This function measures distances at three corners using the `capture_distance` function,
 * applies Heron's formula to calculate the area of the triangle, and displays the measured values
 * and the area on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_triangle(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Array to store distances at three corners of the triangle
    uint16_t corner[3] = {0};

    // Index variable for looping through corners
    uint8_t index = 0;

    // Display a message for corner measurement
    Paint_DrawString_EN(0, 0, "Measuring at Corner:", &Font8, WHITE, BLACK);

    // Loop to measure distances at each corner
    while(index < 3){
        Paint_DrawNum(112, 0, index + 1, &Font8, 0, WHITE, BLACK);
        corner[index] = capture_distance(BlackImage);
        index++;
    }

    // Using Heron's formula to calculate the area of the triangle
    double s = (corner[0] + corner[1] + corner[2]) / 2.0;
    double output = (double)sqrt(s * (s - corner[0]) * (s - corner[1]) * (s - corner[2]));

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}


/**
 * @brief Calculate and display area of an irregular shape
 * 
 * This function displays an irregular shape menu on the OLED screen,
 * allows the user to select a specific irregular shape, and calculates
 * and displays the area of the selected shape using the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_Irregular(UBYTE *BlackImage){
    // Display the irregular shape menu on the OLED screen
    irr_menu(BlackImage);

    // Initialize the selected shape to the first shape in the menu
    char *shape = irr_shapes[0];

    // Sleep for 300 ms to allow the user to read the menu
    sleep_ms(300);

    while(true){
        // Read the state of GPIO10
        bool gpio10_state = gpio_get(10);
        // Read the state of GPIO11
        bool gpio11_state = gpio_get(11);

        // Perform actions based on the GPIO10 state
        if (!gpio10_state) {
            sleep_ms(200);
            // Allow the user to move the cursor and select a shape
            shape = move_cursor_irr_menu(BlackImage);
            printf("GPIO10 is high (pressed)! and shape %s\n\r", shape);
        }
        
        // Perform actions based on the GPIO11 state
        if (!gpio11_state) {
            sleep_ms(200);
            // Clear the OLED screen and set all image values to 0x00
            OLED_Clear();
            memset(BlackImage, 0x00, OLED_IMAGE_SIZE);
            OLED_Display(BlackImage);
            // Calculate and return the area of the selected irregular shape
            return calculate_area_irr_shape(shape, BlackImage);
        }
    }
}

/**
 * @brief Calculate and display area of a specific irregular shape (Shape 1)
 * 
 * This function measures distances at six corners of a specific irregular shape,
 * applies a specific formula to calculate the area, and displays the measured values
 * and the area on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_shape1(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Array to store measurements at six corners of the shape
    int16_t measurement[6] = {0};

    // Display a message for corner measurement
    Paint_DrawString_EN(0, 0, "Measuring at Corner:", &Font8, WHITE, BLACK);

    // Loop to measure distances at each corner
    for(int i = 0; i < sizeof(measurement) / sizeof(measurement[0]); i++){
        Paint_DrawNum(112, 0, i + 1, &Font8, 0, WHITE, BLACK);
        measurement[i] = capture_distance(BlackImage);
    }

    // Using a specific formula to calculate the area of the shape
    double sub_area1 = measurement[0] * measurement[5];
    double sub_area2 = measurement[2] * measurement[3];
    double output = (double)(sub_area1 + sub_area2);

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}


/**
 * @brief Calculate and display area of a specific irregular shape (Shape 2)
 * 
 * This function measures distances at eight corners of a specific irregular shape (Shape 2),
 * applies a specific formula to calculate the area, and displays the measured values
 * and the area on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_shape2(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Array to store measurements at eight corners of the shape
    int16_t measurement[8] = {0};

    // Display a message for corner measurement
    Paint_DrawString_EN(0, 0, "Measuring at Corner:", &Font8, WHITE, BLACK);

    // Loop to measure distances at each corner
    for(int i = 0; i < sizeof(measurement) / sizeof(measurement[0]); i++){
        Paint_DrawNum(112, 0, i + 1, &Font8, 0, WHITE, BLACK);
        measurement[i] = capture_distance(BlackImage);
    }

    // Using a specific formula to calculate the area of the shape
    double sub_area1 = measurement[0] * measurement[7];
    double sub_area2 = measurement[2] * measurement[3];
   
    double output = (double)(sub_area1 + sub_area2);

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}

/**
 * @brief Calculate and display area of a specific irregular shape (Shape 3)
 * 
 * This function measures distances at eight corners of a specific irregular shape (Shape 3),
 * applies a specific formula to calculate the area, and displays the measured values
 * and the area on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_shape3(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Array to store measurements at eight corners of the shape
    int16_t measurement[8] = {0};

    // Display a message for corner measurement
    Paint_DrawString_EN(0, 0, "Measuring at Corner:", &Font8, WHITE, BLACK);

    // Loop to measure distances at each corner
    for(int i = 0; i < sizeof(measurement) / sizeof(measurement[0]) ; i++){
        Paint_DrawNum(112, 0, i + 1, &Font8, 0, WHITE, BLACK);
        measurement[i] = capture_distance(BlackImage);
    }

    // Using a specific formula to calculate the area of the shape
    double sub_area1 = measurement[0] * measurement[1];
    double sub_area2 = (measurement[0] - measurement[2]) * (measurement[7] - measurement[1] - measurement[5]);
    double sub_area3 = measurement[5] * measurement[6];
    
    double output = (double)(sub_area1 + sub_area2 + sub_area3);

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}

/**
 * @brief Calculate and display area of a specific irregular shape (Shape 4)
 * 
 * This function measures distances at twelve corners of a specific irregular shape (Shape 4),
 * applies a specific formula to calculate the area, and displays the measured values
 * and the area on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_shape4(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Array to store measurements at twelve corners of the shape
    int16_t measurement[12] = {0};

    // Display a message for corner measurement
    Paint_DrawString_EN(0, 0, "Measuring at Corner:", &Font8, WHITE, BLACK);

    // Loop to measure distances at each corner
    for(int i = 0; i < sizeof(measurement) / sizeof(measurement[0]) ; i++){
        Paint_DrawNum(112, 0, i + 1, &Font8, 0, WHITE, BLACK);
        measurement[i] = capture_distance(BlackImage);
    }

    // Using a specific formula to calculate the area of the shape
    double sub_area1 = measurement[0] * measurement[11];
    double sub_area2 = measurement[2] * measurement[3];
    double sub_area3 = measurement[5] * measurement[6];
    double sub_area4 = measurement[7] * measurement[8];
    double sub_area5 = measurement[2] * measurement[11];

    double output = (double)(sub_area1 + sub_area2 + sub_area3 + sub_area4 + sub_area5);

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}

/**
 * @brief Calculate and display area of a specific irregular shape (Shape 5)
 * 
 * This function measures distances at three corners of a specific irregular shape (Shape 5),
 * applies a specific formula to calculate the area, and displays the measured values
 * and the area on the OLED screen.
 * 
 * @param BlackImage A pointer to the image cache for OLED display
 * @return A structure containing the calculated area in square centimeters and converted square feet value
 */
double_array calculate_area_shape5(UBYTE *BlackImage){
    // Initialize the result structure
    double_array result = {0};

    // Array to store measurements at three corners of the shape
    int16_t measurement[3] = {0};

    // Display a message for corner measurement
    Paint_DrawString_EN(0, 0, "Measuring at Corner:", &Font8, WHITE, BLACK);

    // Loop to measure distances at each corner
    for(int i = 0; i < sizeof(measurement) / sizeof(measurement[0]) ; i++){
        Paint_DrawNum(112, 0, i + 1, &Font8, 0, WHITE, BLACK);
        measurement[i] = capture_distance(BlackImage);
    }

    // Using a specific formula to calculate the area of the shape
    double sub_area1 = (measurement[0] + measurement[1]) * measurement[2] / 2;
    double output = (double)sub_area1;

    // Store the calculated area and converted area value in the result structure
    result.result[0] = output;
    result.result[1] = output * SQUARE_FEET;

    // Return the result structure
    return result;
}

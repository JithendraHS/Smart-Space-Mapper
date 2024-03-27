# Smart-Space-Mapper

## Introduction

The Smart Space Mapper project addresses the challenge of accurately measuring indoor spaces, inspired by observing workers facing difficulties in tasks such as measuring ceiling height or floor dimensions. This project aims to simplify these tasks and provide a versatile solution for various scenarios. This document provides an overview of the Smart Space Mapper, including its system overview and integrated hardware components.

### System Overview

The Smart Space Mapper integrates advanced hardware components to create a cohesive system for indoor space mapping and optimization. The RP2040 microcontroller serves as the computational core, orchestrating the functionalities of the device. The project incorporates the TF-Luna LiDAR sensor for precise distance measurements and the MPU6050 sensor for orientation data. The user interface is realized through a 1.5-inch OLED display with intuitive navigation buttons.

![image](https://github.com/JithendraHS/Smart-Space-Mapper/assets/37045723/d6a03766-1141-455d-be51-b069f7e1d819)

The integration of these hardware components aims to create a user-friendly solution, simplifying specific tasks while laying the foundation for intelligent indoor space optimization.

## Technical Description

The technical description provides an in-depth overview of each aspect of the Smart Space Mapper project.

### Board Design
![image](https://github.com/JithendraHS/Smart-Space-Mapper/assets/37045723/4cc586d6-8518-48f8-87c9-c285dc5022bc)
![image](https://github.com/JithendraHS/Smart-Space-Mapper/assets/37045723/302c46c6-cdd2-48e6-9b87-9f287a4a634d)

#### RP2040 Microcontroller Overview:

The RP2040 microcontroller offers versatile digital interfaces, dual ARM Cortex M0+ processors, embedded SRAM, GPIO support, and more. It supports USB Type-C connectivity and facilitates code execution from external memory via dedicated interfaces.

#### Power Regulatory Circuit:

The power regulatory circuit ensures stable power distribution and safeguards against electrical anomalies. It includes protective components, voltage regulation mechanisms, and visual power status indication.

#### W25Q128JV Serial Flash Memory:

The flash memory provides critical data storage with features like a large capacity, dual/quad SPI support, and robust write protection mechanisms.

#### Crystal Oscillator:

An external crystal oscillator ensures precise frequency for applications like USB communication.

#### Board Layout:

The designed board layout ensures efficient integration of components for optimal performance.

### TF-Luna LiDAR Sensor

The LiDAR sensor enhances the functionality of the Smart Space Mapper with its accuracy, versatile operating range, and efficient power consumption.

### MPU6050 Sensor

The MPU6050 sensor provides orientation data critical for spatial awareness, featuring a versatile full-scale range and low power consumption modes.

### OLED Display and Navigation Buttons

The OLED display and navigation buttons offer an intuitive user interface, with specifications tailored to the project's requirements.

### Hardware Setup

The hardware setup involves circuit connections centered around the RP2040 microcontroller, facilitating communication between components.
![image](https://github.com/JithendraHS/Smart-Space-Mapper/assets/37045723/1b064354-f0ad-41b2-9a14-b04a2f10d590)

### Firmware Design

The firmware design orchestrates low-level programming aspects, ensuring efficient communication and coordination among hardware components. It employs protocols such as I2C and SPI for synchronized operation.
![image](https://github.com/JithendraHS/Smart-Space-Mapper/assets/37045723/d7f0d2f1-9d47-4e1f-b7ec-fd57932fbbb6)


## Conclusion

The Smart Space Mapper project aims to simplify indoor space mapping tasks through the integration of advanced hardware components and meticulous firmware design. This document attached under report folder provides a comprehensive overview of the project's technical aspects, laying the foundation for further development and optimization.



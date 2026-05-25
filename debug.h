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
 * @file debug.h
 * @brief Compile-time gated debug logging for UART.
 *
 * Enable with CMake: -DSS_MAPPER_DEBUG=ON
 * Or define SS_MAPPER_DEBUG=1 before including this header.
 */
#ifndef DEBUG_H
#define DEBUG_H

#ifndef SS_MAPPER_DEBUG
#define SS_MAPPER_DEBUG 0
#endif

#if SS_MAPPER_DEBUG
#include <stdio.h>
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...) ((void)0)
#endif

#endif /* DEBUG_H */

/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XILINX_PIN_NUMBER(X) (((uint32_t)(X) >> 0) & 0xFF)
#define XILINX_GPIO_MASK(X)  ((uint32_t)(0x01 << XILINX_PIN_NUMBER(X)))
#define XILINX_CHANNEL(X)    (((uint32_t)(X) >> 8) & 0xFF)
#define XILINX_DEV_ID(X)     (((uint32_t)(X) >> 16) & 0xFF)

typedef enum {
    PIN_OUTPUT = 0,
    PIN_INPUT = 1
} PinDirection;

/* PinName (MSB first): 8 Bit: DEVICE_ID; 8 Bit: Channel (1, 2); 8 Bit: Pin Number (0..31) */
typedef enum {
    LED0 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 0),
    LED1 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 1),
    LED2 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 2),
		LED3 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 3),
		LED4 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 4),
		LED5 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 5),
		LED6 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 6),
	  LED7 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 7),

    NC = (int)0xFFFFFFFF,
    USBTX       = 0x01,
    USBRX       = 0x02,
    STDIO_UART_TX = 0x04,
    STDIO_UART_RX = 0x05
} PinName;

typedef enum {
    PullNone          = 0,
    PullUp            = 1,
    PullDown          = 2,
    OpenDrainPullUp   = 3,
    OpenDrainNoPull   = 4,
    OpenDrainPullDown = 5,
    PushPullNoPull    = PullNone,
    PushPullPullUp    = PullUp,
    PushPullPullDown  = PullDown,
    OpenDrain         = OpenDrainPullUp,
    PullDefault       = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif

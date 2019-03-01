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
	
	GPIO0 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 0),
	GPIO1 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 1),
	GPIO2 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 2),
	GPIO3 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 3),
	GPIO4 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 4),
	GPIO5 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 5),
	GPIO6 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 6),
	GPIO7 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 7),
	GPIO8  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 8),
	GPIO9  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 9),
	GPIO10  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 10),
	GPIO11  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 11),
	GPIO12  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 12),
	GPIO13  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 13),
	GPIO14  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 14),
	GPIO15  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 15),
	GPIO16 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 16),
	GPIO17 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 17),
	GPIO18 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 18),
	GPIO19 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 19),
	GPIO20 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 20),
	GPIO21 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 21),
	GPIO22 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 22),
	GPIO23 =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 23),
	GPIO24  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 24),
	GPIO25  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 25),
	GPIO26  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 26),
	GPIO27  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 27),
	GPIO28  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 28),
	GPIO29  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 29),
	GPIO30  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 30),
	GPIO31  =  (uint32_t)(((XPAR_GPIO_0_DEVICE_ID) << 16) | (0x01 << 8) | 31),
	
	LED0 = GPIO0,
	LED1 = GPIO1,
	LED2 = GPIO2,
	LED3 = GPIO3,
	LED4 = GPIO4,
	LED5 = GPIO5,
	LED6 = GPIO6,
	LED7 = GPIO7,
	
	SW0 = GPIO8,
	SW1 = GPIO9,
	SW2 = GPIO10,
	SW3 = GPIO11,
	SW4 = GPIO12,
	SW5 = GPIO13,
	SW6 = GPIO14,
	SW7 = GPIO15,

	NC = (int)0xFFFFFFFF,
	USBTX       = 0x01,
	USBRX       = 0x02,
	STDIO_UART_TX = XPAR_AXI_UARTLITE_0_DEVICE_ID,
	STDIO_UART_RX = XPAR_AXI_UARTLITE_0_DEVICE_ID  
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

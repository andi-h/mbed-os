/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"

/* HAL us ticker */

void us_ticker_init(void)
{
	uint32_t ticks = SystemCoreClock * 1e-2;

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick Timer */
}

void us_ticker_free(void)
{
    us_ticker_disable_interrupt();
    SysTick->VAL   = 0UL;                                       
    SysTick->CTRL  = 0;
}

uint32_t us_ticker_read(void)
{
    return (uint32_t)(SysTick->VAL);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    SysTick_Config(timestamp);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(SysTick_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(SysTick_IRQn);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(SysTick_IRQn);
}

const ticker_info_t *us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        50e6 / 8,  // 50 MHz
        16         // 16 Bit timer (24 Bit is not working)
    };
    return &info;
}

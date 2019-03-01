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
#include "xtmrctr.h"
#include "xtmrctr_l.h"
#define TIMER_CNTR_0	 0
extern void taskclass1(void);
/* HAL us ticker */
XTmrCtr TimerCounterInst;

void us_ticker_irq_handler_wrapper(void *CallBackRef, u8 TmrCtrNumber);

void us_ticker_init(void)
{
	XTmrCtr_Stop(&TimerCounterInst, TIMER_CNTR_0);
	
	XTmrCtr_Initialize(&TimerCounterInst, XPAR_TMRCTR_0_DEVICE_ID);
	
	XTmrCtr_SetOptions(&TimerCounterInst, TIMER_CNTR_0,
				XTC_DOWN_COUNT_OPTION | XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	
	//mrCtr_SetResetValue(&TimerCounterInst, TIMER_CNTR_0, 0xDEADBEEF);
	XTmrCtr_SetResetValue(&TimerCounterInst, TIMER_CNTR_0, 0x00F42400);
	
	XTmrCtr_Reset(&TimerCounterInst, TIMER_CNTR_0);
	
	XTmrCtr_Start(&TimerCounterInst, TIMER_CNTR_0);
}

void us_ticker_free(void)
{
	XTmrCtr_Stop(&TimerCounterInst, TIMER_CNTR_0);
}

uint32_t us_ticker_read(void)
{
	return XTmrCtr_GetValue(&TimerCounterInst, TIMER_CNTR_0);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
	XTmrCtr_Stop(&TimerCounterInst, TIMER_CNTR_0);
	
	XTmrCtr_Initialize(&TimerCounterInst, XPAR_TMRCTR_0_DEVICE_ID);
	
	XTmrCtr_SetHandler(&TimerCounterInst, us_ticker_irq_handler_wrapper,
					   &TimerCounterInst);
	
	XTmrCtr_SetOptions(&TimerCounterInst, TIMER_CNTR_0,
				XTC_DOWN_COUNT_OPTION | XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	
	XTmrCtr_SetResetValue(&TimerCounterInst, TIMER_CNTR_0, timestamp);
	
	XTmrCtr_Reset(&TimerCounterInst, TIMER_CNTR_0);
	
	XTmrCtr_Start(&TimerCounterInst, TIMER_CNTR_0);
	NVIC_SetPriority(GPIO0_IRQn, 1); // set priority level
	NVIC_EnableIRQ(GPIO0_IRQn); // Enable interrupt

}

void us_ticker_disable_interrupt(void)
{
	XTmrCtr_SetOptions(&TimerCounterInst, TIMER_CNTR_0,
				XTC_DOWN_COUNT_OPTION | XTC_AUTO_RELOAD_OPTION);
}

void us_ticker_clear_interrupt(void)
{

}

void us_ticker_fire_interrupt(void)
{

}

const ticker_info_t *us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        8e6, // 8 MHz
        32   // 32 Bit timer
    };
    return &info;
}

void us_ticker_irq_handler_wrapper(void *CallBackRef, u8 TmrCtrNumber)
{
	//XTmrCtr *InstancePtr = (XTmrCtr *)CallBackRef;
	
	us_ticker_irq_handler();
}

void GPIO0_Handler ( void )
{
	uint32_t ControlStatusReg = XTmrCtr_ReadReg(TimerCounterInst.BaseAddress,
						   TIMER_CNTR_0,
						   XTC_TCSR_OFFSET);
	//XTmrCtr_InterruptHandler(&TimerCounterInst);
	XTmrCtr_WriteReg(TimerCounterInst.BaseAddress,
						 TIMER_CNTR_0,
						 XTC_TCSR_OFFSET,
						 ControlStatusReg |
						 XTC_CSR_INT_OCCURED_MASK);
	NVIC_ClearPendingIRQ(GPIO0_IRQn);
	
	//taskclass1();
}

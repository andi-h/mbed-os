/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#if DEVICE_SERIAL

#include "serial_api.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include <string.h>

#define UART_BUFFER_SIZE 10

int stdio_uart_inited = 0; // used in platform/mbed_board.c and platform/mbed_retarget.cpp
uint8_t UARTReceiveBuffer[UART_BUFFER_SIZE];
serial_t stdio_uart;

void serial_tx_handler(void *CallBackRef, unsigned int EventData);
void serial_rx_handler(void *CallBackRef, unsigned int EventData);

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The serial object
 * @param tx  The TX pin name
 * @param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    uint8_t stdio_config = 0;

#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif
	
    XUartLite_Initialize(xuart, XPAR_AXI_UARTLITE_0_DEVICE_ID);
    XUartLite_Recv(xuart, UARTReceiveBuffer, UART_BUFFER_SIZE);

    // For stdio management in platform/mbed_board.c and platform/mbed_retarget.cpp
    if (stdio_config) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The serial object
 */
void serial_free(serial_t *obj)
{
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif
	
    XUartLite_ResetFifos(xuart);
}

/** Configure the baud rate
 *
 * @param obj      The serial object
 * @param baudrate The baud rate to be configured
 */

void serial_baud(serial_t *obj, int baudrate)
{
    //configured in Vivado Blockdiagram
}

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param obj       The serial object
 * @param data_bits The number of data bits
 * @param parity    The parity
 * @param stop_bits The number of stop bits
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    //configured in Vivado Blockdiagram
}

/** The serial interrupt handler registration
 *
 * @param obj     The serial object
 * @param handler The interrupt handler which will be invoked when the interrupt fires
 * @param id      The SerialBase object
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
	
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * @param obj    The serial object
 * @param irq    The serial IRQ type (RX or TX)
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif
	
	  if(irq == TxIrq && enable)
		{
			XUartLite_SetSendHandler(xuart, serial_tx_handler, &obj);
		}
		else if(irq == RxIrq && enable)
		{
			XUartLite_SetRecvHandler(xuart, serial_rx_handler, &obj);
		}
		
		if(enable) XUartLite_EnableInterrupt(xuart);
		else			 XUartLite_DisableInterrupt(xuart);
}

/** Get character. This is a blocking call, waiting for a character
 *
 * @param obj The serial object
 */
int  serial_getc(serial_t *obj)
{
	uint8_t c = 0;
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif

#ifndef SIM_BUILD
		while(XUartLite_IsReceiveEmpty(xuart->RegBaseAddress));
#endif
	
		XUartLite_Recv(xuart, (uint8_t*)&c, 1);
    return c;
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param obj The serial object
 * @param c   The character to be sent
 */
void serial_putc(serial_t *obj, int c)
{
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif

#ifndef SIM_BUILD
		while(XUartLite_IsTransmitFull(xuart->RegBaseAddress));
#endif
	
		XUartLite_Send(xuart, (uint8_t*)&c, 1);
}

/** Check if the serial peripheral is readable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be read, 0 if nothing to read
 */
int  serial_readable(serial_t *obj)
{
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif
	
		if(XUartLite_IsReceiveEmpty(xuart->RegBaseAddress)) return 0;
    else 																								return 1;
}

/** Check if the serial peripheral is writable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be written, 0 otherwise.
 */
int  serial_writable(serial_t *obj)
{
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif
	
	if(XUartLite_IsTransmitFull(xuart->RegBaseAddress)) return 0;
	else													 											return 1;
}

/** Clear the serial peripheral
 *
 * @param obj The serial object
 */
void serial_clear(serial_t *obj)
{
#if	DEVICE_SERIAL_ASYNCH
		XUartLite *xuart = &obj->serial->xuart;
#else
		XUartLite *xuart = &obj->xuart;
#endif
	
    XUartLite_ResetFifos(xuart);
}

/** Set the break
 *
 * @param obj The serial object
 */
void serial_break_set(serial_t *obj)
{
    
}

/** Clear the break
 *
 * @param obj The serial object
 */
void serial_break_clear(serial_t *obj)
{
    
}

/** Configure the TX pin for UART function.
 *
 * @param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx)
{
    //configured in Vivado Blockdiagram
}

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param obj    The serial object
 * @param type   The type of the flow control. Look at the available FlowControl types.
 * @param rxflow The TX pin name
 * @param txflow The RX pin name
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
		//configured in Vivado Blockdiagram
}

#if DEVICE_SERIAL_ASYNCH

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
 *
 * @param obj       The serial object
 * @param tx        The transmit buffer
 * @param tx_length The number of bytes to transmit
 * @param tx_width  Deprecated argument
 * @param handler   The serial handler
 * @param event     The logical OR of events to be registered
 * @param hint      A suggestion for how to use DMA with this transfer
 * @return Returns number of data transfered, otherwise returns 0
 */
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    return 0;
}

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
 *
 * @param obj        The serial object
 * @param rx         The receive buffer
 * @param rx_length  The number of bytes to receive
 * @param rx_width   Deprecated argument
 * @param handler    The serial handler
 * @param event      The logical OR of events to be registered
 * @param handler    The serial handler
 * @param char_match A character in range 0-254 to be matched
 * @param hint       A suggestion for how to use DMA with this transfer
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{

}

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    return 0;
}

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    return 0;
}

/** The asynchronous TX and RX handler.
 *
 * @param obj The serial object
 * @return Returns event flags if an RX transfer termination condition was met; otherwise returns 0
 */
int serial_irq_handler_asynch(serial_t *obj)
{
    return 0;
}

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flushes the TX hardware buffer if TX FIFO is used
 *
 * @param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
    
}

/** Abort the ongoing RX transaction. It disables the enabled interrupt for RX and
 *  flushes the RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{

}

#endif

void serial_tx_handler(void *CallBackRef, unsigned int EventData)
{
	// CallBackRef = serial_t object
}

void serial_rx_handler(void *CallBackRef, unsigned int EventData)
{
  // CallBackRef = serial_t object
}

#else

int serial_is_tx_ongoing(void) {
    return 0;
}

#endif /* DEVICE_SERIAL */

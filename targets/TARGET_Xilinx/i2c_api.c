#include "mbed_assert.h"
#include "i2c_api.h"
#include "xiic.h"

#if DEVICE_I2C

#define I2C_SLAVE_ADDRESS 0x00

static void SendHandler(i2c_t *obj);
static void RecvHandler(void *CallbackRef, int ByteCount);
static void StatusHandler(void *CallbackRef, int Status);

volatile struct {
	int EventStatus;
	int RemainingRecvBytes;
	int EventStatusUpdated;
	int RecvBytesUpdated;
} HandlerInfo;

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param sda  The sda pin (set in hardware)
 *  @param scl  The scl pin (set in hardware)
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
	struct i2c_s* obj_s = I2C_S(obj);
	XIic_Initialize(&obj_s->xi2c, XPAR_IIC_0_DEVICE_ID);

	XIic_SetSendHandler(&obj_s->xi2c, obj, (XIic_Handler) SendHandler);
	XIic_SetRecvHandler(&obj_s->xi2c, (void *)&HandlerInfo, RecvHandler);
	XIic_SetStatusHandler(&obj_s->xi2c, (void *)&HandlerInfo, StatusHandler);
	
	XIic_Start(&obj_s->xi2c);
	XIic_SetAddress(&obj_s->xi2c, XII_ADDR_TO_RESPOND_TYPE, I2C_SLAVE_ADDRESS);
	
	HandlerInfo.EventStatusUpdated = FALSE;
	HandlerInfo.RecvBytesUpdated = FALSE;
	
}

/** Configure the I2C frequency
 *
 *  @param obj The I2C object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz)
{
	// Change in Hardware
}

/** Send START command
 *
 *  @param obj The I2C object
 */
int  i2c_start(i2c_t *obj)
{
	return 0;
}

/** Send STOP command
 *
 *  @param obj The I2C object
 */
int  i2c_stop(i2c_t *obj)
{
	return 0;
}

/** Blocking reading data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
	struct i2c_s* obj_s = I2C_S(obj);
	
	XIic_SetAddress(&obj_s->xi2c, XII_ADDR_TO_SEND_TYPE, address);

	(void)XIic_MasterRecv(&obj_s->xi2c, (uint8_t*)data, length);
	
	while(1) {
		if(HandlerInfo.RecvBytesUpdated == TRUE && HandlerInfo.RemainingRecvBytes == 0) {
			return length;
		}

		if (HandlerInfo.EventStatusUpdated == TRUE) {
			return 0;
		}
	}
}

/** Blocking sending data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    Stop to be generated after the transfer is done
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
	struct i2c_s* obj_s = I2C_S(obj);
	
	XIic_SetAddress(&obj_s->xi2c, XII_ADDR_TO_SEND_TYPE, address);
	
	while(XIic_IsIicBusy(&obj_s->xi2c)); 
	
	(void) XIic_MasterSend(&obj_s->xi2c, (uint8_t*)data, length);
	
	while(XIic_IsIicBusy(&obj_s->xi2c)); 
	
	return length;
}

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop()
 *
 *  @param obj The I2C object
 */
void i2c_reset(i2c_t *obj)
{
	i2c_stop(obj);
}

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last)
{
	struct i2c_s* obj_s = I2C_S(obj);
	uint8_t data;
	
	(void)XIic_MasterRecv(&obj_s->xi2c, &data, 1);
	
	while(1) {
		if(HandlerInfo.RecvBytesUpdated == TRUE && HandlerInfo.RemainingRecvBytes == 0) {
			return data;
		}

		if (HandlerInfo.EventStatusUpdated == TRUE) {
			return 0;
		}
	}
}

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data)
{
	struct i2c_s* obj_s = I2C_S(obj);
	
	while(XIic_IsIicBusy(&obj_s->xi2c)); 
	
	(void) XIic_MasterSend(&obj_s->xi2c, (uint8_t*)data, 1);
	
	while(XIic_IsIicBusy(&obj_s->xi2c)); 
	
	return 1;
}

/**@}*/

#if DEVICE_I2CSLAVE

/**
 * \defgroup SynchI2C Synchronous I2C Hardware Abstraction Layer for slave
 * @{
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{

}

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int  i2c_slave_receive(i2c_t *obj)
{

}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return non-zero if a value is available
 */
int  i2c_slave_read(i2c_t *obj, char *data, int length)
{

}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @return non-zero if a value is available
 */
int  i2c_slave_write(i2c_t *obj, const char *data, int length)
{

}

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{

}

#endif

/**@}*/

#if DEVICE_I2C_ASYNCH

/**
 * \defgroup hal_AsynchI2C Asynchronous I2C Hardware Abstraction Layer
 * @{
 */

/** Start I2C asynchronous transfer
 *
 *  @param obj       The I2C object
 *  @param tx        The transmit buffer
 *  @param tx_length The number of bytes to transmit
 *  @param rx        The receive buffer
 *  @param rx_length The number of bytes to receive
 *  @param address   The address to be set - 7bit or 9bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param event     Event mask for the transfer. See \ref hal_I2CEvents
 *  @param hint      DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{

}

/** The asynchronous IRQ handler
 *
 *  @param obj The I2C object which holds the transfer information
 *  @return Event flags if a transfer termination condition was met, otherwise return 0.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{

}

/** Attempts to determine if the I2C peripheral is already in use
 *
 *  @param obj The I2C object
 *  @return Non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj)
{

}

/** Abort asynchronous transfer
 *
 *  This function does not perform any check - that should happen in upper layers.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj)
{

}

#endif // DEVICE_I2C_ASYNCH

static void RecvHandler(void *CallbackRef, int ByteCount)
{
	HandlerInfo.RemainingRecvBytes = ByteCount;
	HandlerInfo.RecvBytesUpdated = TRUE;
}

static void StatusHandler(void *CallbackRef, int Status)
{
	HandlerInfo.EventStatus |= Status;
	HandlerInfo.EventStatusUpdated = TRUE;
}

static void SendHandler(i2c_t *obj)
{
	//TransmitComplete = 0;
}


#endif // DEVICE_I2C

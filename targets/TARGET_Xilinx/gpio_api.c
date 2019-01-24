#include "platform/mbed_assert.h"
#include "hal/gpio_api.h"
#include "platform/mbed_error.h"
#include "PinNames.h"
#include "xgpio.h"

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC) {
        return;
    }
    int status = XGpio_Initialize(&obj->gpio, XILINX_DEV_ID(pin));

    if (status != XST_SUCCESS) {
		//return XST_FAILURE;
	}
}

inline void gpio_dir(gpio_t *obj, PinDirection direction)
{
    uint32_t current_dir = XGpio_GetDataDirection(&obj->gpio, XILINX_CHANNEL(obj->pin));

    
    if(direction == PIN_INPUT)
    {
        XGpio_SetDataDirection(&obj->gpio, XILINX_CHANNEL(obj->pin), current_dir | XILINX_GPIO_MASK(obj->pin));
    }
    else
    {
        XGpio_SetDataDirection(&obj->gpio, XILINX_CHANNEL(obj->pin), current_dir & ~XILINX_GPIO_MASK(obj->pin));
    }
}

void gpio_write(gpio_t *obj, int value)
{
    if(value == 0)
    {
        XGpio_DiscreteClear(&obj->gpio, XILINX_CHANNEL(obj->pin), XILINX_GPIO_MASK(obj->pin));
    }
    else
    {
        XGpio_DiscreteSet(&obj->gpio, XILINX_CHANNEL(obj->pin), XILINX_GPIO_MASK(obj->pin));
    }
}

int gpio_read(gpio_t *obj)
{
    if(XGpio_DiscreteRead(&obj->gpio, XILINX_CHANNEL(obj->pin)) & XILINX_GPIO_MASK(obj->pin))
    {
        return 1;
    }

    return 0;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    
}

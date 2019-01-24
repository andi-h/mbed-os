#include "mbed_assert.h"
#include "pinmap.h"

void pin_function(PinName name, int function)
{
    MBED_ASSERT(name != (PinName)NC);

}

void pin_mode(PinName name, PinMode mode)
{
    MBED_ASSERT(name != (PinName)NC);
   
}

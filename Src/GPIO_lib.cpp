
#include "GPIO_lib.h"


GPIOx::GPIOx(GPIO_TypeDef* port, uint8_t pin, Mode mode, Pull pull, OType type, Speed speed, uint8_t af) 
: port(port), pin(pin), mode(mode)
{
    port->MODER &= ~(3U << (pin * 2));
    port->MODER |=  (mode << (pin * 2));

    port->PUPDR &= ~(3U << (pin * 2));
    port->PUPDR |=  (pull << (pin * 2));

    if (mode == ModeOutput || mode == ModeAlternate)
    {
        port->OTYPER &= ~(1U << pin);
        port->OTYPER |=  (type << pin);

        port->OSPEEDR &= ~(3U << (pin * 2));
        port->OSPEEDR |=  (speed << (pin * 2));
    }

    if (mode == ModeAlternate)
    {
        uint32_t idx = pin >> 3;
        uint32_t pos = (pin & 7) * 4;

        port->AFR[idx] &= ~(0xF << pos);
        port->AFR[idx] |=  (af << pos);
    }
}


void GPIOx::set()
{
    if (mode != ModeOutput) return;
    port->BSRR = (1U << pin);
}

void GPIOx::reset()
{
    if (mode != ModeOutput) return;
    port->BSRR = (1U << (pin + 16));
}

void GPIOx::write(bool v)
{
    if (v) set();
    else   reset();
}

bool GPIOx::read()
{
    return (port->IDR >> pin) & 1U;
}


#ifndef __GPIO_LIB_H
#define __GPIO_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#ifdef __cplusplus
}
#endif

class GPIO
{
public:
    using Mode  = uint8_t;
    using Pull  = uint8_t;
    using OType = uint8_t;
    using Speed = uint8_t;

    static constexpr Mode  Input     = 0;
    static constexpr Mode  Output    = 1;
    static constexpr Mode  Alternate = 2;
    static constexpr Mode  Analog    = 3;

    static constexpr Pull  PullNone = 0;
    static constexpr Pull  PullUp   = 1;
    static constexpr Pull  PullDown = 2;

    static constexpr OType PushPull  = 0;
    static constexpr OType OpenDrain = 1;

    static constexpr Speed Low      = 0;
    static constexpr Speed Medium   = 1;
    static constexpr Speed High     = 2;
    static constexpr Speed VeryHigh = 3;

    GPIO(GPIO_TypeDef* port,
         uint8_t pin,
         Mode mode,
         Pull pull = PullNone,
         OType type = PushPull,
         Speed speed = High,
         uint8_t af = 0);

    void set();
    void reset();
    void write(bool v);
    void toggle();
    bool read();

private:
    GPIO_TypeDef* port;
    uint8_t pin;
    Mode mode;

};




#endif /* __MAIN_H */

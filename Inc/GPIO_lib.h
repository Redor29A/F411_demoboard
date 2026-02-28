
#ifndef __GPIO_LIB_H
#define __GPIO_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#ifdef __cplusplus
}
#endif

class GPIOx
{
public:

    using Mode  = uint8_t;
    using Pull  = uint8_t;
    using OType = uint8_t;
    using Speed = uint8_t;

    static constexpr uint8_t  ModeInput     = 0;
    static constexpr uint8_t  ModeOutput    = 1;
    static constexpr uint8_t  ModeAlternate = 2;
    static constexpr uint8_t  ModeAnalog    = 3;

    static constexpr uint8_t  PullNone = 0;
    static constexpr uint8_t  PullUp   = 1;
    static constexpr uint8_t  PullDown = 2;

    static constexpr uint8_t OTypePushPull  = 0;
    static constexpr uint8_t OTypeOpenDrain = 1;

    static constexpr uint8_t SpeedLow     = 0;
    static constexpr uint8_t SpeedMedium  = 1;
    static constexpr uint8_t SpeedHigh    = 2;
    static constexpr uint8_t SpeedVeryHigh= 3;

    GPIOx(GPIO_TypeDef* port,
         uint8_t pin,
         Mode mode,
         Pull pull = PullNone,
         OType type = OTypePushPull,
         Speed speed = SpeedHigh,
         uint8_t af = 0);

    void set();
    void reset();
    inline void high(){set();}
    inline void low(){reset();}
    inline void on(){set();};
    inline void off(){reset();}
    void write(bool v);
    void toggle();
    bool read();

private:
    GPIO_TypeDef* port;
    uint8_t pin;
    Mode mode;

};




#endif /* __MAIN_H */

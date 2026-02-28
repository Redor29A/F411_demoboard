#ifndef __SPI_LIB_H
#define __SPI_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#ifdef __cplusplus
}
#endif

class SPIx
{
public:
    using Mode        = uint8_t;
    using Direction   = uint8_t;
    using DataSize    = uint8_t;
    using ClockPol    = uint8_t;
    using ClockPhase  = uint8_t;
    using NSS         = uint8_t;
    using BaudRate    = uint8_t;
    using FirstBit    = uint8_t;

    static constexpr Mode ModeMaster = 1;
    static constexpr Mode ModeSlave  = 0;

    static constexpr Direction DirectionFullDuplex = 0;
    static constexpr Direction DirectionRxOnly     = 1;
    static constexpr Direction DirectionOneLineRx  = 2;
    static constexpr Direction DirectionOneLineTx  = 3;

    static constexpr DataSize DataSize8  = 0;
    static constexpr DataSize DataSize16 = 1;

    static constexpr ClockPol  ClockPol_Low  = 0;
    static constexpr ClockPol  ClockPol_High = 1;

    static constexpr ClockPhase ClockPhase_1Edge = 0;
    static constexpr ClockPhase ClockPhase_2Edge = 1;

    static constexpr NSS Software_NSS = 0;
    static constexpr NSS Hardware_NSS = 1;

    static constexpr FirstBit FirstBit_MSB = 0;
    static constexpr FirstBit FirstBit_LSB = 1;

    static constexpr BaudRate BaudRate_Div2   = 0;
    static constexpr BaudRate BaudRate_Div4   = 1;
    static constexpr BaudRate BaudRate_Div8   = 2;
    static constexpr BaudRate BaudRate_Div16  = 3;
    static constexpr BaudRate BaudRate_Div32  = 4;
    static constexpr BaudRate BaudRate_Div64  = 5;
    static constexpr BaudRate BaudRate_Div128 = 6;
    static constexpr BaudRate BaudRate_Div256 = 7;

    SPIx(SPI_TypeDef* spi,
         Mode mode = ModeMaster,
         Direction dir = DirectionFullDuplex,
         DataSize size = DataSize8,
         ClockPol cpol = ClockPol_Low,
         ClockPhase cpha = ClockPhase_1Edge,
         NSS nss = Software_NSS,
         BaudRate br = BaudRate_Div8,
         FirstBit first = FirstBit_MSB);

    uint8_t transfer(uint8_t data);
    void write(uint8_t data);
    inline void send(uint8_t data){write(data);}
    uint8_t read();
    inline uint8_t get(){return read();}

    void write_buf(const uint8_t* buf, uint32_t len);
    void read_buf(uint8_t* buf, uint32_t len);

    void enable();
    inline void on(){enable();}
    void disable();
    inline void off(){disable();}

private:
    SPI_TypeDef* spi;
};

#endif
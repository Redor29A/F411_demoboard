
#ifndef __SPI_LIB_H
#define __SPI_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#ifdef __cplusplus
}
#endif
#include "GPIO_lib.h"


class SPI {
    public:
        SPI();
        void write(uint8_t data);
        uint8_t read();

    private:

};



#endif /* __MAIN_H */

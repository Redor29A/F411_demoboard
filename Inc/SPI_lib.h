
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
    protected:
        uint8_t xstart, ystart, xend, yend, xoffs, yoffs;
        uint16_t _widthIni, _heightIni;

        void displayInit();
        void writeSPI(uint8_t);
        void writeMulti(uint16_t color, uint16_t num);
        void copyMulti(uint8_t *img, uint16_t num);
        void writeCmd(uint8_t c);
        void writeData(uint8_t d8);
        void writeData16(uint16_t d16);
        void commonInit(const uint8_t *cmdList);
        void delay();

        void csLow();
        void csHigh(); 

        void dcCMD();
        void dcData();

        void blOn(); 
        void blOff();  

    private:
        GPIO cs, dc, rst;
};



#endif /* __MAIN_H */

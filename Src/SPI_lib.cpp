#include "SPI_lib.h"


void SPI::displayInit()
{

}

void SPI::writeSPI(uint8_t)
{

}

void SPI::writeMulti(uint16_t color, uint16_t num)
{

}

void SPI::copyMulti(uint8_t *img, uint16_t num)
{

}

void SPI::writeCmd(uint8_t c)
{

}

void SPI::writeData(uint8_t d8)
{

}

void SPI::writeData16(uint16_t d16)
{

}

void SPI::commonInit(const uint8_t *cmdList)
{

}

void SPI::delay()
{

}

void SPI::csLow()
{

}

void SPI::csHigh() { GPIOA->BSRR = GPIO_BSRR_BR4; }
void SPI::dcCMD() { }
void SPI::dcData() { }

void SPI::blOn() { }

void SPI::blOff() { }

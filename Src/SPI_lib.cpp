#include "SPI_lib.h"

SPIx::SPIx(SPI_TypeDef* spi,
           Mode mode,
           Direction dir,
           DataSize size,
           ClockPol cpol,
           ClockPhase cpha,
           NSS nss,
           BaudRate br,
           FirstBit first) : spi(spi)
{

    spi->CR1 = 0;
    spi->CR2 = 0;

    if (mode == ModeMaster) spi->CR1 |= SPI_CR1_MSTR;

    // Direction
    if (dir == DirectionRxOnly) spi->CR1 |= SPI_CR1_RXONLY;
    else if (dir == DirectionOneLineRx) spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
    else if (dir == DirectionOneLineTx) spi->CR1 |= SPI_CR1_BIDIMODE;

    // Data size
    if (size == DataSize16) spi->CR1 |= SPI_CR1_DFF;

    // Clock
    if (cpol == ClockPol_High)  spi->CR1 |= SPI_CR1_CPOL;
    if (cpha == ClockPhase_2Edge) spi->CR1 |= SPI_CR1_CPHA;

    // NSS
    if (nss == Software_NSS)
        spi->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    else
        spi->CR2 |= SPI_CR2_SSOE;

    // Baud rate
    spi->CR1 |= (br << SPI_CR1_BR_Pos);

    // First bit
    if (first == FirstBit_LSB) spi->CR1 |= SPI_CR1_LSBFIRST;

    // Enable SPI
    spi->CR1 |= SPI_CR1_SPE;
}

uint8_t SPIx::transfer(uint8_t data)
{
    while (!(spi->SR & SPI_SR_TXE));
    spi->DR = data;
    while (!(spi->SR & SPI_SR_RXNE));
    return *(__IO uint8_t*)&spi->DR;
}

void SPIx::write(uint8_t data)
{
    while (!(spi->SR & SPI_SR_TXE));
    spi->DR = data;
}

uint8_t SPIx::read()
{
    return transfer(0xFF);
}

void SPIx::write_buf(const uint8_t* buf, uint32_t len)
{
    while (len--) write(*buf++);
}

void SPIx::read_buf(uint8_t* buf, uint32_t len)
{
    while (len--) *buf++ = read();
}

void SPIx::enable()
{
    spi->CR1 |= SPI_CR1_SPE;
}

void SPIx::disable()
{
    spi->CR1 &= ~SPI_CR1_SPE;
}
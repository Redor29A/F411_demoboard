#include "ST7796_lib.h"

ST7769::ST7769(SPIx& spi, GPIOx& DC, GPIOx& CS, GPIOx& RST): spi(spi), DC(DC), CS(CS), RST(RST){
    CS.high(); 
    RST.high();
}

void ST7769::set_memory_area(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye){
    write_cmd(0x2A);  // Column
    write_data(xs >> 8);
    write_data(xs);
    write_data(ys >> 8);
    write_data(ys);

    write_cmd(0x2B);  // Row
    write_data(xe >> 8);
    write_data(xe);
    write_data(ye >> 8);
    write_data(ye);

    write_cmd(0x2C);  // Memory Write
}

void ST7769::memory_write_start(){
    DC.low();
    CS.low();
    spi.write(ST7769_CMD_RAMWR);
}

void ST7769::memory_write_end(){
    CS.high();
}

void ST7769::set_pixel(uint16_t x, uint16_t y, uint16_t color){
    set_memory_area(x, y, x, y);
    DC.high();
    CS.low();
    spi.write(color >> 8);
    spi.write(color);
    CS.high();
}

void ST7769::write_multi(uint16_t color, uint16_t num){

}

void ST7769::copy_multi(uint8_t *img, uint16_t num){

}

void ST7769::write_cmd(uint8_t cmd){
    DC.low();
    CS.low();
    spi.write(cmd);
    CS.high();

}

void ST7769::write_data(uint8_t data){
    DC.high();
    CS.low();
    spi.write(data);
    CS.high();

}

void ST7769::write_data16(uint16_t data_16){
    DC.high();
    CS.low();
    spi.write(data_16 >> 8);
    spi.write(data_16);
    CS.high();
}

void ST7769::fill(uint16_t color)
{
    set_memory_area(0, 0, 319, 479);

    DC.high();
    CS.low();

    for (uint32_t i = 0; i < 320UL * 480; i++)
    {
        spi.write(color >> 8);
        spi.write(color);
    }

    CS.high();
}
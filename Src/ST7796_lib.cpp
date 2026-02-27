#include "ST7796_lib.h"

ST7769::ST7769(SPI& spi, GPIO& DC, GPIO& CS, GPIO& RST): spi(spi), DC(DC), CS(CS), RST(RST){
    CS.high(); 
    RST.high();
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
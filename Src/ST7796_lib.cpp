#include "ST7796_lib.h"

ST7796::ST7796(SPIx& spi, GPIOx& DC, GPIOx& CS, GPIOx& RST): spi(spi), DC(DC), CS(CS), RST(RST){

}

void ST7796::init(){
    //CS.high(); 
    RST.high();
}

void ST7796::set_memory_area(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye){
    write_cmd(ST7796_CMD_CASET);  // Column
    write_data(xs >> 8);
    write_data(xs);
    write_data(xe >> 8);
    write_data(xe);

    write_cmd(ST7796_CMD_RASET);  // Row
    write_data(ys >> 8);
    write_data(ys);
    write_data(ye >> 8);
    write_data(ye);
}

void ST7796::column_addres_set(uint16_t xs, uint16_t xe){
    write_cmd(ST7796_CMD_CASET);  // Column
    write_data(xs >> 8);
    write_data(xs);
    write_data(xe >> 8);
    write_data(xe);

}

void ST7796::row_addres_set(uint16_t ys, uint16_t ye){
    write_cmd(ST7796_CMD_RASET);  // Row
    write_data(ys >> 8);
    write_data(ys);
    write_data(ye >> 8);
    write_data(ye);
}

void ST7796::memory_write_start(){
    DC.low();
    CS.low();
    spi.write_wait_BSY(ST7796_CMD_RAMWR);
    DC.high();
}

void ST7796::memory_write_end(){
    //CS.high();
}

void ST7796::memory_write(uint8_t data_8){
    spi.write(data_8);
}

void ST7796::memory_write(uint16_t data_16){
    spi.write(data_16 >> 8);
    spi.write(data_16);
}

void ST7796::set_pixel(uint16_t x, uint16_t y, uint16_t color){
    set_memory_area(x, y, x, y);
    DC.high();
    CS.low();
    spi.write(color >> 8);
    spi.write(color);
    //CS.high();
}

void ST7796::write_multi(uint16_t color, uint16_t num){

}

void ST7796::copy_multi(uint8_t *img, uint16_t num){

}

void ST7796::write_cmd(uint8_t cmd){
    DC.low();
    CS.low();
    spi.write(cmd);
    //CS.high();

}

void ST7796::write_data(uint8_t data){
    DC.high();
    CS.low();
    spi.write(data);
    //CS.high();

}

void ST7796::write_data16(uint16_t data_16){
    DC.high();
    CS.low();
    spi.write(data_16 >> 8);
    spi.write(data_16);
    //CS.high();
}
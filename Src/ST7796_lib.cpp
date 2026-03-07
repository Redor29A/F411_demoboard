#include "ST7796_lib.h"

ST7796::ST7796(SPIx& spi, GPIOx& DC, GPIOx& CS, GPIOx& RST): spi(spi), DC(DC), CS(CS), RST(RST){
    CS.high(); 
    RST.high();
}

void ST7796::display_reset(){
    RST.low();
    for(uint32_t t=0; t < 60000; t++){__NOP();}

    RST.high();
    for(uint32_t t=0; t < 600000; t++){__NOP();}
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

void ST7796::write_from_mass(const uint16_t *img, uint32_t num){
     for (uint32_t i = 0; i < num; i++) {
        spi.write(img[i] >> 8);
        spi.write(img[i]);
    }
}

void ST7796::write_cmd(uint8_t cmd){
    DC.low();
    CS.low();
    spi.write(cmd);
    //CS.high();
}

void ST7796::write_data(uint8_t data_8){
    DC.high();
    CS.low();
    spi.write(data_8);
    //CS.high();
}

void ST7796::write_data16(uint16_t data_16){
    DC.high();
    CS.low();
    spi.write(data_16 >> 8);
    spi.write(data_16);
    //CS.high();
}
#include "display_lib.h"

display::display(ST7796& st7796, uint16_t width, uint16_t height) : st7796(st7796), width(width), height(height)
{
    
}

void display::fillScreen(uint16_t color){
    st7796.set_memory_area(0, 0, width, height);

    st7796.memory_write_start();

    for (uint32_t i = 0; i < width * height; i++)
    {
        st7796.memory_write(color);
    }

    st7796.memory_write_end();
}

void display::drawPixel(int16_t x, int16_t y, uint16_t color){
    st7796.set_memory_area(x, y, x+1, y+1);

    st7796.memory_write_start();
    st7796.memory_write(color);
    st7796.memory_write_end();
}

void display::drawVLine(int16_t x, int16_t y1, int16_t y2, uint16_t color, uint8_t thicknes){
    st7796.set_memory_area(x, y1, x+thicknes, y2);

    st7796.memory_write_start();

    for (uint32_t i = 0; i < (uint32_t)(y2 - y1) * thicknes; i++)
    {
        st7796.memory_write(color);
    }

    st7796.memory_write_end();
}

void display::drawHLine(int16_t y, int16_t x1, int16_t x2, uint16_t color, uint8_t thicknes){
    st7796.set_memory_area(x1, y, x2, y+thicknes);

    st7796.memory_write_start();

    for (uint32_t i = 0; i < (uint32_t)(x2 - x1) * thicknes; i++)
    {
        st7796.memory_write(color);
    }

    st7796.memory_write_end();
}

void display::fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
    st7796.set_memory_area(x1, y1, x2, y2);

    st7796.memory_write_start();

    for (uint32_t i = 0; i < (uint32_t)(x2 - x1) * (uint32_t)(y2 - y1); i++)
    {
        st7796.memory_write(color);
    }

    st7796.memory_write_end();
}

void display::drawImage(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const uint16_t *img){
    st7796.set_memory_area(x1, y1, x2, y2);

    st7796.memory_write_start();

    st7796.write_from_mass(img, (uint32_t)((x2 - x1) * (y2 - y1)));

    st7796.memory_write_end();
}
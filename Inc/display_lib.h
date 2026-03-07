

#ifndef __DISPLAY_LIB_H
#define __DISPLAY_LIB_H

#include "ST7796_lib.h"

#define WIDTH 	320
#define HEIGHT 	480

#define RGBto565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)) 
#define RGBIto565(r,g,b,i) ((((((r)*(i))/255) & 0xF8) << 8) | ((((g)*(i)/255) & 0xFC) << 3) | ((((b)*(i)/255) & 0xFC) >> 3)) 

#define	RGB565_BLACK   0x0000
#define	RGB565_BLUE    0x001F
#define	RGB565_RED     0xF800
#define	RGB565_GREEN   0x07E0
#define RGB565_CYAN    0x07FF
#define RGB565_MAGENTA 0xF81F
#define RGB565_YELLOW  0xFFE0
#define RGB565_WHITE   0xFFFF
#define	RGB565_GREY  RGBto565(128,128,128)
#define	RGB565_LGREY RGBto565(160,160,160)
#define	RGB565_DGREY RGBto565( 80, 80, 80)
#define	RGB565_LBLUE RGBto565(100,100,255)
#define	RGB565_DBLUE RGBto565(  0,  0,128)

class display
{

public:
    display(ST7796& st7796, uint16_t width, uint16_t height);
    void fillScreen(uint16_t color=RGB565_BLACK);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawVLine(int16_t x, int16_t y1, int16_t y2, uint16_t color, uint8_t thicknes=1);
    void drawHLine(int16_t y, int16_t x1, int16_t x2, uint16_t color, uint8_t thicknes=1);
    void fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawImage(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const uint16_t* img);

    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return Color565(r, g, b); } 
    void rgbWheel(int idx, uint8_t *_r, uint8_t *_g, uint8_t *_b);
    uint16_t rgbWheel(int idx);
private:
    ST7796& st7796;
    uint16_t width;
    uint16_t height;
};








#endif /* __MAIN_H */

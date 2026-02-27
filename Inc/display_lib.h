

#ifndef __DISPLAY_LIB_H
#define __DISPLAY_LIB_H

#include <cstdint>

#define WIDTH 	320
#define HEIGHT 	480

#define RGBto565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)) 
#define RGBIto565(r,g,b,i) ((((((r)*(i))/255) & 0xF8) << 8) | ((((g)*(i)/255) & 0xFC) << 3) | ((((b)*(i)/255) & 0xFC) >> 3)) 

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define	GREY  RGBto565(128,128,128)
#define	LGREY RGBto565(160,160,160)
#define	DGREY RGBto565( 80, 80, 80)
#define	LBLUE RGBto565(100,100,255)
#define	DBLUE RGBto565(  0,  0,128)

class display
{

public:
    display(int8_t DC, int8_t RST=-1, int8_t CS=-1);
    void init(uint16_t wd, uint16_t ht);
    void begin() { init(WIDTH, HEIGHT); }
    void init() { init(WIDTH, HEIGHT); }
    void setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);
    void pushColor(uint16_t color);
    void fillScreen(uint16_t color=BLACK);
    void clearScreen() { fillScreen(BLACK); }
    void cls() { fillScreen(BLACK); }
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawImage(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img);
    void drawImageF(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img16);
    void setRotation(uint8_t r);
    void invertDisplay(bool mode);
    void partialDisplay(bool mode);
    void sleepDisplay(bool mode);
    void enableDisplay(bool mode);
    void idleDisplay(bool mode);
    void resetDisplay();
    void setScrollArea(uint16_t tfa, uint16_t bfa);
    void setScroll(uint16_t vsp);
    void setPartArea(uint16_t sr, uint16_t er);
    void setBrightness(uint8_t br);
    void powerSave(uint8_t mode);
    void setCGRAM(uint8_t xs, uint8_t xe, uint8_t ys, uint8_t ye);

    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return Color565(r, g, b); } 
    void rgbWheel(int idx, uint8_t *_r, uint8_t *_g, uint8_t *_b);
    uint16_t rgbWheel(int idx);
private:
    /* data */

};








#endif /* __MAIN_H */

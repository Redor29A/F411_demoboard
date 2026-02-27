
#ifndef __ST7769_LIB_H
#define __ST7769_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#ifdef __cplusplus
}
#endif
#define ST7769_CMD_NOP 0x00
#define ST7769_CMD_SWRESET 0x01 //Software Reset
#define ST7769_CMD_RDDID 0x04 //Read Display ID 
#define ST7769_CMD_ReadDSI 0x05 //Read Number of the Errors on DSI
#define ST7769_CMD_RDDST 0x09 //Read Display Status
#define ST7769_CMD_RDDPM 0x0A //Read Display Power Mode
#define ST7769_CMD_RDDMADCTL 0x0B //Read Display MADCTL
#define ST7769_CMD_RDDCOLMOD 0x0C //Read Display Pixel Format 
#define ST7769_CMD_RDDIM 0x0D //Read Display Image Mode 
#define ST7769_CMD_RDDSM 0x0E //Read Display Signal Mode 
#define ST7769_CMD_RDDSDR 0x0F //Read Display Self-Diagnostic Result
#define ST7769_CMD_SLPIN 0x10 //Sleep in
#define ST7769_CMD_SLPOUT 0x11 //Sleep Out 
#define ST7769_CMD_PTLON 0x12 //Partial Display Mode On
#define ST7769_CMD_NORON 0x13 //Normal Display Mode On 
#define ST7769_CMD_INVOFF 0x20 //Display Inversion Off
#define ST7769_CMD_INVON 0x21 //Display Inversion On
#define ST7769_CMD_DISPOFF 0x28 //Display Off
#define ST7769_CMD_DISPON 0x29 //Display On
#define ST7769_CMD_CASET 0x2A //Column Address Set 
#define ST7769_CMD_RASET 0x2B //Row Address Set
#define ST7769_CMD_RAMWR 0x2C //Memory Write 
#define ST7769_CMD_RAMRD 0x2E //Memory Read 
#define ST7769_CMD_PTLAR 0x30 //Partial Area
#define ST7769_CMD_VSCRDEF 0x33 //Vertical Scrolling Definition
#define ST7769_CMD_TEOFF 0x34 //Tearing Effect Line OFF 
#define ST7769_CMD_TEON 0x35 //Tearing Effect Line On
#define ST7769_CMD_MADCTL 0x36 //Memory Data Access Control
#define ST7769_CMD_VSCSAD 0x37 //Vertical Scroll Start Address of RAM 
#define ST7769_CMD_IDMOFF 0x38 //Idle Mode Off 
#define ST7769_CMD_IDMON 0x39 //Idle mode on 
#define ST7769_CMD_COLMOD 0x3A //Interface Pixel Format
#define ST7769_CMD_WRMEMC 0x3C //Write Memory Continue 
#define ST7769_CMD_RDMEMC 0x3E //Read Memory Continue
#define ST7769_CMD_STE 0x44 //Set Tear Scanline
#define ST7769_CMD_GSCAN 0x45 //Get Scanline 
#define ST7769_CMD_WRDISBV 0x51 //Write Display Brightness 
#define ST7769_CMD_RDDISBV 0x52 //Read Display Brightness Value
#define ST7769_CMD_WRCTRLD 0x53 //Write CTRL Display
#define ST7769_CMD_RDCTRLD 0x54 //Read CTRL value Display
#define ST7769_CMD_WRCABC 0x55 //Write Adaptive Brightness Control
#define ST7769_CMD_RDCABC 0x56 //Read Content Adaptive Brightness Control
#define ST7769_CMD_WRCABCMB 0x5E //Write CABC Minimum Brightness
#define ST7769_CMD_RDCABCMB 0x5F //Read CABC Minimum Brightness
#define ST7769_CMD_RDFCS 0xAA //Read First Checksum 
#define ST7769_CMD_RDCFCS 0xAF //Read Continue Checksum
#define ST7769_CMD_RDID1 0xDA //Read ID1
#define ST7769_CMD_RDID2 0xDB //Read ID2
#define ST7769_CMD_RDID3 0xDC //Read ID3
#define ST7769_CMD_IFMODE 0xB0 //Interface Mode Control
#define ST7769_CMD_FRMCTR1 0xB1 //Frame Rate Control (In Normal Mode/Full Colors) 
#define ST7769_CMD_FRMCTR2 0xB2 //Frame Rate Control 2 (In Idle Mode/8 colors)
#define ST7769_CMD_FRMCTR3 0xB3 //Frame Rate Control3 (In Partial Mode/Full Colors)
#define ST7769_CMD_DIC 0xB4 //Display Inversion Control 
#define ST7769_CMD_BPC 0xB5 //Blanking Porch Control 
#define ST7769_CMD_DFC 0xB6 //Display Function Control 
#define ST7769_CMD_EM 0xB7 //Entry Mode Set 
#define ST7769_CMD_PWR1 0xC0 //Power Control 1 
#define ST7769_CMD_PWR2 0xC1 //Power Control 2 
#define ST7769_CMD_PWR3 0xC2 //Power Control 3 
#define ST7769_CMD_VCMPCTL 0xC5 //VCOM Control 
#define ST7769_CMD_VCMOffset 0xC6 //Vcom Offset Register 
#define ST7769_CMD_NVMADW 0xD0 //NVM Address/Data Write
#define ST7769_CMD_NVMBPROG 0xD1 //NVM Byte Program
#define ST7769_CMD_NVMStRead 0xD2 //NVM Status Read
#define ST7769_CMD_RDID4 0xD3 //Read ID4
#define ST7769_CMD_PGC 0xE0 //Positive Gamma Control 
#define ST7769_CMD_NGC 0xE1 //Negative Gamma Control
#define ST7769_CMD_DGC1 0xE2 //Digital Gamma Control 1 
#define ST7769_CMD_DGC2 0xE3 //Digital Gamma Control 2 
#define ST7769_CMD_DOCA 0xE8 //Display Output Ctrl Adjust
#define ST7769_CMD_CSCON 0xF0 //Command Set Control
#define ST7769_CMD_SPIRdCtrl 0xFB //SPI Read Control


class ST7796 {
    public:

        void displayInit();
        void writeMulti(uint16_t color, uint16_t num);
        void copyMulti(uint8_t *img, uint16_t num);
        void writeCmd(uint8_t c);
        void writeData(uint8_t d8);
        void writeData16(uint16_t d16);

        void csLow();
        void csHigh(); 

        void dcCMD();
        void dcData();

        void blOn(); 
        void blOff(); 
    private:

};

#endif /* __MAIN_H */

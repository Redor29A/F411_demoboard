/*
 * Petrik coding this shit 
 * 22.02.26
 * And Lyoha too
 * 23.02.26 
 *
 * Demo board connections
 * 
 * disp st7796U 320x480
 * 1(GND) - GND
 * 2(VCC) - 3v3
 * 3(SCL) - A5
 * 4(SDA) - A7
 * 5(RST) - A3
 * 6(DC) - A2
 * 7(CS) - A4
 * 8(BL) - A1
 * 9(SDA-O) - A6
 * 
 * buttons{
 *     UP
 * LFT MID RHT
 *     DWN
 * SET     RST
 * }
 * COM - GND
 * UP - B6
 * DWN - B5 
 * LFT - C14 
 * RHT - B4
 * MID - B9
 * SET - B8
 * RST - B7
 * 
 * Micro sd card
 * 1(NC) 
 * 2(CS) - B12
 * 3(MOSI) - B15
 * 4(VDD) - 3V3
 * 5(CLOCK) - B13
 * 6(GND) - GND
 * 7(MISO) - B14
 * 8(NC) 
 */

#include "main.h"

GPIOx SCL(GPIOA, 5, GPIOx::ModeAlternate, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh, 5);
GPIOx SDA(GPIOA, 7, GPIOx::ModeAlternate, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh, 5);
GPIOx SDA_O(GPIOA, 6, GPIOx::ModeInput);
GPIOx DC(GPIOA, 2, GPIOx::ModeOutput, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh);
GPIOx CS(GPIOA, 4, GPIOx::ModeOutput, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh);
GPIOx BL(GPIOA, 1, GPIOx::ModeOutput, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh);
GPIOx RST(GPIOA, 3, GPIOx::ModeOutput, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh);

SPIx spi(SPI1, SPIx::ModeMaster, SPIx::DirectionFullDuplex,
             SPIx::DataSize8, SPIx::ClockPol_Low, SPIx::ClockPhase_1Edge,
             SPIx::Software_NSS, SPIx::BaudRate_Div2, SPIx::FirstBit_MSB);
    
ST7796 st7796(spi, DC, CS, RST);

GPIOx BTN_UP (GPIOB, 6, GPIOx::ModeInput, GPIOx::PullUp);
GPIOx BTN_DWN(GPIOB, 5, GPIOx::ModeInput, GPIOx::PullUp);
GPIOx BTN_LFT(GPIOC, 14, GPIOx::ModeInput, GPIOx::PullUp);
GPIOx BTN_RHT(GPIOB, 4, GPIOx::ModeInput, GPIOx::PullUp);
GPIOx BTN_MID(GPIOB, 9, GPIOx::ModeInput, GPIOx::PullUp);
GPIOx BTN_SET(GPIOB, 8, GPIOx::ModeInput, GPIOx::PullUp);
GPIOx BTN_RST(GPIOB, 7, GPIOx::ModeInput, GPIOx::PullUp);

void delay_us(uint32_t us)
{
    volatile uint32_t n;

    while (us--)
    {
        n = 6;
        while (n--)
            __NOP();
    }
}

void delay_ms(uint32_t ms)
{
    delay_us(ms * 1000);
}

void SysTick_Init()
{
    SysTick_Config(SystemCoreClock / 1000);
}

extern "C" void SysTick_Handler(void)
{
    //1ms
}

void RCC_config(){     //Хеллоу дорогоие подпищики сегодня мы будем оверклочить Arm Cortex M4 до 100 Mhz 
    RCC->CR |= RCC_CR_HSEON; // заводим наше кварцевое двигало
    while (!(RCC->CR & RCC_CR_HSERDY));  //ждём прогрева до рабочих температур

    RCC->CR &= ~RCC_CR_PLLON;  // нажимаем тормоз чтобы не улететь раньше времени 

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;  // вырубаем диферециал 
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;
    RCC->PLLCFGR |= (0b00 << RCC_PLLCFGR_PLLP_Pos);  // подключаем турбину 
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR |= (200U << RCC_PLLCFGR_PLLN_Pos);  // закись озота 
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    RCC->PLLCFGR |= (25U << RCC_PLLCFGR_PLLM_Pos);  // пиздатый ручник

    RCC->CR |= RCC_CR_PLLON;  // выжимаем сцепление 
    while (!(RCC->CR & RCC_CR_PLLRDY)); // ждём зелёный свет

    RCC->CFGR &= ~(RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_HPRE | RCC_CFGR_SW);  // врубаем передачу 
    RCC->CFGR |= (0b000 << RCC_CFGR_PPRE2_Pos);
    RCC->CFGR |= (0b100 << RCC_CFGR_PPRE1_Pos);
    RCC->CFGR |= (0b000 << RCC_CFGR_HPRE_Pos);

    FLASH->ACR |= FLASH_ACR_ICEN  | FLASH_ACR_DCEN  | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS;  // Штурман! не ссать!
    // *врум врум врум 
    RCC->CFGR |= (0b10 << RCC_CFGR_SW_Pos);  // >>>>>>вжжуууууу>>>>>>> полетели 
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));  

    SystemCoreClockUpdate();  // Смотрим на спидометр
} 



void display_reset(GPIOx& rst){
    rst.low();
    delay_ms(20);

    rst.high();
    delay_ms(120);
}

void fill(ST7796 st7796, uint16_t color){
    st7796.set_memory_area(0, 0, 479, 319);

    st7796.memory_write_start();

    for (uint32_t i = 0; i < 320UL * 480; i++)
    {
        st7796.memory_write(color);
    }

    st7796.memory_write_end();
}

int main(void)
{
    RCC_config();
    SysTick_Init();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //тактирование GPIOA
    GPIOx SCL(GPIOA, 5, GPIOx::ModeAlternate, GPIOx::PullNone,
            GPIOx::OTypePushPull, GPIOx::SpeedHigh, 5);
    GPIOx SDA(GPIOA, 7, GPIOx::ModeAlternate, GPIOx::PullNone,
                GPIOx::OTypePushPull, GPIOx::SpeedHigh, 5);
    GPIOx SDA_O(GPIOA, 6, GPIOx::ModeInput);
    GPIOx DC(GPIOA, 2, GPIOx::ModeOutput, GPIOx::PullNone,
                GPIOx::OTypePushPull, GPIOx::SpeedHigh);
    GPIOx CS(GPIOA, 4, GPIOx::ModeOutput, GPIOx::PullNone,
                GPIOx::OTypePushPull, GPIOx::SpeedHigh);
    GPIOx BL(GPIOA, 1, GPIOx::ModeOutput, GPIOx::PullNone,
                GPIOx::OTypePushPull, GPIOx::SpeedHigh);
    GPIOx RST(GPIOA, 3, GPIOx::ModeOutput, GPIOx::PullNone,
                GPIOx::OTypePushPull, GPIOx::SpeedHigh);

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //тактирование SPI1
    SPIx spi(SPI1, SPIx::ModeMaster, SPIx::DirectionFullDuplex,
                SPIx::DataSize8, SPIx::ClockPol_Low, SPIx::ClockPhase_1Edge,
                SPIx::Software_NSS, SPIx::BaudRate_Div2, SPIx::FirstBit_MSB);
    
    ST7796 st7796(spi, DC, CS, RST);

    BL.set();
    //st7796.display_reset();
    display_reset(RST);
    delay_ms(120);
    st7796.sleep_out();
    st7796.write_cmd(ST7796_CMD_MADCTL);
    st7796.write_data(0x28); // поворот + RGB
    st7796.write_cmd(ST7796_CMD_COLMOD);
    st7796.write_data(0x55); // Pixel format 16-bit
    st7796.set_inversion_off();
    st7796.display_on();
    delay_ms(100);

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 
    GPIOx BTN_UP (GPIOB, 6, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx BTN_DWN(GPIOB, 5, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx BTN_LFT(GPIOC, 14, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx BTN_RHT(GPIOB, 4, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx BTN_MID(GPIOB, 9, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx BTN_SET(GPIOB, 8, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx BTN_RST(GPIOB, 7, GPIOx::ModeInput, GPIOx::PullUp);
    GPIOx LED(GPIOC, 13, GPIOx::ModeOutput, GPIOx::PullNone,
             GPIOx::OTypePushPull, GPIOx::SpeedHigh);
    
    fill(st7796, 0xFFFF);

    display disp(st7796, 479, 319);

    disp.drawImage(0, 0, 479, 319, bitmap);

    delay_ms(1000);

    while (1)
    {
        /*fill(st7796, 0xFFFF);
        delay_ms(1000);

        fill(st7796, 0x0000);
        delay_ms(1000);

        LED.toggle();*/
    }
}



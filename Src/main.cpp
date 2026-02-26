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

#define CS_LOW()    (GPIOA->BSRR = GPIO_BSRR_BR4)
#define CS_HIGH()   (GPIOA->BSRR = GPIO_BSRR_BS4)

#define DC_CMD()    (GPIOA->BSRR = GPIO_BSRR_BR2)
#define DC_DATA()   (GPIOA->BSRR = GPIO_BSRR_BS2)

#define BL_ON()     (GPIOA->BSRR = GPIO_BSRR_BS1)
#define BL_OFF()    (GPIOA->BSRR = GPIO_BSRR_BR1)

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
/*           † ᏒᏆᎵ †                        (Слишком медленно)
void RCC_config(){ //25 MHz all clocks
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    RCC->CFGR &= ~(RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_HPRE | RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while (!(RCC->CFGR & RCC_CFGR_SWS_HSE));
    SystemCoreClockUpdate(); 
}*/

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

void SPI1_config()
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //тактирование SPI1
    __NOP(); __NOP();
    //NSS output enabled (SSM = 0, SSOE = 1) CPOL = 0 CPHA = 0 LSBFIRST = 0 DFF = 0
    //FRF   MSTR and SPE bits must be set 
    SPI1->CR1 = 0; //сброс всех битов
    SPI1->CR2 = 0; //сброс всех битов
    SPI1->I2SCFGR = 0; //сброс всех битов

    SPI1->CR1 |= SPI_CR1_MSTR; //режим ведущего
    SPI1->CR1 |= (0b000 << SPI_CR1_BR_Pos); //делитель /2 
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; //ручное управление NSS

    SPI1->CR1 |= SPI_CR1_SPE; //включить SPI
}

void SPI1_write(uint8_t data){
    //while (SPI1->SR & SPI_SR_BSY);
    SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_TXE));     
}

uint8_t SPI1_read(uint8_t timeout = 10){
    while (!(SPI1->SR & SPI_SR_RXNE)); 
    uint8_t RX_data = SPI1->DR;
    return RX_data;
}

void display_pins_config(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //тактирование GPIOA
     
    //A5 - SCL
    GPIOA->MODER &= ~GPIO_MODER_MODER5;   
    GPIOA->MODER |=  (2U << GPIO_MODER_MODER5_Pos);   // 10 = AF

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;   
    GPIOA->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED5_Pos);   // высокая скорость

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;   
    GPIOA->AFR[0] |= (5U << GPIO_AFRL_AFSEL5_Pos);   // AF5 = SPI1 SCL

    //A6 - SDA-O
    GPIOA->MODER &= ~GPIO_MODER_MODER6;   // 00 = INPUT

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;   // без подтяжек

    //A7 - SDA
    GPIOA->MODER &= ~GPIO_MODER_MODER7;   
    GPIOA->MODER |=  (2U << GPIO_MODER_MODER7_Pos);   // 10 = AF

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED7;   
    GPIOA->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED7_Pos);   // высокая скорость

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7;   
    GPIOA->AFR[0] |= (5U << GPIO_AFRL_AFSEL7_Pos);   // AF5 = SPI1 SDA

    //A1 - BL
    GPIOA->MODER &= ~GPIO_MODER_MODER1;   
    GPIOA->MODER |=  (1U << GPIO_MODER_MODER1_Pos);   // 01 = output

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT1;        // push-pull 

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1;
    GPIOA->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED1_Pos);   // высокая скорость

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;   // без подтяжек     

    //A2 - DC
    GPIOA->MODER &= ~GPIO_MODER_MODER2;   
    GPIOA->MODER |=  (1U << GPIO_MODER_MODER2_Pos);   // 01 = output

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;        // push-pull

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2;
    GPIOA->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED2_Pos);   // высокая скорость

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;   // без подтяжек 

    //A3 - RST
    GPIOA->MODER &= ~GPIO_MODER_MODER3;   
    GPIOA->MODER |=  (1U << GPIO_MODER_MODER3_Pos);   // 01 = output 

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;       // push-pull

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED3;
    GPIOA->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED3_Pos);   // высокая скорость

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3;   
    GPIOA->PUPDR |= (1U << GPIO_PUPDR_PUPD3_Pos);   // 01 = pull-up

    //A4 - CS
    GPIOA->MODER &= ~GPIO_MODER_MODER4;   
    GPIOA->MODER |=  (2U << GPIO_MODER_MODER4_Pos);   // 10 = AF

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED4;  
    GPIOA->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED4_Pos);   // высокая скорость

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL4; 
    GPIOA->AFR[0] |= (5U << GPIO_AFRL_AFSEL4_Pos);   // AF5 = SPI1  NSS
}

void display_reset(){
    GPIOA->BSRR = GPIO_BSRR_BR3;  // LOW
    delay_ms(20);

    GPIOA->BSRR = GPIO_BSRR_BS3;  // HIGH
    delay_ms(120);
}

void display_set_DC(bool DC){
    if(DC){
        GPIOA->ODR |= GPIO_ODR_OD2; //data
    }
    else{
        GPIOA->ODR &= ~GPIO_ODR_OD2;//command
    }
}

void display_set_BL(bool BL){
    if(BL){
        GPIOA->ODR |= GPIO_ODR_OD1;
    }
    else{
        GPIOA->ODR &= ~GPIO_ODR_OD1;
    }
}

void lcd_cmd(uint8_t cmd)
{
    DC_CMD();
    CS_LOW();

    SPI1_write(cmd);

    CS_HIGH();
}

void lcd_data(uint8_t data)
{
    DC_DATA();
    CS_LOW();

    SPI1_write(data);

    CS_HIGH();
}

void lcd_set_addr(uint16_t x0, uint16_t y0,
                  uint16_t x1, uint16_t y1)
{
    lcd_cmd(0x2A);  // Column
    lcd_data(x0 >> 8);
    lcd_data(x0);
    lcd_data(x1 >> 8);
    lcd_data(x1);

    lcd_cmd(0x2B);  // Row
    lcd_data(y0 >> 8);
    lcd_data(y0);
    lcd_data(y1 >> 8);
    lcd_data(y1);

    lcd_cmd(0x2C);  // Memory Write
}

void lcd_set_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_set_addr(x, y, x, y);

    DC_DATA();
    CS_LOW();

    SPI1_write(color >> 8);
    SPI1_write(color);

    CS_HIGH();
}

void lcd_fill(uint16_t color)
{
    lcd_set_addr(0, 0, 319, 479);

    DC_DATA();
    CS_LOW();

    for (uint32_t i = 0; i < 320UL * 480; i++)
    {
        SPI1_write(color >> 8);
        SPI1_write(color);
    }

    CS_HIGH();
}

void lcd_test_gradient()
{
    lcd_set_addr(0, 0, 319, 479);

    DC_DATA();
    CS_LOW();

    for (uint16_t y = 0; y < 480; y++)
    {
        for (uint16_t x = 0; x < 320; x++)
        {
            uint16_t r = (x * 31) / 319;
            uint16_t g = (y * 63) / 479;
            uint16_t b = ((x + y) * 31) / 798;

            uint16_t color = (r << 11) | (g << 5) | b;

            SPI1_write(color >> 8);
            SPI1_write(color);
        }
    }

    CS_HIGH();
}

uint16_t lcd_read_pixel(uint16_t x, uint16_t y)
{
    uint16_t color;

    lcd_set_addr(x, y, x, y);

    lcd_cmd(0x2E);   // Memory Read

    DC_DATA();
    CS_LOW();

    SPI1_write(0x00);      // dummy
    SPI1_read();

    uint8_t r = SPI1_read();
    uint8_t g = SPI1_read();
    uint8_t b = SPI1_read();

    CS_HIGH();

    // преобразуем RGB888 → RGB565
    color = ((r & 0xF8) << 8) |
            ((g & 0xFC) << 3) |
            (b >> 3);

    return color;
}

void display_init(){
    BL_ON();

    display_reset();

    lcd_cmd(0x11);        // Sleep Out
    delay_ms(120);

    lcd_cmd(0x36);        // MADCTL
    lcd_data(0x48);       // поворот + RGB

    lcd_cmd(0x3A);        // Pixel format
    lcd_data(0x55);       // 16-bit

    lcd_cmd(0x21);        // Inversion ON (часто нужно)

    lcd_cmd(0x29);        // Display ON
    delay_ms(20);

}

int main(void)
{
    delay_ms(1000);
    RCC_config();
    display_pins_config(); //настраиваем пины для дисплея   
    SPI1_config(); //настраиваем SPI1 для дисплея
    display_init();
    
    /* 1. Включаем тактирование GPIOC */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 

    /* Небольшая пауза после включения тактирования */
    __NOP(); __NOP();

    /* 2. PC13 -> режим OUTPUT */
    GPIOC->MODER &= ~(3U << (13 * 2));   
    GPIOC->MODER |=  (1U << (13 * 2));   // 01 = output
    
    /* 3. Тип выхода push-pull */
    GPIOC->OTYPER &= ~(1U << 13);

    /* 4. Скорость (не критично) */
    GPIOC->OSPEEDR |= (3U << (13 * 2));

    /* 5. Без подтяжек */
    GPIOC->PUPDR &= ~(3U << (13 * 2));
    
    while (1)
    {   
        lcd_fill(0xFFFF); // белый
        delay_ms(1000);

        lcd_fill(0x0000); // чёрный
        delay_ms(1000);

        lcd_fill(0xF800); // красный
        delay_ms(1000);

        lcd_fill(0x07E0); // зелёный
        delay_ms(1000);

        lcd_fill(0x001F); // синий
        delay_ms(1000);

        lcd_test_gradient();
        delay_ms(2000);

        GPIOC->ODR ^= (1U << 13);  // переключить пин
        delay_ms(500);
    }
}



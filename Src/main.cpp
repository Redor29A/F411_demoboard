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


void delay(volatile uint32_t t)
{
    while (t--) {
        __NOP();
    }   
}

void RCC_config(){ //25 MHz all clocks
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    RCC->CFGR &= ~(RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_HPRE | RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while (!(RCC->CFGR & RCC_CFGR_SWS_HSE));
    SystemCoreClockUpdate(); 
}

void display_pins_config(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //тактирование GPIOA
    __NOP(); __NOP();
     
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

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
    GPIOA->OTYPER |= (1U << GPIO_OTYPER_OT3_Pos);         // open-drain 

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

void Display_reset(){
    GPIOA->ODR |= GPIO_ODR_OD3;
    delay(1000);
    GPIOA->ODR &= ~GPIO_ODR_OD3;
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
    SPI1->CR1 |= SPI_CR1_BR_2; //делитель 32
    SPI1->CR2 |= SPI_CR2_SSOE; //автоматическое управление NSS

    SPI1->CR1 |= SPI_CR1_SPE; //включить SPI
}

int main(void)
{
    RCC_config();
    display_pins_config(); //настраиваем пины для дисплея   
    //SPI1_config(); //настраиваем SPI1 для дисплея

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
        GPIOC->ODR ^= (1U << 13);  // переключить пин
        delay(500000);
    }
}



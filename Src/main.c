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

#include "stm32f4xx.h"
#include "main.h"


void delay(volatile uint32_t t)
{
    while (t--) {
        __NOP();
    }
}

int main(void)
{
    /* 1. Включаем тактирование GPIOC */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* Небольшая пауза после включения тактирования */
    __NOP(); __NOP();

    /* 2. PC13 -> режим OUTPUT */
    GPIOC->MODER &= ~(3U << (13 * 2));   // очистить биты
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



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
    lv_tick_inc(1);
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



void display_reset(GPIOx& rst){
    rst.low();
    delay_ms(20);

    rst.high();
    delay_ms(120);
}



/*uint16_t lcd_read_pixel(uint16_t x, uint16_t y)
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
}*/

void fill(uint16_t color){
    st7796.set_memory_area(0, 0, 319, 479);

    st7796.memory_write_start();

    for (uint32_t i = 0; i < 320UL * 480; i++)
    {
        st7796.memory_write(color);
    }

    st7796.memory_write_end();
}

void my_flush_cb(lv_display_t * disp,
                 const lv_area_t * area,
                 uint8_t * px_map)
{
    uint16_t * data = (uint16_t *)px_map;

    st7796.set_memory_area(
        area->x1, area->y1,
        area->x2, area->y2
    );

    st7796.memory_write_start();

    uint32_t size =
        (area->x2 - area->x1 + 1) *
        (area->y2 - area->y1 + 1);

    for (uint32_t i = 0; i < size; i++)
        st7796.memory_write(data[i]);

    st7796.memory_write_end();

    lv_display_flush_ready(disp);
}

static void anim_cb(void * obj, int32_t v)
{
    lv_bar_set_value((lv_obj_t*)obj, v, LV_ANIM_OFF);

    static char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)v);

    lv_obj_t * label =
        lv_obj_get_child(lv_obj_get_parent((lv_obj_t*)obj), 2);

    lv_label_set_text(label, buf);
}

static void keypad_read(lv_indev_t * indev,
                        lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    if (!BTN_UP.read())
        last_key = LV_KEY_UP;

    else if (!BTN_DWN.read())
        last_key = LV_KEY_DOWN;

    else if (!BTN_LFT.read())
        last_key = LV_KEY_LEFT;

    else if (!BTN_RHT.read())
        last_key = LV_KEY_RIGHT;

    else if (!BTN_MID.read())
        last_key = LV_KEY_ENTER;

    else if (!BTN_SET.read())
        last_key = LV_KEY_NEXT;   // удобно для вкладок

    else if (!BTN_RST.read())
        last_key = LV_KEY_ESC;    // выход / назад

    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    data->state = LV_INDEV_STATE_PRESSED;
    data->key = last_key;
}

static void encoder_read(lv_indev_t * indev,
                         lv_indev_data_t * data)
{
      static uint32_t last_time = 0;
    int32_t diff = 0;

    if (lv_tick_elaps(last_time) > 120)   // ← скорость (мс)
    {
        if (!BTN_LFT.read()) diff = -1;
        else if (!BTN_RHT.read()) diff = +1;

        if (diff != 0)
            last_time = lv_tick_get();
    }

    data->enc_diff = diff;

    if (!BTN_MID.read())
        data->state = LV_INDEV_STATE_PRESSED;
    else
        data->state = LV_INDEV_STATE_RELEASED;
}

int main(void)
{
    RCC_config();
    SysTick_Init();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //тактирование GPIOA
    SDA.init();
    SDA_O.init();
    SCL.init();
    RST.init();
    DC.init();
    CS.init();
    BL.init();
    RST.init();

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //тактирование SPI1

    spi.init();

    st7796.init();
    BL.set();
    display_reset(RST);
    st7796.sleep_out();
    delay_ms(120);
    st7796.write_cmd(ST7796_CMD_MADCTL);
    st7796.write_data(0x28); // поворот + RGB
    st7796.write_cmd(ST7796_CMD_COLMOD);
    st7796.write_data(0x55); // Pixel format 16-bit
    st7796.set_inversion_off();
    st7796.display_on();
    delay_ms(20);

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 

    GPIOx LED(GPIOC, 13, GPIOx::ModeOutput, GPIOx::PullNone,
             GPIOx::OTypePushPull, GPIOx::SpeedHigh);
    LED.init();
    fill(0xffff);
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 
    BTN_UP.init();
    BTN_DWN.init();
    BTN_LFT.init();
    BTN_RHT.init();
    BTN_MID.init();
    BTN_SET.init();
    BTN_RST.init();

    lv_init();
    
    static lv_color_t buf[480 * 30];

    lv_display_t* disp = lv_display_create(480, 320);

    lv_display_set_flush_cb(disp, my_flush_cb);

    lv_display_set_buffers(
        disp,
        buf,
        NULL,
        sizeof(buf),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );
    
    lv_indev_t * indev = lv_indev_create();

    lv_indev_set_type(indev, LV_INDEV_TYPE_ENCODER);

    lv_indev_set_read_cb(indev, encoder_read);
        
    lv_group_t * group = lv_group_create();

    lv_group_set_default(group);
    lv_indev_set_group(indev, group);
    
    lv_demo_widgets();
    
    //lv_obj_t * scr = lv_screen_active();

    /* Фон */
    //lv_obj_set_style_bg_color(scr, lv_color_hex(0x101820), 0);

    /* Контейнер */
    //lv_obj_t * panel = lv_obj_create(scr);
    //lv_obj_set_size(panel, 300, 200);
    //lv_obj_center(panel);

    /* Заголовок */
    //lv_obj_t * title = lv_label_create(panel);
    //lv_label_set_text(title, "F411 DEMO");
    //lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Полоса прогресса */
    //lv_obj_t * bar = lv_bar_create(panel);
    //lv_obj_set_size(bar, 260, 20);
    //lv_obj_align(bar, LV_ALIGN_CENTER, 0, 20);
    //lv_bar_set_range(bar, 0, 100);

    /* Число */
    //lv_obj_t * value = lv_label_create(panel);
    //lv_label_set_text(value, "0%");
    //lv_obj_align(value, LV_ALIGN_CENTER, 0, 60);

    //lv_anim_t a;
    //lv_anim_init(&a);

    //lv_anim_set_var(&a, bar);
    //lv_anim_set_exec_cb(&a, anim_cb);
    //lv_anim_set_values(&a, 0, 100);
    //lv_anim_set_time(&a, 2000);
    //lv_anim_set_playback_time(&a, 2000);
    //lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    //lv_anim_start(&a);

    while (1)
    {
        lv_timer_handler();
    }
}



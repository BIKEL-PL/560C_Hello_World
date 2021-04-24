/*******************************************************************************
 * File Name: ugui_bafang_lcd.c (original: ugui_SSD1963.c)
 *
 * Description:
 *  This is a driver for the uGui graphical library developed by
 *  Achim Döbler.
 *  It was for ILI9481 (formerly SSD1963, but changed by kevinh@geeksville.com)
 *  Then adapted by Marek Przybylak (BIKEL.PL) for APT 560C (LG LH219WQ1) display from bafang ebike conversion kits.
 *
 * Note:
 *  For more information about uGui...
 *  Website: http://www.embeddedlightning.com/ugui/
 *  Git: https://github.com/achimdoebler/UGUI
 *  Forum: http://www.embeddedlightning.com/forum/
 *  PDF Reference manual (excellent): http://www.embeddedlightning.com/download/reference-guide/?wpdmdl=205
 *
 *  Thanks to Achim Döbler for such a god job.
 *
 * Log version:
 *  1.0 - June, 2018.       First version.
 *  Sept 2019, changed to use a ILI9481 controller instead
 *  April 2021, changed to work with APT 560C (LG LH219WQ1) display
 *
 ********************************************************************************
 * Copyright (c) 2018 Andres F. Navas
 * This driver follows the same license than the uGui library.
 *******************************************************************************/

#include "ugui.h"
#include "ugui_bafang_lcd.h"
#include "pins.h"

#define HDP (DISPLAY_WIDTH - 1)
#define VDP (DISPLAY_HEIGHT - 1)

UG_GUI gui;
uint8_t write_pulse_duration = 75;

inline void Display_Reset() {
    
}

typedef void (*PushPixelFn)(UG_COLOR);

static void push_pixel_560(UG_COLOR c) {
    if (c == C_TRANSPARENT)
        c = C_BLACK; // FIXME, not quite correct - we really should skip that pixel


    uint32_t color18bit;
    color18bit = convertColorTo18bit(c);

    lcd_write_color_9bits(color18bit >> 9);  //first 9 bits
    lcd_write_color_9bits(color18bit); 		//second 9 bits
    
}

/**
 * A ugui acceleration function.  Given a rectangle, return a callback to set pixels in that rect.
 * The draw order will be by rows, starting from x1,y1 down to x2,y2.
 */
PushPixelFn HW_FillArea(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2) {
    
    /**************************************************/
    // Set XY
    //
    lcd_write_command(0x2a); // write coladdr
    lcd_write_data_8bits(x1 >> 8);   // write data to BUS - start addr
    lcd_write_data_8bits(x1);   // write data to BUS
    lcd_write_data_8bits(x2 >> 8);   // write data to BUS - end addr
    lcd_write_data_8bits(x2);   // write data to BUS
    
    lcd_write_command(0x2b); // write pageaddr
    lcd_write_data_8bits(y1 >> 8);   // write data to BUS
    lcd_write_data_8bits(y1);   // write data to BUS
    lcd_write_data_8bits(y2 >> 8);   // write data to BUS
    lcd_write_data_8bits(y2);   // write data to BUS
    
    lcd_write_command(0x2c); // start writing pixels
    
    return push_pixel_560;
}

void bafang_560C_lcd_init() {
    // next step is needed to have PB3 and PB4 working as GPIO
    /* Disable the Serial Wire Jtag Debug Port SWJ-DP */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LCD_READ__PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_READ__PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_RESET__PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RESET__PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_COMMAND_DATA__PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_COMMAND_DATA__PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_CHIP_SELECT__PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_CHIP_SELECT__PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_WRITE__PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_WRITE__PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = 0x1ff;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_BUS__PORT, &GPIO_InitStructure);


    // default to write mode
    GPIO_SetBits(LCD_READ__PORT, LCD_READ__PIN);

    // keep chip select active
    GPIO_ResetBits(LCD_CHIP_SELECT__PORT, LCD_CHIP_SELECT__PIN);

    // disable reset
    GPIO_SetBits(LCD_RESET__PORT, LCD_RESET__PIN);
    delay_ms(2);
    GPIO_ResetBits(LCD_RESET__PORT, LCD_RESET__PIN);
    delay_ms(2);
    GPIO_SetBits(LCD_RESET__PORT, LCD_RESET__PIN);


        delay_ms(5);


        lcd_write_command(0x35);
        lcd_write_data_8bits(0x00);

        lcd_write_command(0x36);
        lcd_write_data_8bits(0x06);

        lcd_write_command(0x3A);
        lcd_write_data_8bits(0x66);

        lcd_write_command(0x2A);
        lcd_write_data_8bits(0x00);
        lcd_write_data_8bits(0x00);
        lcd_write_data_8bits(0x01);
        lcd_write_data_8bits(0x77);

        lcd_write_command(0x2B);
        lcd_write_data_8bits(0x00);
        lcd_write_data_8bits(0x00);
        lcd_write_data_8bits(0x00);
        lcd_write_data_8bits(0xEF);

        lcd_write_command(0xB1);
        lcd_write_data_8bits(0x11);
        lcd_write_data_8bits(0x1B);

        lcd_write_command(0xB2);
        lcd_write_data_8bits(0x11);
        lcd_write_data_8bits(0x1B);

        lcd_write_command(0xB3);
        lcd_write_data_8bits(0x11);
        lcd_write_data_8bits(0x1B);

        lcd_write_command(0xB4);
        lcd_write_data_8bits(0x02);

        lcd_write_command(0xB6);
        lcd_write_data_8bits(0x01);
        lcd_write_data_8bits(0x02);

        lcd_write_command(0xC0);
        lcd_write_data_8bits(0x24);

        lcd_write_command(0xC1);
        lcd_write_data_8bits(0x02);
        lcd_write_data_8bits(0x00);

        lcd_write_command(0xC2);
        lcd_write_data_8bits(0x05);
        lcd_write_data_8bits(0x01);

        lcd_write_command(0xC3);
        lcd_write_data_8bits(0x02);
        lcd_write_data_8bits(0x05);

        lcd_write_command(0xC4);
        lcd_write_data_8bits(0x02);
        lcd_write_data_8bits(0x04);

        lcd_write_command(0xC5);
        lcd_write_data_8bits(0x14);
        lcd_write_data_8bits(0x2E);

        lcd_write_command(0xEA);
        lcd_write_data_8bits(0x01);

        lcd_write_command(0xE0);
        lcd_write_data_8bits(0x27);
        lcd_write_data_8bits(0x2B);
        lcd_write_data_8bits(0x2E);
        lcd_write_data_8bits(0x06);
        lcd_write_data_8bits(0x0D);
        lcd_write_data_8bits(0x11);
        lcd_write_data_8bits(0x28);
        lcd_write_data_8bits(0x7B);
        lcd_write_data_8bits(0x35);
        lcd_write_data_8bits(0x0C);
        lcd_write_data_8bits(0x20);
        lcd_write_data_8bits(0x26);
        lcd_write_data_8bits(0x25);
        lcd_write_data_8bits(0x28);
        lcd_write_data_8bits(0x3C);

        lcd_write_command(0xE1);
        lcd_write_data_8bits(0x08);
        lcd_write_data_8bits(0x21);
        lcd_write_data_8bits(0x26);
        lcd_write_data_8bits(0x09);
        lcd_write_data_8bits(0x0F);
        lcd_write_data_8bits(0x12);
        lcd_write_data_8bits(0X1F);
        lcd_write_data_8bits(0x38);
        lcd_write_data_8bits(0x31);
        lcd_write_data_8bits(0x0D);
        lcd_write_data_8bits(0x23);
        lcd_write_data_8bits(0x29);
        lcd_write_data_8bits(0x2C);
        lcd_write_data_8bits(0x2F);
        lcd_write_data_8bits(0x33);

        lcd_write_command(0xE2);
        lcd_write_data_8bits(0x27);
        lcd_write_data_8bits(0x2C);
        lcd_write_data_8bits(0x2F);
        lcd_write_data_8bits(0x07);
        lcd_write_data_8bits(0x0E);
        lcd_write_data_8bits(0x11);
        lcd_write_data_8bits(0X29);
        lcd_write_data_8bits(0x7A);
        lcd_write_data_8bits(0x35);
        lcd_write_data_8bits(0x0C);
        lcd_write_data_8bits(0x20);
        lcd_write_data_8bits(0x26);
        lcd_write_data_8bits(0x24);
        lcd_write_data_8bits(0x29);
        lcd_write_data_8bits(0x3C);

        lcd_write_command(0xE3);
        lcd_write_data_8bits(0x08);
        lcd_write_data_8bits(0x20);
        lcd_write_data_8bits(0x26);
        lcd_write_data_8bits(0x09);
        lcd_write_data_8bits(0x0F);
        lcd_write_data_8bits(0x12);
        lcd_write_data_8bits(0X1F);
        lcd_write_data_8bits(0x48);
        lcd_write_data_8bits(0x30);
        lcd_write_data_8bits(0x0D);
        lcd_write_data_8bits(0x22);
        lcd_write_data_8bits(0x28);
        lcd_write_data_8bits(0x2B);
        lcd_write_data_8bits(0x2E);
        lcd_write_data_8bits(0x33);

        lcd_write_command(0xE4);
        lcd_write_data_8bits(0x1F);
        lcd_write_data_8bits(0x24);
        lcd_write_data_8bits(0x27);
        lcd_write_data_8bits(0x08);
        lcd_write_data_8bits(0x0F);
        lcd_write_data_8bits(0x12);
        lcd_write_data_8bits(0X25);
        lcd_write_data_8bits(0x7B);
        lcd_write_data_8bits(0x32);
        lcd_write_data_8bits(0x0C);
        lcd_write_data_8bits(0x20);
        lcd_write_data_8bits(0x26);
        lcd_write_data_8bits(0x20);
        lcd_write_data_8bits(0x25);
        lcd_write_data_8bits(0x3C);

        lcd_write_command(0xE5);
        lcd_write_data_8bits(0x08);
        lcd_write_data_8bits(0x24);
        lcd_write_data_8bits(0x2B);
        lcd_write_data_8bits(0x09);
        lcd_write_data_8bits(0x0F);
        lcd_write_data_8bits(0x12);
        lcd_write_data_8bits(0X22);
        lcd_write_data_8bits(0x38);
        lcd_write_data_8bits(0x35);
        lcd_write_data_8bits(0x0C);
        lcd_write_data_8bits(0x21);
        lcd_write_data_8bits(0x27);
        lcd_write_data_8bits(0x33);
        lcd_write_data_8bits(0x36);
        lcd_write_data_8bits(0x3B);

        lcd_write_command(0xF0);
        lcd_write_data_8bits(0xAA);
        lcd_write_data_8bits(0x55);
        lcd_write_data_8bits(0x25);

        lcd_write_command(0xF6);
        lcd_write_data_8bits(0x7F);
        lcd_write_data_8bits(0x8A);
        lcd_write_data_8bits(0x34);
        lcd_write_data_8bits(0x00);
        lcd_write_data_8bits(0x04);
        lcd_write_data_8bits(0x26);
        lcd_write_data_8bits(0X07);

        lcd_write_command(0x11);
        delay_ms(120);

        lcd_write_command(0x29);


    delay_ms(25);

    write_pulse_duration = 0; // enable fast writes

    // Initialize global structure and set PSET to this.PSET.
    UG_Init(&gui, lcd_pixel_set, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    // Register accelerators.
    UG_DriverRegister(DRIVER_FILL_FRAME, (void*) HW_FillFrame);
    UG_DriverRegister(DRIVER_DRAW_LINE, (void*) HW_DrawLine);
    UG_DriverRegister(DRIVER_FILL_AREA, (void*) HW_FillArea);
}

void lcd_window_set(unsigned int s_x, unsigned int e_x, unsigned int s_y,
                    unsigned int e_y) {
    uint16_t x1, x2, y1, y2;
    
    x1 = s_x;
    x2 = e_x;
    y1 = s_y;
    y2 = e_y;
    
    // set XY
    lcd_write_command(0x2a);
    lcd_write_data_8bits(x1 >> 8);
    lcd_write_data_8bits(x1);
    lcd_write_data_8bits(x2 >> 8);
    lcd_write_data_8bits(x2);
    lcd_write_command(0x2b);
    lcd_write_data_8bits(y1 >> 8);
    lcd_write_data_8bits(y1);
    lcd_write_data_8bits(y2 >> 8);
    lcd_write_data_8bits(y2);
    lcd_write_command(0x2c);
}

void lcd_pixel_set(UG_S16 i16_x, UG_S16 i16_y, UG_COLOR ui32_color) {
    if (ui32_color == C_TRANSPARENT)
        return;

    uint32_t color18bit;
    color18bit = convertColorTo18bit(ui32_color);

    // first 8 bits are the only ones that count for the LCD driver
    uint32_t ui32_x_high = i16_x >> 8;
    uint32_t ui32_x_low = i16_x;
    uint32_t ui32_y_high = i16_y >> 8;
    uint32_t ui32_y_low = i16_y;
    
    /**************************************************/
    // Set XY
    //
    lcd_write_command(0x2a); // write coladdr
    lcd_write_data_8bits(ui32_x_high);   // write data to BUS - start addr
    lcd_write_data_8bits(ui32_x_low);   // write data to BUS
    lcd_write_data_8bits(ui32_x_high);   // write data to BUS - end addr
    lcd_write_data_8bits(ui32_x_low);   // write data to BUS
    
    lcd_write_command(0x2b); // write pageaddr
    lcd_write_data_8bits(ui32_y_high);   // write data to BUS
    lcd_write_data_8bits(ui32_y_low);   // write data to BUS
    lcd_write_data_8bits(ui32_y_high);   // write data to BUS
    lcd_write_data_8bits(ui32_y_low);   // write data to BUS
    
    lcd_write_command(0x2c); // write data to BUS
    
    /**************************************************/
    // Set data on bus
    //

    lcd_write_color_9bits(color18bit >> 9);  //first 9 bits
    lcd_write_color_9bits(color18bit); 		//second 9 bits

}


void wait_pulse() {

    for(volatile int numnops = 0; numnops < write_pulse_duration; numnops++)
        __asm volatile(
                "nop\n\t"
                );
}

void lcd_write_cycle() {
    GPIOC->BRR = LCD_WRITE__PIN;
    wait_pulse();
    GPIOC->BSRR = LCD_WRITE__PIN;
    
    // FIXME, total write cycle min time is 100ns, we are probably fine, but nothing is currently guaranteeing it
}


UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2,
                       UG_COLOR ui32_color) {
    if (ui32_color == C_TRANSPARENT)
        return UG_RESULT_OK;
    
    uint32_t ui32_pixels;
    int32_t i32_dx, i32_dy;
    UG_S16 temp;
    
    // calc total of pixels
    if (x2 >= x1) {
        i32_dx = (uint32_t) (x2 - x1 + 1);
    } else {
        i32_dx = (uint32_t) (x1 - x2 + 1);
        temp = x2;
        x2 = x1;
        x1 = temp;
    }
    
    if (y2 >= y1) {
        i32_dy = (uint32_t) (y2 - y1 + 1);
    } else {
        i32_dy = (uint32_t) (y1 - y2 + 1);
        temp = y2;
        y2 = y1;
        y1 = temp;
    }
    
    ui32_pixels = i32_dx * i32_dy;
    
    /**************************************************/
    // Set XY
    //
    lcd_write_command(0x2a); // write data to BUS - set col addr
    lcd_write_data_8bits(x1 >> 8);   // write data to BUS
    lcd_write_data_8bits(x1);   // write data to BUS
    lcd_write_data_8bits(x2 >> 8);   // write data to BUS
    lcd_write_data_8bits(x2);   // write data to BUS
    
    lcd_write_command(0x2b); // write data to BUS
    lcd_write_data_8bits(y1 >> 8);   // write data to BUS
    lcd_write_data_8bits(y1);   // write data to BUS
    lcd_write_data_8bits(y2 >> 8);   // write data to BUS
    lcd_write_data_8bits(y2);   // write data to BUS
    
    lcd_write_command(0x2c); // write data to BUS
    
    LCD_COMMAND_DATA__PORT->BSRR = LCD_COMMAND_DATA__PIN; // data
    



    uint32_t color18bit;
    color18bit = convertColorTo18bit(ui32_color);

    while (ui32_pixels-- > 0) {
    	lcd_write_color_9bits(color18bit >> 9);  //first 9 bits
    	lcd_write_color_9bits(color18bit); 		//second 9 bits
        }

    
    return UG_RESULT_OK;
}

UG_RESULT HW_DrawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c) {
    if (c == C_TRANSPARENT)
        return UG_RESULT_OK;
    
    if ((x1 < 0) || (x1 >= DISPLAY_WIDTH) || (y1 < 0) || (y1 >= DISPLAY_HEIGHT))
        return UG_RESULT_FAIL;
    if ((x2 < 0) || (x2 >= DISPLAY_WIDTH) || (y2 < 0) || (y2 >= DISPLAY_HEIGHT))
        return UG_RESULT_FAIL;
    
    // If it is a vertical or a horizontal line, draw it.
    // If not, then use original drawline routine.
    if ((x1 == x2) || (y1 == y2)) {
        return HW_FillFrame(x1, y1, x2, y2, c);
    }
    
    return UG_RESULT_FAIL;
}




/**
 * For timing information see 13.2.2 in the datasheet
 */
void lcd_write_command(uint16_t ui32_command) {

    
    // command
    LCD_COMMAND_DATA__PORT->BRR = LCD_COMMAND_DATA__PIN;
    
    // write data to BUS

    LCD_BUS__PORT->ODR = ui32_command << 1;

    
    lcd_write_cycle();
    
    // data
    LCD_COMMAND_DATA__PORT->BSRR = LCD_COMMAND_DATA__PIN;
}

void lcd_write_data_8bits(uint16_t ui32_data) {
    // data
    // LCD_COMMAND_DATA__PORT->BSRR = LCD_COMMAND_DATA__PIN;
    
    // write data to BUS

    LCD_BUS__PORT->ODR = ui32_data << 1;


    // pulse low WR pin
    lcd_write_cycle();
}

void lcd_write_color_9bits(uint16_t ui32_data) {
    // data
    // LCD_COMMAND_DATA__PORT->BSRR = LCD_COMMAND_DATA__PIN;

    // write data to BUS
	LCD_BUS__PORT->ODR = ui32_data;
    
    // pulse low WR pin
    lcd_write_cycle();
}

uint32_t convertColorTo18bit(uint16_t color16bit) {
	return ((color16bit & 0xf800) + color16bit) << 1;
}

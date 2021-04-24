/*
 * Bafang / APT 560C display firmware
 *
 * Based on OpenSourceEBike with Copyrights (C) to Casainho, 2018, 2019, 2020
 * https://github.com/OpenSourceEBike/Color_LCD
 *
 * Copyright (C) BIKEL.PL Marek Przybylak 2021
 *
 * Released under the GPL License, Version 3
 */

#include "ugui_bafang_lcd.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "pins.h"
#include "ugui.h"
#include "img/bikelpl.h"

static volatile uint32_t _ms;
volatile uint32_t time_base_counter_1ms = 0;

void delay_ms (uint32_t ms);


void delay_ms (uint32_t ms)
{
  _ms = 1;
  while (ms >= _ms) ;
}

void SysTick_Handler(void) // runs every 1ms
{
  _ms++; // for delay_ms ()

  time_base_counter_1ms++;
}

void systick_init (void)
{
  /* Setup SysTick Timer for 1 millisecond interrupts, also enables Systick and Systick-Interrupt */
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */
    while (1);
  }
}

int main(void)
{
    #ifdef USE_WITH_BOOTLOADER
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t) 0x4000); // if building for original bootloader (from manufacturer), relocate flash after 16K (0x4000) that is the space that bootloader uses
    #endif

    extern const UG_BMP bikelplBmp;

	systick_init ();
	pins_init();
	delay_ms(500);

	system_power();
	bafang_560C_lcd_init();
	UG_FillScreen(C_BLACK);
    delay_ms(500);

	UG_SetForecolor(C_YELLOW);
	UG_SetBackcolor(C_BLACK);
	UG_FontSelect(&FONT_16X26);
	UG_PutString(20,320,"Hello world!");

	GPIO_SetBits(LCD_BACKLIGHT__PORT, LCD_BACKLIGHT__PIN);

	UG_DrawBMP(70,100,&bikelplBmp);

	while(1)
	{

	}
}

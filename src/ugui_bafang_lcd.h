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

#ifndef UGUI_BAFANG_LCD
#define UGUI_BAFANG_LCD

#include "ugui.h"
    
/* *** Configuration. *** */
    
/* Set here your display resolution. */

#define DISPLAY_WIDTH           240
#define DISPLAY_HEIGHT          376
    
/* *** Global GUI structure for this. *** */
extern UG_GUI ugui_lcd;

/* *** Function prototypes. *** */
void bafang_560C_lcd_init();
void lcd_pixel_set(UG_S16 x, UG_S16 y, UG_COLOR c);
void lcd_window_set(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y);
void lcd_write_command(uint16_t ui32_command);
void lcd_write_data_8bits(uint16_t ui32_data);
void lcd_write_data_9bits(uint16_t ui32_data);
void lcd_write_color_9bits(uint16_t ui32_data);
uint32_t convertColorTo18bit(uint16_t color);
extern void delay_ms (uint32_t ms);
    // Accelerators.
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
UG_RESULT HW_DrawLine(UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint8_t *image, uint16_t pSize);

#endif

/* [] END OF FILE */


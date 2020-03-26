/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: OledApi
 * PortingAuthor: Colton Giesbrecht
 * Comments: This is a ported library from Arduino. Original comment below.
 * All conversion code from Arduino to PIC18 is done by Colton.
 * Revision history: 
 */

/*
 * 16x2 OLED-0010 class driver
 *
 * Derived from LiquidCrystal by David Mellis and should be compatible with the LiquidCrystal library and its examples
 * Init and such derived from datasheet via Adafruit https://cdn-shop.adafruit.com/datasheets/NHD-0216KZW-AB5.pdf
 * 
 * @ogauthor Nick Silvestro <nick@silvervest.net>
 * @license BSD License
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TONEMETER_OLED_API_H
#define	TONEMETER_OLED_API_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>
#include <stdarg.h>

#define SPI_PORT            PORTC
#define SPI_LAT             LATC
#define SPI_TRIS            TRISC
#define SPI_SCK_PIN			0x01
#define SPI_MISO_PIN		0x02
#define SPI_MOSI_PIN		0x04
#define OLED_IS_8BIT
#define CHECK_OLED_PRINT_OVERFLOW(x) if(x == 0) exit(1)

// since this device requires 10-bit SPI writes, we have to do software SPI using these pins
#define OLED_DATA			1
#define OLED_COMMAND		0
#define OLED_READ			1
#define OLED_WRITE			0

// main commands
#define OLED_CLEARDISPLAY   0x01
#define OLED_RETURNHOME     0x02
#define OLED_ENTRYMODESET   0x04
#define OLED_DISPLAYCTRL    0x08
#define OLED_CURSORSHIFT    0x10
#define OLED_FUNCTIONSET    0x28
#define OLED_SETCGRAMADDR   0x40
#define OLED_SETDDRAMADDR   0x80

// to control entry mode, pass OLED_ENTRYMODESET OR'd with following flags
#define OLED_ENTRYLEFT      0x02
#define OLED_ENTRYRIGHT     0x00
#define OLED_ENTRYSHIFTINC  0x01
#define OLED_ENTRYSHIFTDEC  0x00

// to control the display, pass OLED_DISPLAYCTRL OR'd with other flags
#define OLED_DISPLAYON      0x04
#define OLED_DISPLAYOFF     0x00
#define OLED_CURSORON       0x02
#define OLED_CURSOROFF      0x00
#define OLED_BLINKON		0x01
#define OLED_BLINKOFF		0x00

// to control display/cursor shifting, pass OLED_CURSORSHIFT OR'd with following flags
#define OLED_SHIFTCURSOR    0x00
#define OLED_SHIFTDISPLAY   0x08
#define OLED_MOVELEFT		0x00
#define OLED_MOVERIGHT      0x04

// to set function, pass OLED_FUNCTIONSET OR'd with following flags
#define OLED_8BITMODE       0x10 // data is sent in 8-bit lengths in DB7..DB0
#define OLED_4BITMODE       0x00  // data is sent in 4-bit lengths in DB7..DB4, in two consecutive writes to make 8-bit data
#define OLED_LANG_EN        0x00
#define OLED_LANG_JP        0x00
#define OLED_LANG_EU1       0x01
#define OLED_LANG_RU        0x02
#define OLED_LANG_EU2       0x03

	void OledApi_init(unsigned int enable);

	static void begin(uint8_t cols, uint8_t rows);
	void OledApi_clear();
	void OledApi_home();

	void OledApi_noDisplay();
	void OledApi_display();
	void OledApi_noBlink();
	void OledApi_blink();
	void OledApi_noCursor();
	void OledApi_cursor();
	void OledApi_scrollDisplayLeft();
	void OledApi_scrollDisplayRight();
	void OledApi_leftToRight();
	void OledApi_rightToLeft();
	void OledApi_autoscroll();
	void OledApi_noAutoscroll();
    
/////////////////Written By Colton Giesbrecht - Not part of ported library!-----
    void OledApi_printStr(const char*);
    void OledApi_printNum(float, int8_t);
    void OledApi_printf(char*, uint8_t, ...);
    void OledApi_printSpec(uint8_t);
////////////////////////////////////////////////////////////////////////////////

	//void setRowOffsets(int row1, int row2, int row3, int row4);
	void OledApi_createChar(uint8_t, uint8_t[]);
	void OledApi_setCursor(uint8_t, uint8_t);
	static void command(uint8_t);
	
    static void write(uint8_t);
    static void writeStr(const uint8_t*, uint8_t);

	static void send(uint8_t, void *, size_t);
	static void sendBit(uint8_t);
	
	static uint8_t _enable_pin;

	static uint8_t _displayfunction;
	static uint8_t _displaycontrol;
	static uint8_t _displaymode;

#endif


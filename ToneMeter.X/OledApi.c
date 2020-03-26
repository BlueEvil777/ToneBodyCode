/* 
 * File: OledApi
 * PortingAuthor: Colton Giesbrecht
 * Comments: This is a ported library from Arduino. Original comment below.
 * All conversion code from Arduino to PIC18 is done by Colton.
 * Revision history: 
 */


#include "OledApi.h"
#include "main.h"


void OledApi_init(unsigned int enable)
{
	_enable_pin = enable;
	
#ifdef OLED_IS_8BIT
		_displayfunction = OLED_4BITMODE;
#else
		_displayfunction = OLED_8BITMODE;
#endif

	// init SPI pins
    SPI_TRIS &= ~SPI_SCK_PIN;   //output
    SPI_TRIS |= SPI_MISO_PIN;   //input
    SPI_TRIS &= ~SPI_MOSI_PIN;  //output
    SPI_TRIS &= ~enable;        //output
	SPI_LAT |= enable;          //disable screen
    
    begin(16,2);
}

// rows and cols are ignore here for now, only supporting 16x2 displays
// but kept for simpler compatibility with LiquidCrystal library
static void begin(uint8_t cols, uint8_t rows)
{
	// init sequence in 8-bit mode
	// 1. wait a bit, >=1ms on the datasheet
    __delaywdt_ms(1);
	// 2. function set
	// we're setting 8-bit mode, english here
	command(OLED_FUNCTIONSET | _displayfunction | OLED_LANG_EN);
	
	// 3. display off
	command(OLED_DISPLAYCTRL | OLED_DISPLAYOFF);
	
	// 4. display clear
	OledApi_clear();
	
	// 5. entry mode set
	_displaymode = OLED_ENTRYLEFT | OLED_ENTRYSHIFTDEC;
	command(OLED_ENTRYMODESET | _displaymode);
	
	// 6. go home
	OledApi_home();
	
	// 7. display on
	_displaycontrol = OLED_DISPLAYON | OLED_CURSOROFF;
	command(OLED_DISPLAYCTRL | _displaycontrol);

	// ready!
    //__delay_us(100);
}

/********** high level commands, for the user! */
void OledApi_clear()
{
	command(OLED_CLEARDISPLAY);
	__delaywdt_ms(1);
	OledApi_home();
}

void OledApi_home()
{
	command(OLED_RETURNHOME);
}

void OledApi_setCursor(uint8_t col, uint8_t row)
{
	// DDRAM address is 7 bits, first row offset by 0x00 and second offset by 0x40
	// so add the row offset then AND mask the column against 0b01111111 to get our position
	command(OLED_SETDDRAMADDR | (row ? 0x40 : 0x00) | (col & 0x3F));
}

// Turn the display on/off (quickly)
void OledApi_noDisplay()
{
	_displaycontrol &= ~OLED_DISPLAYON;
	command(OLED_DISPLAYCTRL | _displaycontrol);
}
void OledApi_display()
{
	_displaycontrol |= OLED_DISPLAYON;
	command(OLED_DISPLAYCTRL | _displaycontrol);
}

// Turns the underline cursor on/off
void OledApi_noCursor()
{
	_displaycontrol &= ~OLED_CURSORON;
	command(OLED_DISPLAYCTRL | _displaycontrol);
}
void OledApi_cursor()
{
	_displaycontrol |= OLED_CURSORON;
	command(OLED_DISPLAYCTRL | _displaycontrol);
}

// Turn on and off the blinking cursor
void OledApi_noBlink()
{
	_displaycontrol &= ~OLED_BLINKON;
	command(OLED_DISPLAYCTRL | _displaycontrol);
}
void OledApi_blink()
{
	_displaycontrol |= OLED_BLINKON;
	command(OLED_DISPLAYCTRL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void OledApi_scrollDisplayLeft(void)
{
	command(OLED_CURSORSHIFT | OLED_SHIFTDISPLAY | OLED_MOVELEFT);
}
void OledApi_scrollDisplayRight(void)
{
	command(OLED_CURSORSHIFT | OLED_SHIFTDISPLAY | OLED_MOVERIGHT);
}

// This is for text that flows Left to Right
void OledApi_leftToRight(void)
{
	_displaymode |= OLED_ENTRYLEFT;
	command(OLED_ENTRYMODESET | _displaymode);
}
// This is for text that flows Right to Left
void OledApi_rightToLeft(void)
{
	_displaymode &= ~OLED_ENTRYLEFT;
	command(OLED_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void OledApi_autoscroll(void)
{
	_displaymode |= OLED_ENTRYSHIFTINC;
	command(OLED_ENTRYMODESET | _displaymode);
}
// This will 'left justify' text from the cursor
void OledApi_noAutoscroll(void)
{
	_displaymode &= ~OLED_ENTRYSHIFTINC;
	command(OLED_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void OledApi_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(OLED_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
}

void OledApi_printSpec(uint8_t schar)
{
    if(schar > 7){
        exit(1);
    }
    write(schar);
}

void OledApi_printf(char* s, uint8_t args, ...)
{
    uint8_t count = 0;
    va_list list;
    va_start(list, args);
    
    while(*s)
    {
        if(*s == '%')
        {
            //too many args if true
            if(count == args)
            {
                exit(1);
            }
            uint8_t dec = 0, findingSpec;
            do
            {
                findingSpec = 0;
                switch(*(++s))
                {
                    case 'd':
                        OledApi_printNum(va_arg(list, int), dec);
                        count++;
                        break;
                    case 'f':
                        OledApi_printNum(va_arg(list, double), dec);
                        count++;
                        break;
                    case 'n':
                        OledApi_setCursor(0,1);
                        break;
                    case 's':
                        OledApi_printStr(va_arg(list, uint8_t*));
                        count++;
                        break;
                    case 'c':
                        OledApi_printSpec(va_arg(list, int));
                        count++;
                        break;
                    default:
                        if(*s >= '0' && *s <= '9')
                        {
                            findingSpec = 1;
                            dec = dec*10 + (*s-'0');
                        }
                }
            }while(findingSpec);
        }
        else
        {
            write(*s);
        }
        s++;
    }
    
    va_end(list);
}

void OledApi_printNum(float num, int8_t dec)
{
    int32_t whole, fract;
    uint8_t s[17] = {};
    int c = 16;
    
    whole = (int32_t) num;
    num -= whole;
    
    for(int i = 0; i < dec; i++)
    {
        num *= 10;
    }
    fract = (int32_t) num;
    //round
    if(dec == 0)
    {
        if(num >= 0.5)
        {
            whole++;
        }
        dec = -1;
    }
    else if(num - fract >= 0.5)
    {
        fract++;
    }
    
    while(dec-- > 0)
    {
        CHECK_OLED_PRINT_OVERFLOW(c);
        s[--c] = '0' + (fract % 10);
        fract /= 10;
    }
    if(dec == -1)
    {
        CHECK_OLED_PRINT_OVERFLOW(c);
        s[--c] = '.';
    }
    
    do
    {
        CHECK_OLED_PRINT_OVERFLOW(c);
        s[--c] = '0' + (whole % 10);
        whole /= 10;
    }while(whole > 0);
    
    if(num < 0)
    {
        CHECK_OLED_PRINT_OVERFLOW(c);
        s[--c] = '-';
    }
    //print to screen
    writeStr(s+c, 16-c);
}

void OledApi_printStr(const char* s)
{
    uint8_t count = 0;
    while(s[count])
    {
        count++;
    }
    if(count > 16)
    {
        writeStr(s, 16);
        OledApi_setCursor(0,1);
        writeStr(s+16, count - 16);
    }
    else
    {
        writeStr(s, count);
    }
}

/*********** mid level commands, for sending data/cmds */
// send a command
static void command(uint8_t value) {
	send(OLED_COMMAND, &value, 1);
}

// write a single character or value
static void write(uint8_t value)
{
	send(OLED_DATA, &value, 1);
}

//write a whole string
static void writeStr(const uint8_t *value, uint8_t len)
{
	send(OLED_DATA, value, len);
}

/************ low level data pushing commands **********/
// write either a command or data
// this bitbangs the SPI pins to send 10-bits of data, as that's pretty impossible using hardware SPI
// this oled driver needs 10 bits for transmissions: 2-bit header, 8-bit data
// 1st header bit is command or data
// 2nd header bit is read or write
// if we're sending a command, we have to send the header before every data transmission
// if we're sending data, we only have to send it at the beginning of the transmission
static void send(uint8_t mode, void *buf, size_t count) {
	if (count == 0)
		return;
	
	uint8_t head_sent = 0;

	// select the device to start
	//digitalWrite(_enable_pin, LOW);
    SPI_LAT &= ~_enable_pin;

	// grab a pointer to the first byte off the buffer
	uint8_t *p = (uint8_t *)buf;
	
	do {
		// manually send the first two bits
		// command = LOW, data = HIGH
		if (head_sent == 0) {
            //__delaywdt_us(1);
			if (mode == OLED_DATA) {
				sendBit(HIGH);
				head_sent = 1; // only send this header once for data transmissions
			} else
				sendBit(LOW);
				
			// read = HIGH, write = LOW
			// we're always writing in this method, so always send write = LOW
			sendBit(0);
		}
	
		// send the remaining 8 bits one at a time
		for (uint8_t mask = 0x80; mask; mask >>= 1) {
			sendBit(mask & *p);
		}
		
		// increment the buffer pointer
		p++;
	} while (--count > 0);
	
	// de-select device
	//digitalWrite(_enable_pin, HIGH);
    SPI_LAT |= _enable_pin;
}

// clock in a single bit
// according to the data sheet, data is read on the rising edge
static void sendBit(uint8_t b)
{
    //__delaywdt_us(1);
	//digitalWrite(SPI_SCK_PIN, LOW);
    SPI_LAT &= ~SPI_SCK_PIN;       //clk low
	//digitalWrite(SPI_MOSI_PIN, bit);
    //__delaywdt_us(1);
    if(b != LOW)
    {
        SPI_LAT |= SPI_MOSI_PIN;   //mosi high
    }
    else
    {
        SPI_LAT &= ~SPI_MOSI_PIN;   //mosi low
    }
    //__delaywdt_us(1);
	//digitalWrite(SPI_SCK_PIN, HIGH);
    SPI_LAT |= SPI_SCK_PIN;         //clk high
}
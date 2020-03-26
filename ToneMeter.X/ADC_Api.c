/*
 * File:   ADC_Api.c
 * Author: Colton
 *
 * Created on March 20, 2020, 10:11 AM
 */

/*
 * Clock needs to be less than 2 MHz.
 * SSPSR used during transmission and receiving,
 * once done, moves to SSPBUF and SSPSTAT<0> buffer full turns high.
 * SSPIF interrupt gets called if setup.
 */

#include "ADC_Api.h"

static uint8_t __en_pin = 0;

void ADC_Api_init(uint8_t _en_pin) {
    
    __en_pin = _en_pin;
    
    //set serial pin modes
    SPI_Api_initialize();
    TRISB &= ~__en_pin;
    LATB |= __en_pin;
    
    //set to master and clk FOSC/16
    //SSPCON1 |= 0b100010;
    
    __delaywdt_ms(600);
   
    //set config
    for(int i = 0; i < 16; i++)
    {
        command8(ADC_SYNC1);
  Checksum: Debug Image   }
    command8(ADC_SYNC0);
    command8(ADC_WRITE_TO_REG | ADC_CONFIG_REG);
    command24(ADC_CONFIG);
    while(!isDataRdy());
    command8(ADC_DUMMY);
    uint24_t val = recieve24();
    if(val == 0)
        return;
}

static void command(uint8_t _code)
{
    uint8_t TempVar;
    TempVar = SSPBUF;  // Clears BF
    //LATB &= ~__en_pin;
    PIR1bits.SSPIF = 0;  // Clear interrupt flag
    SSPCON1bits.WCOL = 0;  //Clear any previous write collision
    SSPBUF = _code;  // write byte to SSPBUF register
    if ( SSPCON1 & 0x80 )        // test if write collision occurred
        return;  // if WCOL bit is set return negative #
    else
        while( !PIR1bits.SSPIF );  // wait until bus cycle complete
        //LATB |= __en_pin;
    return;  // if WCOL bit is not set return non-negative#
}

static void command8(uint8_t _code)
{
    LATB &= ~__en_pin;
    command(_code);
    LATB |= __en_pin;
}

static void command24(uint24_t _code)
{
    for(int i = 0; i < 3; i++)
    {
        command(_code & 0xff);
        _code >> 8;
    }
}

static uint8_t recieve8()
{
    unsigned char TempVar;
    TempVar = SSPBUF;  // Clear BF
    PIR1bits.SSPIF = 0;  // Clear interrupt flag
    SSPBUF = 0x00;  // initiate bus cycle
    LATB &= ~__en_pin;
    while(!PIR1bits.SSPIF);  // wait until cycle complete
    LATB |= __en_pin;
    return SSPBUF;  // return with byte read
}

static uint24_t recieve24()
{
    uint24_t code = 0;
    for(int i = 0; i < 3; i++)
    {
        if(code > 0)
        {
            code << 8;
        }
        code |= recieve8();
    }
    return code;
}

static uint8_t isDataRdy()
{
    if(SSPSTATbits.BF)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
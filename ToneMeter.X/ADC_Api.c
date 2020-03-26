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

static SPI_Api_pConfig __config8, __config24;

void ADC_Api_init(uint8_t _en_pin) {
    
    __config8.u8EnablePin = __config24.u8EnablePin = _en_pin;
    __config8.u8SPIBits = 8;
    __config24.u8SPIBits = 24;
    
    //set serial pin modes
    SPI_Api_initialize();
    TRISB &= ~_en_pin;
    LATB |= _en_pin;
    
    //set to master and clk FOSC/16
    //SSPCON1 |= 0b100010;
    
    __delaywdt_ms(600);
}

void ADC_Api_config()
{
    //set config
    SPI_Api_setSpiDevice(__config8);
    SPI_Api_sendWord(ADC_WRITE_TO_REG | ADC_CONFIG_REG);
    SPI_Api_setSpiDevice(__config24);
    SPI_Api_sendWord(ADC_CONFIG);
}

//static void command(uint8_t _code)
//{
//    uint8_t TempVar;
//    TempVar = SSPBUF;  // Clears BF
//    //LATB &= ~__en_pin;
//    PIR1bits.SSPIF = 0;  // Clear interrupt flag
//    SSPCON1bits.WCOL = 0;  //Clear any previous write collision
//    SSPBUF = _code;  // write byte to SSPBUF register
//    if ( SSPCON1 & 0x80 )        // test if write collision occurred
//        return;  // if WCOL bit is set return negative #
//    else
//        while( !PIR1bits.SSPIF );  // wait until bus cycle complete
//        //LATB |= __en_pin;
//    return;  // if WCOL bit is not set return non-negative#
//}
//
//static void command8(uint8_t _code)
//{
//    LATB &= ~__en_pin;
//    command(_code);
//    LATB |= __en_pin;
//}
//
//static void command24(uint24_t _code)
//{
//    for(int i = 0; i < 3; i++)
//    {
//        command(_code & 0xff);
//        _code >> 8;
//    }
//}
//
//static uint8_t recieve8()
//{
//    unsigned char TempVar;
//    TempVar = SSPBUF;  // Clear BF
//    PIR1bits.SSPIF = 0;  // Clear interrupt flag
//    SSPBUF = 0x00;  // initiate bus cycle
//    LATB &= ~__en_pin;
//    while(!PIR1bits.SSPIF);  // wait until cycle complete
//    LATB |= __en_pin;
//    return SSPBUF;  // return with byte read
//}
//
//static uint24_t recieve24()
//{
//    uint24_t code = 0;
//    for(int i = 0; i < 3; i++)
//    {
//        if(code > 0)
//        {
//            code << 8;
//        }
//        code |= recieve8();
//    }
//    return code;
//}
//
//static uint8_t isDataRdy()
//{
//    if(SSPSTATbits.BF)
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }
//}
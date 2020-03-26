/*
 * File:   SPI_Api.c
 * Author: Colton
 *
 * Created on March 25, 2020, 5:28 PM
 */


#include <xc.h>
#include "SPI_Api.h"
#include "main.h"

static SPI_Api_pConfig __config = {};

void SPI_Api_initialize(void)
{
    //if already initialized, return
    if(!FLAG_READ(G_SPI_Api_u8Flags, SPI_NOT_INIT_FLAG))
    {
        return;
    }
    
    //setup serial pins
    SPI_Api_TRIS &= ~SPI_Api_SCK_PIN;   //output
    SPI_Api_TRIS |= SPI_Api_MISO_PIN;   //input
    SPI_Api_TRIS &= ~SPI_Api_MOSI_PIN;  //output
    
    //reset initialization flag;
    FLAG_RESET(G_SPI_Api_u8Flags, SPI_NOT_INIT_FLAG);
}

bool SPI_Api_setSpiDevice(SPI_Api_pConfig _config)
{
    uint8_t mask;
    for(mask = 1; mask != _config.u8EnablePin; mask <<= 1)
    {
        //Not a valid pin
        if(mask == 0x80)
        {
            FLAG_SET(G_SPI_Api_u8Flags, SPI_EN_PIN_UNSET_FLAG);
            return false;
        }
    }
    if(!FLAG_READ(G_SPI_Api_u8Flags, SPI_EN_HIGH_FLAG))
    {
        FLAG_SET(G_SPI_Api_u8Flags, SPI_EN_PIN_UNSET_FLAG);
        FLAG_SET(G_SPI_Api_u8Flags, SPI_INTERFERENCE_FLAG);
        return false;
    }
    
    __config = _config;
    DIGITAL_WRITE(&SPI_Api_LAT, __config.u8EnablePin, HIGH);
    FLAG_RESET(G_SPI_Api_u8Flags, SPI_EN_PIN_UNSET_FLAG);
    return true;
}

bool SPI_Api_begin()
{
//    if(SPI_Api_bIsSetup)
//    {
//        return false;
//    }
    
    DIGITAL_WRITE(&SPI_Api_LAT, __config.u8EnablePin, LOW);
    FLAG_RESET(G_SPI_Api_u8Flags, SPI_EN_HIGH_FLAG);
    
    return true;
}

bool SPI_Api_end()
{
//    if(SPI_Api_bIsSetup)
//    {
//        return false;
//    }
    
    DIGITAL_WRITE(&SPI_Api_LAT, __config.u8EnablePin, HIGH);
    FLAG_SET(G_SPI_Api_u8Flags, SPI_EN_HIGH_FLAG);
    
    return true;
}

bool SPI_Api_sendWord(uint32_t word)
{
    //if larger than SPIbit length, abort
    if((word >> (__config.u8SPIBits - 1)) > 0)
    {
        return false;
    }
    uint32_t mask = 1 << (__config.u8SPIBits - 1);
    while(mask > 0)
    {
        if((word & mask) > 0)
        {
            SPI_Api_sendBit(HIGH);
        }
        else
        {
            SPI_Api_sendBit(LOW);
        }
        mask >>= 1;
    }
    return true;
}

uint32_t SPI_Api_receiveWord()
{
    uint32_t word = 0;
    uint8_t count = 0;
    while(count++ < __config.u8SPIBits)
    {
        if(count != 1)
        {
            word <<= 1;
        }
        word |= SPI_Api_receiveBit();
    }
    return word;
}

bool SPI_Api_sendBit(uint8_t val)
{
    //if errorflags set, abort
//    if(SPI_Api_bIsOk)
//    {
//        return false;
//    }
    
    DIGITAL_WRITE(&SPI_Api_LAT, SPI_Api_SCK_PIN, LOW);
    DIGITAL_WRITE(&SPI_Api_LAT, SPI_Api_MOSI_PIN, val);
    DIGITAL_WRITE(&SPI_Api_LAT, SPI_Api_SCK_PIN, HIGH);
    //__delaywdt_us(__config.u8ClkUSDelay);
    
    return true;
}

uint8_t SPI_Api_receiveBit()
{
    //if errorflags set, abort
//    if(SPI_Api_bIsOk)
//    {
//        return 2;
//    }
    
    DIGITAL_WRITE(&SPI_Api_LAT, SPI_Api_SCK_PIN, LOW);
    uint8_t val = DIGITAL_READ(SPI_Api_PORT, SPI_Api_MISO_PIN);
    DIGITAL_WRITE(&SPI_Api_LAT, SPI_Api_SCK_PIN, HIGH);
    
    return val;
}
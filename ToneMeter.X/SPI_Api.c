/*
 * File:   SPI_Api.c
 * Author: Colton
 *
 * Created on March 25, 2020, 5:28 PM
 */


#include <xc.h>
#include "SPI_Api.h"
#include "main.h"

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
    for(mask = 1; mask != _config.u8EnablePin ; mask =<< 1);
    //Not a valid pin
    if(mask > 0xFF)
    {
        FLAG_SET(G_SPI_Api_u8Flags, SPI_EN_PIN_UNSET_FLAG);
        return false;
    }
    if(!FLAG_READ(G_SPI_Api_u8Flags, SPI_EN_HIGH_FLAG))
    {
        FLAG_SET(G_SPI_Api_u8Flags, SPI_EN_PIN_UNSET_FLAG);
        FLAG_SET(G_SPI_Api_u8Flags, SPI_INTERFERENCE_FLAG);
        return false;
    }
    
    __config = _config;
    DIGITAL_WRITE(SPI_Api_LAT, __config.u8EnablePin, HIGH);
    FLAG_RESET(G_SPI_Api_u8Flags, SPI_EN_PIN_UNSET_FLAG);
    return true;
}

bool SPI_Api_begin()
{
    if(SPI_Api_bIsSetup)
    {
        return false;
    }
    
    DIGITAL_WRITE(SPI_Api_LAT, __config.u8EnablePin, LOW);
    FLAG_RESET(G_SPI_Api_u8Flags, SPI_EN_HIGH_FLAG);
    
    return true;
}

bool SPI_Api_end()
{
    if(SPI_Api_bIsSetup)
    {
        return false;
    }
    
    DIGITAL_WRITE(SPI_Api_LAT, __config.u8EnablePin, HIGH);
    FLAG_SET(G_SPI_Api_u8Flags, SPI_EN_HIGH_FLAG);
    
    return true;
}

bool SPI_Api_sendBit(uint8_t val)
{
    //if errorflags set, abort
    if(SPI_Api_bIsOk)
    {
        return false;
    }
    
    DIGITAL_WRITE(SPI_Api_LAT, SPI_Api_SCK_PIN, LOW);
    DIGITAL_WRITE(SPI_Api_LAT, SPI_Api_MOSI_PIN, val);
    DIGITAL_WRITE(SPI_Api_LAT, SPI_Api_SCK_PIN, HIGH);
    __delaywdt_us(__config.u8ClkUSDelay);
    
    return true;
}
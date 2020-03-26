/* 
 * File:   SPI_Api.h
 * Author: Colton
 *
 * Created on March 25, 2020, 5:27 PM
 */

#ifndef SPI_API_H
#define	SPI_API_H

#include <stdint.h>
#include <stdbool.h>

#define SPI_Api_PORT            PORTC
#define SPI_Api_LAT             LATC
#define SPI_Api_TRIS            TRISC
#define SPI_Api_SCK_PIN			0x01
#define SPI_Api_MISO_PIN		0x02
#define SPI_Api_MOSI_PIN		0x04

/////////////////////////   FLAGS   /////////////////////////////////////////
#define SPI_NOT_INIT_FLAG 0x01
#define SPI_EN_PIN_UNSET_FLAG 0x02
#define SPI_EN_HIGH_FLAG 0x04
#define SPI_INTERFERENCE_FLAG 0x08

#define SPI_Api_bIsOk (G_SPI_Api_u8Flags & (SPI_NOT_INIT_FLAG | SPI_NOT_INIT_FLAG | SPI_EN_HIGH_FLAG | SPI_INTERFERENCE_FLAG)) == 0
#define SPI_Api_bIsSetup (G_SPI_Api_u8Flags & (SPI_NOT_INIT_FLAG | SPI_NOT_INIT_FLAG | SPI_INTERFERENCE_FLAG)) == 0
/////////////////////////////////////////////////////////////////////////////

typedef struct 
{
    uint8_t u8EnablePin;
    uint8_t u8ClkUSDelay;
    uint8_t u8SPIBits;
} SPI_Api_pConfig;

/**
 * Public Global SPI_Api Flags
 */
uint8_t G_SPI_Api_u8Flags = SPI_NOT_INIT_FLAG | SPI_EN_PIN_UNSET_FLAG | SPI_EN_HIGH_FLAG;

/**
 * @brief Initializes SPI pins. Note that API will not be ready
 * until enable pin is set!
*/
void SPI_Api_initialize(void);

/**
 * @brief sets the active SPI device
 * 
 * @param _config - new active SPI device
 * @return bool is new device selected
 */
bool SPI_Api_setSpiDevice(SPI_Api_pConfig _config);

/**
 * @brief Sends a whole word based on config.
 * Requires begin
 * 
 * @param word
 * @return bool isSuccessful
 */
bool SPI_Api_sendWord(uint32_t word);

/**
 * @brief Receives a whole word based on config.
 * Requires begin

 * @return uint32_t cast word based on config size. 
 */
uint32_t SPI_Api_receiveWord();

/**
 * @brief Begin SPI communication.
 * 
 * @return isSuccessful
 */
bool SPI_Api_begin();

/**
 * @brief End SPI communication.
 * 
 * @return isSuccessful
 */
bool SPI_Api_end();

/**
 * @brief sends a bit through SPI
 * Requires begin
 * 
 * @param val: HIGH or LOW
 * @return bool if send bit was successful without errors
 */
bool SPI_Api_sendBit(uint8_t val);

/**
 * @brief receive a bit through SPI
 * Requires begin
 * 
 * @return uint8_t read bit or greater if failed
 */
uint8_t SPI_Api_receiveBit();

#endif	/* SPI_API_H */


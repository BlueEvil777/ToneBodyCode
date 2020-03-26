/* 
 * File:   ADC_Api.h
 * Author: Colton
 *
 * Created on March 20, 2020, 10:12 AM
 */

#ifndef ADC_API_H
#define	ADC_API_H

#define ADC_SYNC1 0xff
#define ADC_SYNC0 0xfe
#define ADC_DUMMY 0x00

#define ADC_CONFIG ADC_INDICATOR_ON | ADC_FASTWR | ADC_PORT_FLAG_EN

#define ADC_INDICATOR_ON 0x00040000
#define ADC_FASTWR 0x00009000
#define ADC_MIDWR 0x00005000
#define ADC_UNI_MSR 0x00001000
#define ADC_RST 0x00000080
#define ADC_RST_VALID 0x00000040
#define ADC_PORT_FLAG_EN 0x00000020
#define ADC_PWR_SV_SLEEP 0x00000010
#define ADC_DONE_FLAG 0x00000008

#define ADC_WRITE_TO_REG 0x80
#define ADC_READ_FROM_REG 0x90
#define ADC_OFFSET_REG 0x0
#define ADC_GAIN_REG 0x02
#define ADC_CONFIG_REG 0x4
#define ADC_CONV_REG 0x06
#define ADC_SETUP_REGS 0x08

#define ADC_CONT_CONV 0ba0
#define ADC_CONV 0xc0

#include <xc.h>
#include <stdint.h>
#include "main.h"
#include "SPI_Api.h"

/////////////////////// PUBLIC /////////////////////////////////////////

void ADC_Api_init(uint8_t);
void ADC_Api_config();

/////////////////////// PRIVATE ////////////////////////////////////////

static void command(uint8_t _code);
static void command8(uint8_t);
static void command24(uint24_t);

static uint8_t recieve8();
static uint24_t recieve24();
static uint8_t isDataRdy();

#endif	/* ADC_API_H */


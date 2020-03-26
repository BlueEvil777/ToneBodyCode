/*
 * File:   main.c
 * Author: Colton
 *
 * Created on February 28, 2020, 10:26 PM
 */

#include "main.h"
#include "OledApi.h"
#include "ADC_Api.h"

static uint8_t dc[8] = {
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b11011,
    0b00000,
    0b00000
};
static uint8_t ac[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b01010,
    0b10101,
    0b00000,
    0b00000,
    0b00000
};

void setup(void) {
    
    OSCCONbits.IRCF = 111;
    //setup b0-2 as pull up D0 for led colours
    ADCON1bits.PCFG = 0b1111;   //Set all pins to digital.
    SSPCON1bits.SSPEN = 1;
    SSPCON1bits.SSPM = 0b0010;
    OledApi_init(OLED_CS_PIN);
    ADC_Api_init(0b100);
    OledApi_printf("Hi! Int:%n%2f %s %c", 3, 12.6, "Pew!", 0);
    __delaywdt_ms(2000);
    OledApi_createChar(0, dc);
    OledApi_createChar(1, ac);
}

void main(void) {
    
    setup();
    static unsigned char count = 0;
    
    while(1){
        OledApi_printf("Seconds: %d", 1, (int)count++);
        __delay_ms(1000);
        OledApi_clear();
    }
}

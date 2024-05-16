#include <stdlib.h>
#include <stdio.h>
//#include "pottiPong.h"
#include <stdbool.h>
#include "bcm2835.h"
#include "max7219.h"

uint8_t start = 0x01;
uint8_t end = 0x00;
uint8_t chan = 0x00;

int readADC(uint8_t chan);
float volts_adc(int adc);


int main(int argc, char const *argv[])
{
    char input; 
    bool run = 1;

    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }

    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      return 1;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    
    
    while(run){
        input = getc(stdin);
        switch(input){
            case 'e':
                run = false;
                break;
            case 'a':
                printf("here we go:\n");
                for(uint8_t i=0; i<10; i++) {
                    int adc = readADC(chan);
                    printf("ADC level on channel %02x is: %04x (%0.2f v)\n",chan,adc,volts_adc(adc));
                }

                break;
            case 'f':

                break;
        }
    }
    return 0;
}

int readADC(uint8_t chan){
  char buf[] = {start, (0x08|chan)<<4,end};
  char readBuf[3];
  bcm2835_spi_transfernb(buf,readBuf,3);
  return ((int)readBuf[1] & 0x03) << 8 | (int) readBuf[2];
}

float volts_adc(int adc) {
  return (float)adc*3.3f/1023.0f;
}


/*
für ADC:

typedef enum
{
    BCM2835_SPI_CS0 = 0,     /*!< Chip Select 0 
    BCM2835_SPI_CS1 = 1,     /*!< Chip Select 1 
    BCM2835_SPI_CS2 = 2,     /*!< Chip Select 2 (ie pins CS1 and CS2 are asserted) 
    BCM2835_SPI_CS_NONE = 3  /*!< No CS, control it yourself 
} bcm2835SPIChipSelect;

für Leds:
*/

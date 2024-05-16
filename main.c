#include <stdlib.h>
#include <stdio.h>
#include "pottiPong.h"

uint8_t start = 0x01;
uint8_t end = 0x00;
uint8_t chan = 0x00;

int readADC(uint8_t chan){
    char buf[] = {start, (0x08|chan)<<4,end};
    char readBuf[3];
    printf("transfering now!\n");
    bcm2835_spi_transfernb(buf,readBuf,3);
    printf("transfer finished!\n");
    return ((int)readBuf[1] & 0x03) << 8 | (int) readBuf[2];
}

float volts_adc(int adc) {
  return (float)adc*3.3f/1023.0f;
}

int main(int argc, char const *argv[])
{
    char input; 
    bool run = 1;

    Game* game;
    int ball_xy[2] = {4, 16}; //middle
    int player1_xyw[3] = {2, 4, 4};
    int player2_xyw[3] = {2, 28, 4};
    int dimensions_game[2] = {8, 32};

    game_init(&game, ball_xy, player1_xyw, player2_xyw, RIGHT, DOWN, 3, dimensions_game);
    if (!bcm2835_init()){
        printf("bcm2835_init failed. Are you running as root??\n");
        return 1;
    }

    if (!bcm2835_spi_begin()){
        printf("bcm2835_spi_begin failed. Are you running as root??\n");
        return 1;
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128); // The default
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
                printf("volts[0]: %f\n", volts_adc(readADC(0)));
                printf("volts[1]: %f\n\n", volts_adc(readADC(0)));
                
                

                break;
            case 'f':
                game_exit(&game);
                break;
        }
    }
    return 0;
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

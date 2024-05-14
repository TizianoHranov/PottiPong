#include <stdlib.h>
#include <stdio.h>
#include "pottiPong.h"

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

    while(run){
        input = getc(stdin);
        switch(input){
            case 'e':
                run = false;
                break;
            case 'a':
                game_stats(game, ALL);
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
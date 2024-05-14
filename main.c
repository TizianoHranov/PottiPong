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
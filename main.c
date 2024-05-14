#include <stdlib.h>
#include <stdio.h>
#include "ball.h"

int main(int argc, char const *argv[])
{
    char input; 
    bool run = 1;

    Ball* ball;
    ball = calloc(1, sizeof(Ball));

    ball_init(ball);

    while(run){
        input = getc(stdin);
        switch(input){
            case 'w':
                collision_X(ball);
                break;
            case 'p':
                collision_Y(ball);
                break;
            case 'e':
                run = 0;
                break;
            default:
                move(ball);
                printf("x: %d, dirX: %c\n", ball->x, (ball->direction[0]==RIGHT) ? 'R' : 'L');
                printf("y: %d, dirY: %c\n\n", ball->y, (ball->direction[1]==UP) ? 'U' : 'D');
        }

    }

    free(ball);
    return 0;
}

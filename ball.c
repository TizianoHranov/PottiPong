#include "ball.h"



void ball_move(Ball *ball){
    (ball->direction[0] == RIGHT) ? ball->x++ : ball->x--;
    (ball->direction[1] == UP) ? ball->y++ : ball->y--;
}

void ball_move_custom(Ball *ball, int a, int b){
    (ball->direction[0] == RIGHT) ? ball->x+=a : (ball->x-=a);
    (ball->direction[1] == UP) ? ball->y+=b : (ball->y-=b);
}

void collision_X(Ball *ball){
    ball->direction[0] = !(ball->direction[0]);
}

void collision_Y(Ball *ball){
    ball->direction[1] = !(ball->direction[1]);
}

void ball_init(Ball *ball, int x, int y, direction dirX, direction dirY){
    ball->x = x;
    ball->y = y;
    ball->direction[0] = dirX;
    ball->direction[1] = dirY;
}
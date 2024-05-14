#if !defined(POTTIPONG)
#define POTTIPONG

#include "ball.h"
#include "player.h"

typedef struct game{
    int points[2]; //points[0] = points of P1, points[1] = points of P2
    int difficulty; //determines the speed of the ball
    /*I have to think of a good step/ball_move() ratio here
    the players should (I think) be able to move even when the ball is still.
    this means that I gotta test around a bit*/


    int dimensions_screen[2]; //{width of screen, height of screen}

    Player* player1;
    Player* player2;
    Ball* ball;
}Game;

/*initializes a game and starts it*/
void game_start(Game* game);

/*frees all allocated variables and ends a game*/
void game_exit(Game* game);

/*equal to 1 "step" or "frame" ingame*/
void game_step(Game* game);



#endif // POTTIPONG

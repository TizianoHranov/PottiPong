#if !defined(POTTIPONG)
#define POTTIPONG

#include <stdlib.h>
#include <stdio.h>
#include "ball.h"
#include "player.h"


typedef struct game{
    int points[2]; //points[0] = points of P1, points[1] = points of P2
    int difficulty; //determines the speed of the ball
    /*I have to think of a good step/ball_move() ratio here
    the players should (I think) be able to move even when the ball is still.
    this means that I gotta test around a bit*/
    /*edit: 
    solution is to let the user fuck it up and not me :D
    fr tho, game_init takes a difficulty between 1 and 10 and
    the higher the difficulty, the less will be the playermove/ballmove
    ratio -> faster ball

    could maybe turn this into a float tho and let the ball move multiple times
    in one player move
    */
    int step_counter; //counts the steps since the ball movement
                      //once it reaches the value of difficulty, the ball moves
                      //and step_counter is reset to 0
    int dimensions_screen[2]; //{width of screen, height of screen}

    //poti_value1
    //poti_value2

    Player* player1;
    Player* player2;
    Ball* ball;
}Game;

/*used for the game_stats() function to display current values 
of all imnportant used variables in game*/
typedef enum STATS{
    ALL,
    PLAYERS,
    BALL,
    OBJECTS,
    GAME,
}stats;

/*initializes a game, takes:
difficulty = speed of the ball 1...5
dimensions_screen[2] = {x_width, y_width} of the game screen*/
void game_init(Game** ptr_game, int ball_xy[2], int player1_xyw[3], int player2_xyw[3], direction dirX, direction dirY, int difficulty, int dimensions_screen[2]);

/*frees all allocated variables and ends a game*/
void game_exit(Game** ptr_game);

/*equal to 1 "step" or "frame" ingame*/
void game_step(Game* game);

/*debug function that displays stats to the users liking
ALL
PLAYERS
BALL
OBJECTS (same as PLAYERS and BALL)
GAME 
DONT FORGET POTTI AND LEDS!!!DONT FORGET POTTI AND LEDS!!!DONT FORGET POTTI AND LEDS!!!
DONT FORGET POTTI AND LEDS!!!DONT FORGET POTTI AND LEDS!!!DONT FORGET POTTI AND LEDS!!!
DONT FORGET POTTI AND LEDS!!!DONT FORGET POTTI AND LEDS!!!DONT FORGET POTTI AND LEDS!!!
*/
void game_stats(Game* game, stats stat_flag);



#endif // POTTIPONG

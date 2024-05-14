#if !defined(BALL_H)
#define BALL_H

#include <stdbool.h>

typedef struct ball
{
    int x, y;
    bool direction[2]; //[LEFT/RIGHT][UP/DOWN]
}Ball;
 
/*relevant for move() and response on collisions with walls and players */
typedef enum DIRECTION{
    UP = 1,
    DOWN = 0,
    RIGHT = 1,
    LEFT = 0
}direction;

/*initializes a ball with:
x = 0,
y = 0,
direction = {RIGHT, UP}*/
void ball_init(Ball *ball);

/*initializes a ball with custom values*/
void ball_custom_init(Ball *ball, int x, int y, direction dirX, direction dirY);

/*moves the ball: 
 1 in the x direction
 1 in the y direction*/
void ball_move(Ball *ball);

/*moves the ball:
a in the x direction
b in the y direction
*/
void ball_move_custom(Ball *ball, int a, int b);

/*inverts the direction for x*/
void collision_X(Ball *ball);

/*inverts the direction for y*/
void collision_Y(Ball *ball);


#endif // BALL_H

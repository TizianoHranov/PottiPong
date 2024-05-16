#include "pottiPong.h"


void game_init(Game** ptr_game, int ball_xy[2], int player1_xyw[3], int player2_xyw[3], direction dirX, direction dirY, int difficulty, int dimensions_screen[2]){
    *ptr_game = calloc(1, sizeof(Game));
    Game* game = *ptr_game;

    game->points[0] = 0;
    game->points[1] = 0;
    if(difficulty < 1){
        difficulty = 1;
    }
    if(difficulty > 10){
        difficulty = 10;
    }
    game->difficulty = difficulty;
    game->step_counter = 0;
    game->dimensions_screen[0] = dimensions_screen[0];
    game->dimensions_screen[1] = dimensions_screen[1];

    //init potis -> info on what norm the poti values have should be 
    //dealt with in potis.h

    /*allocate all pointers*/
    if((game->player1 = calloc(1, sizeof(Player)) )== NULL){
        printf("ERROR when allocating player1\n");
        return;
    }
    if((game->player2 = calloc(1, sizeof(Player))) == NULL){
        free(game->player1);
        printf("ERROR when allocating player2\n");  
        return; 
    }
    if((game->ball = calloc(1, sizeof(Ball))) == NULL){
        free(game->player1);
        free(game->player2);
        printf("ERROR when allocating ball\n");
        return; 
    }
    
    /*init all pointers*/
    player_init(game->player1, player1_xyw[0], player1_xyw[1], player1_xyw[2]);
    player_init(game->player2, player2_xyw[0], player2_xyw[1], player2_xyw[2]);
    ball_init(game->ball, ball_xy[0], ball_xy[1], dirX, dirY);
}

void game_step(Game* game){

    game->step_counter++;

    /*
    playermovement ... potti reading will be hear
    */

    /*
    colission check, gonna be a whole lot of math cuz of the players
    */

   /*
    ball moves if step_counter
   */
}

void game_exit(Game** ptr_game){
    Game* game = *ptr_game;

    free(game->player1);
    free(game->player2);
    free(game->ball);
    game->player1 = NULL;
    game->player2 = NULL;
    game->ball = NULL;
    free(game);

    *ptr_game = NULL;
}


void game_stats(Game* game, stats stat_flag){
    if(game == NULL){
        printf("ERROR tried to print stats of a game == NULL\n");
        return;
    }
    switch(stat_flag){
        case ALL:
            printf("----ALL-STATS-----\n");
            printf("points1: %d points2: %d\n", game->points[0], game->points[1]);
            printf("difficulty: %d\n", game->difficulty);
            printf("step_cnt: %d dimensions: %dx%d\n", game->step_counter, game->dimensions_screen[0], game->dimensions_screen[1]);
            printf("player1:\n");
            printf("pos: %d|%d\n", game->player1->x, game->player1->y, game->player1->width);
            printf("width: %d\n", game->player1->width);
            printf("player2:\n");
            printf("pos: %d|%d\n", game->player2->x, game->player2->y, game->player2->width);
            printf("width: %d\n", game->player2->width);
            printf("ball:\n");
            printf("pos: %d|%d\n", game->ball->x, game->ball->y);
            printf("dir: %s and %s\n", game->ball->direction[0] ? "UP" : "DOWN", game->ball->direction[1] ? "RIGHT" : "LEFT");
            printf("----END-STATS-----\n");
            break;  
        case PLAYERS:
            printf("--PLAYERS-STATS---\n");
            printf("player1:\n");
            printf("pos: %d|%d\n", game->player1->x, game->player1->y, game->player1->width);
            printf("width: %d\n", game->player1->width);
            printf("player2:\n");
            printf("pos: %d|%d\n", game->player2->x, game->player2->y, game->player2->width);
            printf("width: %d\n", game->player2->width);
            printf("----END-STATS-----\n");
            break;     
        case BALL:
            printf("---BALL-STATS-----\n");
            printf("ball:\n");
            printf("pos: %d|%d\n", game->ball->x, game->ball->y);
            printf("dir: %s and %s\n", game->ball->direction[0] ? "UP" : "DOWN", game->ball->direction[1] ? "RIGHT" : "LEFT");
            printf("----END-STATS-----\n");
            break;  
        case OBJECTS:
            printf("--OBJECTS-STATS---\n");
            printf("player1:\n");
            printf("pos: %d|%d\n", game->player1->x, game->player1->y, game->player1->width);
            printf("width: %d\n", game->player1->width);
            printf("player2:\n");
            printf("pos: %d|%d\n", game->player2->x, game->player2->y, game->player2->width);
            printf("width: %d\n", game->player2->width);
            printf("ball:\n");
            printf("pos: %d|%d\n", game->ball->x, game->ball->y);
            printf("dir: %s and %s\n", game->ball->direction[0] ? "UP" : "DOWN", game->ball->direction[1] ? "RIGHT" : "LEFT");
            printf("----END-STATS-----\n");
            break; 
        case GAME:
            printf("----GAME-STATS----\n");
            printf("points1: %d points2: %d\n", game->points[0], game->points[1]);
            printf("difficulty: %d\n", game->difficulty);
            printf("step_cnt: %d dimensions: %dx%d\n", game->step_counter, game->dimensions_screen[0], game->dimensions_screen[1]);
            printf("----END-STATS-----\n");
            break; 
        default:
            printf("-----NOTHING------\n");
            printf("\n\n");
            printf("---NOTHING-END----\n");
    }
}
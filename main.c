#include <stdlib.h>
#include <stdio.h>
#include "ball.h"
#include "player.h"

int main(int argc, char const *argv[])
{
    char input[100]; 
    bool run = 1;

    Player* player;
    player = calloc(1, sizeof(Player));

    player_init(player);

    while(run){
        int x = 0;
        int y = 0;
        fgets(input, 99, stdin);
        x = atoi(input);

        player_move(player, x);

        printf("x: %d, width: %d\n", player->x, player->width);
        printf("y: %d\n\n", player->y);

    }

    free(player);
    return 0;
}

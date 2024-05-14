#include "player.h"

void player_init(Player *player){
    player->x = 0;
    player->y = 0;
    player->width = 1;
}

void player_init_custom(Player *player, int x, int y, int width){
    player->x = x;
    player->y = y;
    player->width = width;
}

void player_move(Player *player, int x){
    player->x += x;
}
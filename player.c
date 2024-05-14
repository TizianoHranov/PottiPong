#include "player.h"

void player_init(Player *player, int x, int y, int width){
    player->x = x;
    player->y = y;
    player->width = width;
}

void player_move(Player *player, int x){
    player->x += x;
}
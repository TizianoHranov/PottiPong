#if !defined(PLAYER_H)
#define PLAYER_H

typedef struct player
{
    int x;
    int y;     //y mustn't change once set
    int width; //width mustn't change once set
}Player;

/*initializes a player with custom values*/
void player_init(Player *player, int x, int y, int width);

/*moves a player x units from its current position*/
void player_move(Player *player, int x);

#endif // PLAYER_H

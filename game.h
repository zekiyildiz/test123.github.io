// game.h

#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "room.h"

typedef struct {
    Player *player;
    Room **rooms;
    int room_count;
    int game_over;
    int won;
    int turn;
} GameState;

// Fonksiyon prototipleri
GameState* init_game(void);
void free_game(GameState *game);
void show_room(GameState *game);
int move_player(GameState *game, const char *direction);
void attack_creature(GameState *game);
void list_player_inventory_cmd(GameState *game);
int save_game(GameState *game, const char *filepath);
int load_game(GameState *game, const char *filepath);
void list_saved_games(void);
void do_equip(GameState *game, const char *item_name);
void do_unequip(GameState *game, const char *item_name);
void do_status(GameState *game);
void do_rest(GameState *game);
void do_map(GameState *game);
void check_victory_condition(GameState *game);
void print_help(void); // 'print_help' fonksiyonunu ekleyin

#endif

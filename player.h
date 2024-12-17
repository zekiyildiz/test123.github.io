#ifndef PLAYER_H
#define PLAYER_H

#include "item.h"

#define MAX_INVENTORY 5
#define MAX_CAPACITY 20

typedef struct {
    int health;
    int max_health;
    int base_strength;
    int current_room;
    Item *inventory[MAX_INVENTORY];
    int inventory_count;
    int total_capacity;
    int level;
    int xp;
    int gold;
} Player;

// Fonksiyon prototipleri
Player *create_player(void); // void eklendi
void free_player(Player *player);

int add_item_to_player(Player *player, Item *item);
void list_player_inventory(Player *player);
Item* remove_item_from_player(Player *player, const char *item_name);
int player_total_inventory_weight(Player *player);
int player_attack_power(Player *player);
int player_defense(Player *player);
void player_gain_xp(Player *player, int amount);
void player_level_up(Player *player);
int player_equip_item(Player *player, const char *item_name);
int player_unequip_item(Player *player, const char *item_name);
Item *player_find_item(Player *player, const char *item_name);

#endif

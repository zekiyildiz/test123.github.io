#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "item.h"

Player *create_player() {
    Player *p = malloc(sizeof(Player));
    if(!p) {
        fprintf(stderr, "Failed to allocate player.\n");
        return NULL;
    }
    p->level = 1;
    p->xp = 0;
    p->health = 100;
    p->health = 50;
    p->max_health = 100;
    p->base_strength = 10;
    p->current_room = 0;
    p->inventory_count = 0;
    p->total_capacity = MAX_CAPACITY;
    p->gold = 0;
    for(int i=0; i<MAX_INVENTORY; i++)
        p->inventory[i] = NULL;
    return p;
}

void free_player(Player *player) {
    if(player) {
        for(int i=0; i<player->inventory_count; i++) {
            free_item(player->inventory[i]);
        }
        free(player);
    }
}

int player_total_inventory_weight(Player *player) {
    int total = 0;
    for(int i=0; i<player->inventory_count; i++) {
        total += player->inventory[i]->weight;
    }
    return total;
}

int add_item_to_player(Player *player, Item *item) {
    if(player->inventory_count >= MAX_INVENTORY) {
        printf("Your inventory is full.\n");
        return 0;
    }
    int cur_weight = player_total_inventory_weight(player);
    if(cur_weight + item->weight > player->total_capacity) {
        printf("You cannot carry that much weight.\n");
        return 0;
    }
    player->inventory[player->inventory_count++] = item;
    printf("Picked up %s.\n", item->name);
    return 1;
}

Item *player_find_item(Player *player, const char *item_name) {
    for(int i=0; i<player->inventory_count; i++) {
        if(strcmp(player->inventory[i]->name, item_name)==0) {
            return player->inventory[i];
        }
    }
    return NULL;
}

void list_player_inventory(Player *player) {
    if(player->inventory_count == 0) {
        printf("Your inventory is empty.\n");
    } else {
        printf("Your inventory:\n");
        for(int i=0; i<player->inventory_count; i++) {
            Item *it = player->inventory[i];
            printf(" - %s (weight %d)", it->name, it->weight);
            if(it->type == ITEM_TYPE_WEAPON) {
                printf(" [Weapon +%d attack]", it->attack_bonus);
                if(it->equipped) printf(" (equipped)");
            } else if(it->type == ITEM_TYPE_ARMOR) {
                printf(" [Armor +%d defense]", it->defense_bonus);
                if(it->equipped) printf(" (equipped)");
            } else if(it->type == ITEM_TYPE_QUEST) {
                printf(" [Quest Item]");
            } else if(it->type == ITEM_TYPE_FOOD) {
                printf(" [Food]");
            }
            printf("\n");
        }
    }
    int cur_weight = player_total_inventory_weight(player);
    printf("Carrying: %d/%d weight. Gold: %d\n", cur_weight, player->total_capacity, player->gold);
}

int remove_item_from_player(Player *player, const char *item_name) {
    for(int i=0; i<player->inventory_count; i++) {
        if(strcmp(player->inventory[i]->name, item_name) == 0) {
            free_item(player->inventory[i]);
            for(int j=i; j<player->inventory_count-1; j++) {
                player->inventory[j] = player->inventory[j+1];
            }
            player->inventory_count--;
            return 1;
        }
    }
    return 0;
}

int player_attack_power(Player *player) {
    int attack = player->base_strength;
    for(int i=0; i<player->inventory_count; i++) {
        Item *it = player->inventory[i];
        if(it->type == ITEM_TYPE_WEAPON && it->equipped) {
            attack += it->attack_bonus;
        }
    }
    return attack;
}

int player_defense(Player *player) {
    int def = 0;
    for(int i=0; i<player->inventory_count; i++) {
        Item *it = player->inventory[i];
        if(it->type == ITEM_TYPE_ARMOR && it->equipped) {
            def += it->defense_bonus;
        }
    }
    return def;
}

void player_gain_xp(Player *player, int amount) {
    player->xp += amount;
    while(player->xp >= player->level * 100) {
        player_level_up(player);
    }
}

void player_level_up(Player *player) {
    player->level++;
    printf("You leveled up! You are now level %d.\n", player->level);
    player->max_health += 20;
    player->base_strength += 5;
    printf("Your max health is now %d and base strength is now %d.\n", player->max_health, player->base_strength);
}

int player_equip_item(Player *player, const char *item_name) {
    Item *it = player_find_item(player, item_name);
    if(!it) {
        printf("You don't have that item.\n");
        return 0;
    }
    if(it->type != ITEM_TYPE_WEAPON && it->type != ITEM_TYPE_ARMOR) {
        printf("You can't equip that.\n");
        return 0;
    }
    // Unequip other weapons/armors of same type
    if(it->type == ITEM_TYPE_WEAPON) {
        for(int i=0; i<player->inventory_count; i++) {
            if(player->inventory[i]->type == ITEM_TYPE_WEAPON) {
                player->inventory[i]->equipped = 0;
            }
        }
    } else if(it->type == ITEM_TYPE_ARMOR) {
        for(int i=0; i<player->inventory_count; i++) {
            if(player->inventory[i]->type == ITEM_TYPE_ARMOR) {
                player->inventory[i]->equipped = 0;
            }
        }
    }
    it->equipped = 1;
    printf("You equipped %s.\n", it->name);
    return 1;
}

int player_unequip_item(Player *player, const char *item_name) {
    Item *it = player_find_item(player, item_name);
    if(!it || !it->equipped) {
        printf("You don't have that equipped.\n");
        return 0;
    }
    it->equipped = 0;
    printf("You unequipped %s.\n", it->name);
    return 1;
}

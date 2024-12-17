#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "game.h"
#include "player.h"
#include "room.h"
#include "item.h"
#include "creature.h"

GameState *init_game() {
    srand(time(NULL));
    GameState *game = malloc(sizeof(GameState));
    if(!game) {
        fprintf(stderr, "Failed to allocate game state.\n");
        return NULL;
    }

    game->player = create_player();
    if(!game->player) {
        free(game);
        return NULL;
    }

    game->game_over = 0;
    game->won = 0;
    game->turn = 0;

    game->room_count = 10;
    game->rooms = malloc(sizeof(Room*) * game->room_count);
    if(!game->rooms) {
        free_player(game->player);
        free(game);
        return NULL;
    }

    // Oda tanımlamaları
    game->rooms[0] = create_room("You stand in the entrance of the dungeon. Lord Gerede sent you here to find the golden crown deep within.");
    game->rooms[1] = create_room("A narrow corridor with dripping walls.");
    game->rooms[2] = create_room("A small chamber with cracked floor tiles.");
    game->rooms[3] = create_room("A grand hall with strange symbols etched on the walls.");
    game->rooms[4] = create_room("A dark library filled with ancient books.");
    game->rooms[5] = create_room("A damp cave illuminated by glowing fungi.");
    game->rooms[6] = create_room("A bridge over a bottomless pit.");
    game->rooms[7] = create_room("A chamber with a mysterious altar.");
    game->rooms[8] = create_room("A vault guarded by ancient traps.");
    game->rooms[9] = create_room("A treasure room glittering with gold and jewels. The golden crown must be here.");

    // Oda bağlantıları (daha karmaşık bir harita)
    // Room 0 connections
    game->rooms[0]->right = game->rooms[1];

    // Room 1 connections
    game->rooms[1]->left = game->rooms[0];
    game->rooms[1]->right = game->rooms[2];
    game->rooms[1]->down = game->rooms[3];

    // Room 2 connections
    game->rooms[2]->left = game->rooms[1];
    game->rooms[2]->right = game->rooms[4];
    game->rooms[2]->down = game->rooms[5];

    // Room 3 connections
    game->rooms[3]->up = game->rooms[1];
    game->rooms[3]->down = game->rooms[6];

    // Room 4 connections
    game->rooms[4]->left = game->rooms[2];
    game->rooms[4]->up = game->rooms[7];

    // Room 5 connections
    game->rooms[5]->up = game->rooms[2];
    game->rooms[5]->right = game->rooms[8];

    // Room 6 connections
    game->rooms[6]->up = game->rooms[3];

    // Room 7 connections
    game->rooms[7]->down = game->rooms[4];
    game->rooms[7]->right = game->rooms[8];

    // Room 8 connections
    game->rooms[8]->left = game->rooms[7];
    game->rooms[8]->up = game->rooms[5];
    game->rooms[6]->right = game->rooms[9];

    // Room 9 connections
    game->rooms[9]->left = game->rooms[8];
    // Room 9'dan Room 6'ya bağlantıyı kaldırıyoruz
    // veya özel bir mekanizma ekliyoruz

    // Eşyalar
    add_item_to_room(game->rooms[0], create_item("iron_sword", 5, ITEM_TYPE_WEAPON, 5, 0, 10));
    add_item_to_room(game->rooms[1], create_item("leather_armor", 8, ITEM_TYPE_ARMOR, 0, 2, 15));
    add_item_to_room(game->rooms[2], create_item("steel_sword", 6, ITEM_TYPE_WEAPON, 8, 0, 20));
    add_item_to_room(game->rooms[3], create_item("chain_armor", 10, ITEM_TYPE_ARMOR, 0, 5, 30));
    add_item_to_room(game->rooms[4], create_item("magic_ring", 1, ITEM_TYPE_GENERIC, 2, 0, 50));
    add_item_to_room(game->rooms[5], create_item("healing_potion", 2, ITEM_TYPE_FOOD, 0, 0, 25));
    add_item_to_room(game->rooms[6], create_item("silver_sword", 7, ITEM_TYPE_WEAPON, 10, 0, 35));
    add_item_to_room(game->rooms[7], create_item("dragon_scale_armor", 12, ITEM_TYPE_ARMOR, 0, 10, 60));
    add_item_to_room(game->rooms[8], create_item("golden_key", 1, ITEM_TYPE_GENERIC, 0, 0, 20));
    add_item_to_room(game->rooms[9], create_item("golden_crown", 1, ITEM_TYPE_QUEST, 0, 0, 0));

    // Canavarlar (oda zorluğuna göre)
    game->rooms[0]->creature = create_creature("goblin", 30, 5, 30, 10);
    game->rooms[1]->creature = create_creature("orc", 40, 7, 40, 20);
    game->rooms[2]->creature = create_creature("troll", 50, 10, 50, 30);
    game->rooms[3]->creature = create_creature("ogre", 60, 12, 60, 40);
    game->rooms[4]->creature = create_creature("specter", 35, 6, 35, 15);
    game->rooms[5]->creature = create_creature("cave_spider", 25, 4, 25, 10);
    game->rooms[6]->creature = create_creature("stone_golem", 70, 14, 70, 50);
    game->rooms[7]->creature = create_creature("ancient_guardian", 80, 16, 80, 60);
    game->rooms[8]->creature = create_creature("trap_master", 50, 12, 50, 30);
    game->rooms[9]->creature = create_creature("dragon", 100, 20, 100, 100);

    game->rooms[0]->discovered = 1;

    return game;
}



void free_game(GameState *game) {
    if(game) {
        for(int i=0; i<game->room_count; i++) {
            free_room(game->rooms[i]);
        }
        free(game->rooms);
        free_player(game->player);
        free(game);
    }
}

void show_room(GameState *game) {
    if(game->won) return;
    Room *r = game->rooms[game->player->current_room];
    printf("\n%s\n", r->description);

    // Mevcut yönleri göster
    printf("Exits: ");
    int first = 1;
    if(r->up) { printf("up"); first = 0; }
    if(r->down) { if(!first) printf(", "); printf("down"); first = 0; }
    if(r->left) { if(!first) printf(", "); printf("left"); first = 0; }
    if(r->right) { if(!first) printf(", "); printf("right"); }
    printf("\n");

    // Oda eşyalarını göster
    if(r->item_count > 0) {
        printf("Items here:\n");
        for(int i=0; i<r->item_count; i++) {
            printf(" - %s\n", r->items[i]->name);
        }
    }

    // Oda canavarını göster
    if(r->creature) {
        printf("A %s blocks your way.\n", r->creature->name);
    }
}

int move_player(GameState *game, const char *direction) {
    if(game->game_over || game->won) return 0;
    Room *current = game->rooms[game->player->current_room];
    Room *next = NULL;

    if(strcmp(direction,"up")==0) next = current->up;
    else if(strcmp(direction,"down")==0) next = current->down;
    else if(strcmp(direction,"left")==0) next = current->left;
    else if(strcmp(direction,"right")==0) next = current->right;
    else {
        printf("Invalid direction.\n");
        return 0;
    }

    // Room 6'dan Room 9'a geçişi kontrol et
    if(current == game->rooms[6] && next == game->rooms[9]) {
        // golden_key'e sahip olup olmadığını kontrol et
        Item *key = player_find_item(game->player, "golden_key");
        if(!key) {
            printf("The door to the Treasure Room is locked. You need a golden key to enter.\n");
            return 0;
        }
        // İsteğe bağlı olarak anahtarı kullanabilirsiniz:
        // remove_item_from_player(game->player, "golden_key");
    }

    if(next) {
        // Bulunan odayı tespit et ve oyuncuyu oraya taşı
        for(int i = 0; i < game->room_count; i++) {
            if(game->rooms[i] == next) {
                game->player->current_room = i;
                game->rooms[i]->discovered = 1;
                game->turn++;
                show_room(game);
                check_victory_condition(game);
                return 1;
            }
        }
    } else {
        printf("You can't go that way.\n");
    }
    return 0;
}




void attack_creature(GameState *game) {
    if(game->game_over || game->won) return;
    Room *r = game->rooms[game->player->current_room];
    if(!r->creature) {
        printf("There is nothing here to attack.\n");
        return;
    }

    // %20 iskalanma şansı
    int miss_chance = rand() % 100;
    if(miss_chance < 20) {
        printf("You missed your attack!\n");
        // Creature attacks back
        printf("The %s attacks you back!\n", r->creature->name);
        int def = player_defense(game->player);
        int dmg_to_player = r->creature->strength - def;
        if(dmg_to_player < 0) dmg_to_player = 0;
        game->player->health -= dmg_to_player;
        printf("You took %d damage. Your health: %d\n", dmg_to_player, game->player->health);
        if(game->player->health <= 0) {
            printf("You have been killed by the %s...\n", r->creature->name);
            printf("Game Over.\n");
            game->game_over = 1;
        } else {
            printf("%s health: %d\n", r->creature->name, r->creature->health);
        }
        game->turn++;
        return;
    }

    int base_damage = player_attack_power(game->player);
    int dmg = (int)(base_damage);
    // %20 kritik vuruş şansı
    int crit_chance = rand() % 100;
    if(crit_chance < 20) {
        dmg = (int)(base_damage * 1.5);
        printf("Critical hit!\n");
    }


    printf("You attack the %s for %d damage!\n", r->creature->name, dmg);
    r->creature->health -= dmg;
    if(r->creature->health <= 0) {
        printf("You have slain the %s.\n", r->creature->name);
        // Reward
        game->player->gold += r->creature->gold_reward;
        player_gain_xp(game->player, r->creature->xp_reward);
        free_creature(r->creature);
        r->creature = NULL;
        check_victory_condition(game);
        game->turn++;
        return;
    }

    // Creature attacks back
    printf("The %s attacks you back!\n", r->creature->name);
    int def = player_defense(game->player);
    int dmg_to_player = r->creature->strength - def;
    if(dmg_to_player < 0) dmg_to_player = 0;
    game->player->health -= dmg_to_player;
    printf("You took %d damage. Your health: %d\n", dmg_to_player, game->player->health);

    if(game->player->health <= 0) {
        printf("You have been killed by the %s...\n", r->creature->name);
        printf("Game Over.\n");
        game->game_over = 1;
    } else {
        printf("%s health: %d\n", r->creature->name, r->creature->health);
    }
    game->turn++;
}

// Yardım Fonksiyonu
void print_help() {
    printf("\nAvailable commands:\n");
    printf("  move <direction>   - Move in a direction (up, down, left, right)\n");
    printf("  look               - Look around the current room\n");
    printf("  inventory          - Show your inventory\n");
    printf("  pickup <item>      - Pick up an item\n");
    printf("  attack             - Attack the creature in the room\n");
    printf("  equip <item>       - Equip an item from your inventory\n");
    printf("  unequip <item>     - Unequip an equipped item\n");
    printf("  rest               - Rest to regain some health\n");
    printf("  status             - Show your current status\n");
    printf("  map                - Display the map of discovered rooms\n");
    printf("  list               - List saved games\n");
    printf("  save <filepath>    - Save the current game state\n");
    printf("  load <filepath>    - Load a saved game state\n");
    printf("  help               - Show this help message\n");
    printf("  exit               - Exit the game\n\n");
}

void check_victory_condition(GameState *game) {
    if(game->won || game->game_over) return;
    // Victory: If player has "golden_crown" and current_room == 0
    int has_crown = 0;
    for(int i=0; i<game->player->inventory_count; i++) {
        if(strcmp(game->player->inventory[i]->name, "golden_crown")==0) {
            has_crown = 1;
            break;
        }
    }
    if(has_crown && game->player->current_room == 0) {
        printf("\nYou have returned with the golden crown! Lord Gerede will be pleased!\n");
        printf("You win!\n");
        game->won = 1;
        game->game_over = 1;
    }
}

void do_equip(GameState *game, const char *item_name) {
    if(!item_name) {
        printf("Usage: equip <item>\n");
        return;
    }
    player_equip_item(game->player, item_name);
}

void do_unequip(GameState *game, const char *item_name) {
    if(!item_name) {
        printf("Usage: unequip <item>\n");
        return;
    }
    player_unequip_item(game->player, item_name);
}

void do_status(GameState *game) {
    Player *p = game->player;
    printf("Level: %d XP: %d/%d\n", p->level, p->xp, p->level*100);
    printf("Health: %d/%d\n", p->health, p->max_health);
    int att = player_attack_power(p);
    int def = player_defense(p);
    printf("Attack: %d Defense: %d\n", att, def);
    printf("Gold: %d\n", p->gold);
}

void do_rest(GameState *game) {
    // Canavar yoksa dinlen
    Room *r = game->rooms[game->player->current_room];
    if(r->creature) {
        printf("You cannot rest while an enemy is present.\n");
        return;
    }
    // Basit dinlenme: sağlık %20 yenilenir, max'ı geçemez.
    int heal = game->player->max_health/5;
    game->player->health += heal;
    if(game->player->health > game->player->max_health) {
        game->player->health = game->player->max_health;
    }
    printf("You rest and regain some health. Current health: %d/%d\n", game->player->health, game->player->max_health);
    game->turn++;
}

void do_map(GameState *game) {
    // Basit harita: sadece keşfedilen odaları göster.
    // 0 - 1 - 2 - 3 - 4
    printf("Map:\n");
    for(int i=0; i<game->room_count; i++) {
        if(game->rooms[i]->discovered) {
            printf("[%d] %s\n", i, game->rooms[i]->description);
        } else {
            printf("[%d] Unknown\n", i);
        }
    }
}

void list_player_inventory_cmd(GameState *game) {
    list_player_inventory(game->player);
}

int save_game(GameState *game, const char *filepath) {
    FILE *f = fopen(filepath, "w");
    if(!f) {
        perror("Failed to save game");
        return 0;
    }

    Player *p = game->player;
    fprintf(f, "PLAYER %d %d %d %d %d %d %d %d %d\n",
            p->health, p->max_health, p->base_strength, p->current_room,
            p->inventory_count, p->total_capacity, p->level, p->xp, p->gold);

    for(int i=0; i<p->inventory_count; i++) {
        Item *it = p->inventory[i];
        fprintf(f, "PITEM %s %d %d %d %d %d %d\n",
                it->name, it->weight, it->type, it->attack_bonus, it->defense_bonus, it->value, it->equipped);
    }

    fprintf(f, "ROOMCOUNT %d\n", game->room_count);
    for(int i=0; i<game->room_count; i++) {
        Room *r = game->rooms[i];
        fprintf(f, "ROOM %d %s\n", r->discovered, r->description);
        fprintf(f, "RITEMCOUNT %d\n", r->item_count);
        for(int j=0; j<r->item_count; j++) {
            Item *it = r->items[j];
            fprintf(f, "RITEM %s %d %d %d %d %d\n",
                    it->name, it->weight, it->type, it->attack_bonus, it->defense_bonus, it->value);
        }
        if(r->creature) {
            fprintf(f, "CREATURE %s %d %d %d %d\n", r->creature->name, r->creature->health, r->creature->strength, r->creature->xp_reward, r->creature->gold_reward);
        } else {
            fprintf(f, "CREATURE NONE\n");
        }
    }

    fprintf(f, "GAMESTATE %d %d %d\n", game->game_over, game->won, game->turn);

    fclose(f);
    printf("Game saved to %s\n", filepath);
    return 1;
}

int load_game(GameState *game, const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(!f) {
        perror("Failed to load game");
        return 0;
    }

    // Clean up
    for(int i=0; i<game->room_count; i++) {
        free_room(game->rooms[i]);
    }
    free(game->rooms);
    free_player(game->player);

    game->player = create_player();
    char line[1024];
    int room_index = -1;

    while(fgets(line, sizeof(line), f)) {
        char cmd[64];
        sscanf(line, "%s", cmd);

        if(strcmp(cmd, "PLAYER")==0) {
            int health, maxh, bstr, cr, invc, tc, lvl, xp, g;
            sscanf(line, "PLAYER %d %d %d %d %d %d %d %d %d", &health, &maxh, &bstr, &cr, &invc, &tc, &lvl, &xp, &g);
            Player *p = game->player;
            p->health = health;
            p->max_health = maxh;
            p->base_strength = bstr;
            p->current_room = cr;
            p->inventory_count = 0;
            p->total_capacity = tc;
            p->level = lvl;
            p->xp = xp;
            p->gold = g;
        } else if(strcmp(cmd,"PITEM")==0) {
            char iname[64];
            int w,t,ab,db,val,equipd;
            sscanf(line, "PITEM %s %d %d %d %d %d %d", iname, &w,&t,&ab,&db,&val,&equipd);
            Item *it = create_item(iname, w, t, ab, db, val);
            it->equipped = equipd;
            add_item_to_player(game->player, it);
        } else if(strcmp(cmd,"ROOMCOUNT")==0) {
            sscanf(line, "ROOMCOUNT %d", &game->room_count);
            game->rooms = malloc(sizeof(Room*)*game->room_count);
            for(int i=0; i<game->room_count; i++)
                game->rooms[i] = NULL;
        } else if(strcmp(cmd,"ROOM")==0) {
            int disc;
            char *desc = strchr(line,' ')+1;
            sscanf(desc, "%d", &disc);
            char *desc2 = strchr(desc,' ')+1;
            desc2[strcspn(desc2,"\n")] = '\0';
            room_index++;
            game->rooms[room_index] = create_room(desc2);
            game->rooms[room_index]->discovered = disc;
        } else if(strcmp(cmd,"RITEMCOUNT")==0) {
            // Skip
        } else if(strcmp(cmd,"RITEM")==0) {
            char iname[64];
            int w,t,ab,db,val;
            sscanf(line,"RITEM %s %d %d %d %d %d", iname, &w,&t,&ab,&db,&val);
            add_item_to_room(game->rooms[room_index], create_item(iname,w,t,ab,db,val));
        } else if(strcmp(cmd,"CREATURE")==0) {
            char cname[64];
            int chp, cstr, cxp, cg;
            int n = sscanf(line,"CREATURE %s %d %d %d %d", cname,&chp,&cstr,&cxp,&cg);
            if(n==5) {
                game->rooms[room_index]->creature = create_creature(cname,chp,cstr,cxp,cg);
            } else {
                game->rooms[room_index]->creature = NULL;
            }
        } else if(strcmp(cmd,"GAMESTATE")==0) {
            int go, w, turn;
            sscanf(line,"GAMESTATE %d %d %d", &go, &w, &turn);
            game->game_over = go;
            game->won = w;
            game->turn = turn;
        }
    }

    // Link rooms linearly again
    for(int i=0; i<game->room_count; i++) {
        if(i>0) game->rooms[i]->left = game->rooms[i-1];
        if(i<game->room_count-1) game->rooms[i]->right = game->rooms[i+1];
    }

    fclose(f);
    printf("Game loaded from %s\n", filepath);
    return 1;
}

void list_saved_games() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if(d) {
        printf("Saved games:\n");
        while((dir = readdir(d)) != NULL) {
            char *dot = strrchr(dir->d_name, '.');
            if(dot && strcmp(dot, ".sav")==0) {
                printf(" - %s\n", dir->d_name);
            }
        }
        closedir(d);
    } else {
        printf("No saved games found.\n");
    }
}

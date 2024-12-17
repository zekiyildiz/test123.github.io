// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "room.h"
#include "item.h"
#include "creature.h"

int main(int argc, char **argv) {
    GameState *game = init_game();
    if(!game) {
        fprintf(stderr, "Failed to initialize game.\n");
        return 1;
    }

    printf("\n\n\n\n\nWelcome, hero!\n");
    printf("Lord Gerede has tasked you with retrieving the golden crown from deep within this dungeon.\n");
    printf("Return the crown to the start to claim your victory!\n");
    printf("Type 'help' for a list of commands.\n");
    show_room(game);

    char line[256];
    while(!game->game_over) {
        printf(">> ");
        if(!fgets(line, sizeof(line), stdin)) {
            break; // EOF veya hata
        }

        line[strcspn(line,"\n")] = '\0';
        if(strlen(line)==0) continue;


        char *cmd = strtok(line, " ");
        if(!cmd) continue;

        if(strcmp(cmd,"exit")==0) {
            printf("Exiting game.\n");
            break;
        }
        else if(strcmp(cmd,"move")==0) {
            char *dir = strtok(NULL, " ");
            if(!dir) {
                printf("Usage: move <direction>\n");
                continue;
            }
            Room *current_room = game->rooms[game->player->current_room];

            // if theres monster, you cant move
            if(current_room->creature && current_room->creature->health > 0) {
                printf("A %s blocks your way. You must defeat it before moving to another room.\n", current_room->creature->name);
                continue;
            }

            move_player(game, dir);
        }
        else if(strcmp(cmd,"look")==0) {
            show_room(game);
        }
        else if(strcmp(cmd,"inventory")==0) {
            list_player_inventory_cmd(game);
        }
        else if(strcmp(cmd,"pickup")==0) {
            char *item_name = strtok(NULL, " ");
            if(!item_name) {
                printf("Usage: pickup <item>\n");
                continue;
            }
            Room *r = game->rooms[game->player->current_room];

            // if theres monster, you cant use pickup
            if(r->creature) {
                printf("You cannot pick up items while a %s is present. Defeat it first.\n", r->creature->name);
                continue;
            }

            Item *found_item = NULL;
            for(int i = 0; i < r->item_count; i++) {
                if(strcmp(r->items[i]->name, item_name) == 0) {
                    found_item = r->items[i];
                    break;
                }
            }

            if(!found_item) {
                printf("No such item here.\n");
                continue;
            }

            if(found_item->type == ITEM_TYPE_GENERIC) {
                printf("You cannot pick up the %s.\n", found_item->name);
                continue;
            }


            Item *it = remove_item_from_room(r, item_name);
            if(it) {
                if(add_item_to_player(game->player, it)) {
                    printf("Picked up %s.\n", it->name);
                    check_victory_condition(game);
                } else {
                    add_item_to_room(r, it);
                }
            } else {
                printf("No such item here.\n");
                continue;
            }
        }
        else if(strcmp(cmd,"drop")==0) {
            char *item_name = strtok(NULL, " ");
            if(!item_name) {
                printf("\nUsage: drop <item>\n");
                continue;
            }
            // find item
            Item *it = remove_item_from_player(game->player, item_name);
            if(it) {
                Room *current_room = game->rooms[game->player->current_room];
                add_item_to_room(current_room, it);
                printf("\nDropped %s.\n", it->name);
                check_victory_condition(game);
            } else {
                printf("\nYou do not have %s in your inventory.\n", item_name);
            }
        }
        else if(strcmp(cmd,"use")==0) {
            char *item_name = strtok(NULL, " ");
            if(!item_name) {
                printf("\nUsage: use <item>\n");
                continue;
            }

            Room *current_room = game->rooms[game->player->current_room];

            if(strcmp(item_name, "juice_machine") == 0) {
                // checks if theres juice machine
                int has_juice_machine = 0;
                for(int i=0; i<current_room->item_count; i++) {
                    if(strcmp(current_room->items[i]->name, "juice_machine") == 0) {
                        has_juice_machine = 1;
                        break;
                    }
                }

                if(!has_juice_machine) {
                    printf("\nThere is no juice machine here.\n");
                    continue;
                }

                if(game->player->gold < 100) {
                    printf("\nYou do not have enough gold to use the juice machine. It costs 100 gold.\n");
                    continue;
                }

                game->player->gold -= 100;
                game->player->health += 100;
                if(game->player->health > game->player->max_health) {
                    game->player->health = game->player->max_health;
                }

                printf("\nYou used the juice machine. You paid 100 gold and regained 100 health.\n");
                printf("\nCurrent health: %d/%d\n", game->player->health, game->player->max_health);
            }
            else {
                printf("\nYou cannot use %s.\n", item_name);
            }
        }

        else if(strcmp(cmd,"attack")==0) {
            attack_creature(game);
        }
        else if(strcmp(cmd,"equip")==0) {
            char *item_name = strtok(NULL, " ");
            if(!item_name) {
                printf("\nUsage: equip <item>\n");
                continue;
            }
            do_equip(game, item_name);
        }
        else if(strcmp(cmd,"unequip")==0) {
            char *item_name = strtok(NULL, " ");
            if(!item_name) {
                printf("\nUsage: unequip <item>\n");
                continue;
            }
            do_unequip(game, item_name);
        }
        else if(strcmp(cmd,"209")==0) {
            do_rest(game);
        }
        else if(strcmp(cmd,"status")==0) {
            do_status(game);
        }
        else if(strcmp(cmd,"map")==0) {
            do_map(game);
        }
        else if(strcmp(cmd,"list")==0) {
            list_saved_games();
        }
        else if(strcmp(cmd,"save")==0) {
            char *filepath = strtok(NULL, " ");
            if(!filepath) {
                printf("\nUsage: save <filepath>\n");
                continue;
            }
            save_game(game, filepath);
        }
        else if(strcmp(cmd,"load")==0) {
            char *filepath = strtok(NULL, " ");
            if(!filepath) {
                printf("\nUsage: load <filepath>\n");
                continue;
            }
            load_game(game, filepath);
            if(!game->won && !game->game_over) {
                show_room(game);
            }
        }
        else if(strcmp(cmd,"help")==0) {
            print_help();
        }
        else {
            printf("\nUnknown command: %s\n", cmd);
        }

        if(game->won) {
            printf("\nCongratulations! You have completed the game.\n");
            break;
        }
    }

    free_game(game);
    return 0;
}



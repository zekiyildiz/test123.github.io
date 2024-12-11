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

    printf("Welcome, hero!\n");
    printf("Lord Gerede has tasked you with retrieving the golden crown from deep within this dungeon.\n");
    printf("Return the crown to the start to claim your victory!\n");
    printf("Type 'help' for a list of commands.\n");
    show_room(game);

    char line[256];
    while(!game->game_over) {
        printf(">> ");
        if(!fgets(line, sizeof(line), stdin)) {
            break; // EOF or error
        }
        line[strcspn(line,"\n")] = '\0';
        if(strlen(line)==0) continue;

        char *cmd = strtok(line, " ");
        if(!cmd) continue;

        if(strcmp(cmd,"exit")==0) {
            printf("Exiting game.\n");
            break;
        } else if(strcmp(cmd,"move")==0) {
            char *dir = strtok(NULL, " ");
            if(!dir) {
                printf("Usage: move <direction>\n");
                continue;
            }
            move_player(game, dir);
        } else if(strcmp(cmd,"look")==0) {
            show_room(game);
        } else if(strcmp(cmd,"inventory")==0) {
            list_player_inventory_cmd(game);
        } else if(strcmp(cmd,"pickup")==0) {
            char *item_name = strtok(NULL, " ");
            if(!item_name) {
                printf("Usage: pickup <item>\n");
                continue;
            }
            Room *r = game->rooms[game->player->current_room];
            Item *it = remove_item_from_room(r, item_name);
            if(it) {
                add_item_to_player(game->player, it);
                check_victory_condition(game);
            } else {
                printf("No such item here.\n");
                continue;
            }
        } else if(strcmp(cmd,"attack")==0) {
            attack_creature(game);
        } else if(strcmp(cmd,"equip")==0) {
            char *item_name = strtok(NULL, " ");
            do_equip(game, item_name);
        } else if(strcmp(cmd,"unequip")==0) {
            char *item_name = strtok(NULL, " ");
            do_unequip(game, item_name);
        } else if(strcmp(cmd,"rest")==0) {
            do_rest(game);
        } else if(strcmp(cmd,"status")==0) {
            do_status(game);
        } else if(strcmp(cmd,"map")==0) {
            do_map(game);
        } else if(strcmp(cmd,"list")==0) {
            list_saved_games();
        } else if(strcmp(cmd,"save")==0) {
            char *filepath = strtok(NULL, " ");
            if(!filepath) {
                printf("Usage: save <filepath>\n");
                continue;
            }
            save_game(game, filepath);
        } else if(strcmp(cmd,"load")==0) {
            char *filepath = strtok(NULL, " ");
            if(!filepath) {
                printf("Usage: load <filepath>\n");
                continue;
            }
            load_game(game, filepath);
            if(!game->won && !game->game_over) {
                show_room(game);
            }
        } else if(strcmp(cmd,"help")==0) {
            print_help();
        } else {
            printf("Unknown command: %s\n", cmd);
        }

        if(game->won) {
            printf("Congratulations! You have completed the game.\n");
            break;
        }
    }

    free_game(game);
    return 0;
}

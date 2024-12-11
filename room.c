#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"
#include "item.h"
#include "creature.h"

Room *create_room(const char *desc) {
    Room *r = malloc(sizeof(Room));
    if(!r) {
        fprintf(stderr, "Failed to allocate room.\n");
        return NULL;
    }
    r->description = strdup(desc);
    if(!r->description) {
        fprintf(stderr, "Failed to allocate room description.\n");
        free(r);
        return NULL;
    }
    r->up = r->down = r->left = r->right = NULL;
    r->items = NULL;
    r->item_count = 0;
    r->creature = NULL;
    r->discovered = 0;
    return r;
}

void free_room(Room *room) {
    if(room) {
        free(room->description);
        for(int i=0; i<room->item_count; i++) {
            free_item(room->items[i]);
        }
        free(room->items);
        if(room->creature) {
            free_creature(room->creature);
        }
        free(room);
    }
}

int add_item_to_room(Room *room, Item *item) {
    Item **newarr = realloc(room->items, sizeof(Item*)*(room->item_count+1));
    if(!newarr) {
        fprintf(stderr, "Failed to allocate memory for room items.\n");
        return 0;
    }
    room->items = newarr;
    room->items[room->item_count] = item;
    room->item_count++;
    return 1;
}

Item *remove_item_from_room(Room *room, const char *item_name) {
    for(int i=0; i<room->item_count; i++) {
        if(strcmp(room->items[i]->name, item_name) == 0) {
            Item *it = room->items[i];
            for(int j=i; j<room->item_count-1; j++) {
                room->items[j] = room->items[j+1];
            }
            room->item_count--;
            if(room->item_count == 0) {
                free(room->items);
                room->items = NULL;
            } else {
                room->items = realloc(room->items, sizeof(Item*)*room->item_count);
            }
            return it;
        }
    }
    return NULL;
}

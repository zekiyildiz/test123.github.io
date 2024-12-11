#ifndef ROOM_H
#define ROOM_H

#include "item.h"
#include "creature.h"

typedef struct Room {
    char *description;
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
    Item **items;
    int item_count;
    Creature *creature;
    int discovered;
} Room;

Room *create_room(const char *desc);
void free_room(Room *room);
int add_item_to_room(Room *room, Item *item);
Item *remove_item_from_room(Room *room, const char *item_name);

#endif

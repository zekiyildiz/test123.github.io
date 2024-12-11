#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

Item *create_item(const char *name, int weight, ItemType type, int attack_bonus, int defense_bonus, int value) {
    Item *item = malloc(sizeof(Item));
    if(!item) {
        fprintf(stderr, "Memory allocation failed for item.\n");
        return NULL;
    }
    item->name = strdup(name);
    if(!item->name) {
        fprintf(stderr, "Memory allocation failed for item name.\n");
        free(item);
        return NULL;
    }
    item->weight = weight;
    item->type = type;
    item->attack_bonus = attack_bonus;
    item->defense_bonus = defense_bonus;
    item->value = value;
    item->equipped = 0;
    return item;
}

void free_item(Item *item) {
    if(item) {
        free(item->name);
        free(item);
    }
}

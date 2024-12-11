#ifndef ITEM_H
#define ITEM_H

typedef enum {
    ITEM_TYPE_GENERIC,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_QUEST,
    ITEM_TYPE_FOOD
} ItemType;

typedef struct {
    char *name;
    int weight;
    ItemType type;
    int attack_bonus;
    int defense_bonus;
    int value;
    int equipped; // 0/1
} Item;

Item *create_item(const char *name, int weight, ItemType type, int attack_bonus, int defense_bonus, int value);
void free_item(Item *item);

#endif

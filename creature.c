#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creature.h"

Creature *create_creature(const char *name, int health, int strength, int xp_reward, int gold_reward) {
    Creature *c = malloc(sizeof(Creature));
    if(!c) {
        fprintf(stderr, "Memory allocation failed for creature.\n");
        return NULL;
    }
    c->name = strdup(name);
    if(!c->name) {
        fprintf(stderr, "Memory allocation failed for creature name.\n");
        free(c);
        return NULL;
    }
    c->health = health;
    c->strength = strength;
    c->xp_reward = xp_reward;
    c->gold_reward = gold_reward;
    return c;
}

void free_creature(Creature *c) {
    if(c) {
        free(c->name);
        free(c);
    }
}

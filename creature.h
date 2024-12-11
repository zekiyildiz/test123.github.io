#ifndef CREATURE_H
#define CREATURE_H

typedef struct {
    char *name;
    int health;
    int strength;
    int xp_reward;
    int gold_reward;
} Creature;

Creature *create_creature(const char *name, int health, int strength, int xp_reward, int gold_reward);
void free_creature(Creature *c);

#endif

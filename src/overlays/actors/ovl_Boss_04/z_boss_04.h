#ifndef Z_BOSS_04_H
#define Z_BOSS_04_H

#include <global.h>

struct Boss04;

typedef struct Boss04 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x608];
} Boss04; // size = 0x74C

extern const ActorInit Boss_04_InitVars;

#endif // Z_BOSS_04_H

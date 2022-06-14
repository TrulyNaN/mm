#ifndef Z_BG_KIN2_BOMBWALL_H
#define Z_BG_KIN2_BOMBWALL_H

#include <global.h>

struct BgKin2Bombwall;

typedef struct BgKin2Bombwall {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x6C];
} BgKin2Bombwall; // size = 0x1B0

extern const ActorInit Bg_Kin2_Bombwall_InitVars;

#endif // Z_BG_KIN2_BOMBWALL_H

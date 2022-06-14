#ifndef Z_EN_BOMJIMB_H
#define Z_EN_BOMJIMB_H

#include <global.h>

struct EnBomjimb;

typedef struct EnBomjimb {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x1F0];
} EnBomjimb; // size = 0x334

extern const ActorInit En_Bomjimb_InitVars;

#endif // Z_EN_BOMJIMB_H

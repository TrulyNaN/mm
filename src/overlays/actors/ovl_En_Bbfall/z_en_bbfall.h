#ifndef Z_EN_BBFALL_H
#define Z_EN_BBFALL_H

#include <global.h>

struct EnBbfall;

typedef struct EnBbfall {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x320];
} EnBbfall; // size = 0x464

extern const ActorInit En_Bbfall_InitVars;

#endif // Z_EN_BBFALL_H

#ifndef Z_EN_KITAN_H
#define Z_EN_KITAN_H

#include <global.h>

struct EnKitan;

typedef struct EnKitan {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x198];
} EnKitan; // size = 0x2DC

extern const ActorInit En_Kitan_InitVars;

#endif // Z_EN_KITAN_H

#ifndef Z_EN_BAISEN_H
#define Z_EN_BAISEN_H

#include <global.h>

struct EnBaisen;

typedef struct EnBaisen {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x1B8];
} EnBaisen; // size = 0x2FC

extern const ActorInit En_Baisen_InitVars;

#endif // Z_EN_BAISEN_H

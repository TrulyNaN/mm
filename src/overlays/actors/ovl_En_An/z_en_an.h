#ifndef Z_EN_AN_H
#define Z_EN_AN_H

#include <global.h>

struct EnAn;

typedef struct EnAn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x284];
} EnAn; // size = 0x3C8

extern const ActorInit En_An_InitVars;

#endif // Z_EN_AN_H

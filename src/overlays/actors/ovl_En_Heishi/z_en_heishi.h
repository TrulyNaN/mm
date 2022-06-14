#ifndef Z_EN_HEISHI_H
#define Z_EN_HEISHI_H

#include <global.h>

struct EnHeishi;

typedef struct EnHeishi {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x18C];
} EnHeishi; // size = 0x2D0

extern const ActorInit En_Heishi_InitVars;

#endif // Z_EN_HEISHI_H

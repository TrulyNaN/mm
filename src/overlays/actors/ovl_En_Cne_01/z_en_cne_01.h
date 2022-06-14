#ifndef Z_EN_CNE_01_H
#define Z_EN_CNE_01_H

#include <global.h>

struct EnCne01;

typedef struct EnCne01 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x4D8];
} EnCne01; // size = 0x61C

extern const ActorInit En_Cne_01_InitVars;

#endif // Z_EN_CNE_01_H

#ifndef Z_EN_DAIKU_H
#define Z_EN_DAIKU_H

#include <global.h>

struct EnDaiku;

typedef struct EnDaiku {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x1A4];
} EnDaiku; // size = 0x2E8

extern const ActorInit En_Daiku_InitVars;

#endif // Z_EN_DAIKU_H

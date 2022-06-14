#ifndef Z_EN_FALL2_H
#define Z_EN_FALL2_H

#include <global.h>

struct EnFall2;

typedef struct EnFall2 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x1A8];
} EnFall2; // size = 0x2EC

extern const ActorInit En_Fall2_InitVars;

#endif // Z_EN_FALL2_H

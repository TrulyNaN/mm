#ifndef Z_EN_ZOW_H
#define Z_EN_ZOW_H

#include <global.h>

struct EnZow;

typedef struct EnZow {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x4D8];
} EnZow; // size = 0x61C

extern const ActorInit En_Zow_InitVars;

#endif // Z_EN_ZOW_H

#ifndef Z_EN_IK_H
#define Z_EN_IK_H

#include <global.h>

struct EnIk;

typedef struct EnIk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x524];
} EnIk; // size = 0x668

extern const ActorInit En_Ik_InitVars;

#endif // Z_EN_IK_H

#ifndef Z_EFF_STK_H
#define Z_EFF_STK_H

#include <global.h>

struct EffStk;

typedef struct EffStk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0xC];
} EffStk; // size = 0x150

extern const ActorInit Eff_Stk_InitVars;

#endif // Z_EFF_STK_H

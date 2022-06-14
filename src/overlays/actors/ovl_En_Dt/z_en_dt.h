#ifndef Z_EN_DT_H
#define Z_EN_DT_H

#include <global.h>

struct EnDt;

typedef struct EnDt {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x19C];
} EnDt; // size = 0x2E0

extern const ActorInit En_Dt_InitVars;

#endif // Z_EN_DT_H

#ifndef Z_EN_DS2N_H
#define Z_EN_DS2N_H

#include <global.h>

struct EnDs2n;

typedef struct EnDs2n {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0xF0];
} EnDs2n; // size = 0x234

extern const ActorInit En_Ds2n_InitVars;

#endif // Z_EN_DS2N_H

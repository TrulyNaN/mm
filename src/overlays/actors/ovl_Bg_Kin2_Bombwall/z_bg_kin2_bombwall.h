#ifndef Z_BG_KIN2_BOMBWALL_H
#define Z_BG_KIN2_BOMBWALL_H

#include "global.h"

struct BgKin2Bombwall;

typedef void (*BgKin2BombwallActionFunc)(struct BgKin2Bombwall*, GlobalContext*);

typedef struct BgKin2Bombwall {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderCylinder collider;  
    /* 0x01A8 */ BgKin2BombwallActionFunc actionFunc;
    /* 0x01AC */ s8 unk_1AC[0x4];
} BgKin2Bombwall; // size = 0x1B0

#endif // Z_BG_KIN2_BOMBWALL_H

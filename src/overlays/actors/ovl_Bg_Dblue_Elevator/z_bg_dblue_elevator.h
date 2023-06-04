#ifndef Z_BG_DBLUE_ELEVATOR_H
#define Z_BG_DBLUE_ELEVATOR_H

#include "global.h"

struct BgDblueElevator;

typedef void (*BgDblueElevatorActionFunc)(struct BgDblueElevator*, PlayState*);

typedef struct BgDblueElevator {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgDblueElevatorActionFunc actionFunc;
    /* 0x160 */ f32 unk160;
    /* 0x164 */ s8 padding[4]; 
    /* 0x168 */ s8 unk168[4]; //unk168 unk16B used. unk169 used. unk16A is an s8
    /* 0x16C */ f32 unk16C;//used
} BgDblueElevator; // size = 0x170

#endif // Z_BG_DBLUE_ELEVATOR_H

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
    /* 0x168 */ s8 unk168[2]; //unk168 unk16B used. unk169 used. unk16A is an s8
    /* 0x16A*/  s8 unk16A;
    /* 0x16C */ f32 unk16C;//used
} BgDblueElevator; // size = 0x170

struct BgDBlueElevatorStruct1;

typedef s32 (*BgDBlueElevatorStruct1Func)(struct BgDblueElevator*, PlayState*);

typedef struct BgDBlueElevatorStruct1{
    /* 0x000 */ s32 unk0;
    /* 0x004 */ BgDBlueElevatorStruct1Func unk4;
    /* 0x00C */ s8 unkC; //wrong, should be 8, not C.
    /* 0x00D */ s32 unkD;  
} BgDBlueElevatorStruct1;

#endif // Z_BG_DBLUE_ELEVATOR_H

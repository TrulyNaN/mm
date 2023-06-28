#ifndef Z_BG_DBLUE_ELEVATOR_H
#define Z_BG_DBLUE_ELEVATOR_H

#include "global.h"

struct BgDblueElevator;

typedef void (*BgDblueElevatorActionFunc)(struct BgDblueElevator*, PlayState*);

typedef struct BgDblueElevator {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgDblueElevatorActionFunc actionFunc;
    /* 0x160 */ f32 unk160;
    /* 0x164 */ f32 unk164;
    /* 0x168 */ s8 unk168; //velocity
    /* 0x169 */ s8 unk169; //timer
    /* 0x16A */ s8 unk16A; //related to unkC in Struct1
    /* 0x16B */ s8 unk16B;
    /* 0x16C */ f32 unk16C;//y? height? water height? used as a y value in a Vec3f.
} BgDblueElevator; // size = 0x170

struct BgDBlueElevatorStruct1;

typedef s32 (*BgDBlueElevatorStruct1Func)(struct BgDblueElevator*, PlayState*);

typedef struct BgDBlueElevatorStruct1
{
  s32 unk0;
  BgDBlueElevatorStruct1Func unk4;
  f32 unk8;
  s8 unkC;
  s8 unkD; //speed?
  s8 pad_E[2];
  f32 unk10;
  f32 unk14;
  f32 unk18;
} BgDBlueElevatorStruct1; // size = 0x1C

#endif // Z_BG_DBLUE_ELEVATOR_H

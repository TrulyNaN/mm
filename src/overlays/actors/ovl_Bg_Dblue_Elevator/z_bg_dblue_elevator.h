#ifndef Z_BG_DBLUE_ELEVATOR_H
#define Z_BG_DBLUE_ELEVATOR_H

#define BG_DBLUE_ELEVATOR_GET_INDEX(thisx) (((thisx)->params >> 8) & 0x3)
#define BG_DBLUE_ELEVATOR_GET_7F(thisx, x) (((thisx)->params + (x)) & 0x7F) //GET_7F or GET_SWITCH_FLAG?


#include "global.h"

struct BgDblueElevator;

typedef void (*BgDblueElevatorActionFunc)(struct BgDblueElevator*, PlayState*);

typedef struct BgDblueElevator {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgDblueElevatorActionFunc actionFunc;
    /* 0x160 */ f32 unk160; //related to y axis
    /* 0x164 */ f32 speed;
    /* 0x168 */ s8 direction; 
    /* 0x169 */ s8 timer;
    /* 0x16A */ s8 unk16A; //related to unkC in Struct1
    /* 0x16B */ s8 isInWater;
    /* 0x16C */ f32 unk16C;//y? height? water height? used as a y value in a Vec3f. Linked to ripples
} BgDblueElevator; // size = 0x170

struct BgDBlueElevatorStruct1;

typedef s32 (*BgDBlueElevatorStruct1Func)(struct BgDblueElevator*, PlayState*);

typedef struct BgDBlueElevatorStruct1
{
  s32 isHorizontal;
  BgDBlueElevatorStruct1Func unk4;
  f32 unk8;
  s8 unkC;
  s8 initialDirection; 
  s8 pad_E[2];
  f32 unk10;
  f32 unk14;
  f32 unk18;
} BgDBlueElevatorStruct1; // size = 0x1C

#endif // Z_BG_DBLUE_ELEVATOR_H

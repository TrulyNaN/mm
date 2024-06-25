#ifndef Z_BG_DBLUE_ELEVATOR_H
#define Z_BG_DBLUE_ELEVATOR_H

#define BG_DBLUE_ELEVATOR_GET_INDEX(thisx) (((thisx)->params >> 8) & 0x3)
#define BG_DBLUE_ELEVATOR_GET_7F(thisx, x) (((thisx)->params + (x)) & 0x7F) // GET_7F or GET_SWITCH_FLAG?

#include "global.h"

struct BgDblueElevator;

typedef void (*BgDblueElevatorActionFunc)(struct BgDblueElevator*, PlayState*);

typedef struct BgDblueElevator {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgDblueElevatorActionFunc actionFunc;
    /* 0x160 */ f32 posStep;   // rename (posStep?)
    /* 0x164 */ f32 posOffset; // rename posOffset?
    /* 0x168 */ s8 direction;
    /* 0x169 */ s8 activationTimer;
    /* 0x16A */ s8 pauseTimer;
    /* 0x16B */ s8 isInWater;
    /* 0x16C */ f32 ySurface;
} BgDblueElevator; // size = 0x170

struct BgDBlueElevatorStruct1;

typedef s32 (*BgDBlueElevatorStruct1Func)(struct BgDblueElevator*, PlayState*);

typedef struct BgDBlueElevatorStruct1 {
    s32 isHorizontal;
    BgDBlueElevatorStruct1Func activationStateFunc; //checks flags to know when to halt elevator
    f32 targetPosOffset;
    s8 pauseDuration;
    s8 initialDirection;
    s8 pad_E[2];
    f32 accelerationStep; // rename steps variables 1.0
    f32 decelerationStep; // 0.1
    f32 unk18;//5.0 or 6.0 targetPosStep?
} BgDBlueElevatorStruct1; // size = 0x1C

#endif // Z_BG_DBLUE_ELEVATOR_H

/*
 * File: z_bg_dblue_elevator.c
 * Overlay: ovl_Bg_Dblue_Elevator
 * Description: Great Bay Temple - Elevator
 */

#include "z_bg_dblue_elevator.h"
#include "objects/object_dblue_object/object_dblue_object.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgDblueElevator*)thisx)

void BgDblueElevator_SpawnRipplesAndSplashes(BgDblueElevator* this, PlayState* play);
void BgDblueElevator_CheckWaterBoxInfo(BgDblueElevator*, PlayState*);
s32 BgDblueElevator_GetWaterFlowFromCeiling(Actor* thisx, PlayState* play);
s32 BgDblueElevator_GetWaterFlowFromPipes(Actor* thisx, PlayState* play);

void BgDblueElevator_Init(Actor* thisx, PlayState* play2);
void BgDblueElevator_Destroy(Actor* thisx, PlayState* play);
void BgDblueElevator_Update(Actor* thisx, PlayState* play);
void BgDblueElevator_Draw(Actor* thisx, PlayState* play);

void BgDblueElevator_SetupWaitForMove(BgDblueElevator* this);
void BgDblueElevator_WaitForMove(BgDblueElevator* this, PlayState* play);
void BgDblueElevator_SetupPause(BgDblueElevator* this);
void BgDblueElevator_Pause(BgDblueElevator* this, PlayState* play);
void BgDblueElevator_SetupMove(BgDblueElevator* this);
void BgDblueElevator_Move(BgDblueElevator* this, PlayState* play);

ActorInit Bg_Dblue_Elevator_InitVars = {
    ACTOR_BG_DBLUE_ELEVATOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DBLUE_OBJECT,
    sizeof(BgDblueElevator),
    (ActorFunc)BgDblueElevator_Init,
    (ActorFunc)BgDblueElevator_Destroy,
    (ActorFunc)BgDblueElevator_Update,
    (ActorFunc)BgDblueElevator_Draw,
};

static BgDBlueElevatorStruct1 D_80B92960[4] = {
    {
        0,
        ((s32(*)(BgDblueElevator*, PlayState*))BgDblueElevator_GetWaterFlowFromCeiling),
        320.0f,
        30,
        1,
        { 0, 0 },
        1.0f,
        0.1f,
        6.0f,
    },
    {
        1,
        ((s32(*)(BgDblueElevator*, PlayState*))BgDblueElevator_GetWaterFlowFromCeiling),
        195.0f,
        30,
        1,
        { 0, 0 },
        1.0f,
        0.1f,
        5.0f,
    },
    {
        0,
        ((s32(*)(BgDblueElevator*, PlayState*))BgDblueElevator_GetWaterFlowFromPipes),
        280.0f,
        30,
        1,
        { 0, 0 },
        1.0f,
        0.1f,
        6.0f,
    },
    {
        0,
        ((s32(*)(BgDblueElevator*, PlayState*))BgDblueElevator_GetWaterFlowFromPipes),
        280.0f,
        30,
        -1,
        { 0, 0 },
        1.0f,
        0.1f,
        6.0f,
    },
};

static s16 D_80B929D0[4] = { -90, -90, 90, 90 };   // Related to splash x pos
static s16 D_80B929D8[4] = { -100, 90, 90, -100 }; // Related to splash z pos
static s8 sLargeRipplesLives[4] = { 0, 2, 4, 0 };
static s8 sSmallRipplesLives[6] = { 0, 1, 2, 3, 4, 5 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 250, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 250, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgDblueElevator_CheckWaterBoxInfo(BgDblueElevator* this, PlayState* play2) {
    PlayState* play = play2;
    WaterBox* waterBox;
    s32 bgId;

    this->isWithinWaterBoxXZ =
        WaterBox_GetSurfaceImpl(play, &play->colCtx, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.z,
                                &this->waterSurfaceYPos, &waterBox, &bgId);
}

void BgDblueElevator_SpawnRipplesAndSplashes(BgDblueElevator* this, PlayState* play) {
    s32 i;
    Vec3f ripplePos;
    Vec3f splashPos;
    s32 j;
    f32 d1;
    f32 d2;
    f32 v1;
    f32 v2;
    f32 var_fs0;
    s32 rn;

    Matrix_Push();
    Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_NEW);

    for (i = 0; i < 3; i++) {
        v1 = D_80B929D0[i];
        v2 = D_80B929D8[i];
        d1 = D_80B929D0[i + 1] - D_80B929D0[i];
        d2 = D_80B929D8[i + 1] - D_80B929D8[i];

        for (j = 0; j < 7; j++) {
            ripplePos.x = ((d1 * j) * (1.0f / 7.0f)) + v1;
            ripplePos.y = this->waterSurfaceYPos;
            ripplePos.z = ((d2 * j) * (1.0f / 7.0f)) + v2;

            ripplePos.x += (Rand_ZeroOne() - 0.5f) * 20.0f;
            ripplePos.z += (Rand_ZeroOne() - 0.5f) * 20.0f;
            Matrix_MultVec3f(&ripplePos, &splashPos);
            splashPos.x += this->dyna.actor.world.pos.x;
            splashPos.z += this->dyna.actor.world.pos.z;
            EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, (Rand_ZeroOne() * 400.0f) + 210.0f);
        }
    }

    for (i = 0; i < 3; i++) {
        ripplePos.x = ((Rand_ZeroOne() - 0.5f) * 60.0f) + this->dyna.actor.world.pos.x;
        ripplePos.y = this->waterSurfaceYPos;
        ripplePos.z = ((Rand_ZeroOne() - 0.5f) * 60.0f) + this->dyna.actor.world.pos.z;
        EffectSsGRipple_Spawn(play, &ripplePos, 1000, 3000, sLargeRipplesLives[i]);
    }

    for (i = 0; i < 6; i++) {
        var_fs0 = Rand_ZeroOne();
        var_fs0 = 1.0f - (var_fs0 * var_fs0);
        rn = Rand_Next();
        if (rn > 0) {
            var_fs0 = -var_fs0;
        }
        ripplePos.x = (var_fs0 * 100.0f) + this->dyna.actor.world.pos.x;
        ripplePos.y = this->waterSurfaceYPos;
        var_fs0 = Rand_ZeroOne();
        var_fs0 = 1.0f - (var_fs0 * var_fs0);
        rn = Rand_Next();
        if (rn > 0) {
            var_fs0 = -var_fs0;
        }
        ripplePos.z = (var_fs0 * 100.0f) + this->dyna.actor.world.pos.z;
        EffectSsGRipple_Spawn(play, &ripplePos, 400, 800, sSmallRipplesLives[i]);
    }
    Matrix_Pop();
}

s32 BgDblueElevator_GetWaterFlowFromCeiling(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    if (Flags_GetSwitch(play, BG_DBLUE_ELEVATOR_GET_7F(&this->dyna.actor, 0))) {
        return 0;
    }
    return 1;
}

s32 BgDblueElevator_GetWaterFlowFromPipes(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;
    s32 var_s0 = 0;

    // This checks if first the yellow tank turnkey is open.
    if (!Flags_GetSwitch(play, BG_DBLUE_ELEVATOR_GET_7F(&this->dyna.actor, 0))) {
        var_s0 = 1;
    }

    // This checks if water is flowing from the red pipes.
    if (Flags_GetSwitch(play, BG_DBLUE_ELEVATOR_GET_7F(&this->dyna.actor, 1)) &&
        Flags_GetSwitch(play, BG_DBLUE_ELEVATOR_GET_7F(&this->dyna.actor, 2)) &&
        Flags_GetSwitch(play, BG_DBLUE_ELEVATOR_GET_7F(&this->dyna.actor, 3))) {
        var_s0 += 2;
    }
    return var_s0;
}

void BgDblueElevator_Init(Actor* thisx, PlayState* play2) {
    BgDblueElevator* this = THIS;
    PlayState* play = play2;
    s32 index;
    BgDBlueElevatorStruct1* ptr;
    s32 waterFlow;
    index = BG_DBLUE_ELEVATOR_GET_INDEX(&this->dyna.actor);

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, DYNA_TRANSFORM_POS);
    ptr = &D_80B92960[index];
    DynaPolyActor_LoadMesh(play, &this->dyna, &gGreatBayTempleObjectElevatorCol);
    waterFlow = ptr->getWaterFlow(this, play);

    if (waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_REVERSED) {
        this->direction = -ptr->initialDirection;
    } else {
        this->direction = ptr->initialDirection;
    }
    BgDblueElevator_CheckWaterBoxInfo(this, play);
    if ((waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_STOPPED) ||
        (waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_BOTH_DIRECTIONS)) {
        BgDblueElevator_SetupWaitForMove(this);
    } else {
        BgDblueElevator_SetupMove(this);
    }
}

void BgDblueElevator_Destroy(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void BgDblueElevator_SetupWaitForMove(BgDblueElevator* this) {
    this->activationTimer = 60;
    this->actionFunc = BgDblueElevator_WaitForMove;
}

/**
 * The elevator waits for 3 seconds (60 frames) when water restarts flowing before resuming its halted cycle.
 */
void BgDblueElevator_WaitForMove(BgDblueElevator* this, PlayState* play) {
    s32 waterFlow;
    s32 index = BG_DBLUE_ELEVATOR_GET_INDEX(&this->dyna.actor);

    waterFlow = (&D_80B92960[index])->getWaterFlow(this, play);
    if ((waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_STOPPED) ||
        (waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_BOTH_DIRECTIONS)) {
        this->activationTimer = 60;
    } else {
        this->activationTimer--;
        if (this->activationTimer <= 0) {
            BgDblueElevator_SetupMove(this);
        }
    }
}

void BgDblueElevator_SetupPause(BgDblueElevator* this) {
    s32 index = BG_DBLUE_ELEVATOR_GET_INDEX(&this->dyna.actor);

    this->pauseTimer = D_80B92960[index].pauseDuration;
    this->actionFunc = BgDblueElevator_Pause;
}

/**
 * As the elevator cycles up and down or sideways, it will pause for a set amount of time at each end of its path
 * before moving again.
 */
void BgDblueElevator_Pause(BgDblueElevator* this, PlayState* play) {
    s32 waterFlow;
    s32 index = BG_DBLUE_ELEVATOR_GET_INDEX(&this->dyna.actor);

    waterFlow = (&D_80B92960[index])->getWaterFlow(this, play);
    if ((waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_STOPPED) ||
        (waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_BOTH_DIRECTIONS)) {
        BgDblueElevator_SetupWaitForMove(this);
    } else {
        this->pauseTimer--;
        if (this->pauseTimer <= 0) {
            BgDblueElevator_SetupMove(this);
        }
    }
}

void BgDblueElevator_SetupMove(BgDblueElevator* this) {
    this->actionFunc = BgDblueElevator_Move;
    this->posStep = 0.0f;
}

void BgDblueElevator_Move(BgDblueElevator* this, PlayState* play) {
    BgDBlueElevatorStruct1* ptr;
    s32 index;
    s32 waterFlow;

    f32 targetPosOffset;
    s32 hasReachedEnd;
    s32 isDeactivated;
    f32 posStep;

    Vec3f basePosOffset;
    Vec3f posOffset;
    f32 nearWaterSurfaceCheck;

    s32 pad;
    index = BG_DBLUE_ELEVATOR_GET_INDEX(&(this->dyna.actor));
    ptr = &D_80B92960[index];
    waterFlow = (ptr)->getWaterFlow(this, play); // flag stuff
    if ((waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_STOPPED) ||
        (waterFlow == BG_DBLUE_ELEVATOR_WATER_FLOW_BOTH_DIRECTIONS)) { // related to stopping
        isDeactivated = Math_StepToF(&this->posStep, 0.0f, ptr->decelerationStep);
    } else {
        isDeactivated = 0;
        Math_StepToF(&this->posStep, ptr->targetPosStep, ptr->accelerationStep);
    }

    if (this->direction > 0) {
        targetPosOffset = ptr->targetPosOffset;
    } else {
        targetPosOffset = -ptr->targetPosOffset;
    }

    if (this->posStep <= 1.1f) {
        posStep = 1.1f;
    } else {
        posStep = this->posStep;
    }

    if (Math_SmoothStepToF(&this->posOffset, targetPosOffset, 0.4f, posStep, 1.0f) < 0.001f) {
        hasReachedEnd = true;
    } else {
        hasReachedEnd = false;
    }

    if (!ptr->isHorizontal) {
        this->dyna.actor.world.pos.y = this->posOffset + this->dyna.actor.home.pos.y;
        if (((this->dyna.actor.flags & 0x40) == 0x40) && (this->isWithinWaterBoxXZ)) { // macro to write
            if (this->direction > 0) {
                nearWaterSurfaceCheck = ((this->dyna.actor.world.pos.y + (-10.0f)) - this->waterSurfaceYPos) *
                                        ((this->dyna.actor.prevPos.y + (-10.0f)) - this->waterSurfaceYPos);
            } else {
                nearWaterSurfaceCheck = ((this->dyna.actor.world.pos.y + (-30.0f)) - this->waterSurfaceYPos) *
                                        ((this->dyna.actor.prevPos.y + (-30.0f)) - this->waterSurfaceYPos);
            }
            if (nearWaterSurfaceCheck <= 0.0f) {
                BgDblueElevator_SpawnRipplesAndSplashes(this, play);
            }
        }
    } else {
        Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_NEW);
        basePosOffset.x = this->posOffset;
        basePosOffset.y = 0.0f;
        basePosOffset.z = 0.0f;
        Matrix_MultVec3f(&basePosOffset, &posOffset);
        Math_Vec3f_Sum(&this->dyna.actor.home.pos, &posOffset, &this->dyna.actor.world.pos);
    }
    if (isDeactivated) {
        BgDblueElevator_SetupWaitForMove(this);
    } else {
        if (hasReachedEnd) {
            this->direction = -this->direction;
            BgDblueElevator_SetupPause(this);
        }
    }
}

void BgDblueElevator_Update(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    this->actionFunc(this, play);
}

void BgDblueElevator_Draw(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    Gfx_DrawDListOpa(play, gGreatBayTempleObjectElevatorDL);
}

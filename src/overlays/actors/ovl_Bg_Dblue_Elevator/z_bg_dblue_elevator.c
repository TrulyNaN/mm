/*
 * File: z_bg_dblue_elevator.c
 * Overlay: ovl_Bg_Dblue_Elevator
 * Description: Great Bay Temple - Elevator
 */

#include "z_bg_dblue_elevator.h"
#include "objects/object_dblue_object/object_dblue_object.h"
// TODO: include object file

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgDblueElevator*)thisx)

void func_80B91F74(BgDblueElevator* arg0, PlayState* arg1);
void func_80B91F20(BgDblueElevator*, PlayState*);
s32 func_80B922C0(Actor* thisx, PlayState* play);            /* static */

s32 func_80B922FC(Actor* arg0, PlayState* play);            /* static */
void BgDblueElevator_Init(Actor* thisx, PlayState* play2);
void BgDblueElevator_Destroy(Actor* thisx, PlayState* play);
void BgDblueElevator_Update(Actor* thisx, PlayState* play);
void BgDblueElevator_Draw(Actor* thisx, PlayState* play);

void func_80B924DC(BgDblueElevator* arg0);                  /* static */
void func_80B924F8(BgDblueElevator* arg0, PlayState* play); /* static */
void func_80B9257C(BgDblueElevator* arg0);
void func_80B925B8(BgDblueElevator* arg0, PlayState* play); /* static */
void func_80B92644(BgDblueElevator* this);                  /* static */
void func_80B92660(BgDblueElevator* this, PlayState* play); /* static */

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
        ((s32 (*)(BgDblueElevator *, PlayState *)) func_80B922C0),
        320.0f,
        0x1E,
        1,
        { 0, 0 },
        1.0f,
        0.1f,
        6.0f,
    },
    {
        1,
        ((s32 (*)(BgDblueElevator *, PlayState *)) func_80B922C0),
        195.0f,
        0x1E,
        1,
        { 0, 0 },
        1.0f,
        0.1f,
        5.0f,
    },
    {
        0,
        ((s32 (*)(BgDblueElevator *, PlayState *)) func_80B922FC),
        280.0f,
        0x1E,
        1,
        { 0, 0 },
        1.0f,
        0.1f,
        6.0f,
    },
    {
        0,
        ((s32 (*)(BgDblueElevator *, PlayState *)) func_80B922FC),
        280.0f,
        0x1E,
        -1,
        { 0, 0 },
        1.0f,
        0.1f,
        6.0f,
    },
}; 
static s16 D_80B929D0[4] = { -0x5A, -0x5A, 0x5A, 0x5A };
static s16 D_80B929D8[4] = { -0x64, 0x5A, 0x5A, -0x64 }; 
static s8 D_80B929E0[4] = { 0, 2, 4, 0 };
static s8 D_80B929E4[6] = { 0, 1, 2, 3, 4, 5 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 250, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 250, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void func_80B91F20(BgDblueElevator* this, PlayState* play2) {
    PlayState *play = play2;
    WaterBox* waterBox;
    s32 bgId;

    this->unk16B = WaterBox_GetSurfaceImpl(play, &play->colCtx, this->dyna.actor.world.pos.x,
                                           this->dyna.actor.world.pos.z, &this->unk16C, &waterBox, &bgId);
}

void func_80B91F74(BgDblueElevator* this, PlayState* play) {
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
            ripplePos.y = this->unk16C;
            ripplePos.z = ((d2 * j) * (1.0f / 7.0f)) + v2;

            ripplePos.x += (Rand_ZeroOne() - 0.5f) * 20.0f;
            ripplePos.z += (Rand_ZeroOne() - 0.5f) * 20.0f;
            Matrix_MultVec3f(&ripplePos, &splashPos);
            splashPos.x += this->dyna.actor.world.pos.x;
            splashPos.z += this->dyna.actor.world.pos.z;
            EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, (Rand_ZeroOne() * 400.0f) + 210.0f);
        }
    
    }
    
    for (i = 0; i < 3; i++)
    {
        ripplePos.x = ((Rand_ZeroOne() - 0.5f) * 60.0f) + this->dyna.actor.world.pos.x;
        ripplePos.y = this->unk16C;
        ripplePos.z = ((Rand_ZeroOne() - 0.5f) * 60.0f) + this->dyna.actor.world.pos.z;
        EffectSsGRipple_Spawn(play, &ripplePos, 0x3E8, 0xBB8, D_80B929E0[i]);
    }

    for (i = 0; i < 6; i++) {
        var_fs0 = Rand_ZeroOne();
        var_fs0 = 1.0f - (var_fs0 * var_fs0);
        rn = Rand_Next();
        if (rn > 0) {
            var_fs0 = -var_fs0;
        }
        ripplePos.x = (var_fs0 * 100.0f) + this->dyna.actor.world.pos.x;
        ripplePos.y = this->unk16C;
        var_fs0 = Rand_ZeroOne();
        var_fs0 = 1.0f - (var_fs0 * var_fs0);
        rn = Rand_Next();
        if (rn > 0) {
            var_fs0 = -var_fs0;
        }
        ripplePos.z = (var_fs0 * 100.0f) + this->dyna.actor.world.pos.z;
        EffectSsGRipple_Spawn(play, &ripplePos, 0x190, 0x320, D_80B929E4[i]);
    }
    Matrix_Pop();
}

s32 func_80B922C0(Actor* thisx, PlayState* play) {//some boolean? no since other Struct1 function can return 2 and 3.
    BgDblueElevator* this = THIS;

    if (Flags_GetSwitch(play, this->dyna.actor.params & 0x7F)) { // TODO: write flag in header
        return 0;
    }
    return 1;
}

s32 func_80B922FC(Actor* this, PlayState* play) {//counts something?
    s32 var_s0 = 0;

    if (!Flags_GetSwitch(play, this->params & 0x7F)) {
        var_s0 = 1;
    }
    if (Flags_GetSwitch(play, (this->params + 1) & 0x7F) && Flags_GetSwitch(play, (this->params + 2) & 0x7F) &&
        Flags_GetSwitch(play, (this->params + 3) & 0x7F)) {
        var_s0 += 2;
    }
    return var_s0;
}

void BgDblueElevator_Init(Actor* thisx, PlayState* play2) {
    BgDblueElevator* this = THIS;
    PlayState* play = play2;
    s32 index;
    BgDBlueElevatorStruct1* temp_v1;
    s32 temp_v0;
    index = ((this->dyna.actor.params) >> 8) & 0x3;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, 1); //run symbol scripts
    temp_v1 = &D_80B92960[index];
    DynaPolyActor_LoadMesh(play, &this->dyna, &gGreatBayTempleObjectElevatorCol);
    temp_v0 = temp_v1->unk4(this, play);
    if (temp_v0 == 2) {
        this->unk168 = -temp_v1->unkD;
    } else {
        this->unk168 = temp_v1->unkD;
    }
    func_80B91F20(this, play);
    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        func_80B924DC(this);
    } else {
        func_80B92644(this);
    }
}

void BgDblueElevator_Destroy(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void func_80B924DC(BgDblueElevator* this) {
    this->unk169 = 60;
    this->actionFunc = func_80B924F8;
}

void func_80B924F8(BgDblueElevator* this, PlayState* play) {
    s32 temp_v0;
    s32 index = ((s16)this->dyna.actor.params >> 8) & 3;

    temp_v0 = (&D_80B92960[index])->unk4(this, play);
    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        this->unk169 = 60;
    } else {
        this->unk169 -= 1;
        if (this->unk169 <= 0) {
            func_80B92644(this);
        }
    }
}

void func_80B9257C(BgDblueElevator* this) { //weird.
    s8* new_var2;
    s32 new_var;

    new_var = (this->dyna.actor.params >> 8) & 3; // would % 4 work too instead of & 3?
    new_var2 = &(D_80B92960[0].unkC)+(new_var * 0x1C);
    this->unk16A = *new_var2;
    this->actionFunc = func_80B925B8;
}

void func_80B925B8(BgDblueElevator* this, PlayState* play) {
    s32 temp_v0;

    s32 index = (this->dyna.actor.params >> 8) & 3;

    temp_v0 = (&D_80B92960[index])->unk4(this, play);
    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        func_80B924DC(this);
    } else {
        this->unk16A--;
        if (this->unk16A <= 0) {
            func_80B92644(this);
        }
    }
    
}

void func_80B92644(BgDblueElevator* this) {
    this->actionFunc = func_80B92660;
    this->unk160 = 0.0f;
}

void func_80B92660(BgDblueElevator* this, PlayState* play) {
    BgDBlueElevatorStruct1* temp_v1;
    s32 index;
    s32 temp_v0;

    f32 var_fa0;
    s32 sp5C;
    s32 sp58;
    f32 var_fv1;

    Vec3f sp48;
    Vec3f sp3C;
    f32 var_fv1_2;

    s32 pad;
    index = (this->dyna.actor.params >> 8) & 3;
    temp_v1 = &D_80B92960[index];
    temp_v0 = (temp_v1)->unk4(this, play);
    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        sp58 = Math_StepToF(&this->unk160, 0.0f, temp_v1->unk14);
    } else {
        sp58 = 0;
        Math_StepToF(&this->unk160, temp_v1->unk18, temp_v1->unk10);
    }

    if(this->unk168 > 0){
        var_fa0 = temp_v1->unk8;
    } 
    else {
        var_fa0 = -temp_v1->unk8;
    }

    //makes sure var_fv1 at least 1.1.
    if(this->unk160 <= 1.1f){
        var_fv1 = 1.1f;
    } 
    else {
        var_fv1 = this->unk160;
    }

    if(Math_SmoothStepToF(&this->unk164, var_fa0, 0.4f, var_fv1, 1.0f) < 0.001f){
        sp5C = true;
    } 
    else {
        sp5C = false;
    }

    if (temp_v1->unk0 == 0) {
        this->dyna.actor.world.pos.y = this->unk164 + this->dyna.actor.home.pos.y;
        if (((this->dyna.actor.flags & 0x40) == 0x40) && (this->unk16B != 0)) {
            if (this->unk168 > 0) {
                var_fv1_2 = ((this->dyna.actor.world.pos.y + (-10.0f)) - this->unk16C) *
                            ((this->dyna.actor.prevPos.y + (-10.0f)) - this->unk16C);
            } else {
                var_fv1_2 = ((this->dyna.actor.world.pos.y + (-30.0f)) - this->unk16C) *
                            ((this->dyna.actor.prevPos.y + (-30.0f)) - this->unk16C);
            }
            if (var_fv1_2 <= 0.0f) {
                func_80B91F74(this, play);
            }
        }
    } else {
        Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_NEW);
        sp48.x = this->unk164;
        sp48.y = 0.0f;
        sp48.z = 0.0f;
        Matrix_MultVec3f(&sp48, &sp3C);
        Math_Vec3f_Sum(&this->dyna.actor.home.pos, &sp3C, &this->dyna.actor.world.pos);
    }
    if (sp58) {
        func_80B924DC(this);
    } else {
        if (sp5C) {
            this->unk168 = -this->unk168;
            func_80B9257C(this);
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

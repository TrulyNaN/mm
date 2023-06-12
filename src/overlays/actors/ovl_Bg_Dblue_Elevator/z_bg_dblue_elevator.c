/*
 * File: z_bg_dblue_elevator.c
 * Overlay: ovl_Bg_Dblue_Elevator
 * Description: Great Bay Temple - Elevator
 */

#include "z_bg_dblue_elevator.h"
//TODO: include object file

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgDblueElevator*)thisx)

void BgDblueElevator_Init(Actor* thisx, PlayState* play);
void BgDblueElevator_Destroy(Actor* thisx, PlayState* play);
void BgDblueElevator_Update(Actor* thisx, PlayState* play);
void BgDblueElevator_Draw(Actor* thisx, PlayState* play);

#if 0
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

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80B929EC[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 250, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 250, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#endif

void func_80B91F20(BgDblueElevator *, PlayState *);
void BgDblueElevator_Destroy(Actor* thisx, PlayState* play); /* static */
void BgDblueElevator_Draw(Actor* thisx, PlayState* play); /* static */
void BgDblueElevator_Init(Actor *arg0, PlayState *play); /* static */
void BgDblueElevator_Update(Actor* thisx, PlayState* play0);            /* static */
s32 func_80B922C0(Actor *arg0, PlayState *play);       /* static */
s32 func_80B922FC(Actor *arg0, PlayState *play);     /* static */
void func_80B924DC(BgDblueElevator* arg0);                    /* static */
void func_80B924F8(BgDblueElevator *arg0, PlayState* play);                    /* static */
void func_80B9257C(BgDblueElevator *arg0);
void func_80B925B8(BgDblueElevator *arg0, PlayState* play);                    /* static */
void func_80B92644(Actor *arg0);                    /* static */
void func_80B92660(BgDblueElevator* this, PlayState *play);      /* static */
extern Gfx D_060002C8; //gGreatBayTempleObjectElevatorDL
extern UNK_TYPE D_060005C4;
extern s32 D_80B92960;                                /* unable to generate initializer */
extern UNK_TYPE D_80B92964;                                /* unable to generate initializer */
extern UNK_TYPE D_80B9296C;                                /* unable to generate initializer */
extern UNK_TYPE D_80B929D0;                                /* unable to generate initializer */
extern UNK_TYPE D_80B929D8;                                /* unable to generate initializer */
extern UNK_TYPE D_80B929DE;     //fake?                          /* unable to generate initializer */
extern s8* D_80B929E0; //= { 0, 2 };
extern UNK_TYPE D_80B929E3;     //fake?                  /* unable to generate initializer */
extern s8* D_80B929E4; // = { 0, 1, 2, 3, 4, 5 };
extern UNK_TYPE D_80B929EA;                //fake?                /* unable to generate initializer */
extern InitChainEntry D_80B929EC[];                /* unable to generate initializer */

void func_80B91F20(BgDblueElevator* this, PlayState* play) {
    s32 pad; //probably cast of this or play
    WaterBox *sp30; 
    s32 sp2C;
  
    this->unk168[3] = WaterBox_GetSurfaceImpl(play, &play->colCtx, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.z, 
                                           &this->unk16C, &sp30, &sp2C);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B91F74.s")

s32 func_80B922C0(Actor* thisx, PlayState *play) {
    BgDblueElevator* this = THIS;

    if (Flags_GetSwitch(play, this->dyna.actor.params & 0x7F)) { //TODO: write flag in header 
        return 0;
    }
    return 1;
}

s32 func_80B922FC(Actor *arg0, PlayState *play) {
    s32 var_s0;

    var_s0 = 0;
    if (!Flags_GetSwitch(play, arg0->params & 0x7F)) {
        var_s0 = 1;
    }
    if (Flags_GetSwitch(play, (arg0->params + 1) & 0x7F) && Flags_GetSwitch(play, (arg0->params + 2) & 0x7F) &&
        Flags_GetSwitch(play, (arg0->params + 3) & 0x7F)) {
        var_s0 += 2;
    }
    return var_s0;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/BgDblueElevator_Init.s")

// void BgDblueElevator_Init(Actor *thisx, PlayState *play) {
//     s32 sp2C;
//     s32 sp24;
//     void *sp20;
//     s32 temp_v0;
//     void *temp_v1;
//     BgDblueElevator *this = THIS;

//     sp2C = ((s16) this->dyna.actor.params >> 8) & 0x3; //macro this.
//     Actor_ProcessInitChain(&this->dyna.actor, D_80B929EC);
//     DynaPolyActor_Init(&this->dyna, DYNA_TRANSFORM_POS);
//     DynaPolyActor_LoadMesh(play, &this->dyna, (CollisionHeader *) &D_060005C4);
//     temp_v1 = (sp2C * 0x1C) + &D_80B92960; //what?
//     sp20 = temp_v1;
//     temp_v0 = temp_v1->unk4(this, play);
//     if (temp_v0 == 2) {
//         this->unk168[0] = -temp_v1->unkD; 
//     } else {
//         this->unk168[0] = temp_v1->unkD;
//     }
//     sp24 = temp_v0;
//     func_80B91F20(this, play);
//     if ((temp_v0 == 0) || (temp_v0 == 3)) {
//         func_80B924DC(&this->dyna.actor);
//     } else {
//         func_80B92644(&this->dyna.actor);
//     }
// }


void BgDblueElevator_Destroy(Actor* thisx, PlayState *play) {
    BgDblueElevator* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B924DC.s")
void func_80B924DC(BgDblueElevator *arg0) {
    arg0->unk168[1] = 0x3C;
    arg0->actionFunc = func_80B924F8;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B924F8.s")
// void func_80B924F8(BgDblueElevator *arg0, PlayState *play) {
//     s32 temp_v0;

//     temp_v0 = func_80B922C0((Actor*) arg0, play);//*(&D_80B92964 + ((((s16) arg0->dyna.actor.params >> 8) & 3) * 0x1C))();
//     if ((temp_v0 == 0) || (temp_v0 == 3)) {
//         arg0->unk168[1] = 60;
//     } else {
//         arg0->unk168[1] -= 1;
//         if (arg0->unk168[1] <= 0) {
//             func_80B92644(&arg0->dyna.actor);
//         }
//     }
    
// }

void func_80B9257C(BgDblueElevator *arg0)
{
  s8 *new_var2;
  s32 new_var;
  new_var = (arg0->dyna.actor.params >> 8) & 3;
  new_var2 = (s8*) &D_80B9296C;
  new_var2 = new_var2 + (new_var * 0x1C);
  arg0->unk16A = *new_var2;
  arg0->actionFunc = func_80B925B8;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B925B8.s")

void func_80B92644(Actor *thisx) {
    BgDblueElevator* this = THIS;
    
    this->actionFunc = func_80B92660;
    this->unk160 = 0.0f;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B92660.s")

void BgDblueElevator_Update(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;
    
    this->actionFunc(this, play);
}

void BgDblueElevator_Draw(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    Gfx_DrawDListOpa(play, &D_060002C8);
}
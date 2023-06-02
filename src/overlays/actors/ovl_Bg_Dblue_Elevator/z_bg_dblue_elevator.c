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

extern InitChainEntry D_80B929EC[];

void BgDblueElevator_Destroy(Actor* thisx, PlayState* play); /* static */
void BgDblueElevator_Draw(Actor* thisx, PlayState* play); /* static */
void BgDblueElevator_Init(Actor *arg0, PlayState *arg1); /* static */
void BgDblueElevator_Update(Actor* thisx, PlayState* play0);            /* static */
s32 func_80B922C0(Actor *arg0, PlayState *arg1);       /* static */
s32 func_80B922FC(Actor *arg0, PlayState *arg1);     /* static */
void func_80B924DC(Actor *arg0);                    /* static */
void func_80B924F8(Actor *arg0);                    /* static */
void func_80B925B8(Actor *arg0);                    /* static */
void func_80B92644(Actor *arg0);                    /* static */
void func_80B92660(BgDblueElevator* this, PlayState *arg1);      /* static */
extern Gfx D_060002C8; //gGreatBayTempleObjectElevatorDL
extern UNK_TYPE D_060005C4;
extern UNK_TYPE D_80B92960;                                /* unable to generate initializer */
extern UNK_TYPE D_80B92964;                                /* unable to generate initializer */
extern UNK_TYPE D_80B9296C;                                /* unable to generate initializer */
extern UNK_TYPE D_80B929D0;                                /* unable to generate initializer */
extern UNK_TYPE D_80B929D8;                                /* unable to generate initializer */
extern UNK_TYPE D_80B929DE;                                /* unable to generate initializer */
extern s8* D_80B929E0; //= { 0, 2 };
extern UNK_TYPE D_80B929E3;                                /* unable to generate initializer */
extern s8* D_80B929E4; // = { 0, 1, 2, 3, 4, 5 };
extern UNK_TYPE D_80B929EA;                                /* unable to generate initializer */
extern InitChainEntry D_80B929EC[];                /* unable to generate initializer */

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B91F20.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B91F74.s")

s32 func_80B922C0(Actor* thisx, PlayState *arg1) {
    BgDblueElevator* this = THIS;

    if (Flags_GetSwitch(arg1, this->dyna.actor.params & 0x7F)) { //TODO: write flag in header 
        return 0;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B922FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/BgDblueElevator_Init.s")

void BgDblueElevator_Destroy(Actor* thisx, PlayState *play) {
    BgDblueElevator* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B924DC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B924F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B9257C.s")

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
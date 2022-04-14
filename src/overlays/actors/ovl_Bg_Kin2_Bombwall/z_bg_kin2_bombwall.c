/*
 * File: z_bg_kin2_bombwall.c
 * Overlay: ovl_Bg_Kin2_Bombwall
 * Description: Ocean Spider House - Bombable Wall
 */

#include "z_bg_kin2_bombwall.h"

#define FLAGS 0x10000010

#define THIS ((BgKin2Bombwall*)thisx)

void BgKin2Bombwall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgKin2Bombwall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgKin2Bombwall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgKin2Bombwall_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit Bg_Kin2_Bombwall_InitVars = {
    ACTOR_BG_KIN2_BOMBWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_KIN2_OBJ,
    sizeof(BgKin2Bombwall),
    (ActorFunc)BgKin2Bombwall_Init,
    (ActorFunc)BgKin2Bombwall_Destroy,
    (ActorFunc)BgKin2Bombwall_Update,
    (ActorFunc)BgKin2Bombwall_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80B6E6F0 = {
    { COLTYPE_NONE, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_NONE, OC2_NONE, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK0, { 0x00000000, 0x00, 0x00 }, { 0x00000008, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_ON, OCELEM_NONE, },
    { 60, 60, 0, { 0, 0, 0 } },
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80B6E748[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 300, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

#endif

extern UNK_TYPE D_06000128;
extern UNK_TYPE D_06000360;
extern UNK_TYPE D_06000490;

extern ColliderCylinderInit D_80B6E6F0;
extern InitChainEntry D_80B6E748[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E020.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E090.s")

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/BgKin2Bombwall_Init.s")

                                /* size = 0x1B0 */

void func_80B6E4B8(BgKin2Bombwall *);                  /* extern */ //was ? before
ColliderCylinderInit D_80B6E6F0 = {
    { 0xA, 0, 9, 0, 0, 1 },
    { 0, { 0, 0, 0 }, { 8, 0, 0 }, 0, 1, 0 },
    { 0x3C, 0x3C, 0, { 0, 0, 0 } },
};
InitChainEntry D_80B6E748[6];                       /* unable to generate initializer */

void BgKin2Bombwall_Init(Actor* thisx, GlobalContext *globalCtx) {
    BgKin2Bombwall* this = THIS;
    ColliderCylinder *sp24;
    //ColliderCylinder *temp_a1;

    Actor_ProcessInitChain(&this->actor, D_80B6E748);
    DynaPolyActor_Init((DynaPolyActor *) this, 0);
    sp24 = &this->unk15C;
    Collider_InitCylinder(globalCtx, sp24);
    //temp_a1 = &this->unk15C;
    //sp24 = temp_a1;
    //Collider_InitCylinder(globalCtx, temp_a1);
    if (Flags_GetSwitch(globalCtx, this->actor.params & 0x7F) != 0) {
        Actor_MarkForDeath(&this->actor);
        return;
    }
    DynaPolyActor_LoadMesh(globalCtx, (DynaPolyActor *) this, (CollisionHeader *) &D_06000490);
    Collider_SetCylinder(globalCtx, sp24, &this->actor, &D_80B6E6F0);
    Collider_UpdateCylinder(&this->actor, sp24);
    Actor_SetHeight(&this->actor, 60.0f);
    func_80B6E4B8(this);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/BgKin2Bombwall_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E4B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E4CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E544.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E558.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E5F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/func_80B6E614.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/BgKin2Bombwall_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Kin2_Bombwall/BgKin2Bombwall_Draw.s")

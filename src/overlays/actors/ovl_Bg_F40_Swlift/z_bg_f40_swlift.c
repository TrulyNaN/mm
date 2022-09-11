/*
 * File: z_bg_f40_swlift.c
 * Overlay: ovl_Bg_F40_Swlift
 * Description: Unused Stone Tower vertically-oscillating platform
 */

#include "z_bg_f40_swlift.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgF40Swlift*)thisx)

void BgF40Swlift_Init(Actor* thisx, PlayState* play);
void BgF40Swlift_Destroy(Actor* thisx, PlayState* play);
void BgF40Swlift_Update(Actor* thisx, PlayState* play);
void BgF40Swlift_Draw(Actor* thisx, PlayState* play);

#if 0
const ActorInit Bg_F40_Swlift_InitVars = {
    ACTOR_BG_F40_SWLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_F40_OBJ,
    sizeof(BgF40Swlift),
    (ActorFunc)BgF40Swlift_Init,
    (ActorFunc)BgF40Swlift_Destroy,
    (ActorFunc)BgF40Swlift_Update,
    (ActorFunc)BgF40Swlift_Draw,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_8096F540[] = {
    ICHAIN_F32(uncullZoneScale, 550, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 5000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#endif

extern InitChainEntry D_8096F540[];

extern UNK_TYPE D_06003B08;
extern UNK_TYPE D_06003E80;

extern s32 D_8096F510[]; //= { 0xFF };
extern s32 D_8096F514[];// = { 0xFF, 0xFF, 0xFF };
extern s32 D_8096F5D0[];

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Swlift/BgF40Swlift_Init.s")

void BgF40Swlift_Init(Actor *thisx, PlayState *play) {
    s32 temp_v1;
    BgF40Swlift *this = (BgF40Swlift *) thisx;

    Actor_ProcessInitChain(&this->actor, D_8096F540);
    DynaPolyActor_Init((DynaPolyActor *) this, 1);
    temp_v1 = ((s32) this->actor.params >> 8) & 0xFF;
    if ((temp_v1 < 0) || (temp_v1 >= 5)) {
        Actor_MarkForDeath(&this->actor);
    } else{
        D_8096F5D0[temp_v1] = (s32) this->actor.world.pos.y;
        D_8096F510[temp_v1] = this->actor.params & 0xFF;
        if (temp_v1 != 0) {
            Actor_MarkForDeath(&this->actor);
        } else{
            DynaPolyActor_LoadMesh(play, (DynaPolyActor *) this, (CollisionHeader *) &D_06003E80);
            this->actor.params = 0;
        }
    }
    
    
}

void BgF40Swlift_Destroy(Actor *thisx, PlayState *play) {
    BgF40Swlift *this = (BgF40Swlift *) thisx;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->unk144);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Swlift/BgF40Swlift_Update.s")

void BgF40Swlift_Draw(Actor *thisx, PlayState *play) {
    BgF40Swlift *this = THIS;
    Gfx_DrawDListOpa(play, (Gfx *) &D_06003B08);
}

#include "z_en_ending_hero6.h"
#include "objects/object_dt/object_dt.h"
#include "objects/object_daiku/object_daiku.h"
#include "objects/object_bai/object_bai.h"
#include "objects/object_toryo/object_toryo.h"
#include "objects/object_sdn/object_sdn.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8)

#define THIS ((EnEndingHero6*)thisx)

void EnEndingHero6_Init(Actor* thisx, GlobalContext* globalCtx);
void EnEndingHero6_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnEndingHero6_Update(Actor* thisx, GlobalContext* globalCtx);
void EnEndingHero6_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnEndingHero6_InitSkelAnime(EnEndingHero6* this, s32 npcIndex);
void EnEndingHero6_SetupIdle(EnEndingHero6* this);
void EnEndingHero6_Idle(EnEndingHero6* this, GlobalContext* globalCtx);

const ActorInit En_Ending_Hero6_InitVars = {
    ACTOR_EN_ENDING_HERO6,
    ACTORCAT_NPC,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnEndingHero6),
    (ActorFunc)EnEndingHero6_Init,
    (ActorFunc)EnEndingHero6_Destroy,
    (ActorFunc)EnEndingHero6_Update,
    (ActorFunc)EnEndingHero6_Draw,
};
*/

static FlexSkeletonHeader* sSkeletons[] = {
    &object_dt_Skel_00B0CC,    &object_bai_Skel_007908,   &object_toryo_Skel_007150,
    &gSoldierSkeleton,         &object_daiku_Skel_00A850, &object_daiku_Skel_00A850,
    &object_daiku_Skel_00A850, &object_daiku_Skel_00A850, &object_daiku_Skel_00A850,
};

static AnimationHeader* sAnimations[] = {
    &object_dt_Anim_000BE0,    &object_bai_Anim_0011C0,   &object_toryo_Anim_000E50,
    &gSoldierCheerWithSpear,   &object_daiku_Anim_002FA0, &object_daiku_Anim_002FA0,
    &object_daiku_Anim_002FA0, &object_daiku_Anim_002FA0, &object_daiku_Anim_002FA0,
};

static s32 sLimbCounts[] = { 15, 20, 17, 17, 17, 17, 17, 17, 17 };

void EnEndingHero6_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnEndingHero6* this = THIS;

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = 6;
    this->actor.gravity = -3.0f;
    SkelAnime_InitFlex(globalCtx, &this->skelAnime, sSkeletons[this->npcIndex], sAnimations[this->npcIndex],
                       this->jointTable, this->morphTable, sLimbCounts[this->npcIndex]);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    EnEndingHero6_SetupIdle(this);
}

void EnEndingHero6_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnEndingHero6_InitSkelAnime(EnEndingHero6* this, s32 npcIndex) {
    this->animIndex = npcIndex;
    this->frameCount = Animation_GetLastFrame(sAnimations[npcIndex]);
    Animation_Change(&this->skelAnime, sAnimations[this->animIndex], 1.0f, 0.f, this->frameCount, 0, 0.0f);
}

void EnEndingHero6_SetupIdle(EnEndingHero6* this) {
    EnEndingHero6_InitSkelAnime(this, this->npcIndex);
    this->isIdle = 1;
    this->actionFunc = EnEndingHero6_Idle;
}

void EnEndingHero6_Idle(EnEndingHero6* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->skelAnime);
}

void EnEndingHero6_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnEndingHero6* this = THIS;

    if (this->timer != 0) {
        this->timer--;
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;

    if (this->blinkTimer == 0) {
        this->eyeState++;
        if (this->eyeState >= 3) {
            this->eyeState = 0;
            this->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }

    this->actionFunc(this, globalCtx);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 20.0f, 20.0f, 50.0f, 0x1D);
}

void EnEndingHero6_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    static Gfx* D_80C2426C[] = { object_daiku_DL_0070C0, object_daiku_DL_006FB0, object_daiku_DL_006E80,
                                 object_daiku_DL_006D70, object_daiku_DL_00A390 };
    EnEndingHero6* this = THIS;
    s32 index;

    OPEN_DISPS(globalCtx->state.gfxCtx);

    if (this->npcIndex >= 4 && limbIndex == 15) {
        index = this->npcIndex - 4;
        gSPDisplayList(POLY_OPA_DISP++, D_80C2426C[index]);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx);
}

void EnEndingHero6_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static TexturePtr D_80C24280[] = { object_dt_Tex_007350, object_dt_Tex_009590, object_dt_Tex_009F90,
                                       object_dt_Tex_00A790, object_dt_Tex_00AB90 };
    static TexturePtr D_80C24294[] = { object_dt_Tex_007750, object_dt_Tex_00A390, object_dt_Tex_00A490 };
    s32 pad;
    EnEndingHero6* this = THIS;
    s32 index = 0;

    if (this->isIdle == 1) {
        OPEN_DISPS(globalCtx->state.gfxCtx);

        func_8012C28C(globalCtx->state.gfxCtx);
        func_8012C2DC(globalCtx->state.gfxCtx);

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/EnEndingHero6_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/func_80C23D60.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/func_80C23DDC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/func_80C23E18.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/EnEndingHero6_Update.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/func_80C23F14.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ending_Hero6_0x80C23C90/EnEndingHero6_Draw.asm")

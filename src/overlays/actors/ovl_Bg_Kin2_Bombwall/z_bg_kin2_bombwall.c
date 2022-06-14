/*
 * File: z_bg_kin2_bombwall.c
 * Overlay: ovl_Bg_Kin2_Bombwall
 * Description: Ocean Spider House - Bombable Wall
 */
#include "z_bg_kin2_bombwall.h"
#include "objects/object_kin2_obj/object_kin2_obj.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_10000000)

#define THIS ((BgKin2Bombwall*)(thisx))

void BgKin2Bombwall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgKin2Bombwall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgKin2Bombwall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgKin2Bombwall_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 BgKin2Bombwall_IsHitFromNearby(BgKin2Bombwall*, GlobalContext*);
void BgKin2Bombwall_SpawnEffects(BgKin2Bombwall*, GlobalContext*);
void BgKin2Bombwall_SetupWait(BgKin2Bombwall*);
void BgKin2Bombwall_Wait(BgKin2Bombwall*, GlobalContext*);
void BgKin2Bombwall_SetupPlayCutscene(BgKin2Bombwall*);
void BgKin2Bombwall_PlayCutscene(BgKin2Bombwall*, GlobalContext*);
void BgKin2Bombwall_SetupEndCutscene(BgKin2Bombwall*);
void BgKin2Bombwall_EndCutscene(BgKin2Bombwall*, GlobalContext*);

extern Gfx D_06000128[]; // debris
extern Gfx D_060002C0[]; // wall crack
extern Gfx D_06000360[]; // bombable wall
extern CollisionHeader D_06000490;

ActorInit Bg_Kin2_Bombwall_InitVars = {
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

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 60, 60, 0, { 0, 0, 0 } },
};

Color_RGBA8 primColor = { 210, 210, 210, 255 };
Color_RGBA8 envColor = { 140, 140, 140, 255 };

Vec3f dustAccel = { 0.0f, 0.33f, 0.0f };

s8 D_80B6E730[] = { -60, -34, -8, 18, 44 }; // Used to generate a random vector to modify the dust position vector.
s16 scales[] = { 25, 23, 21, 19, 17, 15, 13, 10 }; // Scales for random explosion debris.

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 300, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

s32 BgKin2Bombwall_IsHitFromNearby(BgKin2Bombwall* this, GlobalContext* globalCtx) {
    Actor* bombwallCollider;

    if (this->collider.base.acFlags & AC_HIT) {
        bombwallCollider = this->collider.base.ac;
        // Distance check required to only react to sufficiently close explosions.
        if ((bombwallCollider != NULL) &&
            (Math3D_Vec3fDistSq(&this->dyna.actor.world.pos, &bombwallCollider->world.pos) < 6400.0f)) {
            return true;
        }
    }
    return false;
}

void BgKin2Bombwall_SpawnEffects(BgKin2Bombwall* this, GlobalContext* globalCtx) {
    s32 i;
    Vec3f pos;
    Vec3f velocity;
    Vec3f spD8;
    Vec3f spCC;
    s32 j;
    s32 k;
    f32 temp_a0;
    s32 pad;
    s16 phi_s0;
    s16 phi_s1;

    Matrix_RotateYS(this->dyna.actor.shape.rot.y, 0);

    for (i = 0, k = 0; i < 6; i++) {
        temp_a0 = (i + 1) * 15.f;
        for (j = 0; j < ARRAY_COUNT(D_80B6E730); j++) {
            k++;
            k &= 7;

            spD8.x = D_80B6E730[j] + (s32)(((u32)Rand_Next()) >> 0x1C);
            spD8.y = ((Rand_ZeroOne() - 0.5f) * 15.0f) + temp_a0;
            spD8.z = (Rand_ZeroOne() * 20.0f) - 10.0f;

            spCC.x = (2.0f * (Rand_ZeroOne() - 0.5f)) + (spD8.x * 0.018461538f);
            spCC.y = (Rand_ZeroOne() * 7.0f) + 4.0f;
            spCC.z = spD8.z * 0.3f;

            Matrix_MultVec3f(&spD8, &pos);
            Matrix_MultVec3f(&spCC, &velocity);

            pos.x += this->dyna.actor.world.pos.x;
            pos.y += this->dyna.actor.world.pos.y;
            pos.z += this->dyna.actor.world.pos.z;

            if (Rand_Next() % 4 == 0) {
                phi_s0 = 0x20;
            } else {
                phi_s0 = 0x40;
            }

            if (k < 2 || Rand_Next() > 0) {
                phi_s0 |= 1;
                phi_s1 = 1;
                func_800B0E48(globalCtx, &pos, &gZeroVec3f, &dustAccel, &primColor, &envColor,
                              (((u32)Rand_Next() >> 0x1B) + 70),
                              ((((u32)Rand_Next()) >> 0x1A) + 60)); // for dust spawn
            } else {
                phi_s1 = 0;
            }
            EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, -550, phi_s0, 30, 0, 0, scales[k], phi_s1, 0, 50, -1,
                                 OBJECT_KIN2_OBJ, D_06000128);
        }
    }
}

void BgKin2Bombwall_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgKin2Bombwall* this = THIS;
    ColliderCylinder* bombwallCollider;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, 0);
    bombwallCollider = &this->collider;
    Collider_InitCylinder(globalCtx, bombwallCollider);
    if (Flags_GetSwitch(globalCtx, BG_KIN2_BOMBWALL_SWITCH_FLAG(this))) {
        Actor_MarkForDeath(&this->dyna.actor);
        return;
    }
    DynaPolyActor_LoadMesh(globalCtx, &this->dyna, &D_06000490);
    Collider_SetCylinder(globalCtx, bombwallCollider, &this->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->dyna.actor, bombwallCollider);
    Actor_SetFocus(&this->dyna.actor, 60.0f);
    BgKin2Bombwall_SetupWait(this);
}

void BgKin2Bombwall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgKin2Bombwall* this = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &this->collider);
}

void BgKin2Bombwall_SetupWait(BgKin2Bombwall* this) {
    this->actionFunc = BgKin2Bombwall_Wait;
}

void BgKin2Bombwall_Wait(BgKin2Bombwall* this, GlobalContext* globalCtx) {
    if (BgKin2Bombwall_IsHitFromNearby(this, globalCtx)) { // checks if AC collision happened
        this->collider.base.acFlags &= ~AC_HIT;
        ActorCutscene_SetIntentToPlay(this->dyna.actor.cutscene);
        BgKin2Bombwall_SetupPlayCutscene(this);
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }
}

void BgKin2Bombwall_SetupPlayCutscene(BgKin2Bombwall* this) {
    this->actionFunc = BgKin2Bombwall_PlayCutscene;
}

void BgKin2Bombwall_PlayCutscene(BgKin2Bombwall* this, GlobalContext* globalCtx) {
    if (ActorCutscene_GetCanPlayNext(this->dyna.actor.cutscene)) {
        ActorCutscene_StartAndSetUnkLinkFields(-1, &this->dyna.actor);
        //ActorCutscene_StartAndSetUnkLinkFields(this->dyna.actor.cutscene, &this->dyna.actor);
        Flags_SetSwitch(globalCtx, BG_KIN2_BOMBWALL_SWITCH_FLAG(this));
        SoundSource_PlaySfxAtFixedWorldPos(globalCtx, &this->dyna.actor.world.pos, 0x3C, NA_SE_EV_WALL_BROKEN);
        func_800C62BC(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
        this->dyna.actor.draw = NULL;
        BgKin2Bombwall_SpawnEffects(this, globalCtx);
        BgKin2Bombwall_SetupEndCutscene(this);

    } else {
        ActorCutscene_SetIntentToPlay(this->dyna.actor.cutscene);
    }
}

void BgKin2Bombwall_SetupEndCutscene(BgKin2Bombwall* this) {
    this->timer = 0x28;
    this->actionFunc = BgKin2Bombwall_EndCutscene;
}

void BgKin2Bombwall_EndCutscene(BgKin2Bombwall* this, GlobalContext* globalCtx) {
    this->timer--;
    if (this->timer <= 0) {
        ActorCutscene_Stop(this->dyna.actor.cutscene);
        Actor_MarkForDeath(&this->dyna.actor);
    }
}

void BgKin2Bombwall_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgKin2Bombwall* this = THIS;

    this->actionFunc(this, globalCtx);
}

void BgKin2Bombwall_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgKin2Bombwall* this = THIS;

    Gfx_DrawDListOpa(globalCtx, D_06000360);
    Gfx_DrawDListXlu(globalCtx, D_060002C0);
}

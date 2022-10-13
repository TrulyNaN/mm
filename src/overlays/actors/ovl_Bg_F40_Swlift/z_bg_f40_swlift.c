/*
 * File: z_bg_f40_swlift.c
 * Overlay: ovl_Bg_F40_Swlift
 * Description: Unused Stone Tower vertically-oscillating platform
 */

// authors to add: Darkeye

#include "z_bg_f40_swlift.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgF40Swlift*)thisx)

void BgF40Swlift_Init(Actor* thisx, PlayState* play);
void BgF40Swlift_Destroy(Actor* thisx, PlayState* play);
void BgF40Swlift_Update(Actor* thisx, PlayState* play);
void BgF40Swlift_Draw(Actor* thisx, PlayState* play);

static s32 D_8096F510[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
static s32 D_8096F5D0[4];

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

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 550, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 5000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

extern Gfx D_06003B08;
extern CollisionHeader D_06003E80;

void BgF40Swlift_Init(Actor* thisx, PlayState* play) {
    s32 temp_v1;
    BgF40Swlift* this = THIS;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    DynaPolyActor_Init((DynaPolyActor*)this, 1);
    temp_v1 = ((s32)this->actor.params >> 8) & 0xFF;
    if ((temp_v1 < 0) || (temp_v1 >= 5)) {
        Actor_MarkForDeath(&this->actor);
    } else {
        D_8096F5D0[temp_v1] = (s32)this->actor.world.pos.y;
        D_8096F510[temp_v1] = this->actor.params & 0xFF;
        if (temp_v1) {
            Actor_MarkForDeath(&this->actor);
        } else {
            DynaPolyActor_LoadMesh(play, (DynaPolyActor*)this, &D_06003E80);
            this->actor.params = 0;
        }
    }
}

void BgF40Swlift_Destroy(Actor* thisx, PlayState* play) {
    BgF40Swlift* this = THIS;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->bgId);
}

// from petrie : it checks for one of the three slots to be either 0xFF or have its switch off
// from petrie : if one of those three slots is, then it bobs up and down at height D_8096F5D0[i] with a range of +/- 5

#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

void BgF40Swlift_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    BgF40Swlift* this = THIS;
    s32 i;

    for (i = 1; i < ARRAY_COUNT(D_8096F510); i++) {
        if ((D_8096F510[i] == 0xFF) || (Flags_GetSwitch(play, D_8096F510[i]) == 0)) {
            break;
        }
    }

    i--;
    if (i != this->actor.params) {
        f32 phi_fv1;

        this->actor.params = -1;
        phi_fv1 = (((f32)D_8096F5D0[i]) - this->actor.world.pos.y) * 0.1f;
        if (phi_fv1 > 0.0f) {
            phi_fv1 = CLAMP(phi_fv1, 0.5f, 15.0f);
        } else {
            phi_fv1 = CLAMP(phi_fv1, -15.0f, -0.5f);
        }

        if ((Math_StepToF(&this->actor.speedXZ, phi_fv1, 1.0f) != 0) && (fabsf(phi_fv1) <= 0.5f)) {
            if (Math_StepToF(&this->actor.world.pos.y, (f32)D_8096F5D0[i], fabsf(this->actor.speedXZ)) != 0) {
                this->actor.params = i;
                this->unk15C = 0x30;
                this->actor.speedXZ = 0.0f;
            }
        } else {
            this->actor.world.pos.y += this->actor.speedXZ;
        }
    } else {
        if (this->unk15C == 0) {
            this->unk15C = 0x30;
        }
        this->unk15C--;
        this->actor.world.pos.y =
            D_8096F5D0[this->actor.params] + (sin_rad(((f32)this->unk15C) * (M_PI / 24.0f)) * 5.0f);
    }
}

void BgF40Swlift_Draw(Actor* thisx, PlayState* play) {
    BgF40Swlift* this = THIS;
    Gfx_DrawDListOpa(play, &D_06003B08); //gStoneTowerUnusedVerticallyOscillatingPlatform?
}

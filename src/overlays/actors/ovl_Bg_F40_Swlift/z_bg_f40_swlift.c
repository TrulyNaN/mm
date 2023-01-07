/*
 * File: z_bg_f40_swlift.c
 * Overlay: ovl_Bg_F40_Swlift
 * Description: Unused Stone Tower vertically-oscillating platform
 */

// authors to add: Darkeye

#include "z_bg_f40_swlift.h"
#include "objects/object_f40_obj/object_f40_obj.h"

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

void BgF40Swlift_Init(Actor* thisx, PlayState* play) {
    s32 index;
    BgF40Swlift* this = THIS;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init((DynaPolyActor*)this, 1);
    index = ((s32)this->dyna.actor.params >> 8) & 0xFF;
    if ((index < 0) || (index >= 5)) { //! @bug An index greater than 3 will cause an out of bounds array access.
        Actor_MarkForDeath(&this->dyna.actor);
    } else {
        D_8096F5D0[index] = this->dyna.actor.world.pos.y;
        D_8096F510[index] = BG_F40_SWLIFT_GET_HEIGHT_INDEX(this);
        if (index) {
            Actor_MarkForDeath(&this->dyna.actor);
        } else {
            DynaPolyActor_LoadMesh(play, &this->dyna, &gUnusedStoneTowerVerticallyOscillatingPlatformCol);
            this->dyna.actor.params = 0;
        }
    }
}

void BgF40Swlift_Destroy(Actor* thisx, PlayState* play) {
    BgF40Swlift* this = THIS;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
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
    if (i != this->dyna.actor.params) {
        f32 phi_fv1;

        this->dyna.actor.params = -1;
        phi_fv1 = (((f32)D_8096F5D0[i]) - this->dyna.actor.world.pos.y) * 0.1f;
        if (phi_fv1 > 0.0f) {
            phi_fv1 = CLAMP(phi_fv1, 0.5f, 15.0f);
        } else {
            phi_fv1 = CLAMP(phi_fv1, -15.0f, -0.5f);
        }

        if ((Math_StepToF(&this->dyna.actor.speedXZ, phi_fv1, 1.0f) != 0) && (fabsf(phi_fv1) <= 0.5f)) {
            if (Math_StepToF(&this->dyna.actor.world.pos.y, (f32)D_8096F5D0[i], fabsf(this->dyna.actor.speedXZ)) != 0) {
                this->dyna.actor.params = i;
                this->timer = 48;
                this->dyna.actor.speedXZ = 0.0f;
            }
        } else {
            this->dyna.actor.world.pos.y += this->dyna.actor.speedXZ;
        }
    } else {
        if (this->timer == 0) {
            this->timer = 48;
        }
        this->timer--;
        this->dyna.actor.world.pos.y =
            D_8096F5D0[this->dyna.actor.params] + (sin_rad(((f32)this->timer) * (M_PI / 24.0f)) * 5.0f);
    }
}

void BgF40Swlift_Draw(Actor* thisx, PlayState* play) {
    BgF40Swlift* this = THIS;
    Gfx_DrawDListOpa(play, gUnusedStoneTowerVerticallyOscillatingPlatformDL);
}

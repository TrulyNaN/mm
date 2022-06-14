#include "z_obj_raillift.h"
#include "objects/object_raillift/object_raillift.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjRaillift*)thisx)

void ObjRaillift_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjRaillift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjRaillift_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjRaillift_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjRaillift_DrawDekuFlowerPlatformColorful(Actor* thisx, GlobalContext* globalCtx);
void ObjRaillift_DrawDekuFlowerPlatform(Actor* thisx, GlobalContext* globalCtx);

void ObjRaillift_DoNothing(ObjRaillift* this, GlobalContext* globalCtx);
void ObjRaillift_Idle(ObjRaillift* this, GlobalContext* globalCtx);
void ObjRaillift_UpdatePosition(ObjRaillift* this, s32 arg1);
void ObjRaillift_StartCutscene(ObjRaillift* this, GlobalContext* globalCtx);
void ObjRaillift_Teleport(ObjRaillift* this, GlobalContext* globalCtx);
void ObjRaillift_Wait(ObjRaillift* this, GlobalContext* globalCtx);
void ObjRaillift_Move(ObjRaillift* this, GlobalContext* globalCtx);

const ActorInit Obj_Raillift_InitVars = {
    ACTOR_OBJ_RAILLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_RAILLIFT,
    sizeof(ObjRaillift),
    (ActorFunc)ObjRaillift_Init,
    (ActorFunc)ObjRaillift_Destroy,
    (ActorFunc)ObjRaillift_Update,
    (ActorFunc)ObjRaillift_Draw,
};
*/

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 400, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static CollisionHeader* sColHeaders[] = { &object_raillift_Colheader_004FF8, &object_raillift_Colheader_0048D0 };

void ObjRaillift_UpdatePosition(ObjRaillift* this, s32 idx) {
    Math_Vec3s_ToVec3f(&this->dyna.actor.world.pos, &this->points[idx]);
}

void ObjRaillift_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjRaillift* this = THIS;
    s32 pad;
    Path* path;
    s32 type = OBJRAILLIFT_GET_TYPE(thisx);
    s32 isColorful = false;

    Actor_ProcessInitChain(thisx, sInitChain);

    thisx->shape.rot.x = 0;
    thisx->world.rot.x = 0;
    thisx->shape.rot.z = 0;
    thisx->world.rot.z = 0;
    DynaPolyActor_Init(&this->dyna, 1);
    DynaPolyActor_LoadMesh(globalCtx, &this->dyna, sColHeaders[type]);
    this->speed = OBJRAILLIFT_GET_SPEED(thisx);
    if (this->speed < 0.0f) {
        this->speed = -this->speed;
        isColorful = true;
    }
    if (type == DEKU_FLOWER_PLATFORM) {
        Actor_SpawnAsChild(&globalCtx->actorCtx, thisx, globalCtx, ACTOR_OBJ_ETCETERA, thisx->world.pos.x,
                           thisx->world.pos.y, thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y,
                           thisx->shape.rot.z, 0);
        if (isColorful) {
            thisx->draw = ObjRaillift_DrawDekuFlowerPlatformColorful;
        } else {
            thisx->draw = ObjRaillift_DrawDekuFlowerPlatform;
        }
    }
    if (this->speed < 0.01f) {
        this->actionFunc = ObjRaillift_DoNothing;
    } else {
        path = &globalCtx->setupPathList[OBJRAILLIFT_GET_PATH(thisx)];
        this->curPoint = OBJRAILLIFT_GET_STARTING_POINT(thisx);
        this->endPoint = path->count - 1;
        this->direction = 1;
        this->points = Lib_SegmentedToVirtual(path->points);
        ObjRaillift_UpdatePosition(this, this->curPoint);
        if (OBJRAILLIFT_HAS_FLAG(thisx) && !Flags_GetSwitch(globalCtx, OBJRAILLIFT_GET_FLAG(thisx))) {
            this->actionFunc = ObjRaillift_Idle;
        } else {
            this->actionFunc = ObjRaillift_Move;
        }
    }
}

void ObjRaillift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjRaillift* this = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
}

void ObjRaillift_DoNothing(ObjRaillift* this, GlobalContext* globalCtx) {
}

void ObjRaillift_Move(ObjRaillift* this, GlobalContext* globalCtx) {
    s32 isTeleporting;
    Vec3f nextPoint;
    f32 speed;
    f32 target;
    f32 step;
    s32 isPosUpdated;
    Vec3s* initialPoint;
    Vec3s* endPoint;
    s32 pad;

    if (OBJRAILLIFT_HAS_FLAG(&this->dyna.actor)) {
        if (!Flags_GetSwitch(globalCtx, OBJRAILLIFT_GET_FLAG(&this->dyna.actor))) {
            this->actionFunc = ObjRaillift_Idle;
            return;
        }

        if (OBJRAILLIFT_GET_TYPE(&this->dyna.actor) == DEKU_FLOWER_PLATFORM) {
            func_800B9010(&this->dyna.actor, NA_SE_EV_PLATE_LIFT_LEVEL - SFX_FLAG);
        }
    }

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/ObjRaillift_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/ObjRaillift_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A19B98.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A19BA8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A19E84.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A19EE0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A19F18.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A19F78.asm")

    this->actionFunc(this, globalCtx);
    Actor_SetFocus(&this->dyna.actor, 10.0f);
    if (this->cutsceneTimer > 0) {
        this->cutsceneTimer--;
        if (this->cutsceneTimer == 0) {
            ActorCutscene_Stop(this->dyna.actor.cutscene);
        }
    }
    if (OBJRAILLIFT_SHOULD_REACT_TO_WEIGHT(thisx)) {
        s32 requiredScopeTemp;

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/ObjRaillift_Draw.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Raillift_0x80A19910/func_80A1A330.asm")

    OPEN_DISPS(globalCtx->state.gfxCtx);
    func_8012C28C(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, globalCtx->gameplayFrames, 0, 32, 32, 1, 0, 0, 32,
                                        32, 0, 0, 0, 160));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_raillift_DL_004BF0);
    CLOSE_DISPS(globalCtx->state.gfxCtx);
}

/*
The non-colorful platforms are the ones found in Woodfall Temple
*/
void ObjRaillift_DrawDekuFlowerPlatform(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, object_raillift_DL_000208);
}

/*
The colorful platforms are the ones found in Deku Palace
*/
void ObjRaillift_DrawDekuFlowerPlatformColorful(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, object_raillift_DL_0071B8);
}

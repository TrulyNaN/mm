#include "z_obj_ending.h"
#include "objects/object_ending_obj/object_ending_obj.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((ObjEnding*)thisx)

void ObjEnding_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjEnding_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjEnding_Draw(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit Obj_Ending_InitVars = {
    ACTOR_OBJ_ENDING,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_ENDING_OBJ,
    sizeof(ObjEnding),
    (ActorFunc)ObjEnding_Init,
    (ActorFunc)func_800BDFB0,
    (ActorFunc)ObjEnding_Update,
    (ActorFunc)ObjEnding_Draw,
};
*/

static ObjEndingModelInfo sModelInfo[] = {
    { { object_ending_obj_DL_003440, object_ending_obj_DL_0031A0 }, NULL },
    { { NULL, object_ending_obj_DL_0003D0 }, object_ending_obj_Matanimheader_001FF8 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};
*/

extern unkStruct D_80C25CE0[];
/*
static unkStruct D_80C25CE0[2] = {
    {0x06003440, 0x060031A0, 0x00000000},
    {0x00000000, 0x060003D0, 0x06001FF8}
};
*/

void ObjEnding_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjEnding* this = THIS;
    AnimatedMaterial* texture;

    Actor_ProcessInitChain(&this->actor, D_80C25CF8);
    this->unk144 = &D_80C25CE0[this->actor.params];
    if (false) {}
    texture = this->unk144->texture;

    Actor_ProcessInitChain(thisx, sInitChain);
    this->modelInfo = &sModelInfo[thisx->params];
    animMat = this->modelInfo->animMat;
    if (animMat != NULL) {
        this->animMat = Lib_SegmentedToVirtual(animMat);
    }
}

void ObjEnding_Update(Actor* thisx, GlobalContext* globalCtx) {
}

void ObjEnding_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ObjEnding* this = THIS;
    Gfx* dl;
    Gfx* tempunk4;

    if (this->texture != NULL) {
        AnimatedMat_Draw(globalCtx, this->texture);
    }
    dl1 = this->modelInfo->dLists[0];
    if (dl1 != NULL) {
        Gfx_DrawDListOpa(globalCtx, dl1);
    }
    dl2 = this->modelInfo->dLists[1];
    if (dl2 != NULL) {
        Gfx_DrawDListXlu(globalCtx, dl2);
    }
}

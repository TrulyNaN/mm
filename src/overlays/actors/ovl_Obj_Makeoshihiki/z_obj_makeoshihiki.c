#include "z_obj_makeoshihiki.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjMakeoshihiki*)thisx)

void ObjMakeoshihiki_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjMakeoshihiki_Update(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit Obj_Makeoshihiki_InitVars = {
    ACTOR_OBJ_MAKEOSHIHIKI,
    ACTORCAT_PROP,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ObjMakeoshihiki),
    (ActorFunc)ObjMakeoshihiki_Init,
    (ActorFunc)func_800BDFB0,
    (ActorFunc)ObjMakeoshihiki_Update,
    (ActorFunc)NULL,
};
*/

s32 ObjMakeoshihiki_GetChildSpawnPointIndex(ObjMakeoshihiki* this, GlobalContext* globalCtx) {
    s32 pad;
    s32 pathIndexOffset1 = Flags_GetSwitch(globalCtx, OBJMAKEOSHIHIKI_GET_SWITCHFLAG_1(this)) ? 1 : 0;
    s32 pathIndexOffset2 = Flags_GetSwitch(globalCtx, OBJMAKEOSHIHIKI_GET_SWITCHFLAG_2(this)) ? 2 : 0;

    return pathIndexOffset1 + pathIndexOffset2;
}

void ObjMakeoshihiki_SetSwitchFlags(ObjMakeoshihiki* this, GlobalContext* globalCtx, s32 pathIndex) {
    s32 pad;
    s32 pad2;
    s32 switchFlag1;
    s32 switchFlag2;

    switchFlag2 = OBJMAKEOSHIHIKI_GET_SWITCHFLAG_2(this);
    switchFlag1 = OBJMAKEOSHIHIKI_GET_SWITCHFLAG_1(this);

    if (pathIndex & 2) {
        Flags_SetSwitch(globalCtx, switchFlag2);
    } else {
        Flags_UnsetSwitch(globalCtx, switchFlag2);
    }

    if (pathIndex & 1) {
        Flags_SetSwitch(globalCtx, switchFlag1);
    } else {
        Flags_UnsetSwitch(globalCtx, switchFlag1);
    }
}

void ObjMakeoshihiki_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjMakeoshihiki* this = THIS;
    Vec3s* childPoint;
    Path* path;
    s32 childPointIndex;

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Makeoshihiki_0x80972350/func_809723C4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Makeoshihiki_0x80972350/ObjMakeoshihiki_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Makeoshihiki_0x80972350/ObjMakeoshihiki_Update.asm")

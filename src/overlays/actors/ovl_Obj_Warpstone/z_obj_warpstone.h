#ifndef Z_OBJ_WARPSTONE_H
#define Z_OBJ_WARPSTONE_H

#include <global.h>

struct ObjWarpstone;

typedef struct ObjWarpstone {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x6C];
} ObjWarpstone; // size = 0x1B0

extern const ActorInit Obj_Warpstone_InitVars;

#define OBJ_WARPSTONE_GET_ID(this) ((u16)(this->dyna.actor.params & 0xF))
#define OBJ_WARPSTONE_IS_ACTIVATED(owlId) (((void)0, gSaveContext.save.playerData.owlActivationFlags) & (u16)gBitFlags[owlId])

#endif // Z_OBJ_WARPSTONE_H

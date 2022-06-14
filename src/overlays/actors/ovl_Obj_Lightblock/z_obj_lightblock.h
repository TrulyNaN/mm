#ifndef Z_OBJ_LIGHTBLOCK_H
#define Z_OBJ_LIGHTBLOCK_H

#include <global.h>

struct ObjLightblock;

typedef struct ObjLightblock {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x6C];
} ObjLightblock; // size = 0x1B0

extern const ActorInit Obj_Lightblock_InitVars;

#endif // Z_OBJ_LIGHTBLOCK_H

#ifndef Z_OBJ_PZLBLOCK_H
#define Z_OBJ_PZLBLOCK_H

#include <global.h>

struct ObjPzlblock;

typedef struct ObjPzlblock {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x38];
} ObjPzlblock; // size = 0x17C

extern const ActorInit Obj_Pzlblock_InitVars;

#endif // Z_OBJ_PZLBLOCK_H

#ifndef Z_OBJ_AQUA_H
#define Z_OBJ_AQUA_H

#include "global.h"

#define AQUA_HOT(thisx) ((thisx)->params & 1)
#define OBJAQUA_1 1

struct ObjAqua;

typedef struct ObjAqua {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x58];
} ObjAqua; // size = 0x19C

extern const ActorInit Obj_Aqua_InitVars;

#endif // Z_OBJ_AQUA_H

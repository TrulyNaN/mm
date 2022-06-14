#ifndef Z_OBJ_MURE2_H
#define Z_OBJ_MURE2_H

#include <global.h>

struct ObjMure2;

typedef struct ObjMure2 {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ ObjMure2ActionFunc actionFunc;
    /* 0x0148 */ char unk_148[0x38];
} ObjMure2; // size = 0x180

extern const ActorInit Obj_Mure2_InitVars;

#endif // Z_OBJ_MURE2_H

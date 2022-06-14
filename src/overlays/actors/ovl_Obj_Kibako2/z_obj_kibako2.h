#ifndef Z_OBJ_KIBAKO2_H
#define Z_OBJ_KIBAKO2_H

#include <global.h>

struct ObjKibako2;

typedef struct ObjKibako2 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderCylinder collider;
    /* 0x1A8 */ ObjKibako2ActionFunc actionFunc;
    /* 0x1AC */ s8 unk_1AC;
    /* 0x1AD */ s8 skulltulaNoiseTimer;
} ObjKibako2; // size = 0x1B0

extern const ActorInit Obj_Kibako2_InitVars;

#endif // Z_OBJ_KIBAKO2_H

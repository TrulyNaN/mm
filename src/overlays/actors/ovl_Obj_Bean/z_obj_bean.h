#ifndef Z_OBJ_BEAN_H
#define Z_OBJ_BEAN_H

#include <global.h>

struct ObjBean;

typedef struct ObjBean {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0xC0];
} ObjBean; // size = 0x204

extern const ActorInit Obj_Bean_InitVars;

#endif // Z_OBJ_BEAN_H

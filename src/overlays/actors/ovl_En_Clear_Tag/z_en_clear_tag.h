#ifndef Z_EN_CLEAR_TAG_H
#define Z_EN_CLEAR_TAG_H

#include <global.h>

struct EnClearTag;

typedef struct EnClearTag {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ EnClearTagEffect effect[103];
    /* 0x2E54 */ u8 cameraState;
    /* 0x2E56 */ s16 activeTimer; // Actor Marked for Death when timer runs out
    /* 0x2E58 */ UNK_TYPE1 unk2E58[0xC];
    /* 0x2E64 */ s16 subCamId;
    /* 0x2E66 */ Color_RGBA8 flashEnvColor;
    /* 0x2E6C */ Vec3f subCamEye;
    /* 0x2E78 */ Vec3f subCamAt;
} EnClearTag; // size = 0x2E84

extern const ActorInit En_Clear_Tag_InitVars;

#endif // Z_EN_CLEAR_TAG_H

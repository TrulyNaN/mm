#ifndef Z_EN_CLEAR_TAG_H
#define Z_EN_CLEAR_TAG_H

#include <global.h>

struct EnClearTag;

typedef struct EnClearTag {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ char unk_0144[0x2D40];
} EnClearTag; // size = 0x2E84

extern const ActorInit En_Clear_Tag_InitVars;

#endif // Z_EN_CLEAR_TAG_H

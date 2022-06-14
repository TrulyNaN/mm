#ifndef Z_DM_CHAR05_H
#define Z_DM_CHAR05_H

#include <global.h>

struct DmChar05;

typedef struct DmChar05 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x74];
} DmChar05; // size = 0x1B8

extern const ActorInit Dm_Char05_InitVars;

#endif // Z_DM_CHAR05_H

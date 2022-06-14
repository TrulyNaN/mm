#ifndef Z_EN_PO_COMPOSER_H
#define Z_EN_PO_COMPOSER_H

#include <global.h>

struct EnPoComposer;

typedef struct EnPoComposer {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x1FC];
} EnPoComposer; // size = 0x340

extern const ActorInit En_Po_Composer_InitVars;

#endif // Z_EN_PO_COMPOSER_H

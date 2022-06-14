#ifndef Z_EN_GAMELUPY_H
#define Z_EN_GAMELUPY_H

#include <global.h>

struct EnGamelupy;

typedef struct EnGamelupy {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x60];
} EnGamelupy; // size = 0x1A4

extern const ActorInit En_Gamelupy_InitVars;

#endif // Z_EN_GAMELUPY_H

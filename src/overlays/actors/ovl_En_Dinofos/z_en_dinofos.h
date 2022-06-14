#ifndef Z_EN_DINOFOS_H
#define Z_EN_DINOFOS_H

#include <global.h>

struct EnDinofos;

typedef struct EnDinofos {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x500];
} EnDinofos; // size = 0x644

extern const ActorInit En_Dinofos_InitVars;

#endif // Z_EN_DINOFOS_H

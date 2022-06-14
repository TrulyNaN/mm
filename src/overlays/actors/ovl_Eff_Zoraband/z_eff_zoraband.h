#ifndef Z_EFF_ZORABAND_H
#define Z_EFF_ZORABAND_H

#include <global.h>

struct EffZoraband;

typedef struct EffZoraband {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x8];
} EffZoraband; // size = 0x14C

extern const ActorInit Eff_Zoraband_InitVars;

#endif // Z_EFF_ZORABAND_H

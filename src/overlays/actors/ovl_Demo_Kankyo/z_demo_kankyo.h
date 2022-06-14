#ifndef Z_DEMO_KANKYO_H
#define Z_DEMO_KANKYO_H

#include <global.h>

struct DemoKankyo;

typedef struct DemoKankyo {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ char unk_0144[0x150C];
} DemoKankyo; // size = 0x1650

extern const ActorInit Demo_Kankyo_InitVars;

#endif // Z_DEMO_KANKYO_H

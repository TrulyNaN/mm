#ifndef Z_EN_GO_H
#define Z_EN_GO_H

#include <global.h>

struct EnGo;

typedef struct EnGo {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0xA34];
} EnGo; // size = 0xB78

extern const ActorInit En_Go_InitVars;

#endif // Z_EN_GO_H

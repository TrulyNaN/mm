#ifndef Z_EN_PR_H
#define Z_EN_PR_H

#include <global.h>

struct EnPr;

typedef struct EnPr {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x204];
} EnPr; // size = 0x348

extern const ActorInit En_Pr_InitVars;

#endif // Z_EN_PR_H

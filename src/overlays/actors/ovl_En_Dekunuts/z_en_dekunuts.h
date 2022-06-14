#ifndef Z_EN_DEKUNUTS_H
#define Z_EN_DEKUNUTS_H

#include "global.h"
#include "objects/object_dekunuts/object_dekunuts.h"

struct EnDekunuts;

typedef struct EnDekunuts {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnDekunutsActionFunc actionFunc;
    /* 0x18C */ u8 unk_18C;
    /* 0x18D */ u8 unk_18D;
    /* 0x18E */ u8 drawDmgEffType;
    /* 0x190 */ s16 unk_190;
    /* 0x192 */ s16 unk_192;
    /* 0x194 */ s16 unk_194;
    /* 0x196 */ Vec3s jointTable[DEKU_SCRUB_LIMB_MAX];
    /* 0x1D2 */ Vec3s morphTable[DEKU_SCRUB_LIMB_MAX];
    /* 0x210 */ f32 drawDmgEffAlpha;
    /* 0x214 */ f32 drawDmgEffScale;
    /* 0x218 */ f32 drawDmgEffFrozenSteamScale;
    /* 0x21C */ Vec3f limbPos[8];
    /* 0x27C */ ColliderCylinder collider;
} EnDekunuts; // size = 0x2C8

extern const ActorInit En_Dekunuts_InitVars;

#endif // Z_EN_DEKUNUTS_H

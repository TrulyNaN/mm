#ifndef Z_EN_ELFORG_H
#define Z_EN_ELFORG_H

#include <global.h>

typedef enum {
    /*  0 */ STRAY_FAIRY_LIMB_NONE,
    /*  1 */ STRAY_FAIRY_LIMB_RIGHT_FACING_HEAD,
    /*  2 */ STRAY_FAIRY_LIMB_LEFT_WING,
    /*  3 */ STRAY_FAIRY_LIMB_RIGHT_WING,
    /*  4 */ STRAY_FAIRY_LIMB_GLOW,
    /*  5 */ STRAY_FAIRY_LIMB_TORSO,
    /*  6 */ STRAY_FAIRY_LIMB_RIGHT_ARM,
    /*  7 */ STRAY_FAIRY_LIMB_PELVIS_AND_LEGS,
    /*  8 */ STRAY_FAIRY_LIMB_LEFT_ARM,
    /*  9 */ STRAY_FAIRY_LIMB_LEFT_FACING_HEAD,
    /* 10 */ STRAY_FAIRY_LIMB_MAX,
} StrayFairyLimbs;

struct EnElforg;

typedef struct EnElforg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[STRAY_FAIRY_LIMB_MAX];
    /* 0x1C4 */ ColliderCylinder collider;
    /* 0x210 */ Actor* enemy;
    /* 0x214 */ u16 flags;
    /* 0x216 */ s16 direction; // negative when facing right, positive when facing left
    /* 0x218 */ s16 area;
    /* 0x21C */ s32 timer;
    /* 0x220 */ s32 secondaryTimer;
    /* 0x224 */ f32 targetSpeedXZ;
    /* 0x228 */ f32 targetDistanceFromHome;
    /* 0x22C */ EnElforgActionFunc actionFunc;
} EnElforg; // size = 0x230

extern const ActorInit En_Elforg_InitVars;

#endif // Z_EN_ELFORG_H

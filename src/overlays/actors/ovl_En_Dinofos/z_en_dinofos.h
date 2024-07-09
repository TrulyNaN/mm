#ifndef Z_EN_DINOFOS_H
#define Z_EN_DINOFOS_H

#include "global.h"
#include "objects/object_dinofos/object_dinofos.h"

struct EnDinofos;

typedef void (*EnDinofosActionFunc)(struct EnDinofos*, PlayState*);

typedef enum EnDinofosBodyPart {
    /*  0 */ DINOFOS_BODYPART_LEFT_UPPER_LEG,
    /*  1 */ DINOFOS_BODYPART_LEFT_CLAWS,
    /*  2 */ DINOFOS_BODYPART_RIGHT_UPPER_LEG,
    /*  3 */ DINOFOS_BODYPART_RIGHT_CLAWS,
    /*  4 */ DINOFOS_BODYPART_UPPER_BODY,
    /*  5 */ DINOFOS_BODYPART_LEFT_FOREARM,
    /*  6 */ DINOFOS_BODYPART_LEFT_HAND,
    /*  7 */ DINOFOS_BODYPART_RIGHT_FOREARM,
    /*  8 */ DINOFOS_BODYPART_RIGHT_HAND,
    /*  9 */ DINOFOS_BODYPART_HEAD,
    /* 10 */ DINOFOS_BODYPART_JAW,
    /* 11 */ DINOFOS_BODYPART_LOWER_TAIL,
    /* 12 */ DINOFOS_BODYPART_MAX
} EnDinofosBodyPart;

typedef struct EnDinofos {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[DINOLFOS_LIMB_MAX];
    /* 0x206 */ Vec3s morphTable[DINOLFOS_LIMB_MAX];
    /* 0x284 */ EnDinofosActionFunc actionFunc;
    /* 0x288 */ u8 envColorAlpha;
    /* 0x289 */ u8 eyeTexIndex;
    /* 0x28A */ u8 drawDmgEffType;
    /* 0x28B */ u8 isDodgingGoronPound; //able to dodge Goron Pound bool
    /* 0x28C */ s16 targetRotY;
    /* 0x28E */ s16 headRotY;
    /* 0x290 */ s16 unk_290; //many timers. (choose action timer) certain(cry for start cutscene-, breathe fire-, stunned-, slash+). animationTimer?
    /* 0x292 */ s16 unk_292; //will pick some random action when 292 hits 0. idle- timer? jump forward timer
    /* 0x294 */ UNK_TYPE1 unk_294[4];
    /* 0x298 */ s16 subCamId;
    /* 0x29A */ Vec3s unk_29A;
    /* 0x2A0 */ s32 effectIndex;
    /* 0x2A4 */ f32 unk_2A4;
    /* 0x2A8 */ f32 unk_2A8;
    /* 0x2AC */ f32 unk_2AC;
    /* 0x2B0 */ f32 drawDmgEffAlpha;
    /* 0x2B4 */ f32 drawDmgEffScale;
    /* 0x2B8 */ f32 drawDmgEffFrozenSteamScale;
    /* 0x2BC */ Vec3f unk_2BC; //if like oot, rightFootPos
    /* 0x2C8 */ Vec3f unk_2C8; //if like oot, leftFootPos
    /* 0x2D4 */ Vec3f bodyPartsPos[DINOFOS_BODYPART_MAX];
    /* 0x364 */ ColliderJntSph colliderJntSph;
    /* 0x384 */ ColliderJntSphElement colliderJntSphElement[9];
    /* 0x5C4 */ ColliderQuad colliderQuad;
} EnDinofos; // size = 0x644

#endif // Z_EN_DINOFOS_H

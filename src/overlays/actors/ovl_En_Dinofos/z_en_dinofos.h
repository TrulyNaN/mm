#ifndef Z_EN_DINOFOS_H
#define Z_EN_DINOFOS_H

#include "global.h"
#include "objects/object_dinofos/object_dinofos.h"

struct EnDinofos;

#define DINOFOS_HEALTH 40 //didn't beat it yet with 40 health. This is so fun.

#define DINOFOS_PROB_BLINK 0.05f
#define DINOFOS_PROB_COMPL_CHOOSE_ACTION_NOT_SLASHING_TO_SLASHING 0.1f
#define DINOFOS_PROB_COMPL_CHOOSE_ACTION_SLASHING_TO_SLASHING 0.4f
#define DINOFOS_PROB_CHOOSE_ACTION_NOT_LANDING_TO_JUMP_FORWARD 0.15f
#define DINOFOS_PROB_CHOOSE_ACTION_LANDING_TO_JUMP_FORWARD 0.05f
#define DINOFOS_PROB_CHOOSE_ACTION_TO_TURN_TO_PLAYER 0.9f
#define DINOFOS_PROB_CHOOSE_ACTION_TO_WALK 0.9f
#define DINOFOS_PROB_JUMP_FROM_PLAYER_MELEE_ATTACK 0.85f
#define DINOFOS_PROB_JUMP_BACKWARD_FROM_PLAYER_MELEE_ATTACK 0.35f

#define DINOFOS_PROB_RECOIL_TO_FIRE 0.25f

#define DINOFOS_IDLE_TIMER1_MAX_OFFSET 10.0f
#define DINOFOS_IDLE_TIMER1_MIN 20


#define DINOFOS_WALK_TIMER_MAX_OFFSET 10.0f
#define DINOFOS_WALK_TIMER_MIN 20

#define DINOFOS_PROB_SIDESTEP_LEFT 0.5f

#define DINOFOS_SIDESTEP_TIMER_MAX_OFFSET 5.0f
#define DINOFOS_SIDESTEP_TIMER_MIN 2

#define DINOFOS_PROB_JUMP_BACKWARD_TO_FIRE 0.90f //0.95f for most of testing, trying 0.90f

#define DINOFOS_FREEZE_TIMER 40
#define DINOFOS_IDLE_TIMER 15
#define DINOFOS_FIRE_TIMER 20
#define DINOFOS_DODGE_PROJECTILE_TIMER 10

#define DINOFOS_DIE_FROM_FREEZE_TIMER 3
#define DINOFOS_DMGEFF_STUN_TIMER 20
#define DINOLFOS_DMGEFF_ZORA_MAGIC_TIMER 20

#define DINOFOS_WALK_SPEED_START 7.0f
#define DINOFOS_WALK_SPEED_MAX 14.0f
#define DINOFOS_CIRCLING_SPEED 9.0f

#define DINOFOS_JUMP_BACKWARD_SPEED 8.0f //original: 10f.
#define DINOFOS_JUMP_FORWARD_SPEED 9.0f
#define DINOFOS_SLASH_FROM_GROUND_SPEED 1.75f

#define DINOFOS_RECOIL_SPEED 1.5f
#define DINOFOS_DAMAGED_SPEED 5.0f
#define DINOFOS_DODGE_PROJECTILE_SPEED 15.0f

#define DINOFOS_JUMP_BACKWARD_VELOCITY_Y 9.0f
#define DINOFOS_JUMP_NOT_BACKWARD_VELOCITY_Y 12.5f
#define DINOFOS_JUMP_SLASH_VELOCITY_Y 16.0f

#define DINOFOS_SLASH_ANIM_LAST_FRAME_TO_DODGE 11.0f //original: 7.0f

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

typedef enum EnDinofosJumpType {
    /* 0 */ DINOFOS_JUMP_TYPE_FORWARD,
    /* 1 */ DINOFOS_JUMP_TYPE_IN_PLACE,
    /* 2 */ DINOFOS_JUMP_TYPE_BACKWARD,
    /* 3 */ DINOFOS_JUMP_TYPE_SLASH
} EnDinofosJumpType;

/**
 * unk_294 explanation :
 *
 * The best way to compare is to look from timer1 to circlingRate in this current actor struct and from
 * 0x03F0 to 0x0408 in OoT's struct for Lizalfos/Dinolfos.
 *
 * In OoT, an additionnal ice timer, booleans related to the platforms fight and platforms indices are stored
 * after the two timers EnDinofos still has (all integers). Moreover, the alpha value (envColorAlpha) has been moved up
 * in MM.
 *
 * In MM, subCamId and subCamRot were added to handle the new cutscenes. Also, the effectIndex for blur is new.
 *
 * All in all, unk_294 is probably remnants of the now unnecessary variables mentioned above.
 */

typedef struct EnDinofos {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[DINOLFOS_LIMB_MAX];
    /* 0x206 */ Vec3s morphTable[DINOLFOS_LIMB_MAX];
    /* 0x284 */ EnDinofosActionFunc actionFunc;
    /* 0x288 */ u8 envColorAlpha;
    /* 0x289 */ u8 eyeTexIndex;
    /* 0x28A */ u8 drawDmgEffType;
    /* 0x28B */ u8 isDodgingGoronPound;
    /* 0x28C */ s16 targetRotY;
    /* 0x28E */ s16 headRotY;
    /* 0x290 */ union { // multi-use timer
        s16 timer1;
        s16 cutsceneTimer;
        s16 attackTimer;
        s16 actionTimer;
        s16 headTimer;
        s16 stunTimer;
        s16 jumpType;
        s16 isJumpingBackward;
    };
    /* 0x292 */ union { // second multi-use timer
        s16 timer2;
        s16 sidestepTimer;
        s16 walkTimer;
        s16 idleTimer;
    }; 
    /* 0x294*/ f32 flameMultiplier;
    /* 0x298 */ s16 subCamId;
    /* 0x29A */ Vec3s subCamRot;
    /* 0x2A0 */ s32 effectIndex;
    /* 0x2A4 */ f32 circlingRate;
    /* 0x2A8 */ f32 subCamAtStep;
    /* 0x2AC */ f32 subCamEyeStep;
    /* 0x2B0 */ f32 drawDmgEffAlpha;
    /* 0x2B4 */ f32 drawDmgEffScale;
    /* 0x2B8 */ f32 drawDmgEffFrozenSteamScale;
    /* 0x2BC */ Vec3f subCamEye;
    /* 0x2C8 */ Vec3f subCamAt;
    /* 0x2D4 */ Vec3f bodyPartsPos[DINOFOS_BODYPART_MAX];
    /* 0x364 */ ColliderJntSph colliderJntSph;
    /* 0x384 */ ColliderJntSphElement colliderJntSphElements[9];
    /* 0x5C4 */ ColliderQuad knifeCollider;
} EnDinofos; // size = 0x644

#endif // Z_EN_DINOFOS_H

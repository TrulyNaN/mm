#ifndef Z_EN_BIGSLIME_H
#define Z_EN_BIGSLIME_H

#include <global.h>

struct EnBigslime;

typedef struct EnBigslime {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ SkelAnime skelAnime;
    /* 0x0188 */ EnBigslimeActionFunc actionFunc;
    /* 0x018C */ EnBigslimeActionFunc actionFuncStored;
    /* 0x0190 */ Vec3s jointTable[GEKKO_LIMB_MAX];
    /* 0x0220 */ Vec3s morphTable[GEKKO_LIMB_MAX];
    /* 0x02B0 */ u8 minislimeState;
    /* 0x02B1 */ u8 dynamicVtxState; // Toggles between two states of dynamic Vtx
    /* 0x02B2 */ u8 isAnimUpdate;
    /* 0x02B3 */ union {
                    u8 formBigslimeTimer; // Bigslime will start forming when timer reaches 0
                    u8 minislimeCounter;
                    u8 numGekkoMeleeAttacks; // The Gekko will melee-attack link at 1-3 times at each position while engulfed in bigslime
                };
    /* 0x02B4 */ u8 shockwaveAlpha;
    /* 0x02B5 */ u8 gekkoDrawDmgEffType;
    /* 0x02B6 */ s16 gekkoYaw;
    /* 0x02B8 */ s16 cutscene;
    /* 0x02BA */ union { // multi-use timer
                    s16 idleTimer;
                    s16 noticeTimer;
                    s16 callTimer;
                    s16 jumpTimer;
                    s16 damageSpinTimer;
                    s16 scaleFactor; // Used between individual gekko melee-attacks while engulfed in bigslime
                    s16 windupPunchTimer;
                    s16 throwPlayerTimer;
                    s16 ceilingMoveTimer;
                    s16 squishFlatTimer;
                    s16 riseCounter;
                    s16 grabPlayerTimer;
                    s16 formBigslimeCutsceneTimer;
                    s16 defeatTimer;
                    s16 despawnTimer;
                    s16 spawnFrogTimer;
                    s16 isDespawned;
                    s16 isInitJump;
                };
    /* 0x02BC */ s16 wavySurfaceTimer; // decrements from 24, used in sin_rad for currData2
    /* 0x02BE */ s16 stunTimer;
    /* 0x02C0 */ union {
                    s16 freezeTimer;
                    s16 meltCounter;
                };
    /* 0x02C2 */ s16 ceilingDropTimer; // Bigslime is still during this timer and shakes before dropping
    /* 0x02C4 */ s16 numGekkoPosGrabPlayer; // The Gekko will melee-attack link at 6 positions while engulfed in bigslime
    /* 0x02C6 */ s16 subCamId;
    /* 0x02C8 */ s16 subCamYawGrabPlayer;
    /* 0x02CA */ s16 rotation; // is always 0, used in Matrix_RotateYS
    /* 0x02CC */ s16 itemDropTimer; // items only drop when zero, Set to 40 then decrements, prevents itemDrop spam
    /* 0x02CE */ Vec3s gekkoRot;
    /* 0x02D4 */ Vec3f gekkoPosOffset; // Used when Bigslime grabs link
    /* 0x02E0 */ Vec3f gekkoProjectedPos;
    /* 0x02EC */ union {
                    Vec3f frozenPos;
                    Vec3f subCamDistToFrog; // Used to zoom into frogs as Gekko despawns/Frog spawns
                };
    /* 0x02F8 */ Vec3f limbPos[12];
    /* 0x0388 */ f32 gekkoDrawDmgEffAlpha;
    /* 0x038C */ f32 gekkoDrawDmgEffScale;
    /* 0x0390 */ f32 gekkoDrawDmgEffFrozenSteamScale;
    /* 0x0394 */ f32 gekkoScale;
    /* 0x0398 */ f32 vtxSurfacePerturbation[BIGSLIME_NUM_VTX];
    /* 0x0620 */ f32 vtxScaleX;
    /* 0x0624 */ f32 vtxScaleZ;
    /* 0x0628 */ f32 shockwaveScale;
    /* 0x062C */ ColliderCylinder gekkoCollider;
    /* 0x0678 */ ColliderCylinder bigslimeCollider[BIGSLIME_NUM_RING_FACES];
    /* 0x0A14 */ EnMinislime* minislime[MINISLIME_NUM_SPAWN];
    /* 0x0A44 */ EnMinislime* minislimeToThrow;
    /* 0x0A48 */ AnimatedMaterial* bigslimeFrozenTexAnim;
    /* 0x0A4C */ AnimatedMaterial* minislimeFrozenTexAnim;
    /* 0x0A50 */ AnimatedMaterial* iceShardTexAnim;
    /* 0x0A54 */ EnBigslimeIceShardEffect iceShardEffect[BIGSLIME_NUM_ICE_SHARD]; // 312 = 162 (bigslime) + 10 * 15 (minislime)
} EnBigslime; // size = 0x3634

extern const ActorInit En_Bigslime_InitVars;

#endif // Z_EN_BIGSLIME_H

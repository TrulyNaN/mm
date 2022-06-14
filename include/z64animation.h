#ifndef Z64_ANIMATION_H
#define Z64_ANIMATION_H

#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include <z64dma.h>
#include <z64math.h>

#define LINK_ANIMETION_OFFSET(addr, offset) \
    (((u32)&_link_animetionSegmentRomStart) + ((u32)addr & 0xFFFFFF) + ((u32)offset))
#define LIMB_DONE 0xFF
#define ANIMATION_ENTRY_MAX 50

#define ANIM_FLAG_UPDATEXZ 0x02
#define ANIM_FLAG_UPDATEY 0x10

typedef enum {
    /* 0 */ ANIMMODE_LOOP,
    /* 1 */ ANIMMODE_LOOP_INTERP,
    /* 2 */ ANIMMODE_ONCE,
    /* 3 */ ANIMMODE_ONCE_INTERP,
    /* 4 */ ANIMMODE_LOOP_PARTIAL,
    /* 5 */ ANIMMODE_LOOP_PARTIAL_INTERP
} AnimationModes;

typedef enum { 
    /* -1 */ ANIMTAPER_DECEL = -1,
    /*  0 */ ANIMTAPER_NONE, 
    /*  1 */ ANIMTAPER_ACCEL
} AnimationTapers;

typedef struct {
    /* 0x00 */ Vec3s jointPos; // Root is position in model space, children are relative to parent
    /* 0x06 */ u8 child;       // The first child's index into the limb table.
    /* 0x07 */ u8 sibling;     // The parent limb's next limb index into the limb table.
    /* 0x08 */ Gfx* dList;     // Display lists for the limb. Index 0 is the normal display list, index 1 is the
                               // far model display list.
} StandardLimb; // size = 0xC

typedef struct {
    /* 0x000 */ Vec3s translation;  // Translation relative to parent limb.  root limb is a tranlation for entire model.
    /* 0x006 */ u8 firstChildIndex; // The first child's index into the limb table.
    /* 0x007 */ u8 nextLimbIndex;   // The parent limb's next limb index into the limb table.
    /* 0x008 */ Gfx* displayLists[1]; // Display lists for the limb. Index 0 is the normal display list, index 1 is the
                                      // far model display list.
} StandardLimb; // Size = 0xC

// Model has limbs with only rigid meshes
typedef struct {
    /* 0x000 */ void** skeletonSeg; // Segment address of SkelLimbIndex.
    /* 0x004 */ u8 limbCount;          // Number of limbs in the model.
} SkeletonHeader; // size = 0x8

// Model has limbs with flexible meshes
typedef struct {
    /* 0x000 */ SkeletonHeader sh;
    /* 0x008 */ u8 dListCount;         // Number of display lists in the model.
} FlexSkeletonHeader; // size = 0xC

typedef s16 AnimationRotationValue;

typedef struct {
    /* 0x000 */ u16 x;
    /* 0x002 */ u16 y;
    /* 0x004 */ u16 z;
} JointIndex; // size = 0x06

typedef struct {
    /* 0x000 */ s16 frameCount;
    /* 0x002 */ s16 unk02;
} AnimationHeaderCommon; // size = 0x4

typedef struct {
    /* 0x00 */ AnimationHeaderCommon common;
    /* 0x04 */ s16* frameData;         // referenced as tbl
    /* 0x08 */ JointIndex* jointIndices; // referenced as ref_tbl
    /* 0x0C */ u16 staticIndexMax;
} AnimationHeader; // size = 0x10

typedef enum {
    ANIMATION_LINKANIMETION,
    ANIMATION_TYPE1,
    ANIMATION_TYPE2,
    ANIMATION_TYPE3,
    ANIMATION_TYPE4,
    ANIMATION_TYPE5
} AnimationType;

typedef struct {
    /* 0x000 */ DmaRequest req;
    /* 0x020 */ OSMesgQueue msgQueue;
    /* 0x038 */ OSMesg msg;
} AnimationEntryType0; // size = 0x3C

typedef struct {
    /* 0x000 */ u8 unk00;
    /* 0x001 */ u8 vecCount;
    /* 0x004 */ Vec3s* dst;
    /* 0x008 */ Vec3s* src;
} AnimationEntryType1; // size = 0xC

typedef struct {
    /* 0x000 */ u8 unk00;
    /* 0x001 */ u8 limbCount;
    /* 0x004 */ Vec3s* unk04;
    /* 0x008 */ Vec3s* unk08;
    /* 0x00C */ f32 unk0C;
} AnimationEntryType2; // size = 0x10

typedef struct {
    /* 0x000 */ u8 unk00;
    /* 0x001 */ u8 vecCount;
    /* 0x004 */ Vec3s* dst;
    /* 0x008 */ Vec3s* src;
    /* 0x00C */ u8* index;
} AnimationEntryType3; // size = 0x10

typedef struct {
    /* 0x000 */ u8 unk00;
    /* 0x001 */ u8 vecCount;
    /* 0x002 */ char unk02[0x2];
    /* 0x004 */ Vec3s* dst;
    /* 0x008 */ Vec3s* src;
    /* 0x00C */ u8* index;
} AnimationEntryType4; // size = 0x10

typedef struct {
    /* 0x000 */ struct Actor* actor;
    /* 0x004 */ SkelAnime* skelAnime;
    /* 0x008 */ f32 unk08;
} AnimationEntryType5; // size = 0xC

typedef struct {
    /* 0x000 */ u8 raw[0x3C];
} AnimationEntryRaw; // size = 0x3C

typedef union {
    AnimationEntryRaw raw;
    AnimationEntryType0 type0;
    AnimationEntryType1 type1;
    AnimationEntryType2 type2;
    AnimationEntryType3 type3;
    AnimationEntryType4 type4;
    AnimationEntryType5 type5;
} AnimationEntryType; // size = 0x3C

typedef struct {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 unk01;
    /* 0x004 */ AnimationEntryType types;
} AnimationEntry; // size = 0x40

typedef struct AnimationContext {
    /* 0x000 */ s16 animationCount;
    /* 0x002 */ char unk02[2];
    /* 0x004 */ AnimationEntry entries[ANIMATION_ENTRY_MAX];
} AnimationContext; // size = 0xC84

typedef struct {
    /* 0x00 */ AnimationHeaderCommon common;
    /* 0x04 */ u32 segment;
} LinkAnimationHeader; // size = 0x8

struct SkelAnime {
    /* 0x00 */ u8 limbCount; // joint_Num
                             /* modes 0 and 1 repeat the animation indefinitely
                              * modes 2 and 3 play the animaton once then stop
                              * modes >= 4 play the animation once, and always start at frame 0.
                              */
    /* 0x01 */ u8 mode;
    /* 0x02 */ u8 dListCount;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ void** skeleton;   // An array of pointers to limbs. Can be StandardLimb, LodLimb, or SkinLimb.
    /* 0x08 */ void* animation;   // Can be an AnimationHeader or LinkAnimationHeader.
    /* 0x0C */ f32 startFrame;    // In mode 4, start of partial loop.
    /* 0x10 */ f32 endFrame;      // In mode 2, Update returns true when curFrame is equal to this. In mode 4, end of partial loop.
    /* 0x14 */ f32 animLength;    // Total number of frames in the current animation's file.
    /* 0x18 */ f32 curFrame;      // Current frame in the animation
    /* 0x1C */ f32 playSpeed;     // Multiplied by R_UPDATE_RATE / 3 to get the animation's frame rate.
    /* 0x20 */ Vec3s* jointTable; // Current translation of model and rotations of all limbs
    /* 0x24 */ Vec3s* morphTable; // Table of values used to morph between animations
    /* 0x28 */ f32 morphWeight;   // Weight of the current animation morph as a fraction in [0,1]
    /* 0x2C */ f32 morphRate;     // Reciprocal of the number of frames in the morph
    /* 0x30 */ union {
                    s32 (*normal)(struct SkelAnime*);// Can be Loop, Partial loop, Play once, Morph, or Tapered morph
                    s32 (*link)(struct GlobalContext*, struct SkelAnime*); // Loop, Play once, and Morph
                } update;
    /* 0x34 */ s8 initFlags;      // Flags used when initializing Link's skeleton
    /* 0x35 */ u8 moveFlags;          // Flags used for animations that move the actor in worldspace.
    /* 0x36 */ s16 prevRot;       // Previous rotation in worldspace.
    /* 0x38 */ Vec3s prevTransl;  // Previous modelspace translation.
    /* 0x3E */ Vec3s baseTransl;  // Base modelspace translation.
} SkelAnime; // size = 0x44

typedef s32 (*OverrideLimbDrawOpa)(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                   struct Actor* thisx);

typedef void (*PostLimbDrawOpa)(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot,
                                struct Actor* thisx);

typedef s32 (*OverrideLimbDraw)(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                struct Actor* thisx, Gfx** gfx);

typedef void (*PostLimbDraw)(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot,
                             struct Actor* thisx, Gfx** gfx);

typedef s32 (*OverrideLimbDrawFlex)(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                    struct Actor* thisx);

typedef void (*PostLimbDrawFlex)(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList1, Gfx** dList2, Vec3s* rot,
                                 struct Actor* thisx);

typedef void (*TransformLimbDrawOpa)(struct GlobalContext* globalCtx, s32 limbIndex, struct Actor* thisx);

typedef void (*TransformLimbDraw)(struct GlobalContext* globalCtx, s32 limbIndex, struct Actor* thisx, Gfx** gfx);

typedef void (*AnimationEntryCallback)(struct GlobalContext*, AnimationEntryType*);

extern u32 link_animetion_segment;

// fcurve_skelanime structs
typedef struct {
    /* 0x0000 */ u16 unk00; // appears to be flags
    /* 0x0002 */ s16 unk02;
    /* 0x0004 */ s16 unk04;
    /* 0x0006 */ s16 unk06;
    /* 0x0008 */ f32 unk08;
} TransformData; // size = 0xC

typedef struct {
    /* 0x0000 */ u8* refIndex;
    /* 0x0004 */ TransformData* transformData;
    /* 0x0008 */ s16* copyValues;
    /* 0x000C */ s16 unk0C;
    /* 0x000E */ s16 unk10;
} TransformUpdateIndex; // size 0x10

typedef struct {
    /* 0x0000 */ u8 firstChildIdx;
    /* 0x0001 */ u8 nextLimbIdx;
    /* 0x0004 */ Gfx* dList[2];
} SkelCurveLimb; // size >= 0x8

typedef struct {
    /* 0x0000 */ SkelCurveLimb** limbs;
    /* 0x0004 */ u8 limbCount;
} SkelCurveLimbList; // size = 0x8

typedef struct {
    /* 0x0000 */ Vec3s scale;
    /* 0x0006 */ Vec3s rot;
    /* 0x000C */ Vec3s pos;
} LimbTransform; // size = 0x12

typedef struct {
    /* 0x0000 */ u8 limbCount;
    /* 0x0004 */ SkelCurveLimb** limbList;
    /* 0x0008 */ TransformUpdateIndex* transUpdIdx;
    /* 0x000C */ f32 unk0C; // seems to be unused
    /* 0x0010 */ f32 animFinalFrame;
    /* 0x0014 */ f32 animSpeed;
    /* 0x0018 */ f32 animCurFrame;
    /* 0x001C */ LimbTransform* transforms;
} SkelAnimeCurve; // size = 0x20

typedef s32 (*OverrideCurveLimbDraw)(struct GlobalContext* globalCtx, SkelAnimeCurve* skelCuve, s32 limbIndex,
                                     struct Actor* actor);
typedef void (*PostCurveLimbDraw)(struct GlobalContext* globalCtx, SkelAnimeCurve* skelCuve, s32 limbIndex,
                                  struct Actor* actor);

typedef struct {
    /* 0x00 */ AnimationHeader* animation;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ f32 startFrame;
    /* 0x0C */ f32 frameCount;
    /* 0x10 */ u8 mode;
    /* 0x14 */ f32 morphFrames;
} AnimationInfo; // size = 0x18

typedef struct {
    /* 0x00 */ AnimationHeader* animation;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ s16 startFrame;
    /* 0x0A */ s16 frameCount;
    /* 0x0C */ u8 mode;
    /* 0x0E */ s16 morphFrames;
} AnimationInfoS; // size = 0x10

typedef struct AnimationSpeedInfo {
    /* 0x00 */ AnimationHeader* animation;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} AnimationSpeedInfo; // size = 0x10

struct SkeletonInfo;

typedef s32 (*UnkKeyframeCallback)(struct GlobalContext* globalCtx, struct SkeletonInfo* skeletonInfo, s32* arg2, Gfx** dList,
                                   u8* arg4, void* arg5);

// Keyframe limb?
typedef struct {
    /* 0x00 */ Gfx* dList;
    /* 0x04 */ u8 unk_4;
    /* 0x05 */ u8 flags;
    /* 0x06 */ Vec3s root;
} Struct_801BFA14_Arg1_Field4; // size = 0xC

// Other limb type?
typedef struct {
    /* 0x00 */ Gfx* dList;
    /* 0x04 */ u8 unk_4;
    /* 0x05 */ u8 flags;
    /* 0x06 */ u8 unk_6;         // transform limb draw index
} Struct_801BFA14_Arg1_Field4_2; // size = 0x8

typedef struct {
    /* 0x00 */ u8 limbCount;
    /* 0x01 */ u8 unk_1; // non-zero in object files, number of non-null-dlist limbs?
    /* 0x04 */ union {
        Struct_801BFA14_Arg1_Field4* unk_4; // arrays
        Struct_801BFA14_Arg1_Field4_2* unk_4_2;
    };
    /* 0x08 */ s16* unk_8;
    /* 0x0C */ s16* unk_C;
    /* 0x0C */ char unk_10[0x2];
    /* 0x12 */ s16 unk_12;
} Struct_801BFA14_Arg1;

typedef struct {
    /* 0x00 */ u16* unk_0;
    /* 0x04 */ s16* unk_4;
    /* 0x08 */ s16* unk_8;
    /* 0x0C */ s16* unk_C;
    /* 0x0C */ char unk_10[0x2];
    /* 0x12 */ s16 unk_12;
} SkeletonInfo_1C;

typedef struct {
    /* 0x00 */ f32 unk_0;
    /* 0x04 */ f32 unk_4;
    /* 0x08 */ f32 unk_8;
    /* 0x0C */ f32 unk_C;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ s32 unk_14;
} FrameControl;

// FlexKeyframeSkeleton ?
typedef struct SkeletonInfo {
    /* 0x00 */ FrameControl frameCtrl;
    /* 0x18 */ Struct_801BFA14_Arg1* unk_18;    // array
    /* 0x1C */ SkeletonInfo_1C* unk_1C;
    /* 0x20 */ UnkKeyframeCallback* unk_20;     // pointer to array of functions
    /* 0x24 */ f32 unk_24;                      // duration? current time?
    /* 0x28 */ Vec3s* frameData;                // array of 3 Vec3s
    /* 0x2C */ s16* unk_2C;
} SkeletonInfo;

typedef s32 (*OverrideKeyframeDrawScaled)(struct GlobalContext* globalCtx, SkeletonInfo* skeletonInfo, s32 limbIndex, Gfx** dList,
                                          u8* flags, struct Actor* actor, Vec3f* scale, Vec3s* rot, Vec3f* pos);

typedef void (*PostKeyframeDrawScaled)(struct GlobalContext* globalCtx, SkeletonInfo* skeleton, s32 limbIndex, Gfx** dList,
                                       u8* flags, struct Actor* actor, Vec3f* scale, Vec3s* rot, Vec3f* pos);

#endif

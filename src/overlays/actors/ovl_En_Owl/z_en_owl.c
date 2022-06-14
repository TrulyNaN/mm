#include "z_en_owl.h"
#include "objects/object_owl/object_owl.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10)

#define THIS ((EnOwl*)thisx)

void EnOwl_Init(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_Update(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8095AB1C(EnOwl* this, GlobalContext* globalCtx);
void func_8095ABF0(EnOwl* this, GlobalContext* globalCtx);
void func_8095AD54(EnOwl* this, GlobalContext* globalCtx);
void func_8095AE00(EnOwl* this, GlobalContext* globalCtx);
void func_8095AE60(EnOwl* this, GlobalContext* globalCtx);
void func_8095AEC0(EnOwl* this, GlobalContext* globalCtx);
void func_8095AF2C(EnOwl* this, GlobalContext* globalCtx);
void func_8095AFEC(EnOwl* this, GlobalContext* globalCtx);
void func_8095B254(EnOwl* this, GlobalContext* globalCtx);
void func_8095B2F8(EnOwl* this, GlobalContext* globalCtx);
void func_8095B650(EnOwl* this, GlobalContext* globalCtx);
void func_8095B6C8(EnOwl* this, GlobalContext* globalCtx);
void func_8095B76C(EnOwl* this, GlobalContext* globalCtx);
void func_8095B960(EnOwl* this, GlobalContext* globalCtx);
void func_8095BA84(EnOwl* this, GlobalContext* globalCtx);
void func_8095BE0C(EnOwl* this, GlobalContext* globalCtx);
void func_8095BF20(EnOwl* this, GlobalContext* globalCtx);
void func_8095BF58(EnOwl* this, GlobalContext* globalCtx);
void func_8095C1C8(EnOwl* this, GlobalContext* globalCtx);
void func_8095C328(EnOwl* this);
void func_8095C408(EnOwl* this);
void func_8095C484(EnOwl* this);
void func_8095CCF4(Actor* thisx, GlobalContext* globalCtx);
void func_8095D074(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_ChangeMode(EnOwl* this, EnOwlActionFunc actionFunc, EnOwlFunc unkFunc, SkelAnime* skelAnime,
                      AnimationHeader* animation, f32 morphFrames);

typedef enum {
    /* 0x00 */ OWL_REPEAT,
    /* 0x01 */ OWL_OK
} EnOwlMessageChoice;

const ActorInit En_Owl_InitVars = {
    ACTOR_EN_OWL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OWL,
    sizeof(EnOwl),
    (ActorFunc)EnOwl_Init,
    (ActorFunc)EnOwl_Destroy,
    (ActorFunc)EnOwl_Update,
    (ActorFunc)EnOwl_Draw,
};
*/

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 25, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2400, ICHAIN_STOP),
};

void func_8095A510(EnOwl* this, GlobalContext* globalCtx) {
    this->unk_3FC = ENOWL_GET_F000(&this->actor);
    if (this->unk_3FC == 15) {
        this->unk_3FC = -1;
        this->path = NULL;
    } else {
        this->unk_3F8 = 0;
        this->path = &globalCtx->setupPathList[this->unk_3FC];
    }
}

void EnOwl_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnOwl* this = THIS;
    s32 i;
    s16 cutscene = this->actor.cutscene;
    s32 owlType;
    s32 switchFlag;

    for (i = 0; i < ARRAY_COUNT(this->unk_400); i++) {
        this->unk_400[i] = cutscene;
        if (cutscene != -1) {
            this->actor.cutscene = cutscene;
            cutscene = ActorCutscene_GetAdditionalCutscene(this->actor.cutscene);
        }
    }

    this->actor.cutscene = this->unk_400[0];
    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (ENOWL_GET_TYPE(&this->actor) == ENOWL_GET_TYPE_30) {
        Actor_SetScale(&this->actor, 0.1f);
        this->actor.update = func_8095CCF4;
        this->actor.draw = func_8095D074;
        this->actor.flags &= ~ACTOR_FLAG_1;
        this->unk_3D8 = 0;
        this->unk_3DA = 0x320;
        this->unk_3DC = 0x12C;
        return;
    }

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &this->skelAnime1, &object_owl_Skel_00C5F8, &object_owl_Anim_001ADC,
                       this->jointTable1, this->morphTable1, 21);
    SkelAnime_InitFlex(globalCtx, &this->skelAnime2, &object_owl_Skel_0105C0, &object_owl_Anim_00CDB0,
                       this->jointTable2, this->morphTable2, 16);
    Collider_InitAndSetCylinder(globalCtx, &this->collider, &this->actor, &sCylinderInit);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.terminalVelocity = -10.0f;
    this->actor.targetArrowOffset = 500.0f;
    EnOwl_ChangeMode(this, func_8095BF58, func_8095C484, &this->skelAnime2, &object_owl_Anim_00CDB0, 0.0f);

    this->actionFlags = 0;
    this->unk_40D = 0;
    this->unk_40A = 0;
    this->unk_409 = 4;
    this->unk_40B = this->unk_408 = 0;
    this->unk_40C = 4;
    this->unk_406 = -1;
    owlType = ENOWL_GET_TYPE(&this->actor);
    switchFlag = ENOWL_GET_SWITCHFLAG(&this->actor);

    switch (owlType) {
        case ENOWL_GET_TYPE_1:
            if ((switchFlag < 0x7F) && Flags_GetSwitch(globalCtx, switchFlag)) {
                Actor_MarkForDeath(&this->actor);
                return;
            }
            break;

        case ENOWL_GET_TYPE_2:
            if (gSaveContext.save.inventory.items[ITEM_LENS] == ITEM_LENS) {
                Actor_MarkForDeath(&this->actor);
                return;
            }
            break;

        case ENOWL_GET_TYPE_3:
            if (CHECK_QUEST_ITEM(QUEST_SONG_SOARING)) {
                Actor_MarkForDeath(&this->actor);
                return;
            }
            break;
    }

    this->unk_3DA = 0;
    this->unk_3F0 = this->actor.home.pos.y;
    this->unk_3EC = this->actor.home.rot.y;
    func_8095A510(this, globalCtx);

    switch (owlType) {
        case ENOWL_GET_TYPE_1:
            this->actionFunc = func_8095AEC0;
            break;

        case ENOWL_GET_TYPE_2:
            this->actionFunc = func_8095BE0C;
            if (gSaveContext.save.weekEventReg[9] & 0x20) {
                this->actor.textId = 0xBF0;
            } else {
                this->actor.textId = 0xBEA;
            }
            break;

        case ENOWL_GET_TYPE_3:
            this->actionFunc = func_8095AFEC;
            break;

        case ENOWL_GET_TYPE_1000:
            this->actionFunc = func_8095BF20;
            break;

        default:
            this->actionFlags |= 2;
            this->unk_3DA = 0x20;
            this->actionFunc = func_8095BF58;
            break;
    }
}

void EnOwl_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnOwl* this = THIS;

    if (ENOWL_GET_TYPE(&this->actor) != ENOWL_GET_TYPE_30) {
        Collider_DestroyCylinder(globalCtx, &this->collider);
    }
}

void func_8095A920(EnOwl* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    this->actor.world.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &player->actor.world.pos);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.world.rot.y, 6, 0x3E8, 0xC8);
}

s32 func_8095A978(EnOwl* this, GlobalContext* globalCtx, u16 textId, f32 targetDist, f32 arg4) {
    if (Actor_ProcessTalkRequest(&this->actor, &globalCtx->state)) {
        return true;
    }

    this->actor.textId = textId;
    if (this->actor.xzDistToPlayer < targetDist) {
        this->actor.flags |= ACTOR_FLAG_10000;
        func_800B8500(&this->actor, globalCtx, targetDist, arg4, EXCH_ITEM_NONE);
    }

    return false;
}

s32 func_8095A9FC(EnOwl* this, GlobalContext* globalCtx, u16 textId) {
    if (Actor_ProcessTalkRequest(&this->actor, &globalCtx->state)) {
        return true;
    }

    this->actor.textId = textId;
    if (this->actor.xzDistToPlayer < 120.0f) {
        func_800B8500(&this->actor, globalCtx, 350.0f, 1000.0f, EXCH_ITEM_NONE);
    }

    return false;
}

void func_8095AA70(EnOwl* this) {
    EnOwl_ChangeMode(this, func_8095C1C8, func_8095C484, &this->skelAnime1, &object_owl_Anim_00CB94, 0.0f);
    this->eyeTexIndex = 0;
    this->blinkTimer = Rand_S16Offset(60, 60);
}

void func_8095AAD0(EnOwl* this, GlobalContext* globalCtx) {
    s32 switchFlag = ENOWL_GET_SWITCHFLAG(&this->actor);

    if (switchFlag < 0x7F) {
        Flags_SetSwitch(globalCtx, switchFlag);
    }

    func_8095AA70(this);
}

void func_8095AB1C(EnOwl* this, GlobalContext* globalCtx) {
    if ((this->unk_3DA % 64) == 0) {
        func_8095AAD0(this, globalCtx);
    }
}

void func_8095AB4C(EnOwl* this) {
    if (randPlusMinusPoint5Scaled(1.0f) < 0.0f) {
        this->actionFlags |= 0x20;
    } else {
        this->actionFlags &= ~0x20;
    }
}

// Unused?
void func_8095ABA8(EnOwl* this) {
    func_8095AB4C(this);
    this->unk_40B = 0;
    this->unk_3DE = 0;
    this->actionFlags |= 0x10;
    this->unk_40C = 4;
    this->unk_408 = 0;
    this->unk_40A = 0;
    this->unk_409 = 4;
}

void func_8095ABF0(EnOwl* this, GlobalContext* globalCtx) {
    if (Actor_TextboxIsClosing(&this->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        func_8095AAD0(this, globalCtx);
        this->actor.flags &= ~ACTOR_FLAG_10000;
    }
}

// Unused?
void func_8095AC50(EnOwl* this, GlobalContext* globalCtx) {
    if (Actor_TextboxIsClosing(&this->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        if ((this->unk_3DA % 64) == 0) {
            func_8095AAD0(this, globalCtx);
        } else {
            this->actionFlags &= ~2;
            func_8095ABA8(this);
            this->actionFunc = func_8095AB1C;
        }
        this->actor.flags &= ~ACTOR_FLAG_10000;
    }
}

void func_8095ACEC(EnOwl* this) {
    this->actionFlags &= ~0x40;
    if (this->unk_406 >= 0) {
        if (ActorCutscene_GetCurrentIndex() == this->unk_400[this->unk_406]) {
            ActorCutscene_Stop(this->unk_400[this->unk_406]);
        }
        this->unk_406 = -1;
    }
}

void func_8095AD54(EnOwl* this, GlobalContext* globalCtx) {
    if ((Message_GetState(&globalCtx->msgCtx) == 4) && Message_ShouldAdvance(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_80151938(globalCtx, 0x7D1);
                this->actionFunc = func_8095AE00;
                break;

            case OWL_OK:
                func_80151938(globalCtx, 0x7D3);
                this->actionFunc = func_8095ABF0;
                break;
        }
    }
}

void func_8095AE00(EnOwl* this, GlobalContext* globalCtx) {
    if ((Message_GetState(&globalCtx->msgCtx) == 5) && Message_ShouldAdvance(globalCtx)) {
        func_80151938(globalCtx, 0x7D2);
        this->actionFunc = func_8095AD54;
    }
}

void func_8095AE60(EnOwl* this, GlobalContext* globalCtx) {
    if ((Message_GetState(&globalCtx->msgCtx) == 5) && Message_ShouldAdvance(globalCtx)) {
        func_80151938(globalCtx, 0x7D1);
        this->actionFunc = func_8095AE00;
    }
}

void func_8095AEC0(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A920(this, globalCtx);
    if (func_8095A978(this, globalCtx, 0x7D0, 360.0f, 200.0f)) {
        func_801A3098(NA_BGM_OWL);
        this->actionFunc = func_8095AE60;
    }
}

void func_8095AF2C(EnOwl* this, GlobalContext* globalCtx) {
    switch (Message_GetState(&globalCtx->msgCtx)) {
        case 5:
            if (Message_ShouldAdvance(globalCtx)) {
                if (globalCtx->msgCtx.currentTextId == 0xBFE) {
                    func_8095ACEC(this);
                    func_801477B4(globalCtx);
                    this->actionFunc = func_8095ABF0;
                } else {
                    func_80151938(globalCtx, globalCtx->msgCtx.currentTextId + 1);
                }
            }
            break;

        case 6:
            func_8095ACEC(this);
            this->actionFunc = func_8095ABF0;
            break;
    }
}

void func_8095AFEC(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A920(this, globalCtx);
    if (func_8095A978(this, globalCtx, 0xBF6, 200.0f, 100.0f)) {
        func_801A3098(NA_BGM_OWL);
        this->actionFunc = func_8095AF2C;
        this->unk_406 = 0;
        this->actionFlags |= 0x40;
    }
}

s32 func_8095B06C(EnOwl* this) {
    if ((this->actor.world.pos.y + 2.0f) < this->unk_3F0) {
        this->actor.world.pos.y += 2.0f;
        return false;
    }

    if (this->unk_3F0 < (this->actor.world.pos.y - 2.0f)) {
        this->actor.world.pos.y -= 2.0f;
        return false;
    }

    return true;
}

void func_8095B0C8(EnOwl* this) {
    s32 pad;
    Vec3s* points = Lib_SegmentedToVirtual(this->path->points);

    points += this->unk_3F8;
    this->unk_3EC = Math_FAtan2F(points->z - this->actor.world.pos.z, points->x - this->actor.world.pos.x);
    this->unk_3F0 = points->y;
}

void func_8095B158(EnOwl* this) {
    if (Animation_OnFrame(&this->skelAnime1, 2.0f) || Animation_OnFrame(&this->skelAnime1, 9.0f) ||
        Animation_OnFrame(&this->skelAnime1, 23.0f) || Animation_OnFrame(&this->skelAnime1, 40.0f) ||
        Animation_OnFrame(&this->skelAnime1, 58.0f)) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_OWL_FLUTTER);
    }
}

void func_8095B1E4(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actor.speedXZ < 6.0f) {
        this->actor.speedXZ += 1.0f;
    }

    if (this->actor.xzDistToPlayer > 6000.0f) {
        Actor_MarkForDeath(&this->actor);
    }
}

void func_8095B254(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actor.speedXZ < 6.0f) {
        this->actor.speedXZ += 1.0f;
    }

    if (this->actionFlags & 1) {
        EnOwl_ChangeMode(this, func_8095B1E4, func_8095C328, &this->skelAnime1, &object_owl_Anim_001ADC, 0.0f);
        this->unk_3EA = 6;
        this->actor.flags |= ACTOR_FLAG_20;
    }

    func_8095B158(this);
}

void func_8095B2F8(EnOwl* this, GlobalContext* globalCtx) {
    func_8095B06C(this);
    if (this->skelAnime1.curFrame >= 18.0f) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_3EC, 2, 0x200, 0x80);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    if ((this->actor.shape.rot.y == this->unk_3EC) && (this->actionFlags & 1)) {
        this->actionFunc = func_8095B254;
        Animation_Change(this->skelAnime3, this->skelAnime3->animation, 1.0f, 19.0f,
                         Animation_GetLastFrame(&object_owl_Anim_001168), 2, 0.0f);
        this->unk_414 = func_8095C484;
    }

    func_8095B158(this);
}

void func_8095B3DC(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actionFlags & 1) {
        this->actionFunc = func_8095B2F8;
        Animation_Change(this->skelAnime3, &object_owl_Anim_001168, 1.0f, 0.0f, 35.0f, 2, 0.0f);
        this->unk_414 = func_8095C408;
        this->unk_3EC = 0x5500;
        this->actor.world.pos.y += 100.0f;
        this->unk_3F0 = this->actor.world.pos.y;
    }
}

void func_8095B480(EnOwl* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->stateFlags3 & 0x10000000) {
        this->actor.textId = 0xBF1;
        EnOwl_ChangeMode(this, func_8095BF58, func_8095C484, &this->skelAnime2, &object_owl_Anim_00CDB0, 0.0f);
        this->eyeTexIndex = 0;
        this->actionFlags &= ~8;
        this->blinkTimer = Rand_S16Offset(60, 60);
        this->actor.home.rot.x = 0;
        this->actionFlags |= 8;
        func_8095ACEC(this);
        this->unk_3F0 = this->actor.home.pos.y;
        this->unk_3EC = this->actor.home.rot.y;
        this->actor.world.pos = this->actor.home.pos;
        func_8095A510(this, globalCtx);
        this->actor.speedXZ = 0.0f;
        this->actionFunc = func_8095BE0C;
    }
}

void func_8095B574(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A920(this, globalCtx);
    if (Actor_ProcessTalkRequest(&this->actor, &globalCtx->state)) {
        this->actionFunc = func_8095BA84;
        func_801A3098(NA_BGM_OWL);
        this->actionFlags |= 0x40;
        this->unk_406 = 2;
    } else if (this->actor.xzDistToPlayer < 200.0f) {
        this->actor.flags |= ACTOR_FLAG_10000;
        func_800B8500(&this->actor, globalCtx, 200.0f, 400.0f, EXCH_ITEM_NONE);
    } else {
        this->actor.flags &= ~ACTOR_FLAG_10000;
    }
    func_8095B480(this, globalCtx);
}

void func_8095B650(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actionFlags & 1) {
        EnOwl_ChangeMode(this, func_8095B574, func_8095C484, &this->skelAnime2, &object_owl_Anim_00CDB0, 0.0f);
        this->actor.textId = 0xBF5;
        this->actionFlags &= ~8;
    }
    func_8095B480(this, globalCtx);
}

void func_8095B6C8(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actionFlags & 1) {
        Animation_Change(this->skelAnime3, &object_owl_Anim_00CB94, -1.0f,
                         Animation_GetLastFrame(&object_owl_Anim_00CB94), 0.0f, 2, 0.0f);
        this->unk_414 = func_8095C484;
        this->actionFunc = func_8095B650;
    }
    func_8095B158(this);
    func_8095B480(this, globalCtx);
}

void func_8095B76C(EnOwl* this, GlobalContext* globalCtx) {
    s32 pad;
    s16 sp4A;
    f32 sp44 = Path_OrientAndGetDistSq(&this->actor, this->path, this->unk_3F8, &sp4A);
    Vec3s* points;

    Math_SmoothStepToS(&this->actor.world.rot.y, sp4A, 6, 0x800, 0x200);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if (sp44 < SQ(this->actor.speedXZ)) {
        this->actor.speedXZ = 0.0f;
        points = Lib_SegmentedToVirtual(this->path->points);
        points += this->unk_3F8;

        do {
            this->actor.world.pos.x = points->x;
            this->actor.world.pos.z = points->z;
        } while (0);

        this->unk_3F8++;
        if (this->path->count <= this->unk_3F8) {
            this->actionFunc = func_8095B6C8;
            return;
        }

        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_OWL, this->actor.world.pos.x, this->actor.world.pos.y,
                    this->actor.world.pos.z, 0, 0, 0, 0xF00);
        this->actor.home.rot.x++;
        if (this->actor.home.rot.x >= 3) {
            func_8095ACEC(this);
        }
        func_8095B0C8(this);
    } else if (sp44 < SQ(21.0f)) {
        if (this->actor.speedXZ > 1.0f) {
            this->actor.speedXZ -= 1.0f;
        } else {
            this->actor.speedXZ = 1.0f;
        }
    } else if (this->actor.speedXZ < 6.0f) {
        this->actor.speedXZ += 1.0f;
    }

    func_8095B06C(this);
    func_8095B158(this);
    func_8095B480(this, globalCtx);
}

void func_8095B960(EnOwl* this, GlobalContext* globalCtx) {
    if (this->skelAnime1.curFrame >= 18.0f) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_3EC, 2, 0x200, 0x80);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    func_8095B06C(this);
    if (this->actor.shape.rot.y == this->unk_3EC) {
        this->actionFunc = func_8095B76C;
    }

    func_8095B158(this);
    func_8095B480(this, globalCtx);
}

void func_8095B9FC(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actionFlags & 1) {
        this->actionFunc = func_8095B960;
        Animation_Change(this->skelAnime3, &object_owl_Anim_001168, 1.0f, 0.0f, 35.0f, 2, 0.0f);
        this->unk_414 = func_8095C408;
        func_8095B0C8(this);
    }
}

void func_8095BA84(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A920(this, globalCtx);

    switch (Message_GetState(&globalCtx->msgCtx)) {
        case 4:
            if (Message_ShouldAdvance(globalCtx)) {
                switch (globalCtx->msgCtx.currentTextId) {
                    case 0xBEC:
                        switch (globalCtx->msgCtx.choiceIndex) {
                            case 0:
                                func_8019F208();
                                if (gSaveContext.save.weekEventReg[9] & 0x40) {
                                    func_80151938(globalCtx, 0xBF4);
                                } else {
                                    gSaveContext.save.weekEventReg[9] |= 0x40;
                                    func_80151938(globalCtx, 0xBED);
                                }
                                break;

                            case 1:
                                func_8019F230();
                                func_80151938(globalCtx, 0xBEF);
                                break;
                        }
                        break;

                    case 0xBF2:
                        switch (globalCtx->msgCtx.choiceIndex) {
                            case 0:
                                func_8019F208();
                                func_80151938(globalCtx, 0xBF4);
                                return;

                            case 1:
                                func_8019F230();
                                func_80151938(globalCtx, 0xBF3);
                                return;
                        }
                        break;
                }
            }
            break;

        case 5:
            if (Message_ShouldAdvance(globalCtx)) {
                switch (globalCtx->msgCtx.currentTextId) {
                    case 0xBEA:
                        gSaveContext.save.weekEventReg[9] |= 0x20;
                        func_80151938(globalCtx, 0xBEB);
                        break;

                    case 0xBEB:
                    case 0xBF0:
                        func_80151938(globalCtx, 0xBEC);
                        break;

                    case 0xBED:
                    case 0xBF4:
                        func_80151938(globalCtx, 0xBEE);
                        break;

                    case 0xBEE:
                        func_801477B4(globalCtx);
                        Audio_QueueSeqCmd(0x110000FF);
                        EnOwl_ChangeMode(this, func_8095B9FC, func_8095C484, &this->skelAnime1, &object_owl_Anim_00CB94,
                                         0.0f);
                        this->eyeTexIndex = 0;
                        this->blinkTimer = Rand_S16Offset(60, 60);
                        this->actionFlags |= 8;
                        this->actor.flags &= ~ACTOR_FLAG_10000;
                        this->actor.home.rot.x = 0;
                        func_8095ACEC(this);
                        this->unk_406 = 0;
                        this->actionFlags |= 0x40;
                        break;

                    case 0xBEF:
                    case 0xBF3:
                        func_801477B4(globalCtx);
                        Audio_QueueSeqCmd(0x110000FF);
                        func_8095ACEC(this);
                        this->actor.flags &= ~ACTOR_FLAG_10000;
                        this->actor.textId = 0xBF0;
                        this->actionFunc = func_8095BE0C;
                        break;

                    case 0xBF1:
                        func_80151938(globalCtx, 0xBF2);
                        break;

                    case 0xBF5:
                        func_801477B4(globalCtx);
                        Audio_QueueSeqCmd(0x110000FF);
                        this->actor.flags &= ~ACTOR_FLAG_10000;
                        EnOwl_ChangeMode(this, func_8095B3DC, func_8095C484, &this->skelAnime1, &object_owl_Anim_00CB94,
                                         0.0f);
                        this->eyeTexIndex = 0;
                        this->blinkTimer = Rand_S16Offset(60, 60);
                        this->actionFlags |= 8;
                        func_8095ACEC(this);
                        break;
                }
            }
            break;
    }
}

void func_8095BE0C(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A920(this, globalCtx);
    if (Actor_ProcessTalkRequest(&this->actor, &globalCtx->state)) {
        this->actionFunc = func_8095BA84;
        func_801A3098(NA_BGM_OWL);
        this->unk_406 = 1;
        this->actionFlags |= 0x40;
    } else if (this->actor.textId == 0xBF0) {
        if (this->actor.isTargeted) {
            func_800B8500(&this->actor, globalCtx, 200.0f, 200.0f, EXCH_ITEM_NONE);
        }
    } else if (this->actor.xzDistToPlayer < 200.0f) {
        this->actor.flags |= ACTOR_FLAG_10000;
        func_800B8500(&this->actor, globalCtx, 200.0f, 200.0f, EXCH_ITEM_NONE);
    } else {
        this->actor.flags &= ~ACTOR_FLAG_10000;
    }
}

void func_8095BF20(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A9FC(this, globalCtx, 0x7D0);
    this->actionFunc = func_8095ABF0;
}

void func_8095BF58(EnOwl* this, GlobalContext* globalCtx) {
    func_8095A920(this, globalCtx);
}

void func_8095BF78(EnOwl* this, GlobalContext* globalCtx) {
    this->actor.flags |= ACTOR_FLAG_20;
    if (this->actor.xzDistToPlayer > 6000.0f) {
        Actor_MarkForDeath(&this->actor);
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_3EC, 2, 0x80, 0x40);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if (this->actor.speedXZ < 16.0f) {
        this->actor.speedXZ += 0.5f;
    }

    if ((this->unk_3E4 + 1000.0f) < this->actor.world.pos.y) {
        if (this->actor.velocity.y > 0.0f) {
            this->actor.velocity.y -= 0.4f;
        }
    } else if (this->actor.velocity.y < 4.0f) {
        this->actor.velocity.y += 0.2f;
    }
}

void func_8095C09C(EnOwl* this, GlobalContext* globalCtx) {
    if (this->skelAnime1.curFrame > 10.0f) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_3EC, 2, 0x400, 0x40);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    if (this->skelAnime1.curFrame > 45.0f) {
        this->actor.velocity.y = 2.0f;
        this->actor.gravity = 0.0f;
        this->actor.speedXZ = 8.0f;
    } else if (this->skelAnime1.curFrame > 17.0f) {
        this->actor.velocity.y = 6.0f;
        this->actor.gravity = 0.0f;
        this->actor.speedXZ = 4.0f;
    }

    if (this->actionFlags & 1) {
        EnOwl_ChangeMode(this, func_8095BF78, func_8095C328, &this->skelAnime1, &object_owl_Anim_001ADC, 0.0f);
        this->unk_3EA = 6;
        this->unk_3EC += 0x2000;
    }

    func_8095B158(this);
}

void func_8095C1C8(EnOwl* this, GlobalContext* globalCtx) {
    if (this->actionFlags & 1) {
        this->unk_3EA = 3;
        EnOwl_ChangeMode(this, func_8095C09C, func_8095C484, &this->skelAnime1, &object_owl_Anim_001168, 0.0f);
        this->unk_3EC = BINANG_ADD(this->actor.world.rot.y, 0x4000);
        this->unk_3E4 = this->actor.world.pos.y;
        this->actor.velocity.y = 2.0f;
    }
    this->actionFlags |= 8;
}

void func_8095C258(EnOwl* this) {
    SkelAnime_Update(this->skelAnime3);
    if (this->unk_3EA > 0) {
        this->unk_3EA--;
        this->actor.shape.rot.z = Math_SinS(this->unk_3EA * 0x333) * 1000.0f;
    } else {
        this->unk_414 = func_8095C328;
        this->unk_3EA = 6;
        Animation_Change(this->skelAnime3, &object_owl_Anim_001ADC, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_owl_Anim_001ADC), 2, 5.0f);
    }
}

void func_8095C328(EnOwl* this) {
    if (SkelAnime_Update(this->skelAnime3)) {
        if (this->unk_3EA > 0) {
            this->unk_3EA--;
            Animation_Change(this->skelAnime3, this->skelAnime3->animation, 1.0f, 0.0f,
                             Animation_GetLastFrame(this->skelAnime3->animation), 2, 0.0f);
        } else {
            this->unk_3EA = 160;
            this->unk_414 = func_8095C258;
            Animation_Change(this->skelAnime3, &object_owl_Anim_00C6D4, 1.0f, 0.0f,
                             Animation_GetLastFrame(&object_owl_Anim_00C6D4), 0, 5.0f);
        }
    }
}

void func_8095C408(EnOwl* this) {
    if (SkelAnime_Update(this->skelAnime3)) {
        Animation_Change(this->skelAnime3, this->skelAnime3->animation, 1.0f, 18.0f, 35.0f, 2, 0.0f);
        this->actionFlags |= 1;
    } else {
        this->actionFlags &= ~1;
    }
}

void func_8095C484(EnOwl* this) {
    if (SkelAnime_Update(this->skelAnime3)) {
        Animation_Change(this->skelAnime3, this->skelAnime3->animation, 1.0f, 0.0f,
                         Animation_GetLastFrame(this->skelAnime3->animation), 2, 0.0f);
        this->actionFlags |= 1;
    } else {
        this->actionFlags &= ~1;
    }
}

s32 func_8095C510(EnOwl* this) {
    s32 phi_v1;

    if (this->actionFlags & 2) {
        phi_v1 = 0x20;
    } else {
        phi_v1 = 0;
    }

    if (phi_v1 == (this->unk_3DA & 0x3F)) {
        return true;
    }

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/EnOwl_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/EnOwl_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095A920.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095A978.asm")

    if (this->actor.draw != NULL) {
        Actor_MoveWithGravity(&this->actor);
    }

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AA70.asm")

    this->actionFunc(this, globalCtx);
    func_8095C568(this);
    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 10.0f, 10.0f, 10.0f, 4);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    if (this->actor.update != NULL) {
        if ((this->skelAnime1.animation == &object_owl_Anim_001ADC) && Animation_OnFrame(&this->skelAnime1, 4.0f)) {
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_OWL_FLUTTER);
        }

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AB1C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AB4C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095ABA8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095ABF0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AC50.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095ACEC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AD54.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AE00.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AE60.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AEC0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AF2C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095AFEC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B06C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B0C8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B158.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B1E4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B254.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B2F8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B3DC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B480.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B574.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B650.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B6C8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B76C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B960.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095B9FC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095BA84.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095BE0C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095BF20.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095BF58.asm")

    if (limbIndex == 3) {
        Matrix_MultVec3f(&sp18, &this->actor.focus.pos);
    }
}

void EnOwl_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static TexturePtr eyeTextures[] = {
        object_owl_Tex_008EB8,
        object_owl_Tex_0092B8,
        object_owl_Tex_0096B8,
    };
    s32 pad;
    EnOwl* this = THIS;

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C1C8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C258.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C328.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C408.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C484.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C510.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095C568.asm")

    Matrix_Translate(0.0f, 500.0f, 0.0f, MTXMODE_APPLY);
    Matrix_RotateXS(this->unk_3D8 - 0x4000, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 0.0f, -500.0f, MTXMODE_APPLY);
    if (this->unk_3DC >= 0x20) {
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095CCF4.asm")

        gDPSetRenderMode(POLY_OPA_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
        gSPDisplayList(POLY_OPA_DISP++, object_owl_DL_001200);
    } else {
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095CF44.asm")

        gDPSetRenderMode(POLY_XLU_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_XLU_SURF2);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, (u8)(this->unk_3DC * 8));
        gSPDisplayList(POLY_XLU_DISP++, object_owl_DL_001200);
    }

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Owl_0x8095A510/func_8095D074.asm")

void EnOwl_ChangeMode(EnOwl* this, EnOwlActionFunc actionFunc, EnOwlFunc unkFunc, SkelAnime* skelAnime,
                      AnimationHeader* animation, f32 morphFrames) {
    this->skelAnime3 = skelAnime;
    Animation_Change(this->skelAnime3, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), 2, morphFrames);
    this->actionFunc = actionFunc;
    this->unk_414 = unkFunc;
}

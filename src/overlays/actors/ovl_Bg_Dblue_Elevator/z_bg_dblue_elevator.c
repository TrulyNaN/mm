/*
 * File: z_bg_dblue_elevator.c
 * Overlay: ovl_Bg_Dblue_Elevator
 * Description: Great Bay Temple - Elevator
 */

#include "z_bg_dblue_elevator.h"
//TODO: include object file

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgDblueElevator*)thisx)

void BgDblueElevator_Init(Actor* thisx, PlayState* play);
void BgDblueElevator_Destroy(Actor* thisx, PlayState* play);
void BgDblueElevator_Update(Actor* thisx, PlayState* play);
void BgDblueElevator_Draw(Actor* thisx, PlayState* play);

#if 0
ActorInit Bg_Dblue_Elevator_InitVars = {
    ACTOR_BG_DBLUE_ELEVATOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DBLUE_OBJECT,
    sizeof(BgDblueElevator),
    (ActorFunc)BgDblueElevator_Init,
    (ActorFunc)BgDblueElevator_Destroy,
    (ActorFunc)BgDblueElevator_Update,
    (ActorFunc)BgDblueElevator_Draw,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80B929EC[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 250, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 250, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#endif

void func_80B91F20(BgDblueElevator *, PlayState *);
void BgDblueElevator_Destroy(Actor* thisx, PlayState* play); /* static */
void BgDblueElevator_Draw(Actor* thisx, PlayState* play); /* static */
void BgDblueElevator_Init(Actor *arg0, PlayState *play); /* static */
void BgDblueElevator_Update(Actor* thisx, PlayState* play0);            /* static */
s32 func_80B922C0(Actor *arg0, PlayState *play);       /* static */
void func_80B91F74(BgDblueElevator* arg0, PlayState *arg1);
s32 func_80B922FC(Actor *arg0, PlayState *play);     /* static */
void func_80B924DC(BgDblueElevator* arg0);                    /* static */
void func_80B924F8(BgDblueElevator *arg0, PlayState* play);                    /* static */
void func_80B9257C(BgDblueElevator *arg0);
void func_80B925B8(BgDblueElevator *arg0, PlayState* play);                    /* static */
void func_80B92644(BgDblueElevator* arg0);                    /* static */
void func_80B92660(BgDblueElevator* this, PlayState *play);      /* static */
extern Gfx D_060002C8; //gGreatBayTempleObjectElevatorDL
extern UNK_TYPE D_060005C4;
extern BgDBlueElevatorStruct1 D_80B92960[4];                                /* unable to generate initializer */
// extern UNK_TYPE D_80B92964;                                /* unable to generate initializer */
// extern UNK_TYPE D_80B9296C;                                /* unable to generate initializer */
extern s16 D_80B929D0[4];                                /* unable to generate initializer */
extern s16 D_80B929D8[4];                                /* unable to generate initializer */
extern UNK_TYPE D_80B929DE;     //fake?                          /* unable to generate initializer */
extern s8 D_80B929E0[2]; //= { 0, 2 };
extern UNK_TYPE D_80B929E3;     //fake?                  /* unable to generate initializer */
extern s8 D_80B929E4[16]; // = { 0, 1, 2, 3, 4, 5 };
extern s8 D_80B929EA[2];                //fake?                /* unable to generate initializer */
extern InitChainEntry D_80B929EC[];                /* unable to generate initializer */

void func_80B91F20(BgDblueElevator* this, PlayState* play) {
    s32 pad; //probably cast of this or play
    WaterBox *sp30;  
    s32 sp2C;
  
    this->unk168[3] = WaterBox_GetSurfaceImpl(play, &play->colCtx, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.z, 
                                           &this->unk16C, &sp30, &sp2C);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_Dblue_Elevator/func_80B91F74.s")
// extern ? D_80B929DE;
// extern ? D_80B929E3;
// extern ? D_80B929EA;
// void func_80B91F74(BgDblueElevator *arg0, PlayState *arg1)
// {
//   Vec3f spB0;
//   Vec3f spA4;
//   int new_var;
//   f32 sp98;
//   s32 i_s6;
//   f32 temp_fs3;
//   f32 temp_fs5;
//   f32 temp_fv0;
//   f32 temp_fv0_2;
//   f32 temp_fv0_3;
//   f32 var_fs0;
//   f32 var_fs0_2;
//   s16 temp_v0;
//   s16 temp_v1;
//   s32 var_s0;
//   Matrix_Push();
//   Matrix_RotateYS(arg0->dyna.actor.shape.rot.y, MTXMODE_NEW);
//   new_var = 0;
//   for (i_s6 = new_var; i_s6 < 4; i_s6++)
//   {
//     temp_v1 = D_80B929D8[i_s6];
//     temp_v0 = D_80B929D0[i_s6];
//     var_s0 = new_var;
//     temp_fs5 = (f32) (D_80B929D8[i_s6 + 1] - temp_v0);
//     sp98 = (f32) (D_80B929D8[i_s6 + 1] - temp_v1);
//     temp_fs3 = (f32) temp_v0;
//     for (var_s0 = new_var; var_s0 < 7; var_s0++)
//     {
//       temp_fv0 = (f32) var_s0;
//       spB0.x = ((temp_fv0 * temp_fs5) * (1.0f / 7.0f)) + temp_fs3;
//       spB0.y = arg0->unk16C;
//       spB0.z = ((temp_fv0 * sp98) * (1.0f / 7.0f)) + ((f32) temp_v1);
//       spB0.x += (Rand_ZeroOne() - 0.5f) * 20.0f;
//       spB0.z += (Rand_ZeroOne() - 0.5f) * 20.0f;
//       Matrix_MultVec3f(&spB0, &spA4);
//       spA4.x = spA4.x + arg0->dyna.actor.world.pos.x;
//       spA4.z += arg0->dyna.actor.world.pos.z;
//       EffectSsGSplash_Spawn(arg1, &spA4, NULL, NULL, (s16) 0, (s16) ((s32) ((Rand_ZeroOne() * 400.0f) + 210.0f)));
//     }

//   }

//   for (var_s0 = 0; var_s0 < 3; var_s0++)
//   {
//     spB0.x = ((Rand_ZeroOne() - 0.5f) * 60.0f) + arg0->dyna.actor.world.pos.x;
//     spB0.y = arg0->unk16C;
//     spB0.z = ((Rand_ZeroOne() - 0.5f) * 60.0f) + arg0->dyna.actor.world.pos.z;
//     EffectSsGRipple_Spawn(arg1, (Vec3f *) (&spB0), 0x3E8, 0xBB8, (s16) D_80B929E0[var_s0]);
//   }

//   for (var_s0 = 0; var_s0 < 6; var_s0++)
//   {
//     temp_fv0_2 = Rand_ZeroOne();
//     var_fs0 = 1.0f - (temp_fv0_2 * temp_fv0_2);
//     if (Rand_Next() > 0)
//     {
//       var_fs0 = -var_fs0;
//     }
//     spB0.x = (var_fs0 * 100.0f) + arg0->dyna.actor.world.pos.x;
//     spB0.y = arg0->unk16C;
//     temp_fv0_3 = Rand_ZeroOne();
//     var_fs0_2 = 1.0f - (temp_fv0_3 * temp_fv0_3);
//     if (Rand_Next() > 0)
//     {
//       var_fs0_2 = -var_fs0_2;
//     }
//     spB0.z = (var_fs0_2 * 100.0f) + arg0->dyna.actor.world.pos.z;
//     EffectSsGRipple_Spawn(arg1, &spB0, 0x190, 0x320, (s16) D_80B929E4[var_s0]);
//   }

//   Matrix_Pop();
// }


s32 func_80B922C0(Actor* thisx, PlayState *play) {
    BgDblueElevator* this = THIS;

    if (Flags_GetSwitch(play, this->dyna.actor.params & 0x7F)) { //TODO: write flag in header 
        return 0;
    }
    return 1;
}

s32 func_80B922FC(Actor *arg0, PlayState *play) {
    s32 var_s0;

    var_s0 = 0;
    if (!Flags_GetSwitch(play, arg0->params & 0x7F)) {
        var_s0 = 1;
    }
    if (Flags_GetSwitch(play, (arg0->params + 1) & 0x7F) && Flags_GetSwitch(play, (arg0->params + 2) & 0x7F) &&
        Flags_GetSwitch(play, (arg0->params + 3) & 0x7F)) {
        var_s0 += 2;
    }
    return var_s0;
}

void BgDblueElevator_Init(Actor *thisx, PlayState *play2)
{
  BgDblueElevator *this = (BgDblueElevator *) thisx;
  PlayState *play = play2;
  s32 sp2C;
  BgDBlueElevatorStruct1 *temp_v1;
  s32 temp_v0;
  sp2C = (((s16) this->dyna.actor.params) >> 8) & 0x3;
  Actor_ProcessInitChain(&this->dyna.actor, D_80B929EC);
  DynaPolyActor_Init(&this->dyna, 1 << 0);
  temp_v1 = &D_80B92960[sp2C];
  DynaPolyActor_LoadMesh(play, &this->dyna, (CollisionHeader *) (&D_060005C4));
  temp_v0 = temp_v1->unk4(this, play);
 if (temp_v0 == 2) { 
    this->unk168[0] = -temp_v1->unkD;
  }
  else
  {
    this->unk168[0] = temp_v1->unkD;
  }
  func_80B91F20(this, play);
  if ((temp_v0 == 0) || (temp_v0 == 3))
  {
    func_80B924DC(this);
  }
  else
  {
    func_80B92644(this);
  }
}

void BgDblueElevator_Destroy(Actor* thisx, PlayState *play) {
    BgDblueElevator* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void func_80B924DC(BgDblueElevator *arg0) {
    arg0->unk168[1] = 0x3C;
    arg0->actionFunc = func_80B924F8;
}


void func_80B924F8(BgDblueElevator *arg0, PlayState *play) { 
    s32 temp_v0; 
    s32 index = ((s16) arg0->dyna.actor.params >> 8) & 3;
    
    temp_v0 = (&D_80B92960[index])->unk4(arg0, play);
    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        arg0->unk168[1] = 60;
    } else {
        arg0->unk168[1] -= 1;
        if (arg0->unk168[1] <= 0) {
            func_80B92644(arg0);
        }
    }
    
}

void func_80B9257C(BgDblueElevator *arg0) {
  s8 *new_var2;
  s32 new_var;
  new_var = (arg0->dyna.actor.params >> 8) & 3; //would % 4 work too instead of & 3?
  new_var2 = (s8*) &(D_80B92960[0].unkC);
  new_var2 = new_var2 + (new_var * 0x1C);
  arg0->unk16A = *new_var2;
  arg0->actionFunc = func_80B925B8;
}

void func_80B925B8(BgDblueElevator *arg0, PlayState *play) {
    s32 temp_v0;

    s32 index = ((s16) arg0->dyna.actor.params >> 8) & 3;
    
    temp_v0 = (&D_80B92960[index])->unk4(arg0, play);
    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        func_80B924DC(arg0);
        return;
    }
    arg0->unk16A -= 1;
    if (arg0->unk16A <= 0) {
        func_80B92644(arg0);
    }
}

void func_80B92644(BgDblueElevator *this) {
    // BgDblueElevator* this = THIS;
    
    this->actionFunc = func_80B92660;
    this->unk160 = 0.0f;
}

void func_80B92660(BgDblueElevator *this, PlayState *play)
{
  BgDBlueElevatorStruct1 *temp_v1;
  s32 new_var2;
  s32 temp_v0;
  
  f32 var_fa0;
  s32 sp5C;
  s32 sp58;
  f32 var_fv1;
  
  Vec3f sp48;
  Vec3f sp3C;
  f32 var_fv1_2;

  s32 pad;
  new_var2 = (this->dyna.actor.params >> 8) & 3;
  temp_v1 = &D_80B92960[new_var2];
  temp_v0 = (temp_v1)->unk4(this, play);
  if ((temp_v0 == 0) || (temp_v0 == 3))
  {
    sp58 = Math_StepToF(&this->unk160, 0.0f, temp_v1->unk14);
  }
  else
  { 
    sp58 = 0;
    Math_StepToF(&this->unk160, temp_v1->unk18, temp_v1->unk10);
  }
  var_fa0 = this->unk168[0] > 0 ? temp_v1->unk8 : -temp_v1->unk8;
  var_fv1 = this->unk160 <= 1.1f ? 1.1f : this->unk160;
  sp5C = Math_SmoothStepToF(&this->unk164, var_fa0, 0.4f, var_fv1, 1.0f) < 0.001f ? 1 : 0;

  if (temp_v1->unk0 == 0)
  {
    this->dyna.actor.world.pos.y = this->unk164 + this->dyna.actor.home.pos.y;
    if (((this->dyna.actor.flags & 0x40) == 0x40) && (this->unk16B != 0))
    {
      if (this->unk168[0] > 0)
      {
        // var_fa0_2 = -10.0f;
        var_fv1_2 = ((this->dyna.actor.world.pos.y + (-10.0f)) - this->unk16C) * ((this->dyna.actor.prevPos.y + (-10.0f)) - this->unk16C);
      }
      else
      {
        // var_fa0_2 = -30.0f;
        var_fv1_2 = ((this->dyna.actor.world.pos.y + (-30.0f)) - this->unk16C) * ((this->dyna.actor.prevPos.y + (-30.0f)) - this->unk16C);
      }
      if (var_fv1_2 <= 0.0f)
      {
        func_80B91F74(this, play);
      }
    }
  }
  else
  {
    Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_NEW);
    sp48.x = this->unk164;
    sp48.y = 0.0f;
    sp48.z = 0.0f;
    Matrix_MultVec3f(&sp48, &sp3C);
    Math_Vec3f_Sum(&this->dyna.actor.home.pos, &sp3C, &this->dyna.actor.world.pos);
  }
  if (sp58 != 0)
  {
    func_80B924DC(this);
    return;
  }
  if (sp5C != 0)
  {
    this->unk168[0] = -this->unk168[0];
    func_80B9257C(this);
  }
}

void BgDblueElevator_Update(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;
    
    this->actionFunc(this, play);
}

void BgDblueElevator_Draw(Actor* thisx, PlayState* play) {
    BgDblueElevator* this = THIS;

    Gfx_DrawDListOpa(play, &D_060002C8);
}
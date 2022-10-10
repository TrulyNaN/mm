/*
 * File: z_bg_f40_swlift.c
 * Overlay: ovl_Bg_F40_Swlift
 * Description: Unused Stone Tower vertically-oscillating platform
 */

#include "z_bg_f40_swlift.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgF40Swlift*)thisx)

void BgF40Swlift_Init(Actor* thisx, PlayState* play);
void BgF40Swlift_Destroy(Actor* thisx, PlayState* play);
void BgF40Swlift_Update(Actor* thisx, PlayState* play);
void BgF40Swlift_Draw(Actor* thisx, PlayState* play);

#if 0
const ActorInit Bg_F40_Swlift_InitVars = {
    ACTOR_BG_F40_SWLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_F40_OBJ,
    sizeof(BgF40Swlift),
    (ActorFunc)BgF40Swlift_Init,
    (ActorFunc)BgF40Swlift_Destroy,
    (ActorFunc)BgF40Swlift_Update,
    (ActorFunc)BgF40Swlift_Draw,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_8096F540[] = {
    ICHAIN_F32(uncullZoneScale, 550, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 5000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#endif

extern InitChainEntry D_8096F540[];

extern UNK_TYPE D_06003B08;
extern UNK_TYPE D_06003E80;

extern s32 D_8096F510[];  //= { 0xFF };
extern s32 D_8096F514[3]; // = { 0xFF, 0xFF, 0xFF };
extern s32 D_8096F5D0[];

void BgF40Swlift_Init(Actor* thisx, PlayState* play) {
    s32 temp_v1;
    BgF40Swlift* this = (BgF40Swlift*)thisx;

    Actor_ProcessInitChain(&this->actor, D_8096F540);
    DynaPolyActor_Init((DynaPolyActor*)this, 1);
    temp_v1 = ((s32)this->actor.params >> 8) & 0xFF;
    if ((temp_v1 < 0) || (temp_v1 >= 5)) {
        Actor_MarkForDeath(&this->actor);
    } else {
        D_8096F5D0[temp_v1] = (s32)this->actor.world.pos.y;
        D_8096F510[temp_v1] = this->actor.params & 0xFF;
        if (temp_v1 != 0) {
            Actor_MarkForDeath(&this->actor);
        } else {
            DynaPolyActor_LoadMesh(play, (DynaPolyActor*)this, (CollisionHeader*)&D_06003E80);
            this->actor.params = 0;
        }
    }
}

void BgF40Swlift_Destroy(Actor* thisx, PlayState* play) {
    BgF40Swlift* this = (BgF40Swlift*)thisx;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->unk144);
} 

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Swlift/BgF40Swlift_Update.s")

// void BgF40Swlift_Update(Actor* thisx, PlayState* play) {
//     f32 sp30;
//     f32 temp_fv1;
//     s32 temp_s3_2;
//     s32* temp_s0;
//     s32 temp_a1;
//     s32 temp_t4;
//     s32 temp_v0;
//     s32* phi_s0;
//     // s32 phi_s3;
//     s32 phi_s1;
//     f32 phi_fv1;
//     s32 phi_v0;
//     f32 phi_fv0;
//     s32 i;
//     s32 temp;
//     BgF40Swlift* this = (BgF40Swlift*)thisx;
//     // phi_s0 = D_8096F514;
//     // phi_s3 = 1;
//     phi_s1 = 4;
    

// for(i = 1; i <= ARRAY_COUNT(D_8096F514); i++){
//     temp = D_8096F514[i];
//     if((temp == 0xFF) || (Flags_GetSwitch(play, temp) == 0)){
//         break;
//     }
//     phi_s1 +=4;   
// }

// // loop_1:
// //     temp_a1 = *phi_s0;

// //     if (temp_a1 != 0xFF) {
// //         phi_s0++;
// //         if (Flags_GetSwitch(play, temp_a1) != 0) {
// //             phi_s3 = phi_s3 + 1;
// //             phi_s1 += 4;
// //             if (phi_s3 < 4) {
// //                 goto loop_1;
// //             }
// //         }
// //     }
    
//     i--;
//     phi_s1-=4;
//     if (i!= this->actor.params) {
//         this->actor.params = -1;
//         temp_s0 = (phi_s1 - 4) + D_8096F5D0;
//         temp_fv1 = (((f32)(*temp_s0)) - this->actor.world.pos.y) * 0.1f;
//         if (temp_fv1 > 0.0f) {
//             if (temp_fv1 < 0.5f) {
//                 phi_fv1 = 0.5f;
//             } else if (temp_fv1 > 15.0f) {
//                 phi_fv1 = 15.0f;
//             } else {
//                 phi_fv0 = temp_fv1;
//             }
//         } else if (temp_fv1 < (-15.0f)) {
//             phi_fv0 = -15.0f;
//         } else if (temp_fv1 > (-0.5f)) {
//             phi_fv0 = -0.5f;
//         } else {
//             phi_fv0 = temp_fv1;
//         }
//         phi_fv1 = phi_fv0;
//         sp30 = phi_fv1;
//         if ((Math_StepToF(&this->actor.speedXZ, phi_fv1, 1.0f) != 0) && (fabsf(phi_fv1) <= 0.5f)) {
//             if (Math_StepToF(&this->actor.world.pos.y, (f32)(*temp_s0), fabsf(this->actor.speedXZ)) != 0) {
//                 this->actor.params = temp_s3_2;
//                 this->unk15C = 0x30;
//                 this->actor.speedXZ = 0.0f;
//             }
//         } else {
//             this->actor.world.pos.y += this->actor.speedXZ;
//         }
//     } else {
//         temp_v0 = this->unk15C;
//         phi_v0 = temp_v0;
//         if (temp_v0 == 0) {
//             this->unk15C = 0x30;
//             phi_v0 = 0x30;
//         }
//         temp_t4 = phi_v0 - 1;
//         this->unk15C = temp_t4;
//         this->actor.world.pos.y = (sin_rad(((f32)temp_t4) * 0.1308997f) * 5.0f) + ((f32)D_8096F5D0[this->actor.params]);
//     }
// }

void BgF40Swlift_Draw(Actor* thisx, PlayState* play) {
    BgF40Swlift* this = THIS;
    Gfx_DrawDListOpa(play, (Gfx*)&D_06003B08);
}

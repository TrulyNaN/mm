/*
 * File: z_obj_danpeilift.c
 * Overlay: ovl_Obj_Danpeilift
 * Description: Deku Shrine & Snowhead Temple floating blocks
 */

#include "z_obj_danpeilift.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjDanpeilift*)thisx)

void ObjDanpeilift_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjDanpeilift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjDanpeilift_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjDanpeilift_Draw(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit Obj_Danpeilift_InitVars = {
    ACTOR_OBJ_DANPEILIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_OBJ_DANPEILIFT,
    sizeof(ObjDanpeilift),
    (ActorFunc)ObjDanpeilift_Init,
    (ActorFunc)ObjDanpeilift_Destroy,
    (ActorFunc)ObjDanpeilift_Update,
    (ActorFunc)ObjDanpeilift_Draw,
};
*/

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/func_80C1ADC0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/ObjDanpeilift_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/ObjDanpeilift_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/func_80C1AF84.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/func_80C1AF94.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/func_80C1B210.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/func_80C1B26C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/ObjDanpeilift_Update.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Obj_Danpeilift_0x80C1ADC0/ObjDanpeilift_Draw.asm")

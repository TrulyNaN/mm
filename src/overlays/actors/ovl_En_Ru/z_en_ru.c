/*
 * File: z_en_ru.c
 * Overlay: ovl_En_Ru
 * Description: OoT's Adult Ruto (unused)
 */

#include "z_en_ru.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10)

#define THIS ((EnRu*)thisx)

void EnRu_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRu_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnRu_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRu_Draw(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit En_Ru_InitVars = {
    ACTOR_EN_RU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RU2,
    sizeof(EnRu),
    (ActorFunc)EnRu_Init,
    (ActorFunc)EnRu_Destroy,
    (ActorFunc)EnRu_Update,
    (ActorFunc)EnRu_Draw,
};
*/

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A389A0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A38A68.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A38B7C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A38BF0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A38C70.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A38DF4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/EnRu_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/EnRu_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/EnRu_Update.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A38FB4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/func_80A390F8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Ru_0x80A389A0/EnRu_Draw.asm")

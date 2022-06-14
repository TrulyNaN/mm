/*
 * File: z_eff_change.c
 * Overlay: ovl_Eff_Change
 * Description: Elegy of Emptiness - Beam of Light When Creating Statue
 */

#include "z_eff_change.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((EffChange*)thisx)

void EffChange_Init(Actor* thisx, GlobalContext* globalCtx);
void EffChange_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EffChange_Update(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit Eff_Change_InitVars = {
    ACTOR_EFF_CHANGE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EffChange),
    (ActorFunc)EffChange_Init,
    (ActorFunc)EffChange_Destroy,
    (ActorFunc)EffChange_Update,
    (ActorFunc)NULL,
};
*/

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Eff_Change_0x80A4C490/EffChange_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Eff_Change_0x80A4C490/EffChange_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Eff_Change_0x80A4C490/func_80A4C578.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Eff_Change_0x80A4C490/func_80A4C5CC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Eff_Change_0x80A4C490/EffChange_Update.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Eff_Change_0x80A4C490/func_80A4C7B0.asm")

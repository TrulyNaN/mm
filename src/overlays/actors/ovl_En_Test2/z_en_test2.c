#include "z_en_test2.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_80)

#define THIS ((EnTest2*)thisx)

void EnTest2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTest2_Update(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit En_Test2_InitVars = {
    ACTOR_EN_TEST2,
    ACTORCAT_BG,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnTest2),
    (ActorFunc)EnTest2_Init,
    (ActorFunc)func_800BDFB0,
    (ActorFunc)EnTest2_Update,
    (ActorFunc)NULL,
};
*/

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Test2_0x80A3E390/EnTest2_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Test2_0x80A3E390/EnTest2_Update.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Test2_0x80A3E390/func_80A3E4EC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_En_Test2_0x80A3E390/func_80A3E524.asm")

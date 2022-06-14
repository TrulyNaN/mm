/*
 * File: z_elf_msg5.c
 * Overlay: ovl_Elf_Msg5
 * Description: Tatl Message (Another proximity-activated dialogue?)
 */

#include "z_elf_msg5.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ElfMsg5*)thisx)

void ElfMsg5_Init(Actor* thisx, GlobalContext* globalCtx);
void ElfMsg5_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ElfMsg5_Update(Actor* thisx, GlobalContext* globalCtx);

/*
const ActorInit Elf_Msg5_InitVars = {
    ACTOR_ELF_MSG5,
    ACTORCAT_BG,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ElfMsg5),
    (ActorFunc)ElfMsg5_Init,
    (ActorFunc)ElfMsg5_Destroy,
    (ActorFunc)ElfMsg5_Update,
    (ActorFunc)NULL,
};
*/

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Elf_Msg5_0x80AFD990/func_80AFD990.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Elf_Msg5_0x80AFD990/ElfMsg5_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Elf_Msg5_0x80AFD990/ElfMsg5_Destroy.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Elf_Msg5_0x80AFD990/func_80AFDB38.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/overlays/ovl_Elf_Msg5_0x80AFD990/ElfMsg5_Update.asm")

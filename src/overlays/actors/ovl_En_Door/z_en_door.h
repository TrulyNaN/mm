#ifndef Z_EN_DOOR_H
#define Z_EN_DOOR_H

#include <global.h>

struct EnDoor;

typedef struct EnDoor {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x88];
} EnDoor; // size = 0x1CC

extern const ActorInit En_Door_InitVars;

#endif // Z_EN_DOOR_H

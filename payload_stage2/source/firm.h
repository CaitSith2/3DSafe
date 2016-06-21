/*
*   firm.h
*/

#pragma once

#include "types.h"

#define PDN_MPCORE_CFG  (*(vu32 *)0x10140FFC)
#define PAYLOAD_ADDRESS 0x23F00000

//FIRM Header layout
typedef struct firmSectionHeader {
    u32 offset;
    u8 *address;
    u32 size;
    u32 procType;
    u8 hash[0x20];
} firmSectionHeader;

typedef struct firmHeader {
    u32 magic;
    u32 reserved1;
    u8 *arm11Entry;
    u8 *arm9Entry;
    u8 reserved2[0x30];
    firmSectionHeader section[4];
} firmHeader;

void loadFirm(void);
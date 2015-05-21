#pragma once

/*
        Before reverse engineering new addresses,
        check this thread first:
        http://gtamp.com/forum/viewtopic.php?t=700

        All addresses in this file were found by
        robotanarchy, except for the following.

        Credit to Sektor (gtamp.com):
                GTA2_MPADDR_KILLS
                GTA2_MPADDR_POINTS
*/

char *EXE_OFFSET;
char MP_PLAYER_ID; // 0...5

//
// Addresses that work in both single- (SP) and multiplayer (MP)
//

#define GTA2_ADDR_PLAYER_IN_VEHICLE (char *)0x5e20bc

// See also: native/cmap/cmap_gta2_controls.h
#define GTA2_ADDR_MOVEMENT (uint16_t *)0x5ecacc

// 1: visible, 0: invisible
#define GTA2_ADDR_ESC_TEXT_IS_VISIBLE                                          \
  (char *)(*((uint32_t *)0x5EB4FC + 14) + 1930)

// works for english language. the text is in unicode format (this means
// every second byte is 00!)
#define GTA2_ADDR_ESC_TEXT_LINE_1 (char *)0x03b1abb2 // len: 11
#define GTA2_ADDR_ESC_TEXT_LINE_2 (char *)0x03b1abca // len: 33
#define GTA2_ADDR_ESC_TEXT_LINE_3 (char *)0x03b1ac0e // len: 33

// http://git.io/g2hr-rumble-byte
#define GTA2_ADDR_RUMBLE (char *)0x665770

//
// Different for SP and MP
//

// NOTE: this one only works for the first game started.
// (but G2HR implements its own menu, so that's not an issue :))
#define GTA2_ADDRSP_SCORE (int *)(*((char **)(EXE_OFFSET + 0x1E8DC0)) + 0x188)

#define GTA2_ADDRMP_SCORE (int *)((char *)0x5EC50C + MP_PLAYER_ID * 0x04);
#define GTA2_ADDRMP_KILLS (char *)0x5EC4BA + MP_PLAYER_ID * 0x0A;
#define GTA2_ADDRMP_POINTS (char *)0x5EC500 + MP_PLAYER_ID * 0x02;

/*
        GTA2 Text adresses all have the folowing layout:
                TIMER 0x00 TEXT_AS_UNICODE
        Example for TIMER=90 and the string HELLO:
                63 00 48 00 65 00 6C 00 6C 00 6F 00
        The timer automatatically runs down from the specified value.

        We'll need to find real pointers, these don't work.
*/
// #define GTA2_ADDR_TEXT_BASE (*(void**)0x00282F40)
// #define GTA2_OFFSET_TEXT_ITEM 0x27b8

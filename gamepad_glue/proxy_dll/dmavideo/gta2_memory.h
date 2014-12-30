#pragma once

/*
        Before reverse engineering new addresses,
        check this thread first:
        http://gtamp.com/forum/viewtopic.php?t=700
*/

/*
        Addresses found by robotanarchy
*/

#define GTA2_ADDR_SCORE (int *)(*((int **)0x1F5BBC) + 0x2d4)

#define GTA2_ADDR_PLAYER_IN_VEHICLE (char *)0x5e20bc

// See also: sdl_controller_code/gta2_controls.h
#define GTA2_ADDR_MOVEMENT (char *)0x5ecacc

// http://git.io/g2hr-rumble-byte
#define GTA2_ADDR_RUMBLE (char *)0x665770

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
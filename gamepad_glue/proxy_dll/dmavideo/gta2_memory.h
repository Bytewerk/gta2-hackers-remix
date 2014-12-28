#pragma once

// See also: sdl_controller_code/gta2_controls.h
#define GTA2_ADDR_MOVEMENT (char *)0x5ecacc

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
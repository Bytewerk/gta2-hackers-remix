#include "net/injected_net.h"
#include "thread/thread.h"
#include <process.h>
#include <stdio.h>
#include <windows.h>

// global variables: hold a reference to the original DLL (so we can un-
// load it later) and addresses of the functions we need to proxy
HINSTANCE original_dll = 0;
FARPROC p[22] = {0};

BOOL WINAPI DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID userdata) {
  if (reason == DLL_PROCESS_ATTACH) {
    // redirect stdout to a logfile for easier debugging
    freopen("injected.log", "a", stdout);
    setbuf(stdout, NULL);

    // Load the real dmavideo.dll library (which must be named
    // dmavideo_original.dll now)
    original_dll = LoadLibrary("dmavideo_original.dll");
    if (!original_dll)
      return 0;

    // Get all function addresses in that DLL-file
    p[0] = GetProcAddress(original_dll, "Vid_CheckMode");
    p[1] = GetProcAddress(original_dll, "Vid_ClearScreen");
    p[2] = GetProcAddress(original_dll, "Vid_CloseScreen");
    p[3] = GetProcAddress(original_dll, "Vid_DisableWrites");
    p[4] = GetProcAddress(original_dll, "Vid_EnableWrites");
    p[5] = GetProcAddress(original_dll, "Vid_FindDevice");
    p[6] = GetProcAddress(original_dll, "Vid_FindFirstMode");
    p[7] = GetProcAddress(original_dll, "Vid_FindMode");
    p[8] = GetProcAddress(original_dll, "Vid_FindNextMode");
    p[9] = GetProcAddress(original_dll, "Vid_FlipBuffers");
    p[10] = GetProcAddress(original_dll, "Vid_FreeSurface");
    p[11] = GetProcAddress(original_dll, "Vid_GetSurface");
    p[12] = GetProcAddress(original_dll, "Vid_GetVersion");
    p[13] = GetProcAddress(original_dll, "Vid_GrabSurface");
    p[14] = GetProcAddress(original_dll, "Vid_InitDLL");
    p[15] = GetProcAddress(original_dll, "Vid_Init_SYS");
    p[16] = GetProcAddress(original_dll, "Vid_ReleaseSurface");
    p[17] = GetProcAddress(original_dll, "Vid_SetDevice");
    p[18] = GetProcAddress(original_dll, "Vid_SetGamma");
    p[19] = GetProcAddress(original_dll, "Vid_SetMode");
    p[20] = GetProcAddress(original_dll, "Vid_ShutDown_SYS");
    p[21] = GetProcAddress(original_dll, "Vid_WindowProc");

    // Start the injected thread (see thread/thread.c)
    _beginthread(injected_thread, 0, NULL);
  } else if (reason == DLL_PROCESS_DETACH) {
    net_cleanup();
    FreeLibrary(original_dll);
    fclose(stdout);
  }

  return 1;
}

/*
        The code below is inline assembly that redirects all function calls
        to the same functions of the original dll (which gets loaded, as
        soon as this DLL gets attached). The assembly jump makes sure, that
        the full stack gets preserved, just as if the original function was
        called.

        The PROXY macro below was basically written by Jester01	at freenode
        (also known as "Jester" at stackoverflow) - only with small changes.
        Without it, the build system would still require the MSVC compiler,
        so big thanks for that! He gave permission to use that code under
        the GPLv2+ license.
*/
#define str(x) #x
#define PROXY(INDEX, NAME)                                                     \
  __asm__(".globl _" str(                                                      \
      NAME) "\n\t"                                                             \
            "_" str(NAME) ":\n\t"                                              \
                          "jmp *_p + " str(                                    \
                              INDEX) " * 4\n\t"                                \
                                     ".section .drectve\n\t"                   \
                                     ".ascii \" -export:\\\"" str(             \
                                         NAME) "\\\"\"\n\t"                    \
                                               ".section .text\n\t");

PROXY(0, Vid_CheckMode);
PROXY(1, Vid_ClearScreen);
PROXY(2, Vid_CloseScreen);
PROXY(3, Vid_DisableWrites);
PROXY(4, Vid_EnableWrites);
PROXY(5, Vid_FindDevice);
PROXY(6, Vid_FindFirstMode);
PROXY(7, Vid_FindMode);
PROXY(8, Vid_FindNextMode);
PROXY(9, Vid_FlipBuffers);
PROXY(10, Vid_FreeSurface);
PROXY(11, Vid_GetSurface);
PROXY(12, Vid_GetVersion);
PROXY(13, Vid_GrabSurface);
PROXY(14, Vid_InitDLL);
PROXY(15, Vid_Init_SYS);
PROXY(16, Vid_ReleaseSurface);
PROXY(17, Vid_SetDevice);
PROXY(18, Vid_SetGamma);
PROXY(19, Vid_SetMode);
PROXY(20, Vid_ShutDown_SYS);
PROXY(21, Vid_WindowProc);

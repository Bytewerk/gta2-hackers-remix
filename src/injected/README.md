# Injected Component (proxy_dll)

## What is a Proxy DLL?

A `Proxy DLL` is a shared Windows library `(DLL)`, that has the same interface as the `DLL`-file it imitates and redirects all functions to the original `DLL` (just like a web proxy, hence the name). Just like the original library, it gets loaded in the address space of the process, that opens it. Here, we can modify the behaviour of the process!

## Why dmavideo.dll?
After trying out various DLLs in the GTA2 folder, `dmavideo.dll` seems to be the best fit. It gets attached, when `GTA2.exe` starts (be careful, it also starts with the launcher), and not de-attached before `GTA2.exe` quits.

Rename `dmavideo.dll` to `dmavideo_original.dll`, compile the `proxy_dll` to `dmavideo.dll` and then copy it into the GTA2 game folder.

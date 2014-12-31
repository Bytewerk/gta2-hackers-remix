# proxy_dll
If you want to know, why this is necessary, read the [gamepad_glue](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue) `README` first.

## What is a Proxy DLL?

A `Proxy DLL` is a shared Windows library `(DLL)`, that has the same interface as the `DLL`-file it imitates and redirects all functions to the original `DLL` (just like a web proxy, hence the name). Just like the original library, it gets loaded in the address space of the process, that opens it. Here, we can modify the behaviour of the process!

Michael Chourdakis has automatized the creation of such a proxy dll with his tool [wrappit](http://www.codeproject.com/Articles/16541/Create-your-Proxy-DLLs-automatically), at least for the closed source Visual C++ IDE. Since we don't know any better, this is what we'll use (if you happen to know how to compile the code in this folder with MinGW, please write this in the bugtracker).

## Why dmavideo.dll?
After trying out various DLLs in the GTA2 folder, `dmavideo.dll` seems to be the best fit. It gets attached, when `GTA2.exe` starts (be careful, it also starts with the launcher), and not de-attached before `GTA2.exe` quits.

Rename `dmavideo.dll` to `dmavideo_original.dll`, compile the `proxy_dll` to `dmavideo.dll` and then copy it into the GTA2 game folder.

## What does it currently do?
It starts a TCP server on one of the ports in the range 19990-19995 (it tries them out one after another and uses the first one that is free).
As soon, as a connection is established, it stuffs each incoming pair of bytes (=> always 2 at the same time!) into the address, that stores the player movement in GTA2.exe, overwriting the previous two bytes. See [here](https://github.com/Bytewerk/gta2-hackers-remix/blob/master/gamepad_glue/sdl_controller_code/gta2_controls.h) for a list of possible movement codes.

When the connection goes down, a new TCP server gets started (on the next free port). This way we can change the [SDL controller code](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue/sdl_controller_code) without restarting GTA2 each time!

# Development
Grab the project files from [this release](https://github.com/Bytewerk/gta2-hackers-remix/releases/tag/0.0.3-IDE-projects) and import them with the *freeware-unless-you-got-a-bigass-company* [Visual Studio 2013 Community Edition](http://www.visualstudio.com/news/vs2013-community-vs).
If you already have the 2010 version installed, you can use the project files from [here](https://github.com/Bytewerk/gta2-hackers-remix/releases/tag/0.0.1-proxy_dll).



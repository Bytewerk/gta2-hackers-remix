# SDL Controller Code
This program connects to the [proxy_dll](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue/proxy_dll) via TCP. It also connects to an XInput compatible controller and - based on what the player does with it (press buttons etc.) - calculates the [two movement bytes](https://github.com/Bytewerk/gta2-hackers-remix/blob/master/gamepad_glue/sdl_controller_code/gta2_controls.h) for GTA2 and sends those back to the DLL (which will in turn put these in the right location in the RAM).

Currently this only works for one controller, but the idea is, that we can have up to 6 different controllers (that's the limit of players in one GTA2 network game), with each of them connecting to its own GTA2 instance. This will get relevant for the [splitscreen_mod](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/splitscreen_mod).


## Limitations
The menu can not be controlled with the current code (we want an own menu for the splitscreen_mod anyway).

# IDE Setup
## Why Code::Blocks?

This part of the code is being developed with [Code::Blocks](http://www.codeblocks.org/) on Windows. Why Code::Blocks instead of Visual C++? Because it is OSS, has less bloat and ships a working MinGW toolchain.

It should theoretically compile on Linux too, but we'll need to figure out how much sense that makes, considering we'll need Wine for GTA2 anyway.

## Setup

* [Download Code::Blocks](http://www.codeblocks.org/downloads/26#windows) (the one that has MinGW included) and install it
* [Download SDL2](http://libsdl.org/download-2.0.php) development libraries for MinGWx32
* [Download SDL2_net](http://www.libsdl.org/projects/SDL_net/) *for 32-bit Windows*
* Extract the `bin`, `include`, `lib` and `share` folders from the SDL2 and SDL2_net archives to `%Pogram Files%/Codeblocks/MinGW/` (merge existing folders)
* Start Code::Blocks
* Create a new C-Project (console program, **not** SDL Project - this doesn't work with SDL2)
* Open up the linker settings: `Project build options` / *click the parent entry on the left, that contains Debug and Release* / `Linker Settings`
* Link with: `mingw32`, `SDL2main`, `SDL2`, `SDL2_net`
* In the compiler settings, add the flag: `-std=c99`

## Fix SDL 2.0.3
You'll probably get the error `winapifamily.h: No such file or directory`, when you try to compile.
This is a bug in the mentioned SDL version, [here's the fix.](http://stackoverflow.com/a/22600114)

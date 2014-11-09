# IDE Setup
## Why Code::Blocks?

This part of the code is being developed with [Code::Blocks](http://www.codeblocks.org/) on Windows. Why Code::Blocks instead of Visual C++? Because it is OSS, has less bloat and ships a working MinGW toolchain

It should theoretically compile on Linux too, but we'll need to figure out how much sense that makes, considering we'll need Wine for GTA2 anyway.

## Setup

* [Download Code::Blocks](http://www.codeblocks.org/downloads/26#windows) (the one that has MinGW included) and install it
* [Download SDL2](http://libsdl.org/download-2.0.php) development libraries for MinGWx32
* [Download SDL2_net](http://www.libsdl.org/projects/SDL_net/) *for 32-bit Windows*
* Extract the `bin/`, `include/`, `lib/` and `share/` folders from the SDL2 and SDL2_net archives to `%Pogram Files%/Codeblocks/MinGW` (merge existing folders)
* Start Code::Blocks
* Create a new C-Project (console program, **not** SDL Project - this doesn't work with SDL2)
* Open up the linker settings: `Project build options` / *click the parent entry on the left, that contains Debug and Release* / `Linker Settings`
* Link with: `mingw32`, `SDL2main`, `SDL`
* In the compiler settings, add the flag: `-std=c99`

## Fix SDL 2.0.3
You'll probably get the error `winapifamily.h: No such file or directory`, when you try to compile.
This is a bug in the mentioned SDL version, [here's the fix.](http://stackoverflow.com/q/22446008/)

# splitscreen_mod
This depends on [gamepad_glue](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue).

## Why AutoIt3?
To make this mod happen, we need to set up and control (that is, automating GUI clicking) the network lobby of up to 6 instances of GTA2.exe in the background. We also need to change the GTA2 registry entries and do lots of other Windows-API-spcific stuff to merge all GTA2 windows into one big window.

[AutoIt3](http://autoitscript.com) abstracts the Windows API to a perfectly documented scripting language. We don't need to install a big IDE like Visual Studio, and while AutoIt3 is not open source for [valid reasons](http://www.wikivs.com/wiki/Autohotkey_vs_AutoIt_v3), it is freeware.

## Why is there no code yet?
In 2012 and 2013, there were already two versions of the code (with quite a lot lines of code). Now even more time has passed, and the last missing piece to stick it all together, which is the [gamepad_glue](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue), is finally present.
The new splitscren_mod will be rewritten from scratch and directly focus on integrating that new controller code.

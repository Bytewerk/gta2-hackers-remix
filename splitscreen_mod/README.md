# splitscreen_mod
This depends on [gamepad_glue](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue).

## Why AutoIt3?
To make this mod happen, we need to set up and control (that is, automating GUI clicking) the network lobby of up to 6 instances of GTA2.exe in the background. We also need to change the GTA2 registry entries and do lots of other Windows-API-spcific stuff to merge all GTA2 windows into one big window.

[AutoIt3](http://autoitscript.com) abstracts the Windows API to a perfectly documented scripting language. We don't need to install a big IDE like Visual Studio, and while AutoIt3 is not open source for [valid reasons](http://www.wikivs.com/wiki/Autohotkey_vs_AutoIt_v3), it is freeware.

## Setup Guide
AutoIt3 is just a scripting language that is mostly easy to use and has a very good documentation (`F1` for help in SciTE). Even if you have very little programming knowledge, it might be just enough to start hacking on this mod.
* Grab and install the latest [AutoIt3](https://www.autoitscript.com/site/autoit/downloads/) and their [advanced  SciTE Editor](https://www.autoitscript.com/site/autoit-script-editor/downloads/)
* Download the `splitscreen_mod` source with [Github for Windows](http://windows.github.com/)
* Copy all .exe and .dll files from the bin folder in the [latest release](https://github.com/Bytewerk/gta2-hackers-remix/releases/tag/0.2.0-splitscreen_mod) to the bin folder of the source code you just downloaded
* Start up AutoIt3's SciTE editor and begin hacking on the .au3 files
* Start splitscreen_mod.au3 (shortcut: F5 when open) to run the mod
* Share what you have developed :)

## Current status
There's an [alpha Windows GUI](https://github.com/Bytewerk/gta2-hackers-remix/issues/1#issuecomment-64163492) (we're aiming for a fullscreen SDL GUI that can be controlled with a gamepad later!), that can actually start up to 6 instances of GTA2 and set the right screen resolutions in each instance.

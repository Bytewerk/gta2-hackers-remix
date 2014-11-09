# Gamepad Glue
There are lots of possibilities to send fake input in a Windows application. It works [pretty well,](https://www.autoitscript.com/autoit3/docs/functions/Send.htm) when the window is in focus. If one just wants to do that, he can use an existing application like [Xpadder](http://xpadder.com/) (Warning: not tested with GTA2!) for this purpose.

However, this project is building up to the [splitscreen_mod](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/splitscreen_mod), which will start up to 6 instances of GTA2 at the same time. So we can say for sure, that all windows will never be in focus at the same time and the easy approach above will certainly not work.

After trying out all possible Windows APIs and AutoIt3 UDFs, sending keys to windows which are not in focus didn't work for GTA2 (it does with some other programs, though).

So we're left with the traditional Windows memory hacking. Again, lots of options here, but the only one which does *not* require administrator access and really works with Wine on Linux (!) is creating a [proxy_dll](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue/proxy_dll).

As we only want to make as few changes as necessary to the GTA2.exe process, the proxy_dll itself opens a TCP server and lets the [sdl_controller_code](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue/sdl_controller_code) talk to your gamepad.

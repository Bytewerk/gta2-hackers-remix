
![2014-12-01 splitscreen gamepad glue fancy debug window](https://cloud.githubusercontent.com/assets/7833187/5239777/1b5f3b2a-78ee-11e4-8992-4a47f8038041.jpg)


# GTA2 Hacker's Remix
This is a meta-mod for *Grand Theft Auto 2* (1999), which has an all new **splitscreen mode** as its main feature. In the current state, all players need **Xbox360 controllers** (or compatible). The whole mod is designed to run on both Windows and **Linux** (partially via Wine).
Check out the **alpha** version, which already **works pretty well.** Behind the scenes, a **[post-alpha](https://github.com/Bytewerk/gta2-hackers-remix/tree/post-alpha)** version is being developed with a full reimplementation of the original GTA2 menu that will incorperate the new features (controlling it via gamepad and setting up splitscreen games).

### Links
* **[get the alpha](https://github.com/Bytewerk/gta2-hackers-remix/releases/tag/0.3.2-splitscreen_mod)** ***(0.3.2)***
* **[screenshots](https://github.com/Bytewerk/gta2-hackers-remix/issues/1)** *(and a video, scroll down!)*
* **[bytewerk wiki](http://wiki.bytewerk.org/index.php/GTA2_Hackers_Remix)** *(german)*
* **[gtamp.com thread](http://gtamp.com/forum/viewtopic.php?f=4&t=776)**
* **[tracker](https://github.com/Bytewerk/gta2-hackers-remix/issues)**

#### Technical Details

*Note: The current development code has a [different code structure](https://github.com/Bytewerk/gta2-hackers-remix/wiki/Post-Alpha-Code-Structure), where both mods (along with the new menu) are more tightly integrated.*

Mod | Description | Language
----|-------------|------------
[gamepad_glue](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/gamepad_glue) | Xbox360-Controller support (and compatible) | C, C++
[splitscreen_mod](https://github.com/Bytewerk/gta2-hackers-remix/tree/master/splitscreen_mod) | Ultimate up-to-6-players-on-one-PC party hack | AutoIt3

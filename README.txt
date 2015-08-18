------------------------------------------------------------------------
                      GTA2: Hacker's Remix (G2HR)
                       0.4.0 (technical preview)
                               g2hr.org
------------------------------------------------------------------------

BEWARE: The release is rushed, so it might have some (big) issues. In
case something doesn't work, you might want to play with the previous
version instead!

Please read this file carefully, as you're dealing with a technical
preview. If you want to contribute code, read the "HACKING" file. Provi-
de as much feedback as possible by opening / commenting bug reports [0]
if you find any issues, and for everything else by posting in the gtamp
forums [1].

To get the splitscreen mode with more than 3 players working, a quad-
core is recommended. Yes, it's an old game, but nevertheless it runs in
parallel for each player and there are also processes for the gamepad
input etc. If your CPU can't handle it, the game instances will simply
crash (maybe this will improve a bit with future versions?).


INSTALLATION:
 - Extract the zip file wherever you want, write access during runtime
   IS REQUIRED (this has changed!)
 - Get the latest official GTA2 release, that is free for download, and
   copy the game files in the GTA2 folder in the path where you have
   extracted the mod. More info and download links: [2]
 - Linux only: install wine, sdl2 and sdl2_net
 - Run g2hr.sh (Linux) or g2hr.exe (Windows) to start the meta-mod
 - Allow all G2HR programs (they all have G2HR as prefix) in your fire-
   wall program, in case you get asked. The mod is divided into
   multiple components, which need to connect via TCP in order to make
   everything work.

USAGE:
 - Have the task manager ready and kill all G2HR* processes manually, if
   it has crashed.
 - Even when the game has crashed, you might be able to quit all GTA2
   processes by pressing START twice on the controller, as the con-
   troller code is handled outside of the GTA2 processes.
 - Pressing ESC will not show the 'Is that it?' text anymore, but you
   can still quit the game with ESC and ENTER if it has focus

CHANGES in 0.4.0 (2015-08-18)
	- Re-implemented the original GTA2 menu with SDL2 (not all screens
		work as in the original yet, eg. score, resume savegames,
		save and delete player names, cheats, ...)
	- The menu can now be accessed with game controllers
	- Add a slotmachine mode, that shows a "poweroff" and "reboot"
		menu entry instead of "quit" (see data/g2hr.ini)
	- custom controller configs (data/controller-mappings, they might be
	  incomplete/buggy!)
	- in-game text notifications, when the user changes the controller
		mapping
	- the mod contains Vike the Hube's enhanched GTA2 patch now (see [5]
		for more info)
	- added optional, non-annoying update notifications (when a new up-
		date gets released, you see one line of text in the main menu
		screen of the game if you have enabled the update notifications.
		you get asked at the first start of the mod and the checks run
		daily and only when you open the game)

CHANGES in 0.3.2 (2015-01-03)
	- Mod doesn't need to be started multiple times, before it
		works (fixes [3])
	- Rumble signal when the controller code is ready
	- sdl_controller_code shuts down properly
	- detect when player is in a car (could be used for new
		controller mappings!) and lots of other technical
		improvements that future versions will make use of
		(new 'injected API' network protocol etc.)

CHANGES in 0.3.1 (2014-12-23):
	- Many fixes/additions for Linux (Wine) compatibility.
		Make sure that you read the LINUX NOTES below, if you're
		using Linux.
	- Move the cursor off-screen, before merging windows

CHANGES in 0.3.0 (2014-12-20):
	- the game settings are not replicated in the alpha
		gui anymore. The host window will stay open
		until you manually click the 'Start' button, so
		you can configure it there.
	- all network windows except for the one from the
		host are hidden now
	- added some mod options: you can choose if the windows
		should be merged at all (disable this if you can't
		get it to work otherwise), how long the mod should
		wait before merging and if the taskbar should be
		hidden (also new!)
	- all options in the GUI get saved and load from the
		settings .ini file automatically
	- added 4:3 aspect ratio layouts for all player counts.
		this is a trade of between having blank space and
		working camera when driving. In the future, we might
		be able to fix the camera for non 4:3 ratios.
	- sound should always play in all windows, even on wine
		now. if it doesn't it is possible to press F3 to
		force it again (useful when you gave and removed
		focus manually). Thanks to sqozz for figuring this out!
	- new icon

KNOWN BUGS:
 - Splitscreen options don't get saved and restored yet (but normal
   options do)
 - Camera handling needs to be fixed, the player can be off the screen
   when driving cars and for some resolutions, you see black areas, to
   which GTA2 doesn't render to [3]. To avoid this, use the 4:3 layouts
   only (these are shipped, you just need to select them)
 - Linux: The network windows can't be hidden yet, when creating a
   splitscreen game

UPGRADING from pre 0.4.0:
  You can delete everything in the %APPDATA%\GTA2 Hackers Remix\ folder,
  the application data is now stored in %APPDATA%\bytewerk\ (using a
  SDL2 API for this now, that's why).

UPGRADING from 0.1.0-gamepad_glue:
  Copying a custom dll file to 'dmavideo.dll' in the
  game folder is no longer needed. If you still have
  this set up, please delete your dmavideo.dll (which
  is the proxy dll) and rename dmavideo_original.dll
  back to dmavideo.dll.

URLS:
   [0]: http://git.io/g2hr-bugs
   [1]: http://gtamp.com/forum/viewtopic.php?t=776
   [2]: http://git.io/g2hr-get-gta2
   [3]: https://github.com/Bytewerk/gta2-hackers-remix/issues/4
   [5]: http://gtamp.com/gta2

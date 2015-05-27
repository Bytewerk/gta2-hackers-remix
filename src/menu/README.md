folder|description
------|-------
bg | Backgrounds - loads and draws original `GTA2` `TGA`-files, as well as custom `PNG`-files
hsc | Parses `GTA2` `hsc` (High Score) files (unfinished & unused)
sfx | Audio related (`raw`, `std` files from `GTA2`)
sty | Loads and displays sprites and fonts from fstyle.sty
gxt | Parses the [`GTA Text`](http://www.gtamodding.com/index.php?title=GXT#GTA2_Format) files (for displaying the original credits)
mmp | Plain text map info is stored in `mmp` files. The code in this folder loads all of those files and sorts them by any key, eg. the map name. See also: [this thread](http://gtamp.com/forum/viewtopic.php?t=188)
sl | screen layout related, for the splitscreen mod
server | TCP server for communication between 'menu' and 'meta' components
net | network code (server talks to 'meta', client talks to 'native')
rpc | Registry path changer - replaces the registry strings in GTA2.exe, so each player can have seperate registry locations (and therefore player names, and resolutions). The modified exe-files get cached.
chk | Simple update checker via HTTP. This will only run when enabled by the user, not by default!
tk | Toolkit written from scratch, that emulates the `GTA2` menu controls in `SDL2` and even adds new features. Sketches without `SDL2` and `GTA2` related code can be found in [vinylkit](https://github.com/robotanarchy/vinylkit).
ui | The actual user interface, built with the toolkit.

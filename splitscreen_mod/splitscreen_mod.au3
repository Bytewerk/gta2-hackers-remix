#include <Array.au3>
#include "include\logging.au3"
#include "include\screen_layout\screen_layout_init.au3"
#include "include\screen_layout\screen_layout_calc.au3"
#include "alphagui\alphagui.au3"
#include "include\config.au3"
#include "include\gta2_launcher\registry.au3"
#include "include\gta2_launcher\lobby.au3"
#include "include\merge.au3"

; $config: See arrays.txt
$config = config_init()


screen_layout_init()

; Show the GUI until the user starts the game.
; See arrays.txt for the $gameinfo structure.
$gameinfo = alphagui()
registry_exe_modify($gameinfo[0], $config[0])
$player_res = screen_layout_calc_array($gameinfo, $config)
registry_prepare($gameinfo, $player_res)
lobby_run($gameinfo, $config)
merge($gameinfo, $config, $player_res)
; run gamepad glue (pass stdout through)

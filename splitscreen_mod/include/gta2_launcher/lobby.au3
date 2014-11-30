#include "..\logging.au3"
#include "lobby_controls.au3"

Func lobby_run($gameinfo, $config)
	Local $cache = @AppDataDir & "\GTA2 Hackers Remix\Modified GTA2.exe Cache"

	; Working dir: GTA2.exe path, cut off at the last '\' char
	Local $workingdir = StringMid($config[0], 1, _
		StringInStr($config[0], "\", 1, -1))

	For $i = 1 To $gameinfo[0]
		Local $param = "-j 127.0.0.1"
		If $i == 1 Then $param = "-c"

		l("Launching GTA2 with parameter: "&$param, $i)
		Run($cache & "\Player" & $i & ".exe " & $param,$workingdir)

		l("Waiting for network window. This may take a while!", $i)
		WinWait("Network GTA2", "")
		WinSetTitle("Network GTA2", "", "GTA2: Player "&$i)
	Next
	l("All players have joined the lobby!")

	Local $host = "GTA2: Player 1"
	l("Configuring game...")

	; FIXME: put all of this in the registry before starting the game, it will work better!
	ControlSetText($host,"", $GTA2_LOBBY_MAP, $gameinfo[2]) ; BROKEN
	ControlSetText($host,"", $GTA2_LOBBY_GAME_TYPE, $gameinfo[3]&" Game")
	ControlSetText($host,"", $GTA2_LOBBY_WINNING_CONDITION, $gameinfo[4])
	ControlSetText($host,"", $GTA2_LOBBY_TIME_LIMIT, $gameinfo[5])
	; TODO: Cops field

	l("Launching...")
	ControlClick($host, "", $GTA2_LOBBY_START)
EndFunc
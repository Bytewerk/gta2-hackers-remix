#include "..\logging.au3"
#include "lobby_controls.au3"

; $config: see arrays.txt
Func lobby_run($config)
	Local $cache = @AppDataDir & "\GTA2 Hackers Remix\Modified GTA2.exe Cache"

	; Working dir: GTA2.exe path, cut off at the last '\' char
	Local $workingdir = StringMid($config[0], 1, _
		StringInStr($config[0], "\", 1, -1))

	For $i = 1 To $config[7]
		Local $param = "-j 127.0.0.1"
		If $i == 1 Then $param = "-c"

		l("Launching GTA2 with parameter: "&$param, $i)
		Run($cache & "\Player" & $i & ".exe " & $param,$workingdir)

		l("Waiting for network window. This may take a while!", $i)
		WinWait("Network GTA2", "")
		WinSetTitle("Network GTA2", "", "GTA2: Player "&$i)
	Next
	l("All players have joined the lobby!")

	WinSetState("GTA2: Player 1","",@SW_SHOW)
	WinActivate("GTA2: Player 1")
	l("-------------------------------")
	l("You may now configure the game.")
	l("Click 'Start' when you're ready!")
	l("(or press [F2] to quit)")
	l("-------------------------------")

	While WinExists("GTA2: Player 1")
		Sleep(100)
	WEnd
EndFunc
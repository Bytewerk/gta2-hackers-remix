#include <Misc.au3>
#include <FileConstants.au3>

#cs
	TODO:
	- Try GTA2 Game Hunter's GTA2 path (stored in the registry)
	- Display GTA2.exe version in the GUI
#ce

Global $appdata = @AppDataDir & "\GTA2 Hackers Remix"
Global $ini = $appdata & "\config.ini"


; Returns true, when the GTA2 version is supported (eg. Vike's GTAMP version),
; false otherwise
Func config_check_GTA2_version($gta2_path)
	Return  _VersionCompare ("11.44.0.0", FileGetVersion($gta2_path) ) < 1
EndFunc

Func config_init()
	DirCreate($appdata)

	; Load settings from the ini into $config (read arrays.txt)
	; TODO :multi monitor support
	Local $config[9]
	$config[0] = IniRead($ini, "Settings", "GTA2.exe Path", @ProgramFilesDir & "\Rockstar Games\GTA2\GTA2.exe")
	$config[1] = @DesktopWidth
	$config[2] = @DesktopHeight
	$config[3] = 10
	$config[4] = IniRead($ini, "Settings", "Merge Windows",true)
	$config[5] = IniRead($ini, "Settings", "Sleep Before Merge", 0)
	$config[6] = IniRead($ini, "Settings", "Hide Taskbar", true)
	$config[7] = IniRead($ini, "Settings", "Player Count", 2)
	$config[8] = IniRead($ini, "Settings", "Screen Layout", 0)

	; Ask for the right GTA2.exe
	Local $right_version = "This mod only works with Vike's modified GTA2 version 11.44.0.0 or above." &@CRLF & _
		"Get it from: http://gtamp.com/gta2"
	If Not FileExists($config[0]) Or Not config_check_GTA2_version($config[0]) Then
		MsgBox(0, "G2HR", _
			"Please open the GTA2.exe in the following dialog."&@CRLF&@CRLF&$right_version)
		$config[0] = FileOpenDialog("Open Vike's GTA2.exe",@ProgramFilesDir,"Vike's GTA2.exe (GTA2.exe)",$FD_FILEMUSTEXIST)
		If @error Then Exit 1

		If Not config_check_GTA2_version($config[0]) Then _
			Exit MsgBox(16, "G2HR", "Wrong version!"&@CRLF&$right_version)
		IniWrite($ini,"Settings","GTA2.exe Path",$config[0])
	EndIf

	Return $config
EndFunc


; The GTA2.exe Path gets saved in the init function already.
; Save everything else here! $config is listed in arrays.txt
Func config_save($config)
	IniWrite($ini, "Settings", "Merge Windows", $config[4])
	IniWrite($ini, "Settings", "Sleep Before Merge", $config[5])
	IniWrite($ini, "Settings", "Hide Taskbar", $config[6])
	IniWrite($ini, "Settings", "Player Count", $config[7])
	IniWrite($ini, "Settings", "Screen Layout", $config[8])
EndFunc


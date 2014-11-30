#include <Misc.au3>
#include <FileConstants.au3>

#cs
	TODO:
	- Try GTA2 Game Hunter's GTA2 path (stored in the registry)
	- Display GTA2.exe version in the GUI
#ce

; Returns true, when the GTA2 version is supported (eg. Vike's GTAMP version),
; false otherwise
Func config_check_GTA2_version($gta2_path)
	Return  _VersionCompare ("11.44.0.0", FileGetVersion($gta2_path) ) < 1
EndFunc

Func config_init()
	$appdata = @AppDataDir & "\GTA2 Hackers Remix"
	$ini = $appdata & "\config.ini"
	DirCreate($appdata)

	; Ask for the right GTA2.exe
	Local $right_version = "This mod only works with Vike's modified GTA2 version 11.44.0.0 or above." &@CRLF & _
		"Get it from: http://gtamp.com/gta2"
	Local $gta2_path = IniRead($ini, "Settings", "GTA2.exe Path", @ProgramFilesDir & "\Rockstar Games\GTA2\GTA2.exe")
	If Not FileExists($gta2_path) Or Not config_check_GTA2_version($gta2_path) Then
		MsgBox(0, "GTA2 Hackers Remix", _
			"Please open the GTA2.exe in the following dialog."&@CRLF&@CRLF&$right_version)
		$gta2_path = FileOpenDialog("Open Vike's GTA2.exe",@ProgramFilesDir,"Vike's GTA2.exe (GTA2.exe)",$FD_FILEMUSTEXIST)
		If @error Then Exit 1
	EndIf
	If Not config_check_GTA2_version($gta2_path) Then _
		Exit MsgBox(16, "GTA2 Hackers Remix", $right_version)

	; Write the path back to the ini-file so we don't need to ask again next time
	IniWrite($ini,"Settings","GTA2.exe Path",$gta2_path)


	; Return everything as $config (see arrays.txt)
	Local $config[4]
	$config[0] = $gta2_path

	; TODO: read these from the config
	$config[1] = @DesktopWidth
	$config[2] = @DesktopHeight
	$config[3] = 10
	Return $config
EndFunc


; This command sets the registry configs for all instances.
; Syntax:
;	$cmd[1]: MUSIC_VOL | SFX_VOL | LIGHTING | GAMMA | EXPLODING_SCORES
;	$cmd[2]: value
;

Func opt_to_bool($string)
	If $string == "true" Then Return 1
	Return 0
Endfunc

Func opt_to_num($string, $max_in, $max_out)
	Return Number($string) * $max_out / $max_in
Endfunc

Func cmd_config($cmd)
	Local $folder = ""
	Local $key = ""
	Local $type = "REG_DWORD"
	Local $value = $cmd[2]
	Local $delete = false
	
	Switch $cmd[1]
		case "MUSIC_VOL"
			$folder = "Sound"
			$key = "CDVol"
			$value = opt_to_num($value, 9, 128)
		case "SFX_VOL"
			$folder = "Sound"
			$key = "SFXVol"
			$value = opt_to_num($value, 9, 128)
		case "LIGHTING"
			$folder = "Screen"
			$key = "lighting"
			$value = 1
			If $cmd[2] == "noon" Then $value = 0
		case "EXPLODING_SCORES"
			$folder="Screen"
			$key="exploding_on"
			$value = opt_to_bool($value)
		case "BLOOD"
			$folder = "Debug"
			$key = "do_blood"
			$delete = ($value == "false")
		case "SHOW_NAMES"
			$folder = "Network"
			$key = "show_player_names"
			$value = opt_to_bool($value)
		case "TEXT_SPEED"
			$folder = "Option"
			$key = "text_speed"
			$value = Number($value)
	EndSwitch
	


	Local $root = "HKEY_CURRENT_USER\Software\DMA Design Ltd\GTA2"
	If $delete Then
		RegDelete($root&"\\"&$folder, $key)
	Else
		RegWrite($root&"\\"&$folder, $key, $type, $value)
	Endif
	
	For $i=1 To 6
		$root = "HKEY_CURRENT_USER\Software\GTA2HackersRemix\P"&$i
		
		If $delete Then
			RegDelete($root&"\\"&$folder, $key)
		Else
			RegWrite($root&"\\"&$folder, $key, $type, $value)
		Endif
	Next
Endfunc

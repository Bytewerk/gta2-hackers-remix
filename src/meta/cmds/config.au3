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
			If $value == "noon" Then $value = 0
		case "GAMMA"
			$folder = "Screen"
			$key = "gamma"
			$value = Number($value)
		case "EXPLODING_SCORES"
			$folder="Screen"
			$key="exploding_on"
			$value = opt_to_bool($value)
	EndSwitch
	

	Local $root = "HKCU\Software\DMA Design Ltd\GTA2"
	RegWrite($root&"\\"&$folder, $key, $type, $value)
	
	For $i=1 To 6
		$root = "HKCU\Software\GTA2HackersRemix\P"&$i
		RegWrite($root&"\\"&$folder, $key, $type, $value)
	Next
Endfunc

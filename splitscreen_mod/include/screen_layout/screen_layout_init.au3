#include-once
#include "screen_layout_db.au3"

#cs
	This function parses screen_layouts.cfg
	and stores the unscaled geometrical information
	in the screen_layout "database"
#ce
Func screen_layout_init()
	Local $geometry_chars = ["x", "y", "w", "h"]

	; Current state of parsing the file
	Local $open = false
	Local $players_found = 0
	Local $player_box[6]
	Local $layout_start_line
	Local $layout_width

	; Iterate through the config, line by line
	Local $lines = FileReadToArray(@ScriptDir & "\data\screen_layouts.cfg")
	Local $i
	For $i=0 To Ubound($lines) - 1 Step 1
		$line = $lines[$i]

		; Check if a new layout was opened
		If $open = False And StringMid($line,1,1) == "-" Then
			$open = True
			$layout_start_line = $i
			$layout_width = stringlen($line)
			$players_found = 0
			ContinueLoop
		EndIf

		; Check if a open layout was closed
		; And save everything in the "database"
		If $open == True And $line = "" Then
			$open = false
			If $players_found > 0 Then
				Local $variation = screen_layout_db_variation_count($players_found -1)
				Local $player, $geo
				For $player=0 To $players_found -1
					For $geo = 0 To 3
						Local $box = $player_box[$player]
						screen_layout_db($players_found -1, _
							$variation, $geometry_chars[$geo], _
							$player,$box[$geo])
					Next
				Next

				; Save the full layout width
				screen_layout_db($players_found -1, _
					$variation, "w", _
					"FULL", $layout_width - 1)

				; Save the full layout height
				screen_layout_db($players_found -1, _
					$variation, "h", _
					"FULL", $i - $layout_start_line -1)
			EndIf
			ContinueLoop
		EndIf

		; Ignore lines, if there's no open layout
		If $open = False Then ContinueLoop

		; Iterate through the whole line and look for
		; player numbers. When we find one, we measure
		; the box geometry.
		Local $last_left_border = 0
		For $j=1 To StringLen($line)
			Local $char = StringMid($line,$j,1)
			If $char = "|" Then $last_left_border = $j
			If Not Number($char) Then ContinueLoop

			; We found a number!
			$players_found+=1
			$player_number = $char - 1

			; Examine the box geometry
			; We count the left and top borders as part of the box
			Local $x, $y, $w, $h
			$x = $last_left_border - 1

			; Calculate box width by finding the next '|' in the line
			For $k=$j To StringLen($line) Step 1
				If stringmid($line,$k,1) == '|' Then
					$w = $k - $x - 1
					ExitLoop
				EndIf
			Next

			; Calculate top position by going up until a '-' gets found
			For $k = $i To $layout_start_line Step -1
				If StringMid($lines[$k],$j,1) == "-" Then
					$y = $k - $layout_start_line
					ExitLoop
				EndIf
			Next

			; Calculate height by going down until a '-' gets found
			$h = 0
			For $k = $i To Ubound($lines) -1
				If StringMid($lines[$k], $j, 1) == "-" Then
					$h = $k - $y - $layout_start_line
					ExitLoop
				EndIf
			Next

			; Store the box data!
			Local $box = [ $x, $y, $w, $h ]
			$player_box [ $player_number ] = $box
		Next
	Next
EndFunc
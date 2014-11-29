#include-once

#cs
	Creating recursive arrays with no fixed length is a pain in
	AutoIt3, but unfortunatelly we need something like this here.

	It isn't much data, so performance is no issue. Time for a
	dirty hack: stuff everything in prefixed global variables :V

	Set $value, if you want to save something. Otherwise the data
	will get loaded.

	Returns null if the variable doesn't exist (use this in loops)

	Obviously AutoIt3 isn't the right choice for calculating
	the screen layout from the config file, but we need it for
	accessing all the windows APIs. When we create a custom SDL
	game menu, the screen_layout code should be moved there!
#ce

Func screen_layout_db($player_count, $variation, _
	$geometry, $player = "FULL", $value = Null)
	Local $varname = "LYTDB" _
		& "_t" & $player_count _
		& "_v" & $variation _
		& "_p" & $player _
		& "_"  & $geometry

	If $value == Null Then
		If Not IsDeclared($varname) Then Return Null
		Return Eval($varname)
	Else
		Assign($varname, $value, 2)
	EndIf
EndFunc

Func screen_layout_db_variation_count($player_count)
	Local $c = 0
	While True
		If screen_layout_db($player_count, $c, _
			"w", "FULL") == Null Then ExitLoop
		$c+=1
	WEnd
	Return $c
EndFunc
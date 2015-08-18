#include "installer_globals.au3"
#include "nogui_callback.au3"
#include <GUIConstantsEx.au3>

Func verify_status($text, $verified = false)
	GUICtrlSetData($installer_status, "Verification status: " & $text)
	GUICtrlSetState($installer_button_ctrl[2], $verified ? $GUI_ENABLE : $GUI_DISABLE)
EndFunc


Func verify_installer()
	Local $path = GUICtrlRead($installer_input)
	If Not FileExists($path) Then Return verify_status("file not picked yet.")

	GUICtrlSetState($installer_progress_verify, $GUI_SHOW)
	Local $status_verified = "SHA-512 hash verified!"
	Local $status = $status_verified

	If $installer_verified_gta2_installer_path <> $path Then
		verify_status("Checking SHA-512 hash...")
		Local $disable_while_verifying[] = [$installer_button_ctrl[1], $installer_browse]
		For $i = 0 To Ubound($disable_while_verifying) -1
			GUICtrlSetState($disable_while_verifying[$i], $GUI_DISABLE)
		Next

		$status = nogui_callback("verify", "G2HR_ORIGINAL_INSTALLER", $path, $installer_progress_verify)

		For $i = 0 To Ubound($disable_while_verifying) -1
			GUICtrlSetState($disable_while_verifying[$i], $GUI_ENABLE)
		Next

		verify_status($status, true)
	EndIf

	verify_status($status)

	Local $is_ok = ($status == $status_verified)
	If $is_ok Then $installer_verified_gta2_installer_path = $path

	GUICtrlSetState($installer_button_ctrl[2], $is_ok ? $GUI_ENABLE : $GUI_DISABLE)
	GUICtrlSetState($installer_progress_verify, $GUI_HIDE)
EndFunc

#include <ButtonConstants.au3>
#include <GUIConstantsEx.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>

Global $installer_link_text[] = ["get gta2", "github", "thread"]
Global $installer_link_url[]  = ["http://git.io/g2hr-get-gta2" , "http://git.io/g2hr", "http://gtamp.com/forum/viewtopic.php?f=4&t=776"]
Global $installer_link_ctrl[3]
Global $installer_gui
Global $installer_button_ctrl[3]
Global $installer_page = 0
Global $installer_main
Global $installer_title
Global $installer_input
Global $installer_status
Global $installer_browse
Global $installer_verified_gta2_installer_path = ""
Global $installer_target_path = ""
Global $installer_progress

Func logo()
	Local $text[] = ["GTA2:", "HACKER'S", "REMIX", "0.4"]

	For $i = 0 To Ubound($text) -1
		GUICtrlCreateLabel($text[$i], 25, 20 + $i * 40, 180, 40)
		GUICtrlSetFont(-1, 25, 400, 0, "Segoe UI Light")
	Next
Endfunc

Func links()
	For $i = 0 To Ubound($installer_link_text) -1
		$installer_link_ctrl[$i] = GUICtrlCreateLabel($installer_link_text[$i], 25, 250 + $i * 30, 100)
		GUICtrlSetFont(-1, 12, 400, 4, "Segoe UI Light")
		GuiCtrlSetColor(-1, 0xff0000)
	Next
Endfunc

Func buttons()
	Local $text[] = ["&Back", "&Quit", "&Next"]

	For $i = 0 To Ubound($text) -1
		$installer_button_ctrl[$i] = GUICtrlCreateButton($text[$i], 160 + $i * 150, 385, 130, 40)
		GUICtrlSetFont(-1, 12, 400, 0, "Segoe UI Light")
	Next
Endfunc

Func ctrlshow($ctrl, $text = "")
	If $text Then GUICtrlSetData($ctrl,$text)
	GUICtrlSetState($ctrl, $GUI_SHOW)
EndFunc

Func verify_status($text, $verified = false)
	GUICtrlSetData($installer_status, "Verification status: " & $text)
	GUICtrlSetState($installer_button_ctrl[2], $verified ? $GUI_ENABLE : $GUI_DISABLE)
EndFunc

Func verify_installer()
	Local $path = GUICtrlRead($installer_input)
	If Not FileExists($path) Then Return verify_status("file not picked yet.")

	If $installer_verified_gta2_installer_path <> $path Then
		verify_status("checking sha512 sum...")
		Local $disable_while_verifying[] = [$installer_button_ctrl[1], $installer_browse]
		For $i = 0 To Ubound($disable_while_verifying) -1
			GUICtrlSetState($disable_while_verifying[$i], $GUI_DISABLE)
		Next

		ConsoleWrite("Stub: verify " & $path & @CRLF)
		Sleep(1000)
		For $i = 0 To Ubound($disable_while_verifying) -1
			GUICtrlSetState($disable_while_verifying[$i], $GUI_ENABLE)
		Next


		; TODO: if failed ...
		$installer_verified_gta2_installer_path = $path
	EndIf
	verify_status("OK, sha512 sum matches!", true)
EndFunc

Func disk_space()
	Local $text = "NOTE: Currently G2HR will save a few KB of configs in the default folder, even if you choose another folder." & @CRLF & @CRLF
	Local $drive = StringMid(GUICtrlRead($installer_input),1,3)
	$text &= "Required space: ~380 MB" & @CRLF

	If FileExists($drive) Then
		$text &= "'" & $drive & "' free space: " & Round(DriveSpaceFree($drive)) & " MB"
	EndIf
	GUICtrlSetData($installer_status, $text)
EndFunc

Func pick_original_installer()
	Local $new = FileOpenDialog("Pick the official GTA2 Installer", @UserProfileDir, "GTA2 Installer (GTA2.exe)", 1, GUICtrlRead($installer_input), $installer_gui)
	If Not FileExists($new) Then Return
	GUICtrlSetData($installer_input, $new)
	verify_installer()
EndFunc

Func installation_status()
	GUICtrlSetData($installer_status, "Running the GTA2 installer...")
EndFunc


Func installer_leave_page()
	If $installer_page == 1 Then
		$installer_target_path = GUICtrlRead($installer_input)
	EndIf
Endfunc

Func installer_draw_page()
	GUICtrlSetData($installer_button_ctrl[2], "&Next")
	GUICtrlSetState($installer_progress, $GUI_HIDE)

	If $installer_page == 0 Then
		GUICtrlSetState($installer_button_ctrl[0], $GUI_DISABLE)
		ctrlshow($installer_title, "Welcome")
		ctrlshow($installer_main, "In order to install G2HR, you will need the latest official GTA2 installer (GTA2.exe). If you don't have it already, click the 'get gta2' link on the left to download it for free.")
		ctrlshow($installer_status, "")
		If $installer_verified_gta2_installer_path Then ctrlshow($installer_input, $installer_verified_gta2_installer_path)
		verify_installer()
	Elseif $installer_page == 1 Then
		GUICtrlSetState($installer_button_ctrl[0], $GUI_ENABLE)
		ctrlshow($installer_title, "Installation path")
		ctrlshow($installer_main, "GTA2 (and therefore G2HR) requires old-school write access to its application directory. Therefore we recommend installing it in the path below. Only change this if you know, what you're doing!")
		If Not $installer_target_path Then $installer_target_path = @AppDataDir & "\bytewerk\G2HR"
		ctrlshow($installer_input, $installer_target_path)
		GUICtrlSetData($installer_button_ctrl[2], "&Do it!")
		disk_space()
	ElseIf $installer_page == 2 Then
		GUICtrlSetState($installer_button_ctrl[0], $GUI_DISABLE)
		GUICtrlSetState($installer_button_ctrl[1], $GUI_DISABLE)
		GUICtrlSetState($installer_button_ctrl[2], $GUI_DISABLE)
		GUICtrlSetState($installer_input, $GUI_HIDE)
		GUICtrlSetState($installer_browse, $GUI_HIDE)
		GUICtrlSetState($installer_progress, $GUI_SHOW)
		ctrlshow($installer_title, "Installing...")
		ctrlshow($installer_main, "This will take a while, get your game controllers ready. Please note that G2HR is beta software. Expect and report bugs at http://git.io/g2hr-bugs!")
		installation_status()
	EndIf
Endfunc

Func installer_wm_command()
	If $installer_page == 1 Then
		disk_space()
	EndIf
EndFunc

Func installer_main()
	$installer_gui = GUICreate("G2HR Installer", 615, 438, 541, 253)
	logo()
	links()
	buttons()

	$installer_title = GUICtrlCreateGroup("", 210, 10, 380, 350)
	GUICtrlSetFont(-1, 22, 400, 0, "Segoe UI Light")

	$installer_main = GUICtrlCreateLabel("", 225, 60, 355, 100)
	GUICtrlSetFont(-1, 10, 400, 0, "Segoe UI")

	$installer_input = GUICtrlCreateInput("GTA2.exe", 225, 150, 300, 30)
	GUICtrlSetFont(-1, 12, 400, 0, "Segoe UI")
	$installer_browse = GUICtrlCreateButton("...", 530, 150, 40, 30)
	GUICtrlSetFont(-1, 10, 400, 0, "Segoe UI")

	$installer_progress = GUICtrlCreateProgress(225, 150, 350, 30)
	installer_draw_page()
	GUISetState(@SW_SHOW)

	$installer_status = GUICtrlCreateLabel("", 225, 210, 350, 100)
	GUICtrlSetFont(-1, 10, 400, 0, "Segoe UI")


	Local $back = $installer_button_ctrl[0]
	Local $quit = $installer_button_ctrl[1]
	Local $next = $installer_button_ctrl[2]


	GUIRegisterMsg($WM_COMMAND, "installer_wm_command")
	While 1
		Local $msg = GUIGetMsg()
		If $msg == $GUI_EVENT_CLOSE Or $msg == $quit Then Exit
		For $i=0 To Ubound($installer_link_ctrl) -1
			If $msg == $installer_link_ctrl[$i] Then ShellExecute($installer_link_url[$i])
		Next
		If $msg == $installer_browse Then pick_original_installer()
		If $msg == $next Then
			installer_leave_page()
			$installer_page += 1
			installer_draw_page()
		Elseif $msg == $back Then
			installer_leave_page()
			$installer_page -= 1
			installer_draw_page()
		Endif
	WEnd


EndFunc

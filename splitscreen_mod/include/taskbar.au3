#include-once
#include <WinAPI.au3>

; Credits to SmOke_N from autoit forums for this function:
; http://www.autoitscript.com/forum/topic/126916-cant-hide-start-button/?p=880773

Func taskbar_hide($f_hide = False)
    Local $h_task = WinGetHandle("[CLASS:Shell_TrayWnd]")
    If Not $h_task Then Return SetError(1, 0, 0)

    Local $h_start = 0
    Local $a_wlist = WinList("[CLASS:Button]")
    If Not IsArray($a_wlist) Then Return SetError(2, 0, 0)

    For $iwin = 1 To $a_wlist[0][0]
        If _WinAPI_GetParent($a_wlist[$iwin][1]) = $h_task Then
            $h_start = $a_wlist[$iwin][1]
            ExitLoop
        EndIf
    Next

    If Not $h_start Then Return SetError(3, 0, 0)

    If $f_hide Then
        _WinAPI_ShowWindow($h_start, @SW_HIDE)
        _WinAPI_ShowWindow($h_task, @SW_HIDE)
    Else
        _WinAPI_ShowWindow($h_start, @SW_SHOW)
        _WinAPI_ShowWindow($h_task, @SW_SHOW)
    EndIf
EndFunc
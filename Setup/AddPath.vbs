Dim objShell, RegLocate

Set objShell = WScript.CreateObject("WScript.Shell")
On Error Resume Next
RegLocate = "HKEY_CURRENT_USER\Software\DevAssist\InstallDir"

dim fso: set fso = CreateObject("Scripting.FileSystemObject")
    dim CurrentDirectory
    CurrentDirectory = fso.GetAbsolutePathName(".")

objShell.RegWrite RegLocate,CurrentDirectory,"REG_SZ"

dim newfolder, newfolderpath 
If Not fso.FolderExists("REPOSITORY") Then 
Set newfolder = fso.CreateFolder("REPOSITORY") 
End If 

MsgBox("Done!")
WScript.Quit ' Tells the script to stop and exit.
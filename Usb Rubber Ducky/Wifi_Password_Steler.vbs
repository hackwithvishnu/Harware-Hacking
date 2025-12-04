Set objShell = CreateObject("WScript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject")

' Get the script's directory
strScriptDir = objFSO.GetParentFolderName(WScript.ScriptFullName)

' Build the full path to the text file
strFilePath = objFSO.BuildPath(strScriptDir, "m.txt")

' Open the Command Prompt
Set cmd = objShell.Exec("cmd")

' Run the "netsh" command to show profiles and save to text file
cmd.StdIn.WriteLine("netsh wlan show profiles > """ & strFilePath & """")
cmd.StdIn.WriteLine("netsh wlan show profiles | findstr /C:""All User Profile"" > temp.txt")
cmd.StdIn.WriteLine("for /f ""tokens=2 delims=:"" %i in ('type temp.txt') do (echo Profile: %i & netsh wlan show profile name=%i key=clear | findstr /C:""Key Content"") >> """ & strFilePath & """")
cmd.StdIn.WriteLine("exit")

' Wait for the command to finish
Do While cmd.Status = 0
    WScript.Sleep 100
Loop

' Delete the temporary file used for profile names
objFSO.DeleteFile "temp.txt"
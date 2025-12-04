Set objShell = CreateObject("WScript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject")

' Get the script's directory
strScriptDir = objFSO.GetParentFolderName(WScript.ScriptFullName)

' Build the full path to the text file
strFilePath = objFSO.BuildPath(strScriptDir, "browser_history.txt")

' Open Command Prompt
Set cmd = objShell.Exec("cmd")

' Retrieve Google Chrome History
cmd.StdIn.WriteLine("copy ""%LOCALAPPDATA%\Google\Chrome\User Data\Default\History"" """ & strScriptDir & "\chrome_history""")
cmd.StdIn.WriteLine("sqlite3.exe """ & strScriptDir & "\chrome_history"" ""select * from urls;"" > """ & strFilePath & """")
cmd.StdIn.WriteLine("del """ & strScriptDir & "\chrome_history""")

' Retrieve Mozilla Firefox History
cmd.StdIn.WriteLine("copy ""%APPDATA%\Mozilla\Firefox\Profiles\*.default-release\places.sqlite"" """ & strScriptDir & "\firefox_history.sqlite""")
cmd.StdIn.WriteLine("sqlite3.exe """ & strScriptDir & "\firefox_history.sqlite"" ""select * from moz_places;"" > """ & strFilePath & """")
cmd.StdIn.WriteLine("del """ & strScriptDir & "\firefox_history.sqlite""")

' Retrieve Microsoft Edge History
cmd.StdIn.WriteLine("copy ""%LOCALAPPDATA%\Microsoft\Edge\User Data\Default\History"" """ & strScriptDir & "\edge_history""")
cmd.StdIn.WriteLine("sqlite3.exe """ & strScriptDir & "\edge_history"" ""select * from urls;"" > """ & strFilePath & """")
cmd.StdIn.WriteLine("del """ & strScriptDir & "\edge_history""")

' Exit Command Prompt
cmd.StdIn.WriteLine("exit")

' Wait for the command to finish
Do While cmd.Status = 0
    WScript.Sleep 100
Loop

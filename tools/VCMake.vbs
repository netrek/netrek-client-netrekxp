' *****************************************************************************
' VCMake.vbs
' Copyright © 2000 Kinook Software.  All rights reserved.
' http://www.kinook.com
' Be sure to visit www.kinook.com for more handy utilities and tools, 
' including Visual Build, the build management tool for Windows and web
' developers.
'
' VCMake is freeware.  You may freely distribute it, as long as you do not 
' sell it or make it part of another package or application.  If any 
' modifications are made to the script code, they must be clearly marked in 
' the code and documented in the Modifications section below, and these header 
' comments must not be changed or removed.
'
' VCMake is a VBScript make utility for automating building of Microsoft 
' Visual C++ workspaces and project files.  With Visual C++ 6.0, Microsoft 
' added the ability to build .DSP files from DevStudio without exporting a 
' makefile.  Unfortunately, unlike NMAKE, it doesn't return an exitcode to 
' indicate the success or failure of the build, and it is missing important 
' options such as /A to force a rebuild.  VCMake provides all these missing 
' capabilities, and it works well in conjunction with the Visual Build build 
' management tool.  
'
' This script builds a given project (.dsp) or all the given projects in a 
' workspace (.dsw).  It builds the specified configuration or all 
' configurations if not specified.
'
' Usage: cscript VCMake.vbs <DSP/DSW file> [configuration] [/S] [/F] [/N]
'
' Output is echoed to standard output; returns ERRORLEVEL of 0 if 
' sucessful or < 0 if any errors occur.
'
' The first parameter must be a valid DevStudio workspace file (.DSW) or
' project file (.DSP); the second parameter, if provided, must be the 
' project configuration to build (the configuration must exist for all 
' projects in the workspace), and the following optional flags are 
' supported (non-case-sensitive):
'
'  /S => Suppress default output (only displays error output)
'  /N => displays commands but does not execute.  Useful for debugging.
'  /F => forces all targets to be rebuilt regardless of dependency dates
'
' e.g.: cscript VCMake.vbs MyProjects.dsw "Win32 Release" /s
'
' Dependencies:
'   1) Microsoft Windows Scripting 5.0+ (comes with Win98 & Win2000, download 
'      for NT 4.0 and Win95 at http://msdn.microsoft.com/scripting)
'   2) Visual C++ 6.0+ (might work with 5.0 but not tested). If SourceSafe is 
'      also installed, is recommended that you disable SourceSafe integration 
'      in VC on the build machine to prevent the possibility of dialog boxes 
'      requiring user input when building.  This is done by setting the
'      registry value 'HKEY_CURRENT_USER\Software\Microsoft\DevStudio\6.0\
'      Source Control\Disabled' to a value of 1 (the DevStudio IDE must be
'      closed when setting the entry or it will be overwritten).
'
' Modifications:
' Jan.  1, 2000 (1.0) => initial release
' Jan. 17, 2000 (1.1) => create a temporary, empty .DSW for a .DSP file if 
'     one doesn't exist (otherwise VC fails with a 'file not found' error 
'     when opening the DSP).
' Apr.  2, 2000 (1.2) => creates empty .OPT for a .DSW file if one doesn't
'     exist (otherwise VC prompts user when closing workspace).
' *****************************************************************************

Const SCRIPT_VERSION = "1.2"

' global settings
Dim g_blnSuppress, g_blnRebuild, g_blnDisplayOnly
Dim g_objFS

' *****************************************************************************
' script entry point; retrieve arguments and build

   Dim strProject

   WScript.Echo "VCMake Utility, Version " & SCRIPT_VERSION + vbCrLf + _
      "Copyright (C) 2000 Kinook Software. All rights reserved." + vbCrLf

   Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
   If Err.Number <> 0 Then ExitWithError "Failed to create Scripting.FileSystemObject."

   ' process command-line arguments
   For intIdx = 0 To WScript.Arguments.Count - 1
      If Left(WScript.Arguments(intIdx), 1) = "/" Then
         Select Case (LCase(Mid(WScript.Arguments(intIdx), 2, 1)))
            Case "f"
               g_blnRebuild = True

            Case "n"
               g_blnDisplayOnly = True

            Case "s"
               g_blnSuppress = True

         End Select
      Else
         If Len(strProject) = 0 Then
            strProject = WScript.Arguments(intIdx)
         Else
            strConfig = WScript.Arguments(intIdx)
         End If
      End If
   Next

   If Len(strProject) = 0 Then      ' too few parameters
      WScript.Echo "Usage: cscript " & WScript.ScriptName & " <DSP/DSW file> [configuration] [/S] [/F]"
      WScript.Quit -1
   End If

   ' add current directory to project file if a relative path
   If RelativePath(strProject) Then strProject = _
      g_objFS.BuildPath(g_objFS.GetFolder(".").Path, strProject)
  
   If g_objFS.FileExists(strProject) Then
      ProcessFile Trim(strProject), strConfig
   Else
      WScript.Echo "File not found."
      WScript.Quit -2
   End If

   WScript.Quit 0		' exit with success code if we got here

' *****************************************************************************
' ProcessFile
' Process the project file and return True if it was built, echoing
' any output from NMAKE

Sub ProcessFile(strFilename, strProjName)

   Dim blnConfigFound
   Dim objApp, objProj, objConfig, objFile
   Dim strDSW, strOPT

   On Error Resume Next

   ' instantiate DevStudio as Automation server
   Set objApp = CreateObject("MSDEV.Application")
   If Err.Number <> 0 Then ExitWithError "Failed to create MSDEV.Application.  " & _
      "Visual C++ 6.0 or greater must be installed."
'   objApp.Visible = True

   ' for unknown reasons, DevStudio requires a .DSW to exist when opening a 
   ' .DSP via automation; it doesn't have to be a valid workspace file and
   ' can be empty, so create an empty .DSW if one doesn't exist
   If UCase(Right(strFilename, 4)) = ".DSP" Then
      strDSW = Left(strFilename, Len(strFilename) - 4) & ".DSW"
      If Not g_objFS.FileExists(strDSW) Then
         Set objFile = g_objFS.CreateTextFile(strDSW)
         objFile.Close
      End If
   End If

   ' DevStudio requires the existence of a .OPT when closing a workspace 
	' or it will prompt the user; again, it doesn't have to be a valid file and
   ' can be empty, so create an empty .OPT if one doesn't exist
	' (DevStudio will overwrite it)
   If UCase(Right(strFilename, 4)) = ".DSW" Then
      strOPT = Left(strFilename, Len(strFilename) - 4) & ".OPT"
      If Not g_objFS.FileExists(strOPT) Then
         Set objFile = g_objFS.CreateTextFile(strOPT)
         objFile.Close
      End If
   End If

   ' open the project/workspace
   objApp.Documents.Open strFilename
   If Err.Number <> 0 Then ExitWithError "Failed to open file '" & strFilename & "'."

   ' cleanup the empty DSW if we had to create one to make DevStudio happy
   ' since it's really just an empty file
   If Not IsEmpty(strDSW) Then g_objFS.DeleteFile(strDSW)

   ' iterate through each project in the workspace
   For Each objProj In objApp.Projects
      If Not g_blnSuppress Then WScript.Echo "Processing project '" & objProj.FullName & "'..."
      blnConfigFound = False
      ' and each configuration in the project
      For Each objConfig In objProj.Configurations
         ' if looking for a specific configuration, see if this is it and build
         If Len(strConfig) > 0 Then
            If LCase(objConfig.Name) = LCase(objProj.Name & " - " & strConfig) Then
               BuildConfiguration objApp, objConfig, objProj.FullName
               blnConfigFound = True
            End If
         Else   ' otherwise build all configurations
            BuildConfiguration objApp, objConfig, objProj.FullName
         End If
      Next
      ' check to see if the requested configuration was found
      If Len(strConfig) > 0 And Not blnConfigFound Then
         WScript.Echo "Configuration '" & strConfig & _
            "' not found in project '" & objProj.FullName & "'."
         objApp.Quit
         WScript.Quit -3
      End If
   Next

   objApp.Quit		' close DevStudio automation server

End Sub

' *****************************************************************************
' BuildConfiguration
' builds/rebuilds the project associated with the passed in configuration

Sub BuildConfiguration(objApp, objConfig, strProjName)

   If Not g_blnSuppress Then WScript.Echo "Building configuration '" & objConfig.Name & "'..."
   If g_blnDisplayOnly Then Exit Sub

   ' build/rebuild as necessary
   If g_blnRebuild Then 
      objApp.RebuildAll objConfig 
   Else 
      objApp.Build objConfig
   End If
   ' check for an error from the call
   If Err.Number <> 0 Then 
      objApp.Quit
      ExitWithError "Failed to build configuration '" & objConfig.Name & _
         "' in project '" & strProjName & "'."
   End If
   ' check for build errors
   If objApp.Errors = 0 Then
      If Not g_blnSuppress Then
         If objApp.Warnings = 0 Then
            WScript.Echo "Build of configuration completed successfully."
         Else
            WScript.Echo "Build of configuration completed but had " & _
              objApp.Warnings & " warning(s)."
         End If
      End If
   Else
      WScript.Echo "Build of configuration '" & objConfig.Name & "' in project '" & _
          strProjName & "' failed with " & objApp.Errors & " error(s)."
      objApp.Quit
      WScript.Quit -4
   End If

End Sub

' *****************************************************************************
Function RelativePath(strFilename)
' given a filename, determines if it is a relative or absolute path name

    RelativePath = True
    If Len(strFilename) > 0 Then
        If Left(strFilename, 1) = "\" Or Mid(strFilename, 2, 2) = ":\" Then
            RelativePath = False
        End If
    End If

End Function

' *****************************************************************************
Sub ExitWithError(strExtraDescr)
' write the error description to standard output and exit with a failure exitcode

   If Len(strExtraDescr) > 0 Then 
      WScript.Echo strExtraDescr & vbCrlf & Err.Description
   Else
      WScript.Echo Err.Description
   End If
   WScript.Quit Err.Number

End Sub

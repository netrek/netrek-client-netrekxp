Option Strict Off
Option Explicit On
Friend Class Form3
	Inherits System.Windows.Forms.Form
	' Netrek Configurator v1.0 - written by Joe Evango
	'
	' Program notes from Joe 4/30/06-
	' Code contains some comments, pretty easy to follow the logic.
	' I am currently using a config directory that contains two netrekrc files, netrek.one and netrek.sep.
	' This is done for the two messaging window configs.  This should be easy enough to implement into the code
	' in order to eliminate the extra sub directory and files if someone has time.
	'
	' This utility writes to the netrekrc file and assumes it is in located the same folder as the utility so if the
	' netrekrc file does not exist the utility will not run properly and error out.  When it is run it will also save
	' your old netrekrc file as netrek.sav.
	'
	' Since Netrek code has always been freely shared in the community I will do the same with this config utility.
	' Please feel free to help improve the utility and/or include the utility as an add-on if you are working on
	' a Windows client.  Also feel free to use the logic in this code to create a config utility for another OS.  It
	' took a while to put the keymap portion of this together but it works well. I only ask that if any modifications
	' are made you also make your code changes available to the community.  It would also be nice if I was mentioned as
	' a contributor in your code comments.
	
	
	
	'Keymap configuration
	Private Sub Command1_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Command1.Click
		Dim fnum2 As Object
		Dim fnum3 As Object
		
		Dim FileName As String
		Dim Application As String
		Dim keymap As String
		Dim buttonmap As String
		Dim torpedo As String
		Dim phaser As String
		Dim shield As String
		Dim bomb As String
		Dim cloak As String
		Dim drop As String
		Dim pickup As String
		Dim tractor As String
		Dim pressor As String
		Dim maxwarp As String
		Dim halfspeed As String
		Dim repair As String
		Dim refit As String
		Dim tlock As String
		Dim info As String
		Dim quit As String
		Dim detown As String
		Dim detenemy As String
		Dim plasma As String
		Dim warp0 As String
		Dim warp2 As String
		Dim warp3 As String
		Dim warp4 As String
		
		Dim print1 As String
		Dim print2 As String
		
		Dim keymap1 As String
		Dim keymap2 As String
		Dim keymap3 As String
		Dim keymap4 As String
		Dim keymap5 As String
		Dim keymap6 As String
		Dim keymap7 As String
		Dim keymap8 As String
		Dim keymap9 As String
		Dim keymap10 As String
		Dim keymap11 As String
		Dim keymap12 As String
		Dim keymap13 As String
		Dim keymap14 As String
		Dim keymap15 As String
		Dim keymap16 As String
		Dim keymap17 As String
		Dim keymap18 As String
		Dim keymap19 As String
		Dim keymap20 As String
		Dim keymap21 As String
		Dim keymap22 As String
		Dim keymap23 As String
		
		Dim mousetag As String
		Dim keytag As String
		Dim configtag As String
		
		
		Dim a As Short
		Dim b As Short
		Dim c As Short
		
		Dim m As String
		Dim h As String
		
		Dim duplicate As Short
		Dim speckey As Short
		
		Dim currentdate As String
		
		currentdate = CStr(Today)
		'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		fnum3 = FreeFile
		'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		fnum2 = FreeFile
		
		m = "m"
		h = "h"
		
		torpedo = "t"
		phaser = "f"
		shield = "s"
		bomb = "b"
		cloak = "c"
		drop = "x"
		pickup = "z"
		tractor = "T"
		pressor = "y"
		maxwarp = "@"
		halfspeed = "#"
		repair = "R"
		refit = "r"
		tlock = "l"
		info = "i"
		quit = "\"
		detown = "D"
		detenemy = "d"
		plasma = "g"
		warp0 = "0"
		warp2 = "2"
		warp3 = "3"
		warp4 = "4"
		
		FileName = ".\controls.txt"
		Application = "Notepad.exe"
		
		If torpedo = Text1(0).Text Then
			keymap1 = ""
		Else
			keymap1 = Text1(0).Text & torpedo
		End If
		
		If phaser = Text1(1).Text Then
			keymap2 = ""
		Else
			keymap2 = Text1(1).Text & phaser
		End If
		
		
		If shield = Text1(2).Text Then
			keymap3 = ""
		Else
			keymap3 = Text1(2).Text & shield
		End If
		
		If bomb = Text1(3).Text Then
			keymap4 = ""
		Else
			keymap4 = Text1(3).Text & bomb
		End If
		
		If cloak = Text1(4).Text Then
			keymap5 = ""
		Else
			keymap5 = Text1(4).Text & cloak
		End If
		
		If drop = Text1(5).Text Then
			keymap6 = ""
		Else
			keymap6 = Text1(5).Text & drop
		End If
		
		If pickup = Text1(6).Text Then
			keymap7 = ""
		Else
			keymap7 = Text1(6).Text & pickup
		End If
		
		If pressor = Text1(7).Text Then
			keymap8 = ""
		Else
			keymap8 = Text1(7).Text & pressor
		End If
		
		If tractor = Text1(8).Text Then
			keymap9 = ""
		Else
			keymap9 = Text1(8).Text & tractor
		End If
		
		If maxwarp = Text1(9).Text Then
			keymap10 = ""
		Else
			keymap10 = Text1(9).Text & maxwarp
		End If
		
		If halfspeed = Text1(10).Text Then
			keymap11 = ""
		Else
			keymap11 = Text1(10).Text & halfspeed
		End If
		
		If repair = Text1(11).Text Then
			keymap12 = ""
		Else
			keymap12 = Text1(11).Text & repair
		End If
		
		If refit = Text1(12).Text Then
			keymap13 = ""
		Else
			keymap13 = Text1(12).Text & refit
		End If
		
		If tlock = Text1(13).Text Then
			keymap14 = ""
		Else
			keymap14 = Text1(14).Text & tlock
		End If
		
		If info = Text1(14).Text Then
			keymap15 = ""
		Else
			keymap15 = Text1(14).Text & info
		End If
		
		If quit = Text1(15).Text Then
			keymap16 = ""
		Else
			keymap16 = Text1(15).Text & quit
		End If
		
		If detown = Text1(16).Text Then
			keymap17 = ""
		Else
			keymap17 = Text1(16).Text & detown
		End If
		
		If detenemy = Text1(17).Text Then
			keymap18 = ""
		Else
			keymap18 = Text1(17).Text & detenemy
		End If
		
		If plasma = Text1(18).Text Then
			keymap19 = ""
		Else
			keymap19 = Text1(18).Text & plasma
		End If
		
		If warp0 = Text1(19).Text Then
			keymap20 = ""
		Else
			keymap20 = Text1(19).Text & warp0
		End If
		
		If warp2 = Text1(20).Text Then
			keymap21 = ""
		Else
			keymap21 = Text1(20).Text & warp2
		End If
		
		If warp3 = Text1(21).Text Then
			keymap22 = ""
		Else
			keymap22 = Text1(21).Text & warp3
		End If
		
		If warp4 = Text1(22).Text Then
			keymap23 = ""
		Else
			keymap23 = Text1(22).Text & warp4
		End If
		
		
		
		speckey = 0
		duplicate = 0
		For a = 0 To 22
			For b = 0 To 22
				If a <> b And Text1(a).Text = Text1(b).Text Then
					duplicate = 1
					Exit For
				End If
			Next b
			If duplicate = 1 Then
				MsgBox("The key '" & Text1(a).Text & "' is defined more than once.  This will cause problems while playing.  Please remove the duplicate controls and click on Finish.")
				Exit For
			End If
		Next a
		
		
		For c = 0 To 22
			If Text1(c).Text = "m" Then
				speckey = 1
			End If
			If Text1(c).Text = "h" Then
				speckey = 1
			End If
			If Text1(c).Text = "w" Then
				speckey = 1
			End If
			If speckey = 1 Then
				MsgBox("You cannot use the 'm', 'h', or 'w' keys.  These are used for messaging, help, and war declarations.  Please assign different keys to these controls.")
				Exit For
			End If
		Next c
		
		
		If duplicate = 0 And speckey = 0 Then
			' Open the file for append.
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			FileOpen(fnum2, ".\netrekrc", OpenMode.Append)
			' Add the command.
			If Combo1.Text = "Torpedoes" Then
				buttonmap = "buttonmap:" & "1" & Text1(0).Text
			End If
			If Combo1.Text = "Phasers" Then
				buttonmap = "buttonmap:" & "1" & Text1(1).Text
			End If
			
			keymap = "keymap:" & "\Qq0Q0gffp" & keymap1 & keymap2 & keymap3 & keymap4 & keymap5 & keymap6 & keymap7 & keymap8 & keymap9 & keymap10 & keymap11 & keymap12 & keymap13 & keymap14 & keymap15 & keymap16 & keymap17 & keymap18 & keymap19 & keymap20 & keymap21 & keymap22 & keymap23
			keytag = "###Keymap added by the Netrek Configurator on " & currentdate
			mousetag = "###Buttonmap added by the Netrek Configurator on " & currentdate
			
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum2, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum2, keytag)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum2, keymap)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum2, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum2, mousetag)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum2, buttonmap)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum2. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			FileClose(fnum2)
			
			' Opens the controls you chose in notepad for reference
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			FileOpen(fnum3, ".\controls.txt", OpenMode.Output)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "These are the controls you have chosen:")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Mouse Controls:")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Left Button     -  " & Combo1.Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Middle Button   -  " & "Unmap Special Windows (i.e. Close tip window)")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Right Button    -  " & "Steer")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Keyboard Controls:")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Messaging            -  " & "m (Be sure to hold your mouse cursor over")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "the Galaxy or Tactical maps when you press 'm'.")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "After pressing 'm' type 'A' to send messages to")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "the All Window, type 'T' to send messages")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "to the Team Window, or type the letter/number")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "of the player you would like to send a message to.")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "After you type A, T, or the player letter/number, ")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "                           " & "type your message and press enter.)")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Help                 -  " & "h")
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Torpedoes            -  " & Text1(0).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Phasers              -  " & Text1(1).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Shield               -  " & Text1(2).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Bomb                 -  " & Text1(3).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Cloak                -  " & Text1(4).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Drop Armies          -  " & Text1(5).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Pickup Armies        -  " & Text1(6).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Detonate Own Torps   -  " & Text1(16).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Detonate Enemy Torps -  " & Text1(17).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Tractor              -  " & Text1(8).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Pressor              -  " & Text1(7).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Repair               -  " & Text1(11).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Refit Ship Type      -  " & Text1(12).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Lock on Target       -  " & Text1(13).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Target Info          -  " & Text1(14).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Max Warp             -  " & Text1(9).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Half Speed           -  " & Text1(10).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Quit                 -  " & Text1(15).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Plasma               -  " & Text1(18).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, Chr(13))
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Warp 0               -  " & Text1(19).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Warp 2               -  " & Text1(20).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Warp 3               -  " & Text1(21).Text)
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			PrintLine(fnum3, "Warp 4               -  " & Text1(22).Text)
			
			'UPGRADE_WARNING: Couldn't resolve default property of object fnum3. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			FileClose(fnum3)
			
			
			Me.Close()
			MsgBox("YOUR CONFIGURATION HAS BEEN CREATED!  After you click on OK a document with the controls you have chosen will open.  A shortcut to this document has been created in your Netrek Config Utility program group.  If you need additional assistance please visit www.netrek.org.")
			Shell(Application & " " & FileName, AppWinStyle.NormalFocus)
		Else
		End If
	End Sub
End Class
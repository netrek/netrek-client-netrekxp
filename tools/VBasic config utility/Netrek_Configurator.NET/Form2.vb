Option Strict Off
Option Explicit On
Friend Class Form2
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
	
	' Variables below for game options are self explanitory
	Dim playlist As Short
	Dim sound As Short
	Dim tips As Short
	Dim stars As Short
	Dim warp As Short
	
	
	
	Private Sub Command1_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Command1.Click
		Dim fnum2 As Short
		Dim pltxt As String
		Dim tiptxt As String
		Dim soundtxt As String
		Dim startxt As String
		Dim warptxt As String
		Dim metatxt As String
		
		
		fnum2 = FreeFile
		
		
		' Open the file for append.
		FileOpen(fnum2, ".\netrekrc", OpenMode.Append)
		' Add the command.
		If (Check1.CheckState = System.Windows.Forms.CheckState.Checked) Then
			sound = 1
		End If
		If (Check1.CheckState = System.Windows.Forms.CheckState.Unchecked) Then
			sound = 0
		End If
		
		If (Check2.CheckState = System.Windows.Forms.CheckState.Checked) Then
			tips = 1
		End If
		If (Check2.CheckState = System.Windows.Forms.CheckState.Unchecked) Then
			tips = 0
		End If
		If (Check3.CheckState = System.Windows.Forms.CheckState.Checked) Then
			stars = 1
		End If
		If (Check3.CheckState = System.Windows.Forms.CheckState.Unchecked) Then
			stars = 0
		End If
		
		If (Check4.CheckState = System.Windows.Forms.CheckState.Checked) Then
			warp = 1
		End If
		If (Check4.CheckState = System.Windows.Forms.CheckState.Unchecked) Then
			warp = 0
		End If
		
		
		If playlist = 1 Then
			pltxt = "playerListStyle:   1"
		End If
		
		If playlist = 2 Then
			pltxt = "playerListStyle:    2"
		End If
		
		If playlist = 3 Then
			pltxt = "playerListStyle:    3"
		End If
		
		If playlist = 4 Then
			pltxt = "playerListStyle:   4"
		End If
		
		
		
		If tips = 1 Then
			tiptxt = "showHints: on"
		End If
		
		If tips = 0 Then
			tiptxt = "showHints: off"
		End If
		
		
		If sound = 1 Then
			soundtxt = "sound: on"
		End If
		
		If sound = 0 Then
			soundtxt = "sound: off"
		End If
		
		
		If stars = 0 Then
			startxt = "showStars: off"
		End If
		
		If stars = 1 Then
			startxt = "showStars: on"
		End If
		
		If warp = 1 Then
			warptxt = "warpStreaks: on"
		End If
		
		If warp = 0 Then
			warptxt = "warpStreaks: off"
		End If
		
		
		PrintLine(fnum2, pltxt)
		PrintLine(fnum2, Chr(13))
		PrintLine(fnum2, tiptxt)
		PrintLine(fnum2, Chr(13))
		PrintLine(fnum2, soundtxt)
		PrintLine(fnum2, Chr(13))
		PrintLine(fnum2, startxt)
		PrintLine(fnum2, Chr(13))
		PrintLine(fnum2, warptxt)
		
		
		' Close the file.
		FileClose(fnum2)
		
		'MsgBox "Your configuration has been saved.  Your old configuration file has been saved as netrekrc.sav in your Netrek install directory"
		Form3.Show()
		Me.Close()
	End Sub
	
	
	
	Private Sub Command2_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Command2.Click
		Me.Close()
	End Sub
	
	Private Sub Form2_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		playlist = 3
	End Sub
	
	'UPGRADE_WARNING: Event Option1.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub Option1_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option1.CheckedChanged
		If eventSender.Checked Then
			playlist = 1
			pl1.Visible = True
			pl2.Visible = False
			pl3.Visible = False
			pl4.Visible = False
		End If
	End Sub
	
	'UPGRADE_WARNING: Event Option2.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub Option2_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option2.CheckedChanged
		If eventSender.Checked Then
			playlist = 2
			pl1.Visible = False
			pl2.Visible = True
			pl3.Visible = False
			pl4.Visible = False
		End If
	End Sub
	
	'UPGRADE_WARNING: Event Option3.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub Option3_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option3.CheckedChanged
		If eventSender.Checked Then
			playlist = 3
			pl1.Visible = False
			pl2.Visible = False
			pl3.Visible = True
			pl4.Visible = False
		End If
	End Sub
	
	'UPGRADE_WARNING: Event Option4.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub Option4_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option4.CheckedChanged
		If eventSender.Checked Then
			playlist = 4
			pl1.Visible = False
			pl2.Visible = False
			pl3.Visible = False
			pl4.Visible = True
		End If
	End Sub
	
	Private Sub Text1_Change(ByRef Index As Short)
		
	End Sub
End Class
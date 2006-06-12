Option Strict Off
Option Explicit On
Friend Class Form1
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
	
	
	Dim screen As Short
	Dim dash As Short
	
	
	
	
	
	
	
	Private Sub cmdAdd_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdAdd.Click
		Dim fnum As Short
		Dim txt As String
		Dim configtag As String
		Dim currentdate As String
		currentdate = CStr(Today)
		
		fnum = FreeFile
		
		' Three section message window, saves your old netrekrc
		If screen = 0 Then
			FileCopy(".\netrekrc", ".\netrekrc.sav")
			FileCopy(".\configs\netrekrc.sep", ".\netrekrc")
		Else
			' One section message window, saves your old netrekrc
			FileCopy(".\netrekrc", ".\netrekrc.sav")
			FileCopy(".\configs\netrekrc.one", ".\netrekrc")
		End If
		
		
		' Open the file for append.
		FileOpen(fnum, ".\netrekrc", OpenMode.Append)
		' Add the command.
		If dash = 0 Then
			txt = "newdashboard:  0"
		End If
		
		If dash = 1 Then
			txt = "newdashboard:  1"
		End If
		
		If dash = 2 Then
			txt = "newdashboard:  2"
		End If
		
		If dash = 3 Then
			txt = "newdashboard:  3"
			
			
		End If
		
		configtag = "###Configuration changes added by the Netrek Configurator on " & currentdate
		
		PrintLine(fnum, Chr(13))
		PrintLine(fnum, configtag)
		PrintLine(fnum, Chr(13))
		PrintLine(fnum, txt)
		
		' Close the file.
		FileClose(fnum)
		
		'    MsgBox "Your configuration has been saved"
		
		Form2.Show()
		Me.Close()
	End Sub
	
	
	
	Private Sub Command1_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Command1.Click
		Me.Close()
	End Sub
	' Pick a dashboard
	'UPGRADE_WARNING: Event dash0.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub dash0_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles dash0.CheckedChanged
		If eventSender.Checked Then
			dash = 0
			pic0.Visible = True
			pic1.Visible = False
			pic2.Visible = False
			pic3.Visible = False
		End If
	End Sub
	
	'UPGRADE_WARNING: Event dash1.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub dash1_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles dash1.CheckedChanged
		If eventSender.Checked Then
			dash = 1
			pic0.Visible = False
			pic1.Visible = True
			pic2.Visible = False
			pic3.Visible = False
		End If
	End Sub
	
	'UPGRADE_WARNING: Event dash2.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub dash2_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles dash2.CheckedChanged
		If eventSender.Checked Then
			dash = 2
			pic0.Visible = False
			pic1.Visible = False
			pic2.Visible = True
			pic3.Visible = False
		End If
	End Sub
	
	'UPGRADE_WARNING: Event dash3.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub dash3_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles dash3.CheckedChanged
		If eventSender.Checked Then
			dash = 3
			pic0.Visible = False
			pic1.Visible = False
			pic2.Visible = False
			pic3.Visible = True
		End If
	End Sub
	
	
	
	Private Sub Form1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		dash = 3
	End Sub
	
	'UPGRADE_WARNING: Event Option1.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub Option1_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option1.CheckedChanged
		If eventSender.Checked Then
			screen = 0
			one.Visible = False
			separate.Visible = True
		End If
	End Sub
	
	'UPGRADE_WARNING: Event Option2.CheckedChanged may fire when form is initialized. Click for more: 'ms-help://MS.VSExpressCC.v80/dv_commoner/local/redirect.htm?keyword="88B12AE1-6DE0-48A0-86F1-60C0686C026A"'
	Private Sub Option2_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Option2.CheckedChanged
		If eventSender.Checked Then
			screen = 1
			separate.Visible = False
			one.Visible = True
		End If
	End Sub
End Class
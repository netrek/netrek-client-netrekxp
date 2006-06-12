VERSION 5.00
Begin VB.Form Form2 
   BackColor       =   &H80000008&
   Caption         =   "Netrek Configuration Utility (Page 2 of 3)"
   ClientHeight    =   9855
   ClientLeft      =   2355
   ClientTop       =   480
   ClientWidth     =   10905
   Icon            =   "Form2.frx":0000
   LinkTopic       =   "Form2"
   ScaleHeight     =   9855
   ScaleWidth      =   10905
   Begin VB.CheckBox Check4 
      BackColor       =   &H80000007&
      Caption         =   "Show Warpstreaks"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   3600
      TabIndex        =   14
      Top             =   8400
      Value           =   1  'Checked
      Width           =   2175
   End
   Begin VB.CheckBox Check3 
      BackColor       =   &H80000007&
      Caption         =   "Show Stars"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   720
      TabIndex        =   13
      Top             =   7800
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   5280
      TabIndex        =   12
      Top             =   9240
      Width           =   1095
   End
   Begin VB.CheckBox Check2 
      BackColor       =   &H80000012&
      Caption         =   "Enable Tip Window"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   3600
      TabIndex        =   10
      Top             =   7920
      Value           =   1  'Checked
      Width           =   2175
   End
   Begin VB.CheckBox Check1 
      BackColor       =   &H80000007&
      Caption         =   "Enable Sound"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   720
      TabIndex        =   9
      Top             =   8400
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Next"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3960
      TabIndex        =   8
      Top             =   9240
      Width           =   1095
   End
   Begin VB.OptionButton Option4 
      BackColor       =   &H80000012&
      Caption         =   "BRMH Style"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   360
      TabIndex        =   6
      Top             =   5160
      Width           =   2175
   End
   Begin VB.OptionButton Option3 
      BackColor       =   &H80000012&
      Caption         =   "Kill Watch Style (Default)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   360
      TabIndex        =   5
      Top             =   4800
      Value           =   -1  'True
      Width           =   2535
   End
   Begin VB.OptionButton Option2 
      BackColor       =   &H80000012&
      Caption         =   "COW Style"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   360
      TabIndex        =   4
      Top             =   4440
      Width           =   2295
   End
   Begin VB.OptionButton Option1 
      BackColor       =   &H80000012&
      Caption         =   "Old Style"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   360
      TabIndex        =   3
      Top             =   4080
      Width           =   2295
   End
   Begin VB.Label Label5 
      BackColor       =   &H80000008&
      Caption         =   "Misc. Options"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   375
      Left            =   360
      TabIndex        =   11
      Top             =   7200
      Width           =   1935
   End
   Begin VB.Label Label3 
      BackColor       =   &H80000012&
      Caption         =   $"Form2.frx":08CA
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   615
      Left            =   240
      TabIndex        =   7
      Top             =   3360
      Width           =   7095
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      BackColor       =   &H80000012&
      Caption         =   $"Form2.frx":09C7
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H008080FF&
      Height          =   615
      Left            =   0
      TabIndex        =   2
      Top             =   2160
      Width           =   10935
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   $"Form2.frx":0B16
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   1095
      Left            =   120
      TabIndex        =   1
      Top             =   1320
      Width           =   10695
   End
   Begin VB.Image Image1 
      Height          =   1050
      Left            =   4200
      Picture         =   "Form2.frx":0C7A
      Top             =   240
      Width           =   1980
   End
   Begin VB.Image pl4 
      Height          =   3375
      Left            =   3120
      Picture         =   "Form2.frx":7904
      Top             =   4080
      Visible         =   0   'False
      Width           =   7605
   End
   Begin VB.Image pl3 
      Height          =   3345
      Left            =   3120
      Picture         =   "Form2.frx":5B4BA
      Top             =   4080
      Width           =   7620
   End
   Begin VB.Image pl2 
      Height          =   3360
      Left            =   3120
      Picture         =   "Form2.frx":AE488
      Top             =   4080
      Visible         =   0   'False
      Width           =   7605
   End
   Begin VB.Image pl1 
      Height          =   3390
      Left            =   3120
      Picture         =   "Form2.frx":101A4A
      Top             =   4080
      Visible         =   0   'False
      Width           =   7590
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000008&
      Caption         =   "PlayerList:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   3000
      Width           =   1455
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
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
Option Explicit
Dim playlist As Integer
Dim sound As Integer
Dim tips As Integer
Dim stars As Integer
Dim warp As Integer



Private Sub Command1_Click()
Dim fnum2 As Integer
Dim pltxt As String
Dim tiptxt As String
Dim soundtxt As String
Dim startxt As String
Dim warptxt As String
Dim metatxt As String

   
    fnum2 = FreeFile
    
      
    ' Open the file for append.
    Open ".\netrekrc" For Append As fnum2
    ' Add the command.
   If (Check1.Value = vbChecked) Then
        sound = 1
   End If
   If (Check1.Value = vbUnchecked) Then
        sound = 0
    End If
    
   If (Check2.Value = vbChecked) Then
        tips = 1
   End If
   If (Check2.Value = vbUnchecked) Then
        tips = 0
    End If
    If (Check3.Value = vbChecked) Then
        stars = 1
   End If
   If (Check3.Value = vbUnchecked) Then
        stars = 0
    End If
    
   If (Check4.Value = vbChecked) Then
        warp = 1
   End If
   If (Check4.Value = vbUnchecked) Then
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
    

    Print #fnum2, pltxt
    Print #fnum2, Chr(13)
    Print #fnum2, tiptxt
    Print #fnum2, Chr(13)
    Print #fnum2, soundtxt
    Print #fnum2, Chr(13)
    Print #fnum2, startxt
    Print #fnum2, Chr(13)
    Print #fnum2, warptxt
    
    
    ' Close the file.
    Close fnum2
    
    'MsgBox "Your configuration has been saved.  Your old configuration file has been saved as netrekrc.sav in your Netrek install directory"
    Form3.Show
    Unload Form2
 End Sub



Private Sub Command2_Click()
   Unload Form2
End Sub

Private Sub Form_Load()
    playlist = 3
End Sub

Private Sub Option1_Click()
   playlist = 1
   pl1.Visible = True
   pl2.Visible = False
   pl3.Visible = False
   pl4.Visible = False
End Sub

Private Sub Option2_Click()
   playlist = 2
   pl1.Visible = False
   pl2.Visible = True
   pl3.Visible = False
   pl4.Visible = False
End Sub

Private Sub Option3_Click()
   playlist = 3
   pl1.Visible = False
   pl2.Visible = False
   pl3.Visible = True
   pl4.Visible = False
End Sub

Private Sub Option4_Click()
   playlist = 4
   pl1.Visible = False
   pl2.Visible = False
   pl3.Visible = False
   pl4.Visible = True
End Sub

Private Sub Text1_Change(Index As Integer)

End Sub

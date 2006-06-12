VERSION 5.00
Begin VB.Form Form1 
   BackColor       =   &H00000000&
   Caption         =   "Netrek Configuration Utility (Page 1 of 3)"
   ClientHeight    =   10080
   ClientLeft      =   2355
   ClientTop       =   480
   ClientWidth     =   10830
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   10080
   ScaleWidth      =   10830
   Begin VB.CommandButton Command1 
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
      TabIndex        =   13
      Top             =   9480
      Width           =   1095
   End
   Begin VB.CommandButton cmdAdd 
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
      TabIndex        =   12
      Top             =   9480
      Width           =   1095
   End
   Begin VB.Frame Frame1 
      BackColor       =   &H80000012&
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   2415
      Left            =   120
      TabIndex        =   4
      Top             =   6960
      Width           =   10815
      Begin VB.OptionButton dash0 
         BackColor       =   &H80000012&
         Caption         =   "Text Dashboard"
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
         Left            =   120
         TabIndex        =   8
         Top             =   1440
         Width           =   1815
      End
      Begin VB.OptionButton dash2 
         BackColor       =   &H80000012&
         Caption         =   "KRP Dashboard"
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
         Left            =   120
         TabIndex        =   7
         Top             =   2160
         Width           =   1815
      End
      Begin VB.OptionButton dash1 
         BackColor       =   &H80000012&
         Caption         =   "COW Dashboard"
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
         Left            =   120
         TabIndex        =   6
         Top             =   1800
         Width           =   1935
      End
      Begin VB.OptionButton dash3 
         BackColor       =   &H80000012&
         Caption         =   "Labs Dashboard (Default)"
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
         Left            =   120
         TabIndex        =   5
         Top             =   1080
         Value           =   -1  'True
         Width           =   2535
      End
      Begin VB.Image pic3 
         Height          =   735
         Left            =   2760
         Picture         =   "Form1.frx":08CA
         Top             =   1560
         Width           =   7710
      End
      Begin VB.Image pic2 
         Height          =   690
         Left            =   2760
         Picture         =   "Form1.frx":13094
         Top             =   1560
         Visible         =   0   'False
         Width           =   7665
      End
      Begin VB.Image pic1 
         Height          =   675
         Left            =   2760
         Picture         =   "Form1.frx":244D6
         Top             =   1560
         Visible         =   0   'False
         Width           =   7650
      End
      Begin VB.Image pic0 
         Height          =   720
         Left            =   2760
         Picture         =   "Form1.frx":35264
         Top             =   1560
         Visible         =   0   'False
         Width           =   7650
      End
      Begin VB.Label Label3 
         BackColor       =   &H80000012&
         Caption         =   $"Form1.frx":471E6
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
         Height          =   495
         Index           =   1
         Left            =   120
         TabIndex        =   10
         Top             =   480
         Width           =   6615
      End
      Begin VB.Label Label1 
         BackColor       =   &H00000000&
         Caption         =   "Dashboard:"
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
         Index           =   1
         Left            =   240
         TabIndex        =   9
         Top             =   120
         Width           =   1455
      End
   End
   Begin VB.OptionButton Option1 
      BackColor       =   &H00000000&
      Caption         =   "Separate Message Windows (Default)"
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
      Left            =   240
      TabIndex        =   1
      Top             =   4320
      Value           =   -1  'True
      Width           =   3615
   End
   Begin VB.OptionButton Option2 
      BackColor       =   &H00000000&
      Caption         =   "One Message Window"
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
      Left            =   240
      TabIndex        =   0
      Top             =   4680
      Width           =   2655
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   $"Form1.frx":47278
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
      Height          =   855
      Left            =   0
      TabIndex        =   14
      Top             =   1320
      Width           =   10695
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      BackColor       =   &H80000012&
      Caption         =   $"Form1.frx":473DC
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
      TabIndex        =   11
      Top             =   2280
      Width           =   10695
   End
   Begin VB.Label Label3 
      BackColor       =   &H00000000&
      Caption         =   $"Form1.frx":4752C
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
      Height          =   735
      Index           =   0
      Left            =   120
      TabIndex        =   3
      Top             =   3600
      Width           =   6135
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      Caption         =   "Message Windows:"
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
      Index           =   0
      Left            =   240
      TabIndex        =   2
      Top             =   3240
      Width           =   2415
   End
   Begin VB.Image separate 
      Height          =   2355
      Left            =   4440
      Picture         =   "Form1.frx":475F8
      Top             =   4320
      Width           =   5415
   End
   Begin VB.Image one 
      Height          =   2325
      Left            =   4440
      Picture         =   "Form1.frx":70F06
      Top             =   4320
      Visible         =   0   'False
      Width           =   5355
   End
   Begin VB.Image Image1 
      Height          =   1050
      Left            =   4200
      Picture         =   "Form1.frx":99858
      Top             =   240
      Width           =   1980
   End
End
Attribute VB_Name = "Form1"
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


Option Explicit
Dim screen As Integer
Dim dash As Integer







Private Sub cmdAdd_Click()
Dim fnum As Integer
Dim txt As String
Dim configtag As String
Dim currentdate As String
currentdate = Date
   
    fnum = FreeFile
    
    ' Three section message window, saves your old netrekrc
    If screen = 0 Then
       FileCopy ".\netrekrc", ".\netrekrc.sav"
       FileCopy ".\configs\netrekrc.sep", ".\netrekrc"
    Else
    ' One section message window, saves your old netrekrc
      FileCopy ".\netrekrc", ".\netrekrc.sav"
      FileCopy ".\configs\netrekrc.one", ".\netrekrc"
    End If

    
    ' Open the file for append.
    Open ".\netrekrc" For Append As fnum
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
    
    configtag = "###Configuration changes added by the Netrek Configurator on " + currentdate
    
    Print #fnum, Chr(13)
    Print #fnum, configtag
    Print #fnum, Chr(13)
    Print #fnum, txt
    
    ' Close the file.
    Close fnum
    
'    MsgBox "Your configuration has been saved"
    
    Form2.Show
    Unload Form1
End Sub



Private Sub Command1_Click()
    Unload Form1
End Sub
' Pick a dashboard
Private Sub dash0_Click()
   dash = 0
   pic0.Visible = True
   pic1.Visible = False
   pic2.Visible = False
   pic3.Visible = False
End Sub

Private Sub dash1_Click()
   dash = 1
   pic0.Visible = False
   pic1.Visible = True
   pic2.Visible = False
   pic3.Visible = False
End Sub

Private Sub dash2_Click()
   dash = 2
   pic0.Visible = False
   pic1.Visible = False
   pic2.Visible = True
   pic3.Visible = False
End Sub

Private Sub dash3_Click()
   dash = 3
   pic0.Visible = False
   pic1.Visible = False
   pic2.Visible = False
   pic3.Visible = True
End Sub



Private Sub Form_Load()
   dash = 3
End Sub

Private Sub Option1_Click()
   screen = 0
   one.Visible = False
   separate.Visible = True
End Sub

Private Sub Option2_Click()
    screen = 1
    separate.Visible = False
    one.Visible = True
End Sub


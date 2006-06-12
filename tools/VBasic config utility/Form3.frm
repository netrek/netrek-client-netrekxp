VERSION 5.00
Begin VB.Form Form3 
   BackColor       =   &H80000007&
   Caption         =   "Netrek Configuration Utility (Page 3 of 3)"
   ClientHeight    =   9900
   ClientLeft      =   2475
   ClientTop       =   465
   ClientWidth     =   10875
   Icon            =   "Form3.frx":0000
   LinkTopic       =   "Form3"
   ScaleHeight     =   9900
   ScaleWidth      =   10875
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   22
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   55
      Text            =   "4"
      Top             =   8160
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   21
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   54
      Text            =   "3"
      Top             =   7800
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   20
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   53
      Text            =   "2"
      Top             =   7440
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   19
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   52
      Text            =   "0"
      Top             =   7080
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   18
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   51
      Text            =   "g"
      Top             =   6720
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   17
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   50
      Text            =   "d"
      Top             =   6360
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   16
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   49
      Text            =   "D"
      Top             =   6000
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   15
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   48
      Text            =   "\"
      Top             =   5640
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   14
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   47
      Text            =   "i"
      Top             =   4680
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   13
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   46
      Text            =   "l"
      Top             =   4320
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   12
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   45
      Text            =   "r"
      Top             =   3960
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   11
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   44
      Text            =   "R"
      Top             =   8760
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   10
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   43
      Text            =   "#"
      Top             =   8400
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   9
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   42
      Text            =   "@"
      Top             =   8040
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   8
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   41
      Text            =   "T"
      Top             =   7680
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   7
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   40
      Text            =   "y"
      Top             =   7320
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   6
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   39
      Text            =   "z"
      Top             =   6600
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   5
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   38
      Text            =   "x"
      Top             =   6240
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   4
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   37
      Text            =   "c"
      Top             =   5520
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   3
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   36
      Text            =   "b"
      Top             =   5160
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   2
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   34
      Text            =   "s"
      Top             =   4800
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   1
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   33
      Text            =   "f"
      Top             =   4440
      Width           =   495
   End
   Begin VB.ComboBox Combo1 
      Height          =   315
      ItemData        =   "Form3.frx":08CA
      Left            =   8520
      List            =   "Form3.frx":08D4
      TabIndex        =   32
      Text            =   "Torpedoes"
      Top             =   5160
      Width           =   1935
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Finish"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   4680
      TabIndex        =   21
      Top             =   9360
      Width           =   1095
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   0
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   20
      Text            =   "t"
      Top             =   4080
      Width           =   495
   End
   Begin VB.Label Label31 
      BackColor       =   &H80000007&
      Caption         =   "Quit"
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
      Left            =   4080
      TabIndex        =   35
      Top             =   5640
      Width           =   615
   End
   Begin VB.Label Label33 
      BackColor       =   &H80000008&
      Caption         =   "Mouse Controls:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFF00&
      Height          =   375
      Left            =   7320
      TabIndex        =   31
      Top             =   4680
      Width           =   2055
   End
   Begin VB.Line Line4 
      BorderColor     =   &H80000009&
      X1              =   8400
      X2              =   8400
      Y1              =   3840
      Y2              =   3600
   End
   Begin VB.Label Label30 
      BackColor       =   &H80000007&
      Caption         =   "Left Button"
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
      Left            =   7440
      TabIndex        =   30
      Top             =   5160
      Width           =   1215
   End
   Begin VB.Line Line3 
      BorderColor     =   &H80000009&
      X1              =   120
      X2              =   8400
      Y1              =   3600
      Y2              =   3600
   End
   Begin VB.Line Line2 
      BorderColor     =   &H80000009&
      X1              =   120
      X2              =   120
      Y1              =   3840
      Y2              =   3600
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000009&
      X1              =   120
      X2              =   8400
      Y1              =   3840
      Y2              =   3840
   End
   Begin VB.Label Label29 
      BackColor       =   &H80000012&
      Caption         =   "Key to Assign"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   6480
      TabIndex        =   29
      Top             =   3600
      Width           =   1575
   End
   Begin VB.Label Label28 
      BackColor       =   &H80000012&
      Caption         =   "Control"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFF00&
      Height          =   255
      Left            =   4080
      TabIndex        =   28
      Top             =   3600
      Width           =   855
   End
   Begin VB.Label Label27 
      BackColor       =   &H80000012&
      Caption         =   "Key to Assign"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   2160
      TabIndex        =   27
      Top             =   3600
      Width           =   1575
   End
   Begin VB.Label Label26 
      BackColor       =   &H80000012&
      Caption         =   "Control"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFF00&
      Height          =   255
      Left            =   480
      TabIndex        =   26
      Top             =   3600
      Width           =   855
   End
   Begin VB.Label Label25 
      BackColor       =   &H80000008&
      Caption         =   "Keyboard Controls:"
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
      Left            =   120
      TabIndex        =   25
      Top             =   3120
      Width           =   2535
   End
   Begin VB.Image Image1 
      Height          =   1050
      Left            =   4200
      Picture         =   "Form3.frx":08EC
      Top             =   120
      Width           =   1980
   End
   Begin VB.Label Label23 
      Alignment       =   2  'Center
      BackColor       =   &H80000012&
      Caption         =   $"Form3.frx":7576
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FFFF&
      Height          =   1695
      Left            =   120
      TabIndex        =   24
      Top             =   1320
      Width           =   10695
   End
   Begin VB.Label Label22 
      BackColor       =   &H80000007&
      Caption         =   "Warp 0"
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
      Left            =   4080
      TabIndex        =   23
      Top             =   7080
      Width           =   975
   End
   Begin VB.Label Label21 
      BackColor       =   &H80000007&
      Caption         =   "Warp 4"
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
      Left            =   4080
      TabIndex        =   22
      Top             =   8160
      Width           =   855
   End
   Begin VB.Label Label20 
      BackColor       =   &H80000007&
      Caption         =   "Pickup Armies"
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
      Left            =   480
      TabIndex        =   19
      Top             =   6600
      Width           =   1335
   End
   Begin VB.Label Label19 
      BackColor       =   &H80000007&
      Caption         =   "Pressor"
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
      Left            =   480
      TabIndex        =   18
      Top             =   7320
      Width           =   855
   End
   Begin VB.Label Label18 
      BackColor       =   &H80000007&
      Caption         =   "Warp 3"
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
      Left            =   4080
      TabIndex        =   17
      Top             =   7800
      Width           =   975
   End
   Begin VB.Label Label17 
      BackColor       =   &H80000007&
      Caption         =   "Refit to New Ship"
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
      Left            =   4080
      TabIndex        =   16
      Top             =   4080
      Width           =   1695
   End
   Begin VB.Label Label16 
      BackColor       =   &H80000007&
      Caption         =   "MaxWarp"
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
      Left            =   480
      TabIndex        =   15
      Top             =   8040
      Width           =   975
   End
   Begin VB.Label Label15 
      BackColor       =   &H80000007&
      Caption         =   "Half Speed"
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
      Left            =   480
      TabIndex        =   14
      Top             =   8400
      Width           =   1215
   End
   Begin VB.Label Label14 
      BackColor       =   &H80000007&
      Caption         =   "Target Information"
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
      Left            =   4080
      TabIndex        =   13
      Top             =   4800
      Width           =   1935
   End
   Begin VB.Label Label13 
      BackColor       =   &H80000007&
      Caption         =   "Tractor"
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
      Left            =   480
      TabIndex        =   12
      Top             =   7680
      Width           =   975
   End
   Begin VB.Label Label12 
      BackColor       =   &H80000007&
      Caption         =   "Warp 2"
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
      Left            =   4080
      TabIndex        =   11
      Top             =   7440
      Width           =   1095
   End
   Begin VB.Label Label11 
      BackColor       =   &H80000007&
      Caption         =   "Repair"
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
      Left            =   480
      TabIndex        =   10
      Top             =   8760
      Width           =   1455
   End
   Begin VB.Label Label10 
      BackColor       =   &H80000007&
      Caption         =   "Lock onto Target"
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
      Left            =   4080
      TabIndex        =   9
      Top             =   4440
      Width           =   1935
   End
   Begin VB.Label Label9 
      BackColor       =   &H80000007&
      Caption         =   "Plasma"
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
      Left            =   4080
      TabIndex        =   8
      Top             =   6720
      Width           =   855
   End
   Begin VB.Label Label8 
      BackColor       =   &H80000007&
      Caption         =   "Phasers"
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
      Left            =   480
      TabIndex        =   7
      Top             =   4440
      Width           =   735
   End
   Begin VB.Label Label7 
      BackColor       =   &H80000007&
      Caption         =   "Drop Armies"
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
      Left            =   480
      TabIndex        =   6
      Top             =   6240
      Width           =   1215
   End
   Begin VB.Label Label6 
      BackColor       =   &H80000007&
      Caption         =   "Detonate Enemy Torps"
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
      Left            =   4080
      TabIndex        =   5
      Top             =   6360
      Width           =   2055
   End
   Begin VB.Label Label5 
      BackColor       =   &H80000007&
      Caption         =   "Detonate Own Torps"
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
      Left            =   4080
      TabIndex        =   4
      Top             =   6000
      Width           =   1815
   End
   Begin VB.Label Label4 
      BackColor       =   &H80000007&
      Caption         =   "Bomb"
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
      Left            =   480
      TabIndex        =   3
      Top             =   5160
      Width           =   495
   End
   Begin VB.Label Label3 
      BackColor       =   &H80000007&
      Caption         =   "Cloak"
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
      Left            =   480
      TabIndex        =   2
      Top             =   5520
      Width           =   615
   End
   Begin VB.Label Label2 
      BackColor       =   &H80000007&
      Caption         =   "Shield"
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
      Left            =   480
      TabIndex        =   1
      Top             =   4800
      Width           =   615
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000007&
      Caption         =   "Torpedoes"
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
      Left            =   480
      TabIndex        =   0
      Top             =   4080
      Width           =   1095
   End
End
Attribute VB_Name = "Form3"
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



'Keymap configuration
Private Sub Command1_Click()
    
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
    
    
    Dim a As Integer
    Dim b As Integer
    Dim c As Integer
    
    Dim m As String
    Dim h As String
      
    Dim duplicate As Integer
    Dim speckey As Integer
    
    Dim currentdate As String
    
    currentdate = Date
    fnum3 = FreeFile
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
        
    If torpedo = Text1(0) Then
       keymap1 = ""
    Else
       keymap1 = Text1(0) + torpedo
    End If
    
    If phaser = Text1(1) Then
       keymap2 = ""
    Else
       keymap2 = Text1(1) + phaser
    End If
    
    
    If shield = Text1(2) Then
       keymap3 = ""
    Else
       keymap3 = Text1(2) + shield
    End If
    
    If bomb = Text1(3) Then
       keymap4 = ""
    Else
       keymap4 = Text1(3) + bomb
    End If
    
    If cloak = Text1(4) Then
       keymap5 = ""
    Else
       keymap5 = Text1(4) + cloak
    End If
    
    If drop = Text1(5) Then
       keymap6 = ""
    Else
       keymap6 = Text1(5) + drop
    End If
    
    If pickup = Text1(6) Then
       keymap7 = ""
    Else
       keymap7 = Text1(6) + pickup
    End If
    
    If pressor = Text1(7) Then
       keymap8 = ""
    Else
       keymap8 = Text1(7) + pressor
    End If
    
    If tractor = Text1(8) Then
       keymap9 = ""
    Else
       keymap9 = Text1(8) + tractor
    End If
    
    If maxwarp = Text1(9) Then
       keymap10 = ""
    Else
       keymap10 = Text1(9) + maxwarp
    End If
    
    If halfspeed = Text1(10) Then
       keymap11 = ""
    Else
       keymap11 = Text1(10) + halfspeed
    End If
    
    If repair = Text1(11) Then
       keymap12 = ""
    Else
       keymap12 = Text1(11) + repair
    End If
    
    If refit = Text1(12) Then
       keymap13 = ""
    Else
       keymap13 = Text1(12) + refit
    End If
    
    If tlock = Text1(13) Then
       keymap14 = ""
    Else
       keymap14 = Text1(14) + tlock
    End If
    
    If info = Text1(14) Then
       keymap15 = ""
    Else
       keymap15 = Text1(14) + info
    End If
    
    If quit = Text1(15) Then
       keymap16 = ""
    Else
       keymap16 = Text1(15) + quit
    End If
    
    If detown = Text1(16) Then
       keymap17 = ""
    Else
       keymap17 = Text1(16) + detown
    End If
    
    If detenemy = Text1(17) Then
       keymap18 = ""
    Else
       keymap18 = Text1(17) + detenemy
    End If
    
    If plasma = Text1(18) Then
       keymap19 = ""
    Else
       keymap19 = Text1(18) + plasma
    End If
    
    If warp0 = Text1(19) Then
       keymap20 = ""
    Else
       keymap20 = Text1(19) + warp0
    End If
    
    If warp2 = Text1(20) Then
       keymap21 = ""
    Else
       keymap21 = Text1(20) + warp2
    End If
    
    If warp3 = Text1(21) Then
       keymap22 = ""
    Else
       keymap22 = Text1(21) + warp3
    End If
    
    If warp4 = Text1(22) Then
       keymap23 = ""
    Else
       keymap23 = Text1(22) + warp4
    End If
    
    
      
    speckey = 0
    duplicate = 0
    For a = 0 To 22
       For b = 0 To 22
            If a <> b And Text1(a) = Text1(b) Then
               duplicate = 1
               Exit For
            End If
       Next b
            If duplicate = 1 Then
                MsgBox "The key '" + Text1(a) + "' is defined more than once.  This will cause problems while playing.  Please remove the duplicate controls and click on Finish."
                Exit For
            End If
       Next a
       
     
       For c = 0 To 22
           If Text1(c) = "m" Then
               speckey = 1
           End If
           If Text1(c) = "h" Then
               speckey = 1
           End If
           If Text1(c) = "w" Then
               speckey = 1
           End If
           If speckey = 1 Then
                MsgBox "You cannot use the 'm', 'h', or 'w' keys.  These are used for messaging, help, and war declarations.  Please assign different keys to these controls."
           Exit For
          End If
       Next c
    
    
       If duplicate = 0 And speckey = 0 Then
       ' Open the file for append.
       Open ".\netrekrc" For Append As fnum2
       ' Add the command.
       If Combo1 = "Torpedoes" Then
           buttonmap = "buttonmap:" + "1" + Text1(0)
       End If
       If Combo1 = "Phasers" Then
           buttonmap = "buttonmap:" + "1" + Text1(1)
       End If
       
       keymap = "keymap:" + "\Qq0Q0gffp" + keymap1 + keymap2 + keymap3 + keymap4 + keymap5 + keymap6 + keymap7 + keymap8 + keymap9 + keymap10 + keymap11 + keymap12 + keymap13 + keymap14 + keymap15 + keymap16 + keymap17 + keymap18 + keymap19 + keymap20 + keymap21 + keymap22 + keymap23
       keytag = "###Keymap added by the Netrek Configurator on " + currentdate
       mousetag = "###Buttonmap added by the Netrek Configurator on " + currentdate
       
       Print #fnum2, Chr(13)
       Print #fnum2, keytag
       Print #fnum2, keymap
       Print #fnum2, Chr(13)
       Print #fnum2, mousetag
       Print #fnum2, buttonmap
       Close fnum2
       
       ' Opens the controls you chose in notepad for reference
       Open ".\controls.txt" For Output As #fnum3
       Print #fnum3, "These are the controls you have chosen:"
       Print #fnum3, Chr(13)
       Print #fnum3, Chr(13)
       Print #fnum3, "Mouse Controls:"
       Print #fnum3, Chr(13)
       Print #fnum3, "Left Button     -  " + Combo1
       Print #fnum3, "Middle Button   -  " + "Unmap Special Windows (i.e. Close tip window)"
       Print #fnum3, "Right Button    -  " + "Steer"
       Print #fnum3, Chr(13)
       Print #fnum3, Chr(13)
       Print #fnum3, Chr(13)
       Print #fnum3, "Keyboard Controls:"
       Print #fnum3, Chr(13)
       Print #fnum3, "Messaging            -  " + "m (Be sure to hold your mouse cursor over"
       Print #fnum3, "                           " + "the Galaxy or Tactical maps when you press 'm'."
       Print #fnum3, "                           " + "After pressing 'm' type 'A' to send messages to"
       Print #fnum3, "                           " + "the All Window, type 'T' to send messages"
       Print #fnum3, "                           " + "to the Team Window, or type the letter/number"
       Print #fnum3, "                           " + "of the player you would like to send a message to."
       Print #fnum3, "                           " + "After you type A, T, or the player letter/number, "
       Print #fnum3, "                           " + "type your message and press enter.)"
       Print #fnum3, "Help                 -  " + "h"
       Print #fnum3, Chr(13)
       Print #fnum3, "Torpedoes            -  " + Text1(0)
       Print #fnum3, "Phasers              -  " + Text1(1)
       Print #fnum3, "Shield               -  " + Text1(2)
       Print #fnum3, "Bomb                 -  " + Text1(3)
       Print #fnum3, "Cloak                -  " + Text1(4)
       Print #fnum3, Chr(13)
       Print #fnum3, "Drop Armies          -  " + Text1(5)
       Print #fnum3, "Pickup Armies        -  " + Text1(6)
       Print #fnum3, Chr(13)
       Print #fnum3, "Detonate Own Torps   -  " + Text1(16)
       Print #fnum3, "Detonate Enemy Torps -  " + Text1(17)
       Print #fnum3, Chr(13)
       Print #fnum3, "Tractor              -  " + Text1(8)
       Print #fnum3, "Pressor              -  " + Text1(7)
       Print #fnum3, Chr(13)
       Print #fnum3, "Repair               -  " + Text1(11)
       Print #fnum3, "Refit Ship Type      -  " + Text1(12)
       Print #fnum3, "Lock on Target       -  " + Text1(13)
       Print #fnum3, "Target Info          -  " + Text1(14)
       Print #fnum3, Chr(13)
       Print #fnum3, "Max Warp             -  " + Text1(9)
       Print #fnum3, "Half Speed           -  " + Text1(10)
       Print #fnum3, "Quit                 -  " + Text1(15)
       Print #fnum3, "Plasma               -  " + Text1(18)
       Print #fnum3, Chr(13)
       Print #fnum3, "Warp 0               -  " + Text1(19)
       Print #fnum3, "Warp 2               -  " + Text1(20)
       Print #fnum3, "Warp 3               -  " + Text1(21)
       Print #fnum3, "Warp 4               -  " + Text1(22)
            
       Close #fnum3

      
       Unload Form3
       MsgBox "YOUR CONFIGURATION HAS BEEN CREATED!  After you click on OK a document with the controls you have chosen will open.  A shortcut to this document has been created in your Netrek Config Utility program group.  If you need additional assistance please visit www.netrek.org."
       Shell Application & " " & FileName, vbNormalFocus
     Else
     End If
End Sub


#
# =========================================================================
# COW on Win32 Makefile
#
# This makefile is written for Borland C++ 5.5
#
#

# Definitions needed to be changed before compile

#DEBUG = 1

RSA = 1

RSAKEY = \netrek_files\rsa\NetrekXP-Mod-RSA-Key-Win32.secret

# Borland C++ 5.5
cc     = bcc32
rc     = brcc32
link   = ilink32
compileandlink = bcc32

# Borland C++ 5.5 options
# /MT       - Multi-threaded app
# /Ox       - max optimization
# /Od       - disable optimizations
# -4        - 486 optimized code
# -5        - Pentium optimized code
# -c        - compile only
# -v        - debugging info
# -O1       - Optimize for size
# -O2       - Optimize for speed


!IFDEF DEBUG
cflags = -O2 -5 -v -DWIN32 -DBorland -tW -tWM
ccompileonly = -c
!ELSE
cflags = -O2 -5 -DWIN32 -DBorland -tW -tWM
ccompileonly = -c
!ENDIF

# resource compiler
rcvars = -DWIN32 

########################################
# Link flags
#

!IFDEF DEBUG
ldebug = /Gn /v 
!ELSE
ldebug = /Gn /w-dup
!ENDIF

########################################

# declarations common to all linker options

#Dunno here...
conlflags = $(lflags) /NODEFAULTLIB /SUBSYSTEM:CONSOLE /MACHINE:IX86
lout      = "/OUT:"

#LIBS =  libcmt.lib user32.lib gdi32.lib\
#        wsock32.lib kernel32.lib shell32.lib winmm.lib mpr.lib

# Borland Libs
LIBOPTS = /LC:\Borland\bcc55\lib;c:\Borland\bcc55\lib\psdk /Tpe

!IFDEF RSA
GMPLIB = ..\Win32\lib\libgmpbcc.lib 
#..\Win32\lib\libmpbcc.lib
!ENDIF

LIBS = CW32mt.LIB IMPORT32.LIB user32.lib gdi32.lib wsock32.lib kernel32.lib shell32.lib winmm.lib mpr.lib $(GMPLIB)

# ---------------------------------------------------------------------------
# - Start of makefile proper

INCS = -I..\win32 -I..\win32\h -I..\win32\h\gmp -I..\include
ARCH = Intel/Win32

!IFDEF RSA
RSADEFS = -DRSA
!endif

!ifdef DEBUG
DEBUGDEFS = -DDEBUG
!endif

ROBJ            = check.obj colors.obj data.obj death.obj defaults.obj dmessage.obj\
                  enter.obj findslot.obj getname.obj getship.obj helpwin.obj hintwin.obj inform.obj\
                  interface.obj newwin.obj option.obj planetlist.obj macrowin.obj\
                  map.obj playerlist.obj ranklist.obj reserved.obj sintab.obj\
                  smessage.obj socket.obj stats.obj util.obj war.obj warning.obj\
                  udpopt.obj ping.obj pingstats.obj rotate.obj parsemeta.obj\
                  spopt.obj dashboard.obj dashboard3.obj\
                  short.obj distress.obj senddist.obj defwin.obj tools.obj sound.obj\
                  docwin.obj feature.obj\
                  string_util.obj local.obj cowmain.obj playback.obj

RSRC            = check.c colors.c data.c death.c defaults.c dmessage.c\
                  enter.c findslot.c getname.c getship.c helpwin.c hintwin.c inform.c\
                  input.c interface.c newwin.c option.c planetlist.c\
                  macrowin.c map.c playerlist.c ranklist.c redraw.c\
                  smessage.c parsemeta.c socket.c stats.c util.c war.c\
                  warning.c udpopt.c sintab.c ping.c pingstats.c rotate.c\
                  spopt.c dashboard.c dashboard3.c\
                  short.c distress.c senddist.c defwin.c tools.c sound.c\
                  docwin.c feature.c reserved.c\
                  string_util.c local.c cowmain.c playback.c

INPUTOBJ        = input.obj redraw.obj 
MAINOBJ         = main.obj
MAINSRC         = main.c

!ifdef RSA
RSASRC = rsa_box.c rsa_box_0.c rsa_box_1.c rsa_box_2.c\
		rsa_box_3.c rsa_box_4.c
RSAOBJ = rsa_box.obj rsa_box_0.obj rsa_box_1.obj rsa_box_2.obj\
		rsa_box_3.obj rsa_box_4.obj
!endif

WIN32_SRCS = winmain.c mswindow.c winsndlib.c
WIN32_OBJS = winmain.obj mswindow.obj winsndlib.obj

RANDOMOBJ = random.obj

INCLUDES        = struct.h packets.h defs.h copyright.h bitmaps.h data.h\
                  oldbitmaps.h tngbitmaps.h hullbitmaps.h rabbitbitmaps.h\
                  sound.h audio.h litebitmaps.h copyright2.h Wlib.h\
		  playerlist.h version.h moobitmaps.h\
		  parsemeta.h spopt.h map.h string_util.h wtext.h local.h

all: netrek.exe


OBJS = $(ROBJ) $(MAINOBJ) $(RSAOBJ) $(INPUTOBJ) $(WIN32_OBJS)
.c.obj:
        $(cc) $(cflags) $(ccompileonly) $(cdebug) $(DEFS) $(RSADEFS) $(DEBUGDEFS) $(INCS) $<

RESOURCES = bccnetrek.res

$(RESOURCES): bccnetrek.rc
        $(rc) bccnetrek.rc

rsa_box.c: 
	..\tools\mkkey -nt -k $(RSAKEY)

netrek.exe:  $(OBJS) $(RESOURCES) main.ico
                $(link) $(ldebug) $(LIBOPTS) $(OBJS) C0x32.OBJ, netrek.exe,, $(LIBS), , $(RESOURCES)

clean:
	del *.obj
	del rsa*.c
	del netrek.exe netrek.ilc netrek.ild netrek.ild netrek.ilf netrek.ils netrek.map bccnetrek.res netrek.tds
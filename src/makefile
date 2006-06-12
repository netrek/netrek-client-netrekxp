#
# This makefile is written for Borland C++ 5.5
#

#DEBUG = 1
RSA = 1

# Borland C++ 5.5
cc     = bcc32
rc     = brcc32
link   = ilink32
compileandlink = bcc32

# Compiler and linker flags
!IFDEF DEBUG
cflags = -O2 -5 -v -DWIN32 -DBorland -tW -tWM
ccompileonly = -c
ldebug = -Gn -v 
DEBUGDEFS = -DDEBUG
!ELSE
cflags = -O2 -5 -DWIN32 -DBorland -tW -tWM
ccompileonly = -c
ldebug = -Gn -w-dup
!ENDIF

# Libs
LIBOPTS = /L..\Win32\lib\BCC /Tpe

LIBS = SDL.LIB SDL_mixer.LIB SDLmain.LIB CW32mt.LIB IMPORT32.LIB user32.lib gdi32.lib wsock32.lib kernel32.lib shell32.lib winmm.lib mpr.lib msimg32.lib $(GMPLIB)

INCS = -I..\win32 -I..\win32\h -I..\win32\SDL -I..\win32\h\gmp -I..\include

ROBJ            = beeplite.obj check.obj colors.obj console.obj data.obj death.obj defaults.obj dmessage.obj\
                  enter.obj findslot.obj getname.obj getship.obj helpwin.obj hintwin.obj inform.obj\
                  interface.obj newwin.obj option.obj planetlist.obj macrowin.obj\
                  map.obj playerlist.obj ranklist.obj reserved.obj sintab.obj\
                  smessage.obj socket.obj stats.obj util.obj war.obj warning.obj\
                  udpopt.obj ping.obj pingstats.obj rotate.obj parsemeta.obj\
                  spopt.obj dashboard.obj dashboard3.obj\
                  short.obj distress.obj senddist.obj defwin.obj tools.obj sound.obj\
                  docwin.obj feature.obj\
                  string_util.obj local.obj cowmain.obj playback.obj

RSRC            = beeplite.c check.c colors.c console.c data.c death.c defaults.c dmessage.c\
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
GMPLIB = ..\Win32\lib\BCC\libgmpbcc.lib
RSADEFS = -DRSA
RSASRC = rsa_box.c rsa_box_0.c rsa_box_1.c rsa_box_2.c\
                  rsa_box_3.c rsa_box_4.c
RSAOBJ = rsa_box.obj rsa_box_0.obj rsa_box_1.obj rsa_box_2.obj\
                  rsa_box_3.obj rsa_box_4.obj
!endif

WIN32_SRCS = winmain.c mswindow.c winsndlib.c
WIN32_OBJS = winmain.obj mswindow.obj winsndlib.obj

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

netrek.exe:  $(OBJS) $(RESOURCES) main.ico
                $(link) $(ldebug) $(LIBOPTS) $(OBJS) C0x32.OBJ, netrek.exe,, $(LIBS), , $(RESOURCES)

clean:
	del *.obj
	del netrek.exe netrek.ilc netrek.ild netrek.ild netrek.ilf netrek.ils netrek.map bccnetrek.res netrek.tds
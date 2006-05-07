#
# This makefile is written for Borland C++ 5.5
#

# Borland C++ 5.5
cc     = bcc32
rc     = brcc32
link   = ilink32
compileandlink = bcc32

!IFDEF DEBUG
cflags = -O2 -5 -v -pr -DWIN32 -DBorland -tW -tWM
ccompileonly = -c
!ELSE
cflags = -O2 -5 -pr -DWIN32 -DBorland -tW -tWM
ccompileonly = -c
!ENDIF

all: winkey.dll

.cpp.obj:
        $(cc) $(cflags) $(ccompileonly) $<

winkey.dll:  winkey.obj winkeyshared.obj
	$(link) $(LIBOPTS) -Tpd -aa -V4.0 -c -x -GS:SHSEG=rws C0d32.OBJ winkey.obj winkeyshared.obj, winkey.dll,, import32.lib cw32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib, winkeybcc.def

clean:
	del winkey.dll *.obj *.ilc *.ild *.ilf *.ils *.tds
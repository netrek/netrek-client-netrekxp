#
# This makefile is written for Borland C++ 5.5
#

# Borland C++ 5.5
cc     = bcc32
rc     = brcc32
link   = ilink32
compileandlink = bcc32

all: curslib.dll

RESOURCES = curslib.res

$(RESOURCES): curslib.rc
        $(rc) -FOcurslib.res curslib.rc

curslib.dll:  $(RESOURCES)
	$(link) -Tpd -aa -V4.0 -c -x C0d32.OBJ, curslib.dll,, import32.lib cw32i.lib, , $(RESOURCES)

clean:
	del curslib.dll curslib.ilc curslib.ild curslib.ilf curslib.ils curslib.res curslib.tds
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

# Linker options
# -Tpd 				This tells the linker the type of file you want to create. You can use -Tpe to create an exe. 
# -aa 				This tells the linker that this will be a 32 bit Windows application. 
# -V4.0 			This specifies the target Windows version that this DLL will be used for. 4.0 represents Windows 95 and later. 
# -c 				This tells the linker that you want case sensitive symbols.  I'm not sure that this is critical but since C is case sensitive it's a good idea to include it. 
# -x 				This suppresses the creation of a MAP file. A MAP file is a text file which list all the memory locations of the symbols of a C program. Since this is simply a resource file we're making, it's not that useful to have a MAP file. 
# c0d32.obj 			This object file is required to link into the dll. It allows the DLL to be loaded into memory only when needed. 
# myresource.dll 		This is the output DLL file that will be created. 
# import32.lib and cw32i.lib 	These libraries are needed for the DLL. They're used to access the Windows API. 
# myresource.res 		This is your compiled RES file that will be linked. 

# ---------------------------------------------------------------------------
# - Start of makefile proper

all: curslib.dll

RESOURCES = curslib.res

$(RESOURCES): curslib.rc
        $(rc) -FOcurslib.res curslib.rc


curslib.dll:  $(RESOURCES)
	$(link) -Tpd -aa -V4.0 -c -x C0d32.OBJ, curslib.dll,, import32.lib cw32i.lib, , $(RESOURCES)


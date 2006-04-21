/******************************************************************************/
/***  File:  richedit.c
/******************************************************************************/

/* WINVER and _WIN32_WINNT are for wheel mouse and 5 mouse button support */
#ifndef WINVER
#define WINVER 0x5000
#endif
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>            // Had to include to support wheel mouse
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <richedit.h>

#include "copyright2.h"
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "xclrs.h"
#include "cursors.h"
#include "proto.h"
#include "resource.h"



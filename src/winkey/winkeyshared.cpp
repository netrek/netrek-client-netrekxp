#pragma option -zRSHSEG	// sets segment name for this source file to SHARESEG
#pragma option -zTSHARECLS		// sets class name for segment to SHARECLS
#include <windows.h>

HHOOK g_hMsgHook = NULL;
HHOOK g_hKeyHook = NULL;

int   g_iFlags = NULL;

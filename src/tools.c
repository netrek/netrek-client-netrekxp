/* tools.c - shell escape, graphic toolsw - 10/10/93
 * 
 * copyright 1993 Kurt Siegl <siegl@risc.uni-linz.ac.at> Free to use, hack, etc.
 * Just keep these credits here. Use of this code may be dangerous to your
 * health and/or system. Its use is at your own risk. I assume no
 * responsibility for damages, real, potential, or imagined, resulting  from
 * the use of it.
 * 
 */

#include "config.h"

#ifdef TOOLS
#include <stdio.h>
#include "math.h"
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

void
sendTools (char *str)
{
    char pipebuf[100];
    char c;
    int i;

    if (sscanf (str, "set %s %c", pipebuf, &c) == 2)
    {
        for (i = 0; keys[i]; i++)
        {
            if (strcmpi (macroKeys[i].name, pipebuf) == 0)
            {
                macroKeys[i].dest = c;
                W_WriteText (toolsWin, 0, 0, textColor, str, strlen (str),
                             W_RegularFont);
                return;
            }
        }
    }

    if (!W_IsMapped (toolsWin))
        showToolsWin ();
    W_WriteText (toolsWin, 0, 0, textColor, str, strlen (str), W_RegularFont);
}

showToolsWin (void)
{
    if (W_IsMapped (toolsWin))
        W_UnmapWindow (toolsWin);
    else
        W_MapWindow (toolsWin);
}

#endif /* TOOLS */

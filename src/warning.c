/******************************************************************************/
/***  File:  warning.c
/***
/***  Function:
/***
/***  Revisions:
/***
/******************************************************************************/

#include "config.h"
#include "copyright.h"

#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <time.h>

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* * The warning in text will be printed in the warning window. * The message
 * will last WARNTIME/10 seconds unless another message * comes through and
 * overwrites it. */
warning (char *text)
{
    int doPhaser;
    LONG curtime;
    struct tm *tm;
    char newtext[128];
    char newtext2[128];
    if (warncount > 0)
    {
        /* XFIX */
        W_ClearArea (warnw, 5, 5, W_Textwidth * warncount, W_Textheight);
    }

    doPhaser = (strncmp (text, "Phaser burst", 12) == 0);

    warncount = strlen (text);
#ifdef PHASER_STATS
    if (doPhaser && phaserStats)
    {
        sprintf (newtext, "%s [%3d%%] [%2u]", text,
                 phaserStatTry ? (phaserStatHit * 100) / phaserStatTry : 0,
                 phaserStatTry ? phaserStatDamage / phaserStatTry : 0);
        warncount += 13;
        W_WriteText (warnw, 5, 5, textColor, newtext, warncount,
                     W_RegularFont);
    }
    else
#endif
        W_WriteText (warnw, 5, 5, textColor, text, warncount, W_RegularFont);
    warntimer = udcounter + WARNTIME;   /* set the line to be
                                         * cleared */


    if (logging || doPhaser)
    {
        time (&curtime);
        tm = localtime (&curtime);
#ifdef PHASER_STATS
        sprintf (newtext2, "%.100s %02d:%02d:%02d",
                 (doPhaser && phaserStats) ? newtext : text, tm->tm_hour,
#else
        sprintf (newtext2, "%.100s %02d:%02d:%02d", text, tm->tm_hour,
#endif
                 tm->tm_min, tm->tm_sec);
        warncount = (warncount > 100) ? 109 : warncount + 9;

        if (logging)
        {
            if (logFile != NULL)
            {
                fprintf (logFile, "%s\n", newtext2);
                fflush (logFile);
            }
            else
                puts (newtext2);
        }

        if (doPhaser)
            W_MessageAllowedWindows (WAM_PHASER, 0, 0, textColor, newtext2, warncount, 0);
    }
}

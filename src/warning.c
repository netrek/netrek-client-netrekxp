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
void
warning (char *text)
{
    int doPhaser, doRefit, doDeclare;
    time_t curtime;
    struct tm *tm;
    char newtext[128];
    char tmptext[128]; // addition to fix sprintf bug when compiling with bcc32

    if (warncount > 0)
    {
        /* XFIX */
        W_ClearArea (warnw, 5, 5, W_Textwidth * warncount, W_Textheight);
    }

    doPhaser = (strncmp (text, "Phaser burst", 12) == 0);
    doRefit = (strncmp (text, "You are being transported", 25) == 0);
    doDeclare = (strncmp (text, "Pausing ten seconds to re-program", 33) == 0);
    warncount = strlen (text);
    
    if (doRefit)
       rdelay = time (0) + REFITTIME;
    if (doDeclare)
       delay = time (0) + DECLARETIME;
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
        strcpy (tmptext, newtext);
        sprintf (newtext, "%.100s %02d:%02d:%02d",
                 (doPhaser && phaserStats) ? tmptext : text, tm->tm_hour,
#else
        sprintf (newtext, "%.100s %02d:%02d:%02d", text, tm->tm_hour,
#endif
                 tm->tm_min, tm->tm_sec);
        warncount = (warncount > 100) ? 109 : warncount + 9;

        if (logging)
        {
            if (logFile != NULL)
            {
                fprintf (logFile, "%s\n", newtext);
                fflush (logFile);
            }
            else
                puts (newtext);
        }

        if (doPhaser)
            W_MessageAllowedWindows (WAM_PHASER, 0, 0, textColor, newtext, warncount, 0);
    }
}

/******************************************************************************/
/***  File:  hintwin.c                                                      ***/
/***                                                                        ***/
/***  Function: set of hint window manipulation functions                   ***/
/***                                                                        ***/
/******************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* Some local variables */
int hintsInitialized = 0;

struct hints {
    char hint[10][250];
    int hintlines;
};

struct hints hints[250];
int curhint = 0;
int numhints = 0;

/******************************************************************************/
/***  fillhint()                                                            ***/
/******************************************************************************/
void
fillhint (void)
{
    if (!hintsInitialized)
        inithints ();

    if (hintWin == NULL)
    {
        hintWin = W_MakeTextWindow ("hints", 300, 30,
                                             67, 8,
                                             NULL, BORDER);
        W_SetWindowExposeHandler (hintWin, hintshow);
        W_SetWindowButtonHandler (hintWin, hintaction);
        W_DefineTrekCursor (hintWin);
    }

    W_UnmapWindow (hintWin);
}

/******************************************************************************/
/***  inithints()                                                           ***/
/******************************************************************************/
void
inithints (void)
{
    FILE *fp;
    char *exe_dir, hints_file[256], str[250], *str1;
    unsigned int i, j, k;

    exe_dir = GetExeDir ();
    if (exe_dir)
    {
        int len = strlen (exe_dir);
        if (exe_dir[len - 1] == '/' || exe_dir[len - 1] == '\\')
            sprintf (hints_file, "%s%s", exe_dir, "docs/hints.dat");
        else
            sprintf (hints_file, "%s/%s", exe_dir, "docs/hints.dat");
    }

    fp = fopen (hints_file, "r");
    if (fp == NULL)
        return;

    i = 0;
    while (fgets (str, 250, fp))
    {
        j = 0;
        hints[i].hintlines = 0;
        str1 = str;
        while ((unsigned int) (str1 - str) < strlen (str))
        {
            strncpy (hints[i].hint[j], str1, 62);
            k = strlen (hints[i].hint[j]) - 1;
            if (k < 61)
            {
                hints[i].hint[j][k] = '\0';
                hints[i].hintlines++;
                str1 += k + 1;
                j++;
            }
            else
            {
                if (hints[i].hint[j][k] == ' ')
                {
                    hints[i].hint[j][k] = '\0';
                    hints[i].hintlines++;
                    str1 += k + 1;
                    j++;
                }
                else
                {
                    while (hints[i].hint[j][k] != ' ' && k >= 0)
                        k--;
                    if (k > 0)
                    {
                        hints[i].hint[j][k] = '\0';
                        hints[i].hintlines++;
                        str1 += k + 1;
                        j++;
                    }
                    else
                    {
                        k = 61;
                        hints[i].hint[j][k] = '\0';
                        hints[i].hintlines++;
                        str1 += k + 1;
                        j++;
                    }
                }
            }
        }
        i++;
    }

    fclose (fp);

    hintsInitialized = 1;
    numhints = i - 1;

    srand ((unsigned) time (NULL));
    curhint = (int) (rand() % numhints);
}

/******************************************************************************/
/***  showhint()                                                            ***/
/******************************************************************************/
void
showhint (int hint)
{
    int i;

    W_ClearWindow (hintWin);

    W_WriteText (hintWin, 2, 1, W_Yellow, "Did you know that", 17, W_RegularFont);

    for (i = 0; i < hints[hint].hintlines; i++)
        W_WriteText (hintWin, 2, i + 3, textColor, hints[hint].hint[i], strlen (hints[hint].hint[i]), W_RegularFont);

    W_WriteText (hintWin, 2, 8, W_Yellow, "Prev - Right Click | Close - Middle Click | Next - Left Click",
                61, W_RegularFont);
}


/******************************************************************************/
/***  hintaction()                                                          ***/
/******************************************************************************/
void
hintaction (W_Event * data)
{
    switch (data->key)
    {
    case W_LBUTTON:
        curhint++;
        if (curhint > numhints)
            curhint = 0;
        showhint (curhint);
        return;
    case W_MBUTTON:
        showHints = 0;
        W_UnmapWindow (hintWin);
        if (optionWin)
            optionredrawoption (&showHints);
        return;
    case W_RBUTTON:
        curhint--;
        if (curhint < 0)
            curhint = numhints;
        showhint (curhint);
        return;
    }
}

/******************************************************************************/
/***  hintshow()                                                            ***/
/******************************************************************************/
void
hintshow (W_Event * data)
{
    showhint (curhint);
}
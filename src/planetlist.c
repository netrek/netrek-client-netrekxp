/******************************************************************************/
/***  File: planetlist.c
/***
/***  Function:
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/

#include <stdio.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

static char priorplanets[MAXPLANETS][BUFSIZ];

static char *teamname[9] = {
    "IND",
    "FED",
    "ROM",
    "",
    "KLI",
    "",
    "",
    "",
    "ORI"
};

int planet_row[MAXPLANETS];  /* planets location in current plist */

/* * Open a window which contains all the planets and their current *
 * statistics.  Players will not know about planets that their team * has not
 * orbited. */
void
planetlist (void)
{
    register int i;
    char buf[BUFSIZ];

    /* W_ClearWindow(planetw); */
    (void) sprintf (buf, "Planet Name      Own Armies REPAIR FUEL AGRI CORE Info");
    W_WriteText (planetw, 2, 1, textColor, buf, strlen (buf), W_RegularFont);
    /* Underline heading */
    W_MakeLine (planetw, 2, 2 + 2 * W_Textheight, 2 + 57 * W_Textwidth , 2 + 2 * W_Textheight, W_White);
    /* Initialize planet window string array */
    for (i = 0; i < MAXPLANETS; i++)
        strcpy(priorplanets[i], "");
    updatePlanetw ();
}

/* Update only lines that have changed */
void
updatePlanetw (void)
{
    register int i;
    char buf[BUFSIZ];
    register struct planet *j;

    for (i = 0, j = &planets[i]; i < MAXPLANETS; i++, j++)
    {
        /* Fill planet_row to get right planet placement in the list */
        planet_row[i] = j->pl_no;

        if (j->pl_info & me->p_team)
        {
            (void) sprintf (buf, "%-16s %3s %3d    %6s %4s %4s %4s %c%c%c%c",
                            j->pl_name,
                            teamname[j->pl_owner],
                            j->pl_armies,
                            (j->pl_flags & PLREPAIR ? "REPAIR" : "      "),
                            (j->pl_flags & PLFUEL ? "FUEL" : "    "),
                            (j->pl_flags & PLAGRI ? "AGRI" : "    "),
                            (j->pl_flags & PLCORE ? "CORE" : "    "),
                            (j->pl_info & FED ? 'F' : ' '),
                            (j->pl_info & ROM ? 'R' : ' '),
                            (j->pl_info & KLI ? 'K' : ' '),
                            (j->pl_info & ORI ? 'O' : ' '));
            if (strcmp(priorplanets[i], buf))
            {
                W_ClearArea (planetw, 2, i+2, 55, 1);
                W_WriteText (planetw, 2, i+2, planetColor (j), buf, strlen (buf),
                             planetFont (j));
                strcpy(priorplanets[i], buf);
            }
        }
        else
        {
            (void) sprintf (buf, "%-16s", j->pl_name);
            if (strcmp(priorplanets[i], buf))
            {
            	W_ClearArea (planetw, 2, i+2, 55, 1);
                W_WriteText (planetw, 2, i+2, unColor, buf, strlen (buf),
                             W_RegularFont);
                strcpy(priorplanets[i], buf);
            } 
        }
        if (i != 0 && (i % 10) == 0)
        {
              W_MakeLine (planetw,
                          2 + 18 * W_Textwidth,
                          2 + W_Textheight * (i+2),
                          2 + 39 * W_Textwidth,
                          2 + W_Textheight * (i+2),
                          W_White);
        }
    }
}

int
GetPlanetFromPlist (int x, int y)
{
    int i;
    int planet_no;

    /* Let's find what planet sits in poition y in the list */
    for (i = 0; i < MAXPLANETS; i++)
    {
        if (planet_row[i] == y)
        {
            planet_no = i;
            return planet_no;
        }
    }

    // We didn't find planet
    return (-1);
}
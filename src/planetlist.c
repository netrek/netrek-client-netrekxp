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

/* Local functions */
void updatePlanetw (void);

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

/* * Open a window which contains all the planets and their current *
 * statistics.  Players will not know about planets that their team * has not
 * orbited. */
void
planetlist (void)
{
    register int i;
    char buf[BUFSIZ];

    /* W_ClearWindow(planetw); */
    (void) sprintf (buf, "Planet Name      own armies REPAIR FUEL AGRI CORE info");
    W_WriteText (planetw, 2, 1, textColor, buf, strlen (buf), W_RegularFont);
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
    }
}
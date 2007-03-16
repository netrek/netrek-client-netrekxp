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
int planet_row[MAXPLANETS];  /* planets location in current plist */

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
    register int i, pos;
    char buf[BUFSIZ];
    register struct planet *j;
    int planetCount[NUMTEAM + 2]; /* Ind Fed Rom Kli Ori Unknown */
    int planetOffset[NUMTEAM + 2];
    int counter[NUMTEAM + 2];
    int playercount = 0;
    int largestteam = -1, nextlargestteam= -1;
    int largestteamcount = -1, nextlargestteamcount = -1;
    
    if (sortPlanets)
    {
        /* Must first know how many planets we have info on, that belong to
           our team, then the ones belonging to the other 3 teams, plus
           independent.  Order on planet list will be independent planets,
           then our team, then the next largest team (player-wise), and
           then the second-largest team (player-wise). */
        
        /* Init arrays.  Array position 0 will be always be independents,
           array position 1 will always be my team, and final array position
           will always be unknown planets */
        for (i = NUMTEAM + 1; i >= 0; --i)
        {
            planetCount[i] = 0;
            planetOffset[i] = 0;
            counter[i] = 0;
        }

        /* Find the 2 largest teams enemy teams.  Team bits suck. */
        for (i = 0; i < NUMTEAM; i++)
        {
            if (me->p_team == (1 << i))
                continue;

            playercount = realNumShips(1 << i);
            if (playercount > largestteamcount)
            {
                nextlargestteam = largestteam;
                nextlargestteamcount = largestteamcount;
                largestteam = (1 << i);
                largestteamcount = playercount;
            }
            else if (playercount > nextlargestteamcount)
            {
                nextlargestteam = (1 << i);
                nextlargestteamcount = playercount;
            }
        }

        /* Store # of visible planets from each team */
        for (i = 0, j = &planets[i]; i < MAXPLANETS; i++, j++)
        {
            if (j->pl_info & me->p_team)
            {
                if (j->pl_owner == 0) /* Independent */
                    ++planetCount[0];
                else if (j->pl_owner == me->p_team) /* My team */
                    ++planetCount[1];
                else if (j->pl_owner == largestteam) /* Largest enemy */
                    ++planetCount[2];
                else if (j->pl_owner == nextlargestteam) /* Next largest enemy */
                    ++planetCount[3];
                else /* Smallest enemy */
                    ++planetCount[4];
            }
            else
                ++planetCount[5];
        }
        /* Set the offsets */
        planetOffset[0] = 0;
        planetOffset[1] = planetOffset[0] + planetCount[0];
        planetOffset[2] = planetOffset[1] + planetCount[1];
        planetOffset[3] = planetOffset[2] + planetCount[2];
        planetOffset[4] = planetOffset[3] + planetCount[3];
        planetOffset[5] = planetOffset[4] + planetCount[4];
    }
#if 0 /* Debug */
    LineToConsole("My team %d, Largest enemy %d, next %d\n",
    remap[me->p_team], largestteam, nextlargestteam);
    LineToConsole("Pl counts are %d %d %d %d %d\n",
    planetCount[1],planetCount[2],planetCount[3],planetCount[4],planetCount[5]);
    LineToConsole("Offsets are %d %d %d %d %d\n",
    planetOffset[1],planetOffset[2],planetOffset[3],planetOffset[4],planetOffset[5]);
#endif
    for (i = 0, j = &planets[i]; i < MAXPLANETS; i++, j++)
    {
        if (sortPlanets)
        {
            if (j->pl_info & me->p_team)
            {
                if (j->pl_owner == 0) /* Independent */
                {
                    pos = planetOffset[0] + counter[0];
                    counter[0]++;
                }
                else if (j->pl_owner == me->p_team) /* My team */
                {
                    pos = planetOffset[1] + counter[1];
                    counter[1]++;
                }
                else if (j->pl_owner == largestteam) /* Largest enemy */
                {
                    pos = planetOffset[2] + counter[2];
                    counter[2]++;
                }
                else if (j->pl_owner == nextlargestteam) /* Next largest enemy */
                {
                    pos = planetOffset[3] + counter[3];
                    counter[3]++;
                }
                else /* Smallest enemy */
                {
                    pos = planetOffset[4] + counter[4];
                    counter[4]++;
                }
            }
            else
            {
                pos = planetOffset[5] + counter[5];
                counter[5]++;
            }
        }
        else
            pos = i;

        /* Fill planet_row to get right planet placement in the list */
        planet_row[pos] = j->pl_no;

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
        }
        else
            (void) sprintf (buf, "%-16s", j->pl_name);

        if (strcmp(priorplanets[pos], buf))
        {
            W_ClearArea (planetw, 2, pos+2, 55, 1);
            if (j->pl_info & me->p_team)
                W_WriteText (planetw, 2, pos+2, planetColor (j), buf, strlen (buf),
                             planetFont (j));
            else
                W_WriteText (planetw, 2, pos+2, unColor, buf, strlen (buf),
                             W_RegularFont);
            strcpy(priorplanets[pos], buf);
            /* Do we need to redraw a team separator line? */
            if (!sortPlanets)
            {
                /* Static line positions - only redraw them if the relevant
                   line changed */
                if (pos == 9 || pos == 19 || pos == 29)
                {
                    W_MakeLine (planetw,
                                2 + 18 * W_Textwidth,
                                2 + W_Textheight * (pos+3),
                                2 + 39 * W_Textwidth,
                                2 + W_Textheight * (pos+3),
                                W_White);
                }
            }
            else
            {
                /* Dynamic team separators, sorted map */
                if ((pos == planetOffset[1] - 1)
                    || (pos == (planetOffset[2] - 1))
                    || (pos == (planetOffset[3] - 1))
                    || (pos == (planetOffset[4] - 1))
                    || (pos == (planetOffset[5] - 1)))
                {
                    /* Erase any line above it first, if it was 
                       part of the same team */
                    if (pos != 0 && (planets[planet_row[pos]].pl_owner == 
                        planets[planet_row[pos-1]].pl_owner))
                    {
                        W_MakeLine (planetw,
                                    2 + 18 * W_Textwidth,
                                    2 + W_Textheight * (pos+2),
                                    2 + 39 * W_Textwidth,
                                    2 + W_Textheight * (pos+2),
                                    W_Black);
                    }
                    W_MakeLine (planetw,
                                2 + 18 * W_Textwidth,
                                2 + W_Textheight * (pos+3),
                                2 + 39 * W_Textwidth,
                                2 + W_Textheight * (pos+3),
                                W_White);
                }
            }
        }
    }
}

int
GetPlanetFromPlist (int x, int y)
{
    if (y < MAXPLANETS && y >= 0)
        return planet_row[y];
    else
        return (-1);
}
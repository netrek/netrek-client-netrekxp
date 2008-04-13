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

/* Prototypes */
#ifdef PARADISE
static void planet_list_paradise (void);
static void print_planet (W_Window wind, int line, struct planet * );
#endif
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

void initialize_planets(void)
{
    int     i;

    planets = (struct planet *)malloc(sizeof(*planets) * MAXPLANETS);

    for(i = 0; i < MAXPLANETS; i++) {
	struct planet *curr = &planets[i];
	curr->pl_no = i;
	curr->pl_flags = 0;
	curr->pl_owner = 0;
	curr->pl_x = curr->pl_y = -10000;
	sprintf(curr->pl_name, "planet%d", i);
	curr->pl_namelen = strlen(curr->pl_name);
	curr->pl_armies = 0;
	curr->pl_info = 0;
	curr->pl_deadtime = 0;
	curr->pl_couptime = 0;
#ifdef PARADISE
	curr->pl_timestamp = 0;
#endif

	/* initialize planet redraw for moving planets */
	pl_update[i].plu_update = -1;
    }
}

/* * Open a window which contains all the planets and their current *
 * statistics.  Players will not know about planets that their team * has not
 * orbited. */
void
planetlist (void)
{
#ifdef PARADISE
    planet_list_paradise();
#else
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
#endif
}

#ifdef PARADISE
int mask_to_idx(int m)
{
    switch(m) {
      case NOBODY:
        return -1;
      case FED:
        return 0;
      case ROM:
        return 1;
      case KLI:
        return 2;
      case ORI:
        return 3;
      default:
        return number_of_teams;
    }
}

/*This function provides the planet list for a paradise server version 2.0 */
static void planet_list_paradise (void)
{
    typedef struct planet *plptr;

    int     k = 0;		/* for row number */
    int     i, team_pnum;	/* looping var */
    plptr   j;			/* to point to a planet */
    char    buf[100];		/* to sprintf into */
    W_Window wind;
    extern int number_of_teams;
    plptr **team_p;
    int    *team_pcount;

    wind = planetw;

    /* this malloc stuff will handle any number of teams/races */

    /* team's planet counters */
    team_pcount = (int *) malloc((number_of_teams + 2) * sizeof(int));
    for (i = 0; i < number_of_teams + 2; i++)
	team_pcount[i] = 0;

    if (sortPlanets) {
	/* make some memory */
	team_p = (plptr **) malloc((number_of_teams + 2) * sizeof(struct planet *));
	for (i = 0; i < (number_of_teams + 2); i++)
	    team_p[i] = (plptr *) malloc(nplanets * sizeof(struct planet *));

	/* loop thru and put proper team planeter point on each planet */
	for (i = 0, j = &planets[i]; i < nplanets; i++, j++) {
	    k = mask_to_idx(j->pl_owner) + 1;	/* which team gets planet */
	    team_p[k][team_pcount[k]] = j;
	    team_pcount[k]++;
	}

	/* go thru each teams planet list and display */
	for (i = 0, k = 0; i < (number_of_teams + 2); i++) {
	    for (team_pnum = 0; team_pnum < team_pcount[i]; team_pnum++, k++) {

		j = team_p[i][team_pnum];
		if (k == 0) {
		    sprintf(buf, "Planet name      sctr own armies RESOURCES  SURFC  ATMOS    VISIT    TIME");
		    W_WriteText(wind, 2, 1, textColor, buf, strlen(buf),
		    		W_RegularFont);
		    k = 2;
		}
		print_planet(wind, k, j);
	    }			/* end of 2nd for */
	    if (team_pcount[i] > 0)
		k++;
	}			/* end of 1st for */

	for (i = 0; i < (number_of_teams + 2); i++)
	    free(team_p[i]);
	free(team_p);
    } else {			/* do the original alpa only sort planet list */

	for (i = 0, j = &planets[i]; i < nplanets; i++, j++, k++) {
	    if (i == 0) {

		sprintf(buf, "Planet name      sctr own armies RESOURCES  SURFC  ATMOS    VISIT    TIME");
		W_WriteText(wind, 2, 1, textColor, buf, strlen(buf), 
		  	    W_RegularFont);
		k = 2;
	    }
	    team_pcount[mask_to_idx(j->pl_owner) + 1]++;
	    print_planet(wind, k, j);
	}
    }

    k++;
    for (i = 0; i < (number_of_teams + 1); i++) {
	W_Color cur_color;

	cur_color = shipCol[i];
	sprintf(buf, "%s: ", teaminfo[i-1].shortname);
	W_WriteText(wind, i * 7 + 2, k, cur_color, buf, strlen(buf), 
		    W_RegularFont);
	sprintf(buf, " %-2d", team_pcount[i]);
	W_WriteText(wind, i * 7 + 2, k + 1, cur_color, buf, strlen(buf), 
	            W_RegularFont);
    }

    free(team_pcount);

}				/* end of planet_list_paradise */

/****************************** print_planet() ************************/
static void print_planet (W_Window wind, int line, struct planet *j)
{
    char    buf[100];		/* to sprintf into */

    sprintf(buf, "%-16s %d-%d", j->pl_name, (j->pl_x / GRIDSIZE) + 1,
	    (j->pl_y / GRIDSIZE) + 1);
    W_WriteText(wind, 2, line, textColor, buf, strlen(buf),
		W_RegularFont);

    if (j->pl_info & me->p_team) {
	if (PL_TYPE(*j) == PLSTAR) {	/* if planet actually a star */
	    W_WriteText(wind, 24, line, textColor, "---S T A R---", 13,
		       W_RegularFont);
	} else if (PL_TYPE(*j) == PLWHOLE) { /* if wormhole... */
	   W_WriteText(wind, 24, line, textColor, "---W O R M H O L E---", 21,
			W_RegularFont);
	} else {		/* else planet not a star */
	    char   *s = NULL;

	    switch (j->pl_flags & PLATMASK) {
	    case PLPOISON:
		s = "TOXC";
		break;
	    case PLATYPE3:
		s = "TNTD";
		break;
	    case PLATYPE2:
		s = "THIN";
		break;
	    case PLATYPE1:
		s = "STND";
		break;
	    };
	    sprintf(buf, "%3s %3d       %c%c%c%c     %c%c%c    %4s",
	            teamname[j->pl_owner],
		    //teaminfo[mask_to_idx(j->pl_owner)].shortname,
		    j->pl_armies,
		    (j->pl_flags & PLREPAIR ? 'R' : ' '),
		    (j->pl_flags & PLFUEL ? 'F' : ' '),
		    (j->pl_flags & PLAGRI ? 'A' : ' '),
		    (j->pl_flags & PLSHIPYARD ? 'S' : ' '),
		    (j->pl_flags & PLDILYTH ? 'D' : ' '),
		    (j->pl_flags & PLMETAL ? 'M' : ' '),
		    (j->pl_flags & PLARABLE ? 'A' : ' '),
		    s);

	    W_WriteText(wind, 24, line, planetColor(j), buf, strlen(buf),
			planetFont(j));

	    sprintf(buf, "%c%c%c%c   %3ld", /* Remove team_bit_string reference */
                    (j->pl_info & FED ? 'F' : ' '),
                    (j->pl_info & ROM ? 'R' : ' '),
                    (j->pl_info & KLI ? 'K' : ' '),
                    (j->pl_info & ORI ? 'O' : ' '),
		    (me->p_team == j->pl_owner ? 0 : (status2->clock - j->pl_timestamp)));

	    W_WriteText(wind, 64, line, planetColor(j), buf, strlen(buf),
			planetFont(j));
	}
    } else {
	sprintf(buf, "--- No info; Scout me ---");
	W_WriteText(wind, 24, line, textColor, buf, strlen(buf),
		    W_RegularFont);
    }
}				/* end of print_planet */
#endif

/* Update only lines that have changed */
void
updatePlanetw (void)
{
    register int i, k, pos;
    char buf[BUFSIZ];
    register struct planet *j;
    int planetCount[NUMTEAM + 2]; /* Ind Fed Rom Kli Ori Unknown */
    int planetOffset[NUMTEAM + 2];
    int counter[NUMTEAM + 2];
    int planetPos[NUMTEAM + 2][MAXPLANETS];
    int planetArm[NUMTEAM + 2][MAXPLANETS];
    int playercount = 0;
    int largestteam = -1, nextlargestteam= -1;
    int largestteamcount = -1, nextlargestteamcount = -1;
    int temp, temparm, z;
    
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
            for (k = 0; k < MAXPLANETS; k++)
            {
                planetArm[i][k] = 0;
                planetPos[i][k] = 0;
            }
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

        /* Store # of visible planets from each team.  And let's sort them
           by army count (low to high) while we are at it. */
        for (i = 0, j = &planets[i]; i < nplanets; i++, j++)
        {
            if (j->pl_info & me->p_team)
            {
                if (j->pl_owner == 0) /* Independent (no sorting)*/
                {
                    planetPos[0][planetCount[0]] = planetCount[0];
                    planetCount[0]++;
                }
                else if (j->pl_owner == me->p_team) /* My team */
                {
                    planetPos[1][planetCount[1]] = planetCount[1];
                    planetArm[1][planetCount[1]] = j->pl_armies;
                    k = planetCount[1];
                    while (k > 0 && planetArm[1][k] < planetArm[1][k - 1])
                    {
                            temp = planetPos[1][k];
                            temparm = planetArm[1][k];
                            planetPos[1][k] = planetPos[1][k - 1];
                            planetArm[1][k] = planetArm[1][k - 1];
                            planetPos[1][k - 1] = temp;
                            planetArm[1][k - 1] = temparm;
                            k--;
                    }
                    planetCount[1]++;
                }
                else if (j->pl_owner == largestteam) /* Largest enemy */
                {
                    planetPos[2][planetCount[2]] = planetCount[2];
                    planetArm[2][planetCount[2]] = j->pl_armies;
                    k = planetCount[2];
                    while (k > 0 && planetArm[2][k] < planetArm[2][k - 1])
                    {
                            temp = planetPos[2][k];
                            temparm = planetArm[2][k];
                            planetPos[2][k] = planetPos[2][k - 1];
                            planetArm[2][k] = planetArm[2][k - 1];
                            planetPos[2][k - 1] = temp;
                            planetArm[2][k - 1] = temparm;
                            k--;
                    }
                    planetCount[2]++;
                }
                else if (j->pl_owner == nextlargestteam) /* Next largest enemy */
                {
                    planetPos[3][planetCount[3]] = planetCount[3];
                    planetArm[3][planetCount[3]] = j->pl_armies;
                    k = planetCount[3];
                    while (k > 0 && planetArm[3][k] < planetArm[3][k - 1])
                    {
                            temp = planetPos[3][k];
                            temparm = planetArm[3][k];
                            planetPos[3][k] = planetPos[3][k - 1];
                            planetArm[3][k] = planetArm[3][k - 1];
                            planetPos[3][k - 1] = temp;
                            planetArm[3][k - 1] = temparm;
                            k--;
                    }
                    planetCount[3]++;
                }
                else /* Smallest enemy */
                {
                    planetPos[4][planetCount[4]] = planetCount[4];
                    planetArm[4][planetCount[4]] = j->pl_armies;
                    k = planetCount[4];
                    while (k > 0 && planetArm[4][k] < planetArm[4][k - 1])
                    {
                            temp = planetPos[4][k];
                            temparm = planetArm[4][k];
                            planetPos[4][k] = planetPos[4][k - 1];
                            planetArm[4][k] = planetArm[4][k - 1];
                            planetPos[4][k - 1] = temp;
                            planetArm[4][k - 1] = temparm;
                            k--;
                    }
                    planetCount[4]++;
                }
            }
            else
            {
                planetPos[5][planetCount[5]] = planetCount[5];
                planetCount[5]++;
            }
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

    for (i = 0, j = &planets[i]; i < nplanets; i++, j++)
    {
        if (sortPlanets)
        {
            if (j->pl_info & me->p_team)
            {
                if (j->pl_owner == 0) /* Independent */
                {
                    pos = planetOffset[0] + planetPos[0][counter[0]];
                    counter[0]++;
                }
                else if (j->pl_owner == me->p_team) /* My team */
                {
                    for (z = 0; z < planetCount[1]; z++)
                    {
                        if (planetPos[1][z] == counter[1])
                            break;
                    }
                    pos = z + planetOffset[1];
                    counter[1]++;
                }
                else if (j->pl_owner == largestteam) /* Largest enemy */
                {
                    for (z = 0; z < planetCount[2]; z++)
                    {
                        if (planetPos[2][z] == counter[2])
                            break;
                    }
                    pos = z + planetOffset[2];
                    counter[2]++;
                }
                else if (j->pl_owner == nextlargestteam) /* Next largest enemy */
                {
                    for (z = 0; z < planetCount[3]; z++)
                    {
                        if (planetPos[3][z] == counter[3])
                            break;
                    }
                    pos = z + planetOffset[3];
                    counter[3]++;
                }
                else /* Smallest enemy */
                {
                    for (z = 0; z < planetCount[4]; z++)
                    {
                        if (planetPos[4][z] == counter[4])
                            break;
                    }
                    pos = z + planetOffset[4];
                    counter[4]++;
                }
            }
            else
            {
                pos = planetOffset[5] + planetPos[5][counter[5]];
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
    if (y < nplanets && y >= 0)
        return planet_row[y];
    else
        return (-1);
}
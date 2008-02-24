/******************************************************************************/
/***  File:  inform.c                                                       ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <signal.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* Display information about the nearest objext to mouse */

/* * When the player asks for info, this routine finds the object * nearest
 * the mouse, either player or planet, and pop up a window * with the desired
 * information in it. *
 * 
 * We intentionally provide less information than is actually * available. Keeps
 * the fog of war up. *
 * 
 * There is a different sized window for each type player/planet * and we take
 * care to keep it from extending beyond the main * window boundaries. */

#ifdef PARADISE
static char *my_classes[NUM_TYPES] = {
    "SC", "DD", "CA", "BB", "AS", "SB", "AT", "JS", "FL", "WB", "CL", "CV", "UT", "PT", "PU",
};
#else
static char *my_classes[NUM_TYPES] = {
    "SC", "DD", "CA", "BB", "AS", "SB", "GA", "AT"
};
#endif

#ifdef PARADISE
static void Info_list_paradise (struct player * j);
#endif

/******************************************************************************/
/***  inform()                                                              ***/
/******************************************************************************/
void
inform (W_Window ww,
        int x,
        int y,
        char key)
{
    char buf[BUFSIZ];
    int line = 0;
    register struct player *j;
    register struct planet *k;
    int mx, my;
    struct obtype *gettarget (W_Window ww,
                              int x,
                              int y,
                              int targtype),
     *target;
    int windowWidth, windowHeight;

    mx = x;
    my = y;
    infomapped = 1;
    if (key == 'i')
    {
        target = gettarget (ww, x, y, TARG_PLAYER | TARG_PLANET);
    }
    else
    {
        target = gettarget (ww, x, y, TARG_PLAYER | TARG_SELF);
    }

	// Here we got into situation where player number is -1
	if (target->o_num == -1)
	{
		infomapped = 0;
		return;
	}

    /* This is pretty lame.  We make a graphics window for the info window so
     * we can accurately space the thing to barely fit into the galactic map or
     * whatever. */

    windowWidth = W_WindowWidth (ww);
    windowHeight = W_WindowHeight (ww);
    if (target->o_type == PLAYERTYPE)
    {
        if (key == 'i')
        {
#ifdef PARADISE
            double dist;
            /* Too close to the edge? */
            if (mx + 23 * W_Textwidth + 2 > windowWidth)
                mx = windowWidth - 23 * W_Textwidth - 2;
            if (my + 6 * W_Textheight + 2 > windowHeight)
                my = windowHeight - 6 * W_Textheight - 2;

            infow =
                W_MakeWindow ("info", mx, my, 23 * W_Textwidth,
                              7 * W_Textheight, ww, 2, foreColor);
            W_MapWindow (infow);
            j = &players[target->o_num];
            (void) sprintf (buf, "%s (%c%c)", j->p_name, teamlet[j->p_team],
                            shipnos[j->p_no]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), shipFont (j));
            (void) sprintf (buf, "Speed:   %-d", j->p_speed);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);

            (void) sprintf (buf, "kills:   %-4.2f", j->p_kills);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            dist = hypot((double) (me->p_x - j->p_x),
                         (double) (me->p_y - j->p_y)) / (double) GRIDSIZE;
            (void) sprintf(buf, "dist:    %-1.2f sectors", dist);
             W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            (void) sprintf (buf, "Ship Type: %-s",
                            my_classes[j->p_ship.s_type]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);

            if (j->p_swar & me->p_team)
                W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                             playerColor (j), "WAR", 3, W_RegularFont);
            else if (j->p_hostile & me->p_team)
                W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                             playerColor (j), "HOSTILE", 7, W_RegularFont);
            else
                W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                             playerColor (j), "PEACEFUL", 8, W_RegularFont);
            (void) sprintf (buf, "%s@%s", j->p_login, j->p_monitor);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
#else
            /* Too close to the edge? */
            if (mx + 23 * W_Textwidth + 2 > windowWidth)
                mx = windowWidth - 23 * W_Textwidth - 2;
            if (my + 5 * W_Textheight + 2 > windowHeight)
                my = windowHeight - 5 * W_Textheight - 2;

            infow =
                W_MakeWindow ("info", mx, my, 23 * W_Textwidth,
                              6 * W_Textheight, ww, 2, foreColor);
            W_MapWindow (infow);
            j = &players[target->o_num];
            (void) sprintf (buf, "%s (%c%c)", j->p_name, teamlet[j->p_team],
                            shipnos[j->p_no]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), shipFont (j));
            (void) sprintf (buf, "Speed:   %-d", j->p_speed);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);

            (void) sprintf (buf, "kills:   %-4.2f", j->p_kills);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            (void) sprintf (buf, "Ship Type: %-s",
                            my_classes[j->p_ship.s_type]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);

            if (j->p_swar & me->p_team)
                W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                             playerColor (j), "WAR", 3, W_RegularFont);
            else if (j->p_hostile & me->p_team)
                W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                             playerColor (j), "HOSTILE", 7, W_RegularFont);
            else
                W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                             playerColor (j), "PEACEFUL", 8, W_RegularFont);
            (void) sprintf (buf, "%s@%s", j->p_login, j->p_monitor);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
#endif
        }
        else
        {                       /* New information window! */
#ifdef PARADISE
            if (mx + 50 * W_Textwidth + 2 > windowWidth)
                mx = windowWidth - 50 * W_Textwidth - 2;
            if (my + 25 * W_Textheight + 2 > windowHeight)
                my = windowHeight - 22 * W_Textheight - 2;

            infow =
                W_MakeWindow ("info", mx, my, 50 * W_Textwidth,
                              25 * W_Textheight, ww, 2, foreColor);
            W_MapWindow (infow);
            j = &players[target->o_num];
            (void) sprintf (buf, "%s (%c%c):", j->p_name, teamlet[j->p_team],
                            shipnos[j->p_no]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), shipFont (j));
            Info_list_paradise(j);
#else
            float KillsPerHour, LossesPerHour;  /* SB info window changed to
                                                 * use these instead of
                                                 * Offense and Defense. */
            if (mx + 24 * W_Textwidth + 2 > windowWidth)
                mx = windowWidth - 24 * W_Textwidth - 2;
            if (my + 10 * W_Textheight + 2 > windowHeight)
                my = windowHeight - 10 * W_Textheight - 2;

            infow =
                W_MakeWindow ("info", mx, my, 24 * W_Textwidth,
                              10 * W_Textheight, ww, 2, foreColor);
            W_MapWindow (infow);
            j = &players[target->o_num];
            (void) sprintf (buf, "%s (%c%c):", j->p_name, teamlet[j->p_team],
                            shipnos[j->p_no]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), shipFont (j));
            (void) sprintf (buf, "Login   %-s", j->p_login);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            (void) sprintf (buf, "Display %-s", j->p_monitor);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            STRNCPY (buf, "        Rating    Total", 25);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            sprintf (buf, "Bombing: %5.2f  %5d", bombingRating (j),
                     j->p_stats.st_armsbomb + j->p_stats.st_tarmsbomb);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            sprintf (buf, "Planets: %5.2f  %5d", planetRating (j),
                     j->p_stats.st_planets + j->p_stats.st_tplanets);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            if ((j->p_ship.s_type == STARBASE) && (SBhours))
            {
                KillsPerHour = (float) ((float) (j->p_stats.st_sbticks == 0) ?
                    0.0 :
                    (float) j->p_stats.st_sbkills * 36000.0 /
                    (float) j->p_stats.st_sbticks);
                sprintf (buf, "KPH:     %5.2f  %5d",
                         KillsPerHour, j->p_stats.st_sbkills);
            }
            else
            {
                sprintf (buf, "Offense: %5.2f  %5d",
                         offenseRating (j),
                         j->p_stats.st_kills + j->p_stats.st_tkills);
            }
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            if ((j->p_ship.s_type == STARBASE) && (SBhours))
            {
                LossesPerHour = (float) ((float) (j->p_stats.st_sbticks == 0) ?
                    0.0 :
                    (float) j->p_stats.st_sblosses * 36000.0 /
                    (float) j->p_stats.st_sbticks);
                sprintf (buf, "DPH:     %5.2f  %5d",
                         LossesPerHour, j->p_stats.st_sblosses);
            }
            else
            {
                sprintf (buf, "Defense: %5.2f  %5d",
                         defenseRating (j),
                         j->p_stats.st_losses + j->p_stats.st_tlosses);
            }
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            if (j->p_ship.s_type == STARBASE)
            {
                sprintf (buf, "  Maxkills: %6.2f", j->p_stats.st_sbmaxkills);
            }
            else
            {
                sprintf (buf, "  Maxkills: %6.2f", j->p_stats.st_maxkills);
            }
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
            if ((j->p_ship.s_type == STARBASE) && (SBhours))
            {
                sprintf (buf, "  Hours:    %6.2f",
                         (float) j->p_stats.st_sbticks / 36000.0);
            }
            else
            {
                sprintf (buf, "  Hours:    %6.2f",
                         (float) j->p_stats.st_tticks / 36000.0);
            }
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         playerColor (j), buf, strlen (buf), W_RegularFont);
#endif
        }
    }
    else
    {                           /* Planet */
#ifdef PARADISE
        /* Too close to the edge? */
        if (mx + 23 * W_Textwidth + 2 > windowWidth)
            mx = windowWidth - 28 * W_Textwidth - 2;
        if (my + 5 * W_Textheight + 2 > windowHeight)
            my = windowHeight - 5 * W_Textheight - 2;

        infow =
            W_MakeWindow ("info", mx, my, W_Textwidth * 28, W_Textheight * 5,
                          ww, 2, foreColor);
        W_MapWindow (infow);
        k = &planets[target->o_num];
        if ((k->pl_info & me->p_team)
#ifdef RECORDGAME
            || playback
#endif
            )
        {
            if (k->pl_flags & PLSTAR) /* test if planet is a star */
            {
                (void) sprintf(buf, "%s", k->pl_name);
                W_WriteText(infow, W_Textwidth, W_Textheight * line++, textColor,
                            buf, strlen(buf), W_RegularFont);
                (void) sprintf(buf, "STAR  ");
                W_WriteText(infow, W_Textwidth, W_Textheight * line++,
                            textColor, buf, strlen(buf), W_RegularFont);
            }
            else /* not a star */
            {
                (void) sprintf(buf, "%s (%c)", k->pl_name, teamlet[k->pl_owner]);
                W_WriteText(infow, W_Textwidth, W_Textheight * line++, planetColor (k),
                            buf, strlen(buf), planetFont (k));
                (void) sprintf(buf, "Armies %d", k->pl_armies);
                W_WriteText(infow, W_Textwidth, W_Textheight * line++, planetColor (k),
                            buf, strlen(buf), W_RegularFont);
                (void) sprintf(buf, "%s %s %s %s",
                              (k->pl_flags & PLREPAIR ? "RPR" : "      "),
                              (k->pl_flags & PLFUEL ? "FUEL" : "    "),
                              (k->pl_flags & PLAGRI ? "AGRI" : "    "),
                              (k->pl_flags & PLSHIPYARD ? "SHPYD" : "     "));
                W_WriteText(infow, W_Textwidth, W_Textheight * line++, planetColor (k),
                            buf, strlen(buf), W_RegularFont);
                (void) sprintf(buf, "atmos: ");
                switch (k->pl_flags & PLATMASK) {
                    case PLPOISON:
                        strcat(buf, "TOXC  surfc: ");
                        break;
                    case PLATYPE3:
                        strcat(buf, "TNTD  surfc: ");
                        break;
                    case PLATYPE2:
                        strcat(buf, "THIN  surfc: ");
                        break;
                    case PLATYPE1:
                        strcat(buf, "STND  surfc: ");
                        break;
                    default:
                        strcat(buf, "      surfc: ");
                        break;
                };
                if (k->pl_flags & PLDILYTH)
                    strcat(buf, "D");
                else
                    strcat(buf, " ");
                if (k->pl_flags & PLMETAL)
                    strcat(buf, "M");
                else
                    strcat(buf, " ");
                if (k->pl_flags & PLARABLE)
                    strcat(buf, "A");
                else
                    strcat(buf, " ");
                W_WriteText(infow, W_Textwidth, W_Textheight * line++, planetColor (k),
                            buf, strlen(buf), W_RegularFont);
                sprintf(buf, "Time: %-5d Visit: %c%c%c%c",
                       ((me->p_team == k->pl_owner) ? 0 : (int) (status2->clock - k->pl_timestamp)),
                       (k->pl_info & FED ? 'F' : ' '),
                       (k->pl_info & ROM ? 'R' : ' '),
                       (k->pl_info & KLI ? 'K' : ' '),
                       (k->pl_info & ORI ? 'O' : ' '));
                W_WriteText(infow, W_Textwidth, W_Textheight * line++, planetColor (k),
                            buf, strlen(buf), W_RegularFont);
            }
        }
        else
        {
            (void) sprintf (buf, "%s", k->pl_name);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), W_RegularFont);
            (void) sprintf (buf, "No other info");
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), W_RegularFont);
        }
#else
        /* Too close to the edge? */
        if (mx + 23 * W_Textwidth + 2 > windowWidth)
            mx = windowWidth - 28 * W_Textwidth - 2;
        if (my + 3 * W_Textheight + 2 > windowHeight)
            my = windowHeight - 3 * W_Textheight - 2;

        infow =
            W_MakeWindow ("info", mx, my, W_Textwidth * 28, W_Textheight * 3,
                          ww, 2, foreColor);
        W_MapWindow (infow);
        k = &planets[target->o_num];
        if ((k->pl_info & me->p_team)
#ifdef RECORDGAME
            || playback
#endif
            )
        {
            (void) sprintf (buf, "%s (%c)", k->pl_name, teamlet[k->pl_owner]);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), planetFont (k));
            (void) sprintf (buf, "Armies %d", k->pl_armies);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), W_RegularFont);
            (void) sprintf (buf, "%s %s %s %s %c%c%c%c",
                            (k->pl_flags & PLREPAIR ? "REPAIR" : "      "),
                            (k->pl_flags & PLFUEL ? "FUEL" : "    "),
                            (k->pl_flags & PLAGRI ? "AGRI" : "    "),
                            (k->pl_flags & PLCORE ? "CORE" : "    "),
                            (k->pl_info & FED ? 'F' : ' '),
                            (k->pl_info & ROM ? 'R' : ' '),
                            (k->pl_info & KLI ? 'K' : ' '),
                            (k->pl_info & ORI ? 'O' : ' '));
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), W_RegularFont);
        }
        else
        {
            (void) sprintf (buf, "%s", k->pl_name);
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), W_RegularFont);
            (void) sprintf (buf, "No other info");
            W_WriteText (infow, W_Textwidth, W_Textheight * line++,
                         planetColor (k), buf, strlen (buf), W_RegularFont);
        }
#endif
    }
}

#ifdef PARADISE
/*
   fills the ratings struct pointed to by r with the stats for the player
   pointed to by j [BDyess]
*/
struct ratings *
get_ratings(struct player *j, struct ratings *r)
{
	struct stats2 *s;	/* point to player's paradise stats */
	float   t, t2;		/* temps */

	s = &(j->p_stats2);
	/* fill in kills, losses, and maxkills based on ship type */
	if (j->p_ship.s_type == STARBASE) {
	    r->r_kills = s->st_sbkills;
	    r->r_losses = s->st_sblosses;
	    r->r_maxkills = s->st_sbmaxkills;
	} else if (j->p_ship.s_type == WARBASE) {
	    r->r_kills = s->st_wbkills;
	    r->r_losses = s->st_wblosses;
	    r->r_maxkills = s->st_wbmaxkills;
	} else {
	    r->r_kills = s->st_tkills;
	    r->r_losses = s->st_tlosses;
	    r->r_maxkills = s->st_tmaxkills;
	}
	/* calculate ratio */
	r->r_ratio = (r->r_losses != 0) ? r->r_kills / (float) r->r_losses
	    : r->r_kills;
	/*
	   r->r_ratio = (s->st_tlosses != 0) ? (float) s->st_tkills / (float)
	   s->st_tlosses : s->st_tkills;
	*/
	status2->timeprod = status2->timeprod ? status2->timeprod : 1;
	t = (float) s->st_tticks / (float) status2->timeprod;	/* hour ratio */
	if (t == 0.0)
	    t = 1.0;
	t2 = t * (float) status2->losses;	/* get expected losses */
	if(t2 == 0) t2=1;
	r->r_defrat = s->st_tlosses / t2;	/* calc defense rating */

	t2 = t * (float) status2->kills;	/* get expected kills */
	if(t2 == 0) t2=1;
	r->r_offrat = s->st_tkills / t2;	/* calc offense rating */

	t2 = t * (float) status2->armsbomb;	/* expected armies bombed */
	if(t2 == 0) t2=1;
	r->r_bombrat = (float) s->st_tarmsbomb / t2;	/* bomb rating */

	t2 = t * (float) status2->resbomb;	/* expected resources bmbd */
	if(t2 == 0) t2=1;
	r->r_resrat = (float) s->st_tresbomb / t2;	/* resrce bmbd rating */

	t2 = t * (float) status2->dooshes;	/* expected armies dooshed */
	if(t2 == 0) t2=1;
	r->r_dooshrat = (float) s->st_tdooshes / t2;	/* doosh rating */

	r->r_batrat = r->r_dooshrat + r->r_offrat;	/* get battle rating */

	t2 = t * (float) status2->planets;	/* expected planets */
	if(t2 == 0) t2=1;
	r->r_planetrat = (float) s->st_tplanets / t2;	/* get planet rating */
	/* strategy rating */
	r->r_stratrat = r->r_bombrat + r->r_resrat + r->r_planetrat;
	/* calculate sb rating */
	t2 = (float) status2->sbkills / (float)( (status2->sblosses > 0) ? status2->sblosses : 1);
	if (s->st_sblosses == 0)
	    r->r_sbrat = 0.0;
	else if (t2 == 0)
	    r->r_sbrat = 99.0;
	else
	    r->r_sbrat = ((float) s->st_sbkills / (float) s->st_sblosses) / t2;
	/* calculate wb rating */
	t2 = (float) status2->wbkills / (float)( (status2->wblosses > 0) ? status2->wblosses : 1);
	if (s->st_wblosses == 0)
	    r->r_wbrat = 0.0;
	else if (t2 == 0)
	    r->r_wbrat = 99.0;
	else
	    r->r_wbrat = ((float) s->st_wbkills / (float) s->st_wblosses) / t2;
	/* calculate js rating */
	t = (float) s->st_jsticks / (float)( (status2->jstime > 0) ? status2->jstime : 1);
	t2 = t * (float) status2->jsplanets;	/* get expected js planets */
	if (t2 == 0.0)
	    r->r_jsrat = 0.0;
	else if (t2 == 0)
	    r->r_jsrat = 99.0;
	else
	    r->r_jsrat = (float) s->st_jsplanets / t2;	/* js rating */
	r->r_jsplanets = s->st_jsplanets;	/* store js planets */

	r->r_specrat = r->r_sbrat + r->r_wbrat + r->r_jsrat;	/* get special ship
								   rating */
	/* put the sum of the three major ratings in the 'ratings' slot */
	r->r_ratings = r->r_specrat + r->r_batrat + r->r_stratrat;
	r->r_genocides = s->st_genocides;	/* get # genocides */
	r->r_di = s->st_di;	/* get player's DI */
	t = (s->st_tticks) ? s->st_tticks : (float) 1.0;
	r->r_killsPerHour = r->r_kills * (float) 36000.0 / t;
	r->r_lossesPerHour = r->r_losses * (float) 36000.0 / t;
	r->r_planets = s->st_tplanets;
	r->r_armies = s->st_tarmsbomb;
	r->r_resources = s->st_tresbomb;
	r->r_dooshes = s->st_tdooshes;
	/* r->r_jsplanets = s->st_jsplanets; */
        return r;
}

static void
Info_list_paradise(struct player *j)
{
    char buf[80];
    int line = 0;
    struct ratings r;

    get_ratings(j, &r);

    sprintf(buf, "Name: %s", j->p_name);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Rank: %s", ranks2[j->p_stats2.st_rank].name);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Royalty: %s", royal[j->p_stats2.st_royal].name);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Genocides: %4d", r.r_genocides);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "DI:     %7.2f", r.r_di);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Maxkills:%6.2f", j->p_stats2.st_tmaxkills);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Kills:     %4d", j->p_stats2.st_tkills);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Losses:    %4d", j->p_stats2.st_tlosses);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "T-hours: %6.2f", (float) j->p_stats2.st_tticks / 36000.0);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "   ");
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    strcpy(buf, "            Rating Total");
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Bombing:   %5.2f  %6d", r.r_bombrat, r.r_armies);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Planets:   %5.2f  %6d", r.r_planetrat, r.r_planets);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Resources: %5.2f  %6d", r.r_resrat, r.r_resources);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Dooshes:   %5.2f  %6d", r.r_dooshrat, r.r_dooshes);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Ratio:     %5.2f", j->p_stats2.st_tkills /
	    ((j->p_stats2.st_tlosses) ? j->p_stats2.st_tlosses : 1.0));
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Offense:   %5.2f", r.r_offrat);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "   ");
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "RATINGS");
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Special ships: %7.2f", r.r_specrat);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Battle:        %7.2f", r.r_batrat);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Strategy:      %7.2f", r.r_stratrat);
    W_WriteText(infow, W_Textwidth, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);

    line = 1;
    sprintf(buf, "   ");
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "JUMPSHIP STATS");
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Planets:    %7d", j->p_stats2.st_jsplanets);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Hours:      %7.2f", (float) j->p_stats2.st_jsticks / 36000.0);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "JS rating:  %7.2f", r.r_jsrat);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);

    sprintf(buf, "   ");
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "STARBASE STATS");
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Kills:        %4d", j->p_stats2.st_sbkills);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Losses:       %4d", j->p_stats2.st_sblosses);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Hours:     %7.2f", (float) j->p_stats2.st_sbticks / 36000.0);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Maxkills:  %7.2f", j->p_stats2.st_sbmaxkills);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "SB rating: %7.2f", r.r_sbrat);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);

    sprintf(buf, "   ");
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "WARBASE STATS");
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Kills:        %4d", j->p_stats2.st_wbkills);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Losses:       %4d", j->p_stats2.st_wblosses);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Hours:     %7.2f", (float) j->p_stats2.st_wbticks / 36000.0);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "Maxkills:  %7.2f", j->p_stats2.st_wbmaxkills);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
    sprintf(buf, "WB rating: %7.2f", r.r_wbrat);
    W_WriteText(infow, W_Textwidth * 28, W_Textheight * line++, playerColor(j),
		buf, strlen(buf), W_RegularFont);
}
#endif

/******************************************************************************/
/***  destroyInfo()                                                         ***/
/******************************************************************************/
void
destroyInfo (void)
{
    W_DestroyWindow (infow);
    infow = 0;
    infomapped = 0;
}

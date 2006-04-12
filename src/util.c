/******************************************************************************/
/***  File:  util.c
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

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* * Provide the angular distance between two angles. */
angdist (unsigned char x,
         unsigned char y)
{
    register unsigned char res;

    if (x > y)
        res = x - y;
    else
        res = y - x;
    if (res > 128)
        return (256 - (int) res);
    return ((int) res);
}

/* * Find the object nearest mouse.  Returns a pointer to an * obtype
 * structure.  This is used for info and locking on. *
 * 
 * Because we are never interested in it, this function will * never return your
 * own ship as the target. *
 * 
 * Finally, this only works on the two main windows */

struct obtype *
gettarget (W_Window ww,
           int x,
           int y,
           int targtype)
{
    int g_x, g_y;
    struct obtype *gettarget2 (int x,
                               int y,
                               int targtype);
    struct obtype *targ;

    if (ww == playerw)
    {
        /* Here we will get player from player list */
        static struct obtype _targ;
        g_x = (x - 1) / W_Textwidth;
        g_y = (y - 4) / W_Textheight;
        _targ.o_type = PLAYERTYPE;
        _targ.o_num = GetPlayerFromPlist (g_x, g_y);
        targ = &_targ;
        return (targ);
    }

    if (ww == mapw)
    {
        g_x = x * GWIDTH / WINSIDE;
        g_y = y * GWIDTH / WINSIDE;
    }
    else
    {
        g_x = me->p_x + ((x - WINSIDE / 2) * SCALE);
        g_y = me->p_y + ((y - WINSIDE / 2) * SCALE);
    }

    targ = gettarget2 (g_x, g_y, targtype);

    return (targ);
}

static struct obtype _target;

struct obtype *
gettarget2 (int x,
            int y,
            int targtype)
{
    register int i;
    register struct player *j;
    register struct planet *k;
    double dist, closedist;
    int friendly;

    closedist = GWIDTH;
    if (targtype & TARG_PLANET)
    {
        for (i = 0, k = &planets[i]; i < MAXPLANETS; i++, k++)
        {
            dist = hypot ((double) (x - k->pl_x), (double) (y - k->pl_y));
            if (dist < closedist)
            {
                _target.o_type = PLANETTYPE;
                _target.o_num = i;
                closedist = dist;
            }

        }
    }

    if (targtype & (TARG_PLAYER | TARG_FRIEND | TARG_ENEMY))
    {
        for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++)
        {
            if (j->p_status != PALIVE)
                continue;
            if ((j->p_flags & PFCLOAK) && (!(targtype & TARG_CLOAK)))
                continue;
            if (j == me && !(targtype & TARG_SELF))
                continue;
            friendly = friendlyPlayer (j);
            if (friendly && (targtype & TARG_ENEMY))
                continue;
            if (!friendly && (targtype & TARG_FRIEND))
                continue;

            dist = hypot ((double) (x - j->p_x), (double) (y - j->p_y));
            if (dist < closedist)
            {
                _target.o_type = PLAYERTYPE;
                _target.o_num = i;
                closedist = dist;
            }
        }
    }

    if (closedist == GWIDTH)
    {                           /* Didn't get one.  bad news */
        _target.o_type = PLAYERTYPE;
        _target.o_num = me->p_no;       /* Return myself.  Oh
                                         * well... */
        return (&_target);
    }
    else
    {
        return (&_target);
    }
}

troop_capacity (void)
{
    if (me->p_ship.s_type == ASSAULT)
        return (((me->p_kills * 3) > me->p_ship.s_maxarmies) ?
                me->p_ship.s_maxarmies : (int) (me->p_kills * 3));
    else if (me->p_ship.s_type != STARBASE)
        return (((me->p_kills * 2) > me->p_ship.s_maxarmies) ?
                me->p_ship.s_maxarmies : (int) (me->p_kills * 2));
    else
        return me->p_ship.s_maxarmies;
}

void
setObserverMode (int port)
{
	int i = 0;

	/* Let's find whether we are observing or not */
	while (observerPorts[i] != 0)
	{
		if (port == observerPorts[i])
		{
			observerMode = 1;
			break;
		}
		i++;
	}
}
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
int
angdist (unsigned char x,
         unsigned char y)
{
    register unsigned char res;

    if (x > y)
        res = (unsigned char) (x - y);
    else
        res = (unsigned char) (y - x);
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
        _targ.o_dist_x = -1;
        _targ.o_dist_y = -1;
        targ = &_targ;
        return (targ);
    }
    if (ww == planetw)
    {
        /* Here we will get planet from planet list */
        static struct obtype _targ;
        g_x = (x - 1) / W_Textwidth;
        g_y = (y - 4) / W_Textheight - 2; // Two header lines
        _targ.o_type = PLANETTYPE;
        _targ.o_num = GetPlanetFromPlist (g_x, g_y);
        _targ.o_dist_x = -1;
        _targ.o_dist_y = -1;
        targ = &_targ;
        return (targ);
    }

    if (ww == mapw)
    {
        g_x = x * GWIDTH / GWINSIDE;
        g_y = y * GWIDTH / GWINSIDE;
    }
    else
    {
        g_x = me->p_x + ((x - TWINSIDE / 2) * scaleFactor);
        g_y = me->p_y + ((y - TWINSIDE / 2) * scaleFactor);
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
    register struct plasmatorp *pt;
    double dist, closedist;
    int friendly;

    closedist = GWIDTH;
    if (targtype & TARG_PLANET)
    {
        for (i = 0, k = &planets[i]; i < nplanets; i++, k++)
        {
            dist = hypot ((double) (x - k->pl_x), (double) (y - k->pl_y));
            if (dist < closedist)
            {
                _target.o_type = PLANETTYPE;
                _target.o_num = i;
                _target.o_dist_x = k->pl_x;
                _target.o_dist_y = k->pl_y;
                closedist = dist;
            }

        }
    }

    if (targtype & (TARG_PLAYER | TARG_FRIEND | TARG_ENEMY))
    {
        for (i = 0, j = &players[i]; i < nplayers; i++, j++)
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
                _target.o_dist_x = j->p_x;
                _target.o_dist_y = j->p_y;
                closedist = dist;
            }
        }
    }
    if (targtype & TARG_PLASMA)
    {
        for (pt = plasmatorps + (nplasmas * nplayers) - 1; pt >= plasmatorps; --pt)
        {
            if (!pt->pt_status)
                continue;
            if (pt->pt_status == PTEXPLODE || pt->pt_status == PTFREE)
                continue;

            dist = hypot ((double) (x - pt->pt_x), (double) (y - pt->pt_y));
            if (dist < closedist)
            {
                _target.o_type = PLASMATYPE;
                _target.o_num = -1;
                _target.o_dist_x = pt->pt_x;
                _target.o_dist_y = pt->pt_y;
                closedist = dist;
            }
        }
    }

    if (closedist == GWIDTH)
    {                           /* Didn't get one.  bad news */
        _target.o_type = PLAYERTYPE;
        _target.o_num = me->p_no;       /* Return myself.  Oh
                                         * well... */
        _target.o_dist_x = -1;
        _target.o_dist_y = -1;
        return (&_target);
    }
    else
    {
        return (&_target);
    }
}

short
troop_capacity (void)
{
    if (me->p_ship.s_type == ASSAULT)
        return (short) ((((me->p_kills * 3) > me->p_ship.s_maxarmies) ?
                me->p_ship.s_maxarmies : (short) (me->p_kills * 3)));
    else if (me->p_ship.s_type != STARBASE)
        return (short) ((((me->p_kills * 2) > me->p_ship.s_maxarmies) ?
                me->p_ship.s_maxarmies : (short) (me->p_kills * 2)));
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
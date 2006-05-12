/******************************************************************************/
/***  File:  map.c
/***
/***  Function:
/***    Functions to maintain the galactic map.  This file is a merger
/***    of code from redraw.c, which was too big before, and planets.c,
/***    which was too small.
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/

#include <ctype.h>
#include <stdio.h>

#include "config.h"
#include "copyright2.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "map.h"
#include "proto.h"

/*
 *  Local Constants:
 *  
 *  DETAIL              -- Size of the redraw array.
 *  SIZE                -- Scale of the rough planet map "roughMap".
 *  
 *  Note: Detail *MUST* be a factor of GWIDTH.
 */

#define DETAIL 40
#define SIZE (GWIDTH/DETAIL)


/*
 *  Local Variables:
 *  
 *  roughMap[x][y]      -- Rough map of planets to help find overlaps.
 *  roughMap2[x][y]     -- Secondary rought map, to help with overlap.
 *  initialized         -- Has initPlanets() been called?
 */

static signed char roughMap[DETAIL][DETAIL];
static signed char roughMap2[DETAIL][DETAIL];
static int initialized = 0;


/*
 *  Global Variables:
 *
 *  redrawall           -- Erase and redraw the galactic?  Must be true
 *                              on first map() call.
 *  redrawPlayer[]      -- Flag for each player on whether their position
 *                              on the galactic is not out of date.
 */

int redrawall = 1;
unsigned char redrawPlayer[MAXPLAYER];



/* Function Definitions */

/******************************************************************************/
/***  initPlanets()
/******************************************************************************/
void
initPlanets (void)
/*
 *  Make a rough map of the location of all the planets to help decide
 *  whether a ship is possibly overlapping a planet.
 */
{
    int x, y, k;
    int startX, startY;
    int endX, endY;
    struct planet *pl;
    const int pRadius = BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 2;
    const int tHeight = W_Textheight * GWIDTH / WINSIDE;
    const int tWidth = W_Textwidth * GWIDTH / WINSIDE;

    for (x = 0; x < DETAIL; x++)
    {
        for (y = 0; y < DETAIL; y++)
        {
            roughMap[x][y] = -1;
            roughMap2[x][y] = -1;
        }
    }

    for (k = 0, pl = planets; k < MAXPLANETS; k++, pl++)
    {
        /*
           Size of planet is pRadius but a ship will touch the planet if
           it is one character away horizontally or half a character
           vertically.  Also remember the planet name at the bottom.
           This name can stick out about half a character to the right.
         */

        startX = (pl->pl_x - pRadius - tWidth) / SIZE;
        endX = (pl->pl_x + pRadius + tWidth + (tWidth / 2)) / SIZE;

        startY = (pl->pl_y - pRadius - (tHeight / 2)) / SIZE;
        endY = (pl->pl_y + pRadius + tHeight + (tHeight / 2)) / SIZE;

        if (startX < 0)
            startX = 0;

        if (endX >= DETAIL)
            endX = DETAIL - 1;

        if (startY < 0)
            startY = 0;

        if (endY >= DETAIL)
            endY = DETAIL - 1;

        for (x = startX; x <= endX; x++)
        {
            for (y = startY; y <= endY; y++)
            {
                if (roughMap[x][y] == -1)
                    roughMap[x][y] = (char) k;
                else
                    roughMap2[x][y] = (char) k;
            }
        }
    }

    initialized = 1;
}


#ifdef none                     /* Debugging code */

/******************************************************************************/
/***  showRegions()
/******************************************************************************/
void
showRegions (void)
/*
 *  Make a rough map of the location of all the planets to help decide
 *  whether a ship is possibly overlapping a planet.
 */
{
    int x, y, k;
    int startX, startY, centre;
    int endX, endY;
    struct planet *pl;
    const int pRadius = BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 2;
    const int tHeight = W_Textheight * GWIDTH / WINSIDE;
    const int tWidth = W_Textwidth * GWIDTH / WINSIDE;

    for (k = 0, pl = planets; k < MAXPLANETS; k++, pl++)
    {
        startX = (pl->pl_x - pRadius - tWidth) / SIZE;
        endX = (pl->pl_x + pRadius + tWidth + (tWidth / 2)) / SIZE;

        startY = (pl->pl_y - pRadius - (tHeight / 2)) / SIZE;
        endY = (pl->pl_y + pRadius + tHeight + (tHeight / 2)) / SIZE;

        if (startX < 0)
            startX = 0;

        if (endX > DETAIL)
            endX = DETAIL;

        if (startY < 0)
            startY = 0;

        if (endY > DETAIL)
            endY = DETAIL;

        startX = startX * SIZE * WINSIDE / GWIDTH;
        startY = startY * SIZE * WINSIDE / GWIDTH;
        endX = (endX * SIZE + SIZE - 1) * WINSIDE / GWIDTH;
        endY = (endY * SIZE + SIZE - 1) * WINSIDE / GWIDTH;

        W_MakeLine (mapw, startX, startY, startX, endY, W_White);
        W_MakeLine (mapw, startX, startY, endX, startY, W_White);
        W_MakeLine (mapw, endX, endY, startX, endY, W_White);
        W_MakeLine (mapw, endX, endY, endX, startY, W_White);
    }
}
#endif  /* none */           /* Debugging code */

/******************************************************************************/
/***  checkRedraw()
/******************************************************************************/
inline static void
checkRedraw (int x,
             int y)
/*
 *  Compare the given location of a ship with the rough planet map created
 *  by initPlanets() to decide if part of the planet may have been erased
 *  by the ship.
 */
{
    int i;

    x /= SIZE;
    y /= SIZE;

    i = roughMap[x][y];

    if (i != -1)
    {
        planets[i].pl_flags |= PLREDRAW;

        i = roughMap2[x][y];

        if (i != -1)
        {
            planets[i].pl_flags |= PLREDRAW;
        }
    }
}

/******************************************************************************/
/***  planetmBitmap()
/******************************************************************************/
inline static W_Icon
planetmBitmap (register struct planet *p)
/*
 *  Choose the bitmap for a planet.
 */
{
    int i;

    if ((p->pl_info & me->p_team)
#ifdef RECORDGAME
        || playback
#endif
        )
    {
        i = 0;
        if (p->pl_armies > 4)
            i += 4;
        if (p->pl_flags & PLREPAIR)
            i += 2;
        if (p->pl_flags & PLFUEL)
            i += 1;
        return bmplanets[i];
    }
    else
    {
        return (bmplanets[8]);
    }
}

/******************************************************************************/
/***  agriColor()
/******************************************************************************/
W_Color getAgriColor (struct planet *l)
{
    if (agriColor == 1)
        return W_White;
    else if (agriColor == 2)
        return W_Grey;
    else
        return planetColor (l);
}

/******************************************************************************/
/***  DrawPlanets()
/******************************************************************************/
static void
DrawPlanets ()
/*
 *  Draw the planets on the galactic map.
 */
{
    register struct planet *l;
    register int dx, dy;
    char ch, agri_name[3];

    for (l = planets + MAXPLANETS - 1; l >= planets; --l)
    {
        if (!(l->pl_flags & PLREDRAW))
            continue;

        l->pl_flags &= ~PLREDRAW;       /* Turn redraw flag off! */

        dx = l->pl_x * WINSIDE / GWIDTH;
        dy = l->pl_y * WINSIDE / GWIDTH;


        /* Erase the planet first */

        if (pl_update[l->pl_no].plu_update == 1)
        {
            /* Allow Moving Planets */

            int odx, ody;

            odx = pl_update[l->pl_no].plu_x * WINSIDE / GWIDTH;
            ody = pl_update[l->pl_no].plu_y * WINSIDE / GWIDTH;

            /* XFIX */
#ifndef DOUBLE_BUFFERING
            W_ClearArea (mapw, odx - (BMP_MPLANET_WIDTH / 2),
                         ody - (BMP_MPLANET_HEIGHT / 2),
                         BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT);
            W_WriteText (mapw, odx - (BMP_MPLANET_WIDTH / 2),
                         ody + (BMP_MPLANET_HEIGHT / 2),
                         backColor, l->pl_name, 3, planetFont (l));
#else
            W_ClearAreaDB (mapSDB, odx - (BMP_MPLANET_WIDTH / 2),
                           ody - (BMP_MPLANET_HEIGHT / 2),
                           BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT);
            W_WriteTextDB (mapSDB, odx - (BMP_MPLANET_WIDTH / 2),
                           ody + (BMP_MPLANET_HEIGHT / 2),
                           backColor, l->pl_name, 3, planetFont (l));
#endif
            pl_update[l->pl_no].plu_update = 0;
        }
        else
        {
            /* Clear the planet normally */

            /* XFIX */
#ifndef DOUBLE_BUFFERING
            W_ClearArea (mapw, dx - (BMP_MPLANET_WIDTH / 2 + 4),
                         dy - (BMP_MPLANET_HEIGHT / 2 + 4),
                         BMP_MPLANET_WIDTH + 8, BMP_MPLANET_HEIGHT + 8);
#else
            W_ClearAreaDB (mapSDB, dx - (BMP_MPLANET_WIDTH / 2 + 4),
                           dy - (BMP_MPLANET_HEIGHT / 2 + 4),
                           BMP_MPLANET_WIDTH + 8, BMP_MPLANET_HEIGHT + 8);
#endif
            l->pl_flags &= ~PLCLEAR;
        }


        /* Draw the new planet */
        
#ifdef BEEPLITE
        if (useLite && emph_planet_seq_n[l->pl_no] > 0)
	{
	    int     seq_n = emph_planet_seq_n[l->pl_no] % emph_planet_seq_frames;

#ifndef DOUBLE_BUFFERING
	    W_OverlayBitmap(dx - (emph_planet_seq_width / 2 + 1),
			    dy - (emph_planet_seq_height / 2),
			    emph_planet_seq[seq_n],
			    W_White);

	    W_WriteBitmap(dx - (BMP_MPLANET_WIDTH / 2), dy - (BMP_MPLANET_HEIGHT / 2),
			  planetmBitmap(l), planetColor(l));
#else
	    W_OverlayBitmapDB(mapSDB, dx - (emph_planet_seq_width / 2 + 1),
			      dy - (emph_planet_seq_height / 2),
			      emph_planet_seq[seq_n],
			      W_White);

	    W_WriteBitmapDB(mapSDB, dx - (BMP_MPLANET_WIDTH / 2), dy - (BMP_MPLANET_HEIGHT / 2),
			    planetmBitmap(l), planetColor(l));
#endif
	    emph_planet_seq_n[l->pl_no] -= 1;
	    l->pl_flags |= PLREDRAW;		 /* Leave redraw on until * * 
						  * done highlighting */
	    l->pl_flags |= PLCLEAR;		 /* Leave redraw on until * * 
						  * done highlighting */
	}
        else
	{
#endif
#ifndef DOUBLE_BUFFERING
        W_OverlayBitmap (dx - (BMP_MPLANET_WIDTH / 2),
                         dy - (BMP_MPLANET_HEIGHT / 2), planetmBitmap (l),
                         planetColor (l));
#else
        W_OverlayBitmapDB (mapSDB, dx - (BMP_MPLANET_WIDTH / 2),
                           dy - (BMP_MPLANET_HEIGHT / 2), planetmBitmap (l),
                           planetColor (l));
#endif
#ifdef BEEPLITE
	}
#endif

        if ((l->pl_flags & PLAGRI) && (l->pl_info & me->p_team))
        {
            if (agriCAPS)
            {
                agri_name[0] = (char) (toupper (l->pl_name[0]));
                agri_name[1] = (char) (toupper (l->pl_name[1]));
                agri_name[2] = (char) (toupper (l->pl_name[2]));
#ifndef DOUBLE_BUFFERING
                W_WriteText (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                            dy + (BMP_MPLANET_HEIGHT / 2), getAgriColor (l),
                            agri_name, 3, planetFont (l));
#else
                W_WriteTextDB (mapSDB, dx - (BMP_MPLANET_WIDTH / 2),
                               dy + (BMP_MPLANET_HEIGHT / 2), getAgriColor (l),
                               agri_name, 3, planetFont (l));
#endif
            }
            else
            {
#ifndef DOUBLE_BUFFERING
                W_WriteText (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                            dy + (BMP_MPLANET_HEIGHT / 2), getAgriColor (l),
                            l->pl_name, 3, planetFont (l));
#else
                W_WriteTextDB (mapSDB, dx - (BMP_MPLANET_WIDTH / 2),
                               dy + (BMP_MPLANET_HEIGHT / 2), getAgriColor (l),
                               l->pl_name, 3, planetFont (l));
#endif
            }
        }
        else
        {
#ifndef DOUBLE_BUFFERING
            W_WriteText (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                        dy + (BMP_MPLANET_HEIGHT / 2), planetColor (l),
                        l->pl_name, 3, planetFont (l));
#else
            W_WriteTextDB (mapSDB, dx - (BMP_MPLANET_WIDTH / 2),
                           dy + (BMP_MPLANET_HEIGHT / 2), planetColor (l),
                           l->pl_name, 3, planetFont (l));
#endif
        }

        if (showIND && ((l->pl_info & me->p_team)
#ifdef RECORDGAME
                        || playback
#endif
            ) && (l->pl_owner == NOBODY))
        {
#ifndef DOUBLE_BUFFERING
            W_MakeLine (mapw, dx + (BMP_MPLANET_WIDTH / 2 - 1),
                        dy + (BMP_MPLANET_HEIGHT / 2 - 1),
                        dx - (BMP_MPLANET_WIDTH / 2),
                        dy - (BMP_MPLANET_HEIGHT / 2), W_White);
            W_MakeLine (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                        dy + (BMP_MPLANET_HEIGHT / 2 - 1),
                        dx + (BMP_MPLANET_WIDTH / 2 - 1),
                        dy - (BMP_MPLANET_HEIGHT / 2), W_White);
#else
            W_MakeLineDB (mapSDB, dx + (BMP_MPLANET_WIDTH / 2 - 1),
                          dy + (BMP_MPLANET_HEIGHT / 2 - 1),
                          dx - (BMP_MPLANET_WIDTH / 2),
                          dy - (BMP_MPLANET_HEIGHT / 2), W_White);
            W_MakeLineDB (mapSDB, dx - (BMP_MPLANET_WIDTH / 2),
                          dy + (BMP_MPLANET_HEIGHT / 2 - 1),
                          dx + (BMP_MPLANET_WIDTH / 2 - 1),
                          dy - (BMP_MPLANET_HEIGHT / 2), W_White);
#endif
        }

        if (showPlanetOwner)
        {
            ch = (char) (((l->pl_info & me->p_team)
#ifdef RECORDGAME
                  || playback
#endif
                )? tolower (teamlet[l->pl_owner]) : '?');
#ifndef DOUBLE_BUFFERING
            W_WriteText (mapw, dx + (BMP_MPLANET_WIDTH / 2) + 2,
                         dy - 6, planetColor (l), &ch, 1, planetFont (l));
#else
            W_WriteTextDB (mapSDB, dx + (BMP_MPLANET_WIDTH / 2) + 2,
                           dy - 6, planetColor (l), &ch, 1, planetFont (l));
#endif
        }
    }
}


#ifdef HOCKEY_LINES

void
DrawGalaxyHockeyLines (void)
{
    register struct s_line *sl;

    for (sl = map_hockey_lines + NUM_HOCKEY_LINES - 1; sl >= map_hockey_lines;
         --sl)
    {
#ifndef DOUBLE_BUFFERING
        W_CacheLine (mapw, sl->begin_x, sl->begin_y, sl->end_x, sl->end_y, sl->color);
#else
        W_CacheLineDB (mapSDB, sl->begin_x, sl->begin_y, sl->end_x, sl->end_y, sl->color);
#endif

    }
}

void
DrawGalaxyHockeyScore (void)
{
	int kli_score = 0;
	int ori_score = 0;
	int kli_light = ROM;
	int ori_light = FED;
	char kli_score_line[5];
	char ori_score_line[5];
	int ori_offset, kli_offset;

	int i;

	/* Let's check KLI scores */
	for (i = 10; i < 15; i++)
		if (planets[i].pl_owner == kli_light)
			kli_score++;
		else
			break;
	for (i = 0; i < 5; i++)
		if (planets[i].pl_owner == kli_light)
			kli_score += 5;
		else
			break;

	/* Let's check ORI scores */
	for (i = 9; i >= 5; i--)
		if (planets[i].pl_owner == ori_light)
			ori_score++;
		else
			break;
	for (i = 19; i >= 15; i--)
		if (planets[i].pl_owner == ori_light)
			ori_score += 5;
		else
			break;

	/* Now we have scores, so let's draw them */
	sprintf (ori_score_line, "%2d", ori_score);
	sprintf (kli_score_line, "%-2d", kli_score);
	
	/* If we rotate galaxy 180 degrees the scoreboard flips */
	if (rotate == 2)
	{
        	ori_offset = 8;
        	kli_offset = 3;
        }
        else
        {
        	ori_offset = 3;
        	kli_offset = 8;
        }

#ifndef DOUBLE_BUFFERING
	W_WriteText (mapw, ori_offset * W_Textwidth, 1 * W_Textheight, W_Ori, 
				ori_score_line, strlen (ori_score_line), W_RegularFont);
	W_WriteText (mapw, 6 * W_Textwidth, 1 * W_Textheight, W_White, 
				":", 1, W_RegularFont);
	W_WriteText (mapw, kli_offset * W_Textwidth, 1 * W_Textheight, W_Kli, 
				kli_score_line, strlen (kli_score_line), W_RegularFont);
#else
	W_WriteTextDB (mapSDB, ori_offset * W_Textwidth, 1 * W_Textheight, W_Ori, 
				   ori_score_line, strlen (ori_score_line), W_RegularFont);
	W_WriteTextDB (mapSDB, 6 * W_Textwidth, 1 * W_Textheight, W_White, 
				   ":", 1, W_RegularFont);
	W_WriteTextDB (mapSDB, kli_offset * W_Textwidth, 1 * W_Textheight, W_Kli, 
				   kli_score_line, strlen (kli_score_line), W_RegularFont);
#endif

}
#endif /* HOCKEY_LINES */


/******************************************************************************/
/***  map()
/******************************************************************************/
void
map (void)
/*
 *  Update the 'galactic' map.
 */
{
    register int i;
    register unsigned char *update;
    register struct player *j;
    register struct planet *l;
    register int dx, dy;

    static char clearlock = 0;
    static int mclearzone[6][MAXPLAYER];
    static int clearlmark[4];
    static unsigned char lastUpdate[MAXPLAYER];

    static int viewx = 0, viewy = 0;
    static char clearviewbox = 0;
    static char viewboxcleared = 0;
    int viewdist = (WINSIDE / 2 * SCALE) / (GWIDTH / WINSIDE);
    int view = WINSIDE * SCALE / 2;
    int mvx, mvy;

#ifdef DOUBLE_BUFFERING
    W_Win2Mem (mapSDB);
#endif

    dx = (me->p_x) / (GWIDTH / WINSIDE);
    dy = (me->p_y) / (GWIDTH / WINSIDE);

    if (redrawall)
    {
        /*
           Set things up so that the galactic will be redraw completely.
           This code should also put critical variables into a sane state
           in case COW has just started.
         */

        if (!initialized)
        {
            /* Don't do anything if initPlanets() has not been called */
            return;
        }

#ifndef DOUBLE_BUFFERING
        W_ClearWindow (mapw);
#else
        W_ClearWindowDB (mapSDB);
#endif
        clearlock = 0;
        clearviewbox = 0;
        viewboxcleared = 0;

        for (i = 0; i < MAXPLAYER; i++)
        {
            lastUpdate[i] = 0;
            mclearzone[2][i] = 0;
            redrawPlayer[i] = 1;
        }

        for (l = planets + MAXPLANETS - 1; l >= planets; --l)
            l->pl_flags |= PLREDRAW;

        redrawall = 0;
    }
    else
    {
        if (clearlock)
        {
            clearlock = 0;
#ifndef DOUBLE_BUFFERING
            W_WriteTriangle (mapw, clearlmark[0], clearlmark[1],
                             clearlmark[2], clearlmark[3], backColor);
#else
            W_WriteTriangleDB (mapSDB, clearlmark[0], clearlmark[1],
                               clearlmark[2], clearlmark[3], backColor);
#endif
        }

        if (clearviewbox)
        {
            clearviewbox = 0;
            /* clear old dots - placed here for less flicker */
            if (viewx + viewdist < WINSIDE && viewy + viewdist < WINSIDE)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, viewx + viewdist, viewy + viewdist, backColor);
#else
                W_MakePointDB (mapSDB, viewx + viewdist, viewy + viewdist, backColor);
#endif
            if (viewx + viewdist < WINSIDE && viewy - viewdist > 0)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, viewx + viewdist, viewy - viewdist, backColor);
#else
                W_MakePointDB (mapSDB, viewx + viewdist, viewy - viewdist, backColor);
#endif
            if (viewx - viewdist > 0 && viewy + viewdist < WINSIDE)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, viewx - viewdist, viewy + viewdist, backColor);
#else
                W_MakePointDB (mapSDB, viewx - viewdist, viewy + viewdist, backColor);
#endif
            if (viewx - viewdist > 0 && viewy - viewdist > 0)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, viewx - viewdist, viewy - viewdist, backColor);
#else
                W_MakePointDB (mapSDB, viewx - viewdist, viewy - viewdist, backColor);
#endif

            /* redraw any planets they overwrote */
            mvx = viewx * (GWIDTH / WINSIDE); /* correct from view scale */
            mvy = viewy * (GWIDTH / WINSIDE);
            checkRedraw(mvx + view, mvy + view);
            checkRedraw(mvx + view, mvy - view);
            checkRedraw(mvx - view, mvy + view);
            checkRedraw(mvx - view, mvy - view);
            viewboxcleared = 1;
        }

        /* Erase the ships */

        for (i = 0, update = lastUpdate; i < MAXPLAYER; i++, update++)
        {
            if (redrawPlayer[i])
            {
                /* Erase the player if redrawPlayer[i] is set
                 * or lastUpdate allows it. */

                if (mclearzone[2][i])
                {
                    /* XFIX */
#ifndef DOUBLE_BUFFERING
                    W_ClearArea (mapw, mclearzone[0][i], mclearzone[1][i],
                                 mclearzone[2][i], mclearzone[3][i]);
#else
                    W_ClearAreaDB (mapSDB, mclearzone[0][i], mclearzone[1][i],
                                   mclearzone[2][i], mclearzone[3][i]);
#endif

                    /* Redraw the hole just left next update */
                    checkRedraw (mclearzone[4][i], mclearzone[5][i]);
                    mclearzone[2][i] = 0;
                }


                /* Reset the last redrawn counter */

                *update = 0;
            }
            else if (*update == 10)
            {
                /*
                 *  Redraw stationary ships every update so that these
                 *  ships are not hidden by planet updates.
                 */

                redrawPlayer[i] = 1;
            }
            else
            {
                ++(*update);
            }
        }
    }


    /* Draw Planets */

    DrawPlanets ();

    /* draw viewBox */
    if (viewBox)
    {
        if (viewboxcleared || viewx != dx || viewy != dy)
        {
            /* draw the new points */
            if (dx + viewdist < WINSIDE && dy + viewdist < WINSIDE)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, dx + viewdist, dy + viewdist, W_White);
#else
                W_MakePointDB (mapSDB, dx + viewdist, dy + viewdist, W_White);
#endif
            if (dx + viewdist < WINSIDE && dy - viewdist > 0)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, dx + viewdist, dy - viewdist, W_White);
#else
                W_MakePointDB (mapSDB, dx + viewdist, dy - viewdist, W_White);
#endif
            if (dx - viewdist > 0 && dy + viewdist < WINSIDE)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, dx - viewdist, dy + viewdist, W_White);
#else
                W_MakePointDB (mapSDB, dx - viewdist, dy + viewdist, W_White);
#endif
            if (dx - viewdist > 0 && dy - viewdist > 0)
#ifndef DOUBLE_BUFFERING
                W_MakePoint (mapw, dx - viewdist, dy - viewdist, W_White);
#else
                W_MakePointDB (mapSDB, dx - viewdist, dy - viewdist, W_White);
#endif

            viewx = dx;         /* store the points for later */
            viewy = dy;         /* clearing */
            clearviewbox = 1;   /* and flag */
        }
    }

#ifdef HOCKEY_LINES
    if (hockey_mode ())
    {
        /* Draw Hockey Lines */
	if (showHockeyLinesMap)
	    DrawGalaxyHockeyLines ();

        /* Draw Hockey Score */
        if (showHockeyScore)
	    DrawGalaxyHockeyScore ();
    }
#endif /* HOCKEY_LINES */

    /* Draw ships */

    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++)
    {
        /* lastUpdate[i] has been set to 0 if redrawall or the ship has
         * been erased or a redraw has not taken place for a while.  These
         * decisions are made at the top of the file.             */

        if (!redrawPlayer[i])
            continue;
        if (j->p_status != PALIVE)
            continue;
        if (j->p_flags & PFOBSERV)
            continue;           /* jmn - observer support */
        if (j->p_x < 0 || j->p_x >= GWIDTH || j->p_y < 0 || j->p_y >= GWIDTH)
            continue;


        dx = j->p_x * WINSIDE / GWIDTH;
        dy = j->p_y * WINSIDE / GWIDTH;


        if (j->p_flags & PFCLOAK)
        {
            if (omitTeamLetter)
#ifndef DOUBLE_BUFFERING
                W_WriteText (mapw, dx - (W_Textwidth / 2),
                             dy - W_Textheight / 2, unColor, cloakChars,
                             1, W_RegularFont);
#else
                W_WriteTextDB (mapSDB, dx - (W_Textwidth / 2),
                               dy - W_Textheight / 2, unColor, cloakChars,
                               (cloakChars[1] == '\0' ? 1 : 2), W_RegularFont);
#endif
            else
#ifndef DOUBLE_BUFFERING
                W_WriteText (mapw, dx - W_Textwidth,
                             dy - W_Textheight / 2, unColor, cloakChars,
                             (cloakChars[1] == '\0' ? 1 : 2), W_RegularFont);
#else
                W_WriteTextDB (mapSDB, dx - W_Textwidth,
                               dy - W_Textheight / 2, unColor, cloakChars,
                               (cloakChars[1] == '\0' ? 1 : 2), W_RegularFont);
#endif
        }
        else
        {
            if (omitTeamLetter)
#ifndef DOUBLE_BUFFERING
                W_WriteText (mapw, dx - (W_Textwidth / 2),
                             dy - W_Textheight / 2, playerColor (j),
                             (j->p_mapchars)+1, 1, shipFont (j));
#else
                W_WriteTextDB (mapSDB, dx - (W_Textwidth / 2),
                               dy - W_Textheight / 2, playerColor (j),
                               (j->p_mapchars)+1, 1, shipFont (j));
#endif
            else
#ifndef DOUBLE_BUFFERING
                W_WriteText (mapw, dx - W_Textwidth,
                             dy - W_Textheight / 2, playerColor (j),
                             j->p_mapchars, 2, shipFont (j));
#else
                W_WriteTextDB (mapSDB, dx - W_Textwidth,
                               dy - W_Textheight / 2, playerColor (j),
                               j->p_mapchars, 2, shipFont (j));
#endif
        }

#ifdef BEEPLITE
        if ((useLite && emph_player_seq_n[i] > 0)
	  && (liteflag & LITE_PLAYERS_MAP))
	{
	    int     seq_n = emph_player_seq_n[i] % emph_player_seq_frames;

#ifndef DOUBLE_BUFFERING
	    W_WriteBitmap(dx - (emph_player_seq_width / 2 - 1),
			  dy - (emph_player_seq_height / 2 + 1),
			  emph_player_seq[seq_n],
			  W_White);
#else
	    W_WriteBitmapDB(mapSDB, dx - (emph_player_seq_width / 2 - 1),
			    dy - (emph_player_seq_height / 2 + 1),
			    emph_player_seq[seq_n],
			    W_White);
#endif
	    emph_player_seq_n[i] -= 1;
	    mclearzone[0][i] = dx - (emph_player_seq_width / 2 - 1);
	    mclearzone[1][i] = dy - (emph_player_seq_height / 2 + 1);
	    mclearzone[2][i] = emph_player_seq_width;
	    mclearzone[3][i] = emph_player_seq_height;
	    mclearzone[4][i] = j->p_x;
	    mclearzone[5][i] = j->p_y;

	    /* Leave redraw on until done highlighting */
	    redrawPlayer[i] = 1;
	}
        else
	{
#endif
	    mclearzone[0][i] = dx - W_Textwidth;
	    mclearzone[1][i] = dy - W_Textheight / 2;
	    mclearzone[2][i] = W_Textwidth * 2;
	    mclearzone[3][i] = W_Textheight;

	    /* Set these so we can checkRedraw() next time */
	    mclearzone[4][i] = j->p_x;
	    mclearzone[5][i] = j->p_y;
	    redrawPlayer[i] = 0;

#ifdef BEEPLITE
	}
#endif
    }

    /* Draw the lock symbol (if needed) */

    if ((me->p_flags & PFPLOCK) && (showLock & 1))
    {
        j = &players[me->p_playerl];

        if (j->p_status == PALIVE && !(j->p_flags & PFCLOAK))
        {
            dx = j->p_x * WINSIDE / GWIDTH;
            dy = j->p_y * WINSIDE / GWIDTH;
#ifndef DOUBLE_BUFFERING
            W_WriteTriangle (mapw, dx, dy + 6, 4, 1, foreColor);
#else
            W_WriteTriangleDB (mapSDB, dx, dy + 6, 4, 1, foreColor);
#endif

            clearlmark[0] = dx;
            clearlmark[1] = dy + 6;
            clearlmark[2] = 4;
            clearlmark[3] = 1;
            clearlock = 1;
        }
    }
    else if ((me->p_flags & PFPLLOCK) && (showLock & 1))
    {
        struct planet *l = &planets[me->p_planet];

        dx = l->pl_x * WINSIDE / GWIDTH;
        dy = l->pl_y * WINSIDE / GWIDTH;
#ifndef DOUBLE_BUFFERING
        W_WriteTriangle (mapw, dx, dy - (BMP_MPLANET_HEIGHT) / 2 - 4, 4, 0, foreColor);
#else
        W_WriteTriangleDB (mapSDB, dx, dy - (BMP_MPLANET_HEIGHT) / 2 - 4, 4, 0, foreColor);
#endif

        clearlmark[0] = dx;
        clearlmark[1] = dy - (BMP_MPLANET_HEIGHT) / 2 - 4;
        clearlmark[2] = 4;
        clearlmark[3] = 0;
        clearlock = 1;
    }

#ifdef DOUBLE_BUFFERING
    W_Mem2Win (mapSDB);
#endif
}
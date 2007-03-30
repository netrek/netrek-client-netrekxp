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

/* debugging feature, show rectangular redraw regions on galactic */
#undef DEBUG_SHOW_REGIONS

/*
 *  Local Constants:
 *  
 *  DETAIL              -- Size of the redraw array.
 *  SIZE                -- Scale of the rough planet map "roughMap".
 *  
 *  Note: Detail *MUST* be a factor of GWIDTH.
 */

#define DETAIL 100
#define SIZE (GWIDTH/DETAIL)


/*
 *  Local Variables:
 *  
 *  roughMap[x][y]      -- Rough map of planets to help find overlaps.
 *  roughMap2[x][y]     -- Secondary rough map, to help with overlap.
 *  roughMap3[x][y]     -- Rough map of new planets to help find overlaps.
 *  roughMap4[x][y]     -- Secondary rough map for new planets, to help with overlap.
 *  initialized         -- Has initPlanets() been called?
 */

static signed char roughMap[DETAIL][DETAIL];
static signed char roughMap2[DETAIL][DETAIL];
static signed char roughMap3[DETAIL][DETAIL];
static signed char roughMap4[DETAIL][DETAIL];
static int initialized = 0;
static int maplockline[4];	/* Coordinates for lock line on map */
static int mclearlcount;	/* For phasers */
static int mclearline[4][MAXPLAYER + 2 * MAXPLAYER];
static int mclearpcount;	/* For torps */
static int mclearpoint[2][(MAXTORP + 1) * MAXPLAYER];
static int mclearacount; 	/* For torp explosions, plasmas, and plasma explosions */
static int mcleararea[4][(MAXTORP + 1) * MAXPLAYER + (MAXPLASMA + 1) * MAXPLAYER];
static int mclearx, mcleary, mclearr;	/* x,y,radius for range circle */
static int mclearccount;

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
    int pRadius;
    const int tHeight = W_Textheight * GWIDTH / WINSIDE;
    const int tWidth = W_Textwidth * GWIDTH / WINSIDE;

    for (x = 0; x < DETAIL; x++)
    {
        for (y = 0; y < DETAIL; y++)
        {
            roughMap[x][y] = -1;
            roughMap2[x][y] = -1;
            roughMap3[x][y] = -1;
            roughMap4[x][y] = -1;
        }
    }
    /* Treating the planet a little bigger makes beeplite flashes redraw planets more
       consistently - I don't think this function was written with the possibility of a
       ship cutting out twice it's normal size on the galactic map.  The player's ship gets
       assigned to a x,y point and that is intersected with the rough planet map in a 
       checkredraw call, which doesn't work so good if the player ship is beeplite highlited */ 
#ifdef BEEPLITE
    pRadius = 3 * BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 5;
#else
    pRadius = BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 2;
#endif
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
    /* Now loop for the new planet bitmaps, which are roughly 50% wider */
    pRadius = 3 * BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 4;
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
                if (roughMap3[x][y] == -1)
                    roughMap3[x][y] = (char) k;
                else
                    roughMap4[x][y] = (char) k;
            }
        }
    }
    initialized = 1;
}

#ifdef DEBUG_SHOW_REGIONS                        /* Debugging code */
void
showRegions(void)
/*
 *  Make a rough map of the location of all the planets to help decide
 *  whether a ship is possibly overlapping a planet.
 */
{
    int     k;
    int     startX, startY;
    int     endX, endY;
    struct planet *pl;
#ifdef BEEPLITE
    const int pRadius = 3 * BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 5;
#else
    const int pRadius = BMP_MPLANET_WIDTH * GWIDTH / WINSIDE / 2;
#endif
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

        if (endX >= DETAIL)
            endX = DETAIL - 1;

        if (startY < 0)
            startY = 0;

        if (endY >= DETAIL)
            endY = DETAIL - 1;

        startX = startX * SIZE * WINSIDE / GWIDTH;
        startY = startY * SIZE * WINSIDE / GWIDTH;
        endX = (endX * SIZE + SIZE - 1) * WINSIDE / GWIDTH;
        endY = (endY * SIZE + SIZE - 1) * WINSIDE / GWIDTH;

        W_MakeLine(mapw, startX, startY, startX, endY, W_White);
        W_MakeLine(mapw, startX, startY, endX, startY, W_White);
        W_MakeLine(mapw, endX, endY, startX, endY, W_White);
        W_MakeLine(mapw, endX, endY, endX, startY, W_White);
    }
}
#endif  /* Debugging code */

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
 *
 *  Also force a redraw of every other player in the same grid square,
 *  otherwise they will blink out of existence as a result of the
 *  planet redraw.
 */
{
    int i;
    struct player *j;

    x /= SIZE;
    y /= SIZE;

    if (planetBitmapGalaxy != 3)
    {
        i = roughMap[x][y];

        if (i == -1) return;
        planets[i].pl_flags |= PLREDRAW;

        for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++)
        {
            if (j->p_status != PALIVE) continue;
            if (j->p_flags & PFOBSERV) continue;
            if (roughMap[j->p_x / SIZE][j->p_y / SIZE] != -1)
                redrawPlayer[i] = 1;
        }

        i = roughMap2[x][y];
        if (i == -1) return;
        planets[i].pl_flags |= PLREDRAW;
    }
    else
    {
        i = roughMap3[x][y];

        if (i == -1) return;
        planets[i].pl_flags |= PLREDRAW;

        for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++)
        {
            if (j->p_status != PALIVE) continue;
            if (j->p_flags & PFOBSERV) continue;
            if (roughMap3[j->p_x / SIZE][j->p_y / SIZE] != -1)
                redrawPlayer[i] = 1;
        }

        i = roughMap4[x][y];
        if (i == -1) return;
        planets[i].pl_flags |= PLREDRAW;
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
/***  mplanetBitmapC()
/******************************************************************************/
static inline W_Icon
mplanetBitmapC (register struct planet *p)
/*
 *  Choose the map color bitmap for a planet.
 */
{
    int i;
    W_Icon (*mplanet_bits);
    
    if ((p->pl_info & me->p_team)
#ifdef RECORDGAME
        || playback
#endif
        )
    {
    	/* Logic for planet assignment:
    	   1) Check if it's a homeworld.  If so, find which homeworld it is
    	   2) Check if it's a core planet
    	   2a) Check if it's agri
    	   3) Check if it's agri
    	   4) Default planet bitmap
    	   5) Switch statement by owner to determine color
        */
    	if (p->pl_flags & PLHOME)
    	{
    	    if (!strcmp(p->pl_name, "Earth"))
    	        mplanet_bits = mplanet_earth;
    	    else if (!strcmp(p->pl_name, "Klingus"))
    	        mplanet_bits = mplanet_klingus;
    	    else if (!strcmp(p->pl_name, "Romulus"))
    	        mplanet_bits = mplanet_romulus;
    	    else if (!strcmp(p->pl_name, "Orion"))
    	        mplanet_bits = mplanet_orion;
    	    else // This should never be called
    	        mplanet_bits = mplanet_rock1;
    	}
    	else if (p->pl_flags & PLCORE)
    	{
    	    if (p->pl_flags & PLAGRI)
    	        mplanet_bits = mplanet_agri1;
    	    else
    	        mplanet_bits = mplanet_rock1;
    	}
    	else if (p->pl_flags & PLAGRI)
            mplanet_bits = mplanet_agri2;
        else
            mplanet_bits = mplanet_rock2;

        switch (p->pl_owner)
        {
        case FED:
            i = 0;
            break;
        case KLI:
            i = 2;
            break;
        case ORI:
            i = 3;
            break;
        case ROM:
            i = 4;
            break;
        default: // IND
            i = 1;
            break;
        }
        return mplanet_bits[i];
    }
    else // Unknown planet
    {
        return mplanet_unknown;
    }
}

/******************************************************************************/
/***  mplanetResourcesC()
/******************************************************************************/		           
static inline void
mplanetResourcesC (register struct planet *p, int destwidth, int destheight,
                  int dx, int dy, W_Window window)
/*
 *  Draw the map resources for a colorized planet.
 */
{   
    if ((p->pl_info & me->p_team)
#ifdef RECORDGAME
        || playback
#endif
        )
    {
    	/* Select resources */
        if (p->pl_armies > 4)
            W_WriteScaleBitmap(dx - 7 * destwidth / 8 - 1,
                               dy - (destheight / 2),
                               destwidth/3 + 1,
                               destheight,
                               BMP_ARMY_WIDTH,
                               BMP_ARMY_HEIGHT,
                               0,
                               marmy_bitmap, planetColor(p),
                               window);       
        if (p->pl_flags & PLREPAIR)
            W_WriteScaleBitmap(dx - (destwidth / 2),
                               dy - (5 * destheight / 6),
                               destwidth,
                               destheight/3 + 1,
                               BMP_WRENCH_WIDTH,
                               BMP_WRENCH_HEIGHT,
                               0,
                               mwrench_bitmap, planetColor(p),
                               window);
        if (p->pl_flags & PLFUEL)
            W_WriteScaleBitmap(dx + 3 * destwidth / 5 - 1,
                               dy - (destheight / 2),
                               destwidth/3 + 1,
                               destheight,
                               BMP_FUEL_WIDTH,
                               BMP_FUEL_HEIGHT,
                               0,
                               mfuel_bitmap, planetColor(p),
                               window);
    }
    return;
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
            if (planetBitmapGalaxy == 3)  // Needs adjusting
                W_ClearArea (mapw, odx - (7 * BMP_MPLANET_WIDTH / 8 + 1),
                             ody - (5 * BMP_MPLANET_HEIGHT / 6),
                             7 * BMP_MPLANET_WIDTH / 4 + 2, 4 * BMP_MPLANET_HEIGHT / 3);
            else
                W_ClearArea (mapw, odx - (BMP_MPLANET_WIDTH / 2),
                             ody - (BMP_MPLANET_HEIGHT / 2),
                             BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT);
            W_WriteText (mapw, odx - (BMP_MPLANET_WIDTH / 2),
                         ody + (BMP_MPLANET_HEIGHT / 2),
                         backColor, l->pl_name, 3, planetFont (l));
            pl_update[l->pl_no].plu_update = 0;
        }
        else
        {
            /* Clear the planet normally */

            /* XFIX */
            if (planetBitmapGalaxy == 3)  // Needs adjusting
                W_ClearArea (mapw, dx - (7 * BMP_MPLANET_WIDTH / 8 + 5),
                             dy - (5 * BMP_MPLANET_HEIGHT / 6 + 4),
                             7 * BMP_MPLANET_WIDTH / 4 + 10, 4 * BMP_MPLANET_HEIGHT / 3 + 8);

            else
                W_ClearArea (mapw, dx - (BMP_MPLANET_WIDTH / 2 + 4),
                             dy - (BMP_MPLANET_HEIGHT / 2 + 4),
                             BMP_MPLANET_WIDTH + 8, BMP_MPLANET_HEIGHT + 8);
            l->pl_flags &= ~PLCLEAR;
        }


        /* Draw the new planet */
        
#ifdef BEEPLITE
        if (useLite && emph_planet_seq_n[l->pl_no] > 0)
	{
	    int seq_n = (emph_planet_seq_n[l->pl_no] * 10 / server_ups) % emph_planet_seq_frames;
            
            if (planetBitmapGalaxy == 3)
            {
	        W_WriteBitmap(dx - (emph_planet_seq_width / 2 + 1),
			        dy - (emph_planet_seq_height / 2),
			        emph_planet_seq[seq_n],
			        W_White, mapw);

	        W_WriteScaleBitmap(dx - (BMP_MPLANET_WIDTH / 2),
	                           dy - (BMP_MPLANET_HEIGHT / 2),
	                           BMP_MPLANET_WIDTH,
                                   BMP_MPLANET_HEIGHT,
                                   BMP_CPLANET_WIDTH,
			           BMP_CPLANET_HEIGHT,
			           0,
			           mplanetBitmapC(l), planetColor(l),
			           mapw);
			           
                /* Draw planet resources */
                mplanetResourcesC(l, BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT, dx, dy, mapw);
            }
            else
            {
	        W_WriteBitmap(dx - (emph_planet_seq_width / 2 + 1),
			        dy - (emph_planet_seq_height / 2),
			        emph_planet_seq[seq_n],
			        W_White, mapw);

	        W_WriteBitmap(dx - (BMP_MPLANET_WIDTH / 2), dy - (BMP_MPLANET_HEIGHT / 2),
			      planetmBitmap(l), planetColor(l), mapw);
            }
	    emph_planet_seq_n[l->pl_no] -= 1;
	    l->pl_flags |= PLREDRAW;		 /* Leave redraw on until * * 
						  * done highlighting */
	    l->pl_flags |= PLCLEAR;		 /* Leave redraw on until * * 
						  * done highlighting */
	}
        else
	{
#endif   // Beeplite end        
        if (planetBitmapGalaxy == 3)
        {
            W_WriteScaleBitmap (dx - (BMP_MPLANET_WIDTH / 2),
                                dy - (BMP_MPLANET_HEIGHT / 2),
                                BMP_MPLANET_WIDTH,
                                BMP_MPLANET_HEIGHT,
                                BMP_CPLANET_WIDTH,
			        BMP_CPLANET_HEIGHT,
			        0,
                                mplanetBitmapC (l),
                                planetColor (l),
                                mapw);                  
            /* Draw planet resources */
            mplanetResourcesC(l, BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT, dx, dy, mapw);
        }
        else
        {
            W_WriteBitmap (dx - (BMP_MPLANET_WIDTH / 2),
                             dy - (BMP_MPLANET_HEIGHT / 2), planetmBitmap (l),
                             planetColor (l), mapw);

        }
#ifdef BEEPLITE
	}
#endif
        if (planetHighlighting && (l->pl_info & me->p_team)) /* Draw halo */
            W_WriteCircle(mapw, dx, dy, BMP_MPLANET_WIDTH / 2,
                          l->pl_armies > 4 ? 1 : 0, 0, planetColor(l));

        if ((l->pl_flags & PLAGRI) && (l->pl_info & me->p_team))
        {
            if (agriCAPS)
            {
                agri_name[0] = (char) (toupper (l->pl_name[0]));
                agri_name[1] = (char) (toupper (l->pl_name[1]));
                agri_name[2] = (char) (toupper (l->pl_name[2]));
                W_WriteText (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                            dy + (BMP_MPLANET_HEIGHT / 2), getAgriColor (l),
                            agri_name, 3, planetFont (l));
            }
            else
            {
                W_WriteText (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                            dy + (BMP_MPLANET_HEIGHT / 2), getAgriColor (l),
                            l->pl_name, 3, planetFont (l));
            }
        }
        else
        {
            W_WriteText (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                        dy + (BMP_MPLANET_HEIGHT / 2), planetColor (l),
                        l->pl_name, 3, planetFont (l));
        }

        if (showIND && ((l->pl_info & me->p_team)
#ifdef RECORDGAME
                        || playback
#endif
            ) && (l->pl_owner == NOBODY))
        {
            W_MakeLine (mapw, dx + (BMP_MPLANET_WIDTH / 2 - 1),
                        dy + (BMP_MPLANET_HEIGHT / 2 - 1),
                        dx - (BMP_MPLANET_WIDTH / 2),
                        dy - (BMP_MPLANET_HEIGHT / 2), W_White);
            W_MakeLine (mapw, dx - (BMP_MPLANET_WIDTH / 2),
                        dy + (BMP_MPLANET_HEIGHT / 2 - 1),
                        dx + (BMP_MPLANET_WIDTH / 2 - 1),
                        dy - (BMP_MPLANET_HEIGHT / 2), W_White);
        }

        if (showPlanetOwner)
        {
            ch = (char) (((l->pl_info & me->p_team)
#ifdef RECORDGAME
                  || playback
#endif
                )? tolower (teamlet[l->pl_owner]) : '?');
            W_WriteText (mapw, dx + (BMP_MPLANET_WIDTH / 2) + 2,
                         dy - (BMP_MPLANET_HEIGHT / 2) - 6, planetColor (l), &ch, 1, planetFont (l));
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
        W_CacheLine (mapw, sl->begin_x, sl->begin_y, sl->end_x, sl->end_y, sl->color);
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
	W_WriteText (mapw, ori_offset * W_Textwidth, 1 * W_Textheight, W_Ori, 
				ori_score_line, strlen (ori_score_line), W_RegularFont);
	W_WriteText (mapw, 6 * W_Textwidth, 1 * W_Textheight, W_White, 
				":", 1, W_RegularFont);
	W_WriteText (mapw, kli_offset * W_Textwidth, 1 * W_Textheight, W_Kli, 
				kli_score_line, strlen (kli_score_line), W_RegularFont);
}
#endif /* HOCKEY_LINES */


/******************************************************************************/
/***  map()
/******************************************************************************/
inline void
map (void)
/*
 *  Update the 'galactic' map.
 */
{
    register int i;
    register struct player *j;
    register struct planet *l;
    register int dx, dy;

    static char clearlock = 0;
    static char clearlockline = 0;
    static int mclearzone[6][MAXPLAYER];
    static int clearlmark[4];
    static unsigned int lastRedraw[MAXPLAYER];

    static int viewx = 0, viewy = 0;
    static char clearviewbox = 0;
    static char viewboxcleared = 0;
    int viewdist = (WINSIDE / 2 * SCALE) / (GWIDTH / WINSIDE);
    int view = WINSIDE * SCALE / 2;
    int mvx, mvy;

#ifdef RECORDGAME
    if (doubleBuffering && !inplayback)
        W_Win2Mem (mapSDB);
#else
    if (doubleBuffering)
        W_Win2Mem (mapSDB);
#endif
    clearMap ();

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
        W_ClearWindow (mapw);

        clearlock = 0;
        clearlockline = 0;
        clearviewbox = 0;
        viewboxcleared = 1;

        for (i = 0; i < MAXPLAYER; i++)
        {
            lastRedraw[i] = 0;
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
            W_WriteTriangle (mapw, clearlmark[0], clearlmark[1],
                             clearlmark[2], clearlmark[3], backColor);
        }

        if (clearlockline)
        {
            W_MakeTractLine (mapw, maplockline[0], maplockline[1],
                             maplockline[2], maplockline[3], backColor);
            clearlockline  = 0;
        }

        if (clearviewbox)
        {
            clearviewbox = 0;
            /* clear old dots - placed here for less flicker */
            if (viewx + viewdist < WINSIDE && viewy + viewdist < WINSIDE)
                W_MakePoint (mapw, viewx + viewdist, viewy + viewdist, backColor);
            if (viewx + viewdist < WINSIDE && viewy - viewdist > 0)
                W_MakePoint (mapw, viewx + viewdist, viewy - viewdist, backColor);
            if (viewx - viewdist > 0 && viewy + viewdist < WINSIDE)
                W_MakePoint (mapw, viewx - viewdist, viewy + viewdist, backColor);
            if (viewx - viewdist > 0 && viewy - viewdist > 0)
                W_MakePoint (mapw, viewx - viewdist, viewy - viewdist, backColor);
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
        for (i = 0; i < MAXPLAYER; i++)
        {
            /* Erase the player if redrawPlayer[i] is set and there
               is an active clearzone */
            if (redrawPlayer[i])
            {
                if (mclearzone[2][i])
                {
                    W_ClearArea (mapw, mclearzone[0][i], mclearzone[1][i],
                                 mclearzone[2][i], mclearzone[3][i]);
                    /* Redraw the hole just left next update */
                    checkRedraw (mclearzone[4][i], mclearzone[5][i]);
                    mclearzone[2][i] = 0;
                }
                /* Reset the last redrawn counter */
                lastRedraw[i] = 0;
            }

            if (lastRedraw[i] == (u_int) server_ups)
            {
                /* Redraw stationary ships every update so that these
                   ships are not hidden by planet updates. */
                redrawPlayer[i] = 1;
            }
            else
            {
            	/* Just increment the counter instead */
                lastRedraw[i]++;
            }
        }
    }


    /* Draw Planets */

    DrawPlanets ();

#ifdef DEBUG_SHOW_REGIONS	/* Debugging code */
   showRegions();
#endif

    /* draw viewBox */
    if (viewBox)
    {
        if (viewboxcleared || viewx != dx || viewy != dy)
        {
            /* draw the new points */
            if (dx + viewdist < WINSIDE && dy + viewdist < WINSIDE)
                W_MakePoint (mapw, dx + viewdist, dy + viewdist, W_White);
            if (dx + viewdist < WINSIDE && dy - viewdist > 0)
                W_MakePoint (mapw, dx + viewdist, dy - viewdist, W_White);
            if (dx - viewdist > 0 && dy + viewdist < WINSIDE)
                W_MakePoint (mapw, dx - viewdist, dy + viewdist, W_White);
            if (dx - viewdist > 0 && dy - viewdist > 0)
                W_MakePoint (mapw, dx - viewdist, dy - viewdist, W_White);

            viewx = dx;         /* store the points for later */
            viewy = dy;         /* clearing */
            clearviewbox = 1;   /* and flag */
        }
    }

#ifdef HOCKEY_LINES
    if (playing_hockey)
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
        /* redrawPlayer[i] has been set to 1 if redrawall or the ship has
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
                W_WriteText (mapw, dx - (W_Textwidth / 2),
                             dy - W_Textheight / 2, unColor, cloakChars,
                             1, W_RegularFont);
            else
                W_WriteText (mapw, dx - W_Textwidth,
                             dy - W_Textheight / 2, unColor, cloakChars,
                             (cloakChars[1] == '\0' ? 1 : 2), W_RegularFont);
        }
        else
        {
            if (omitTeamLetter)
                W_WriteText (mapw, dx - (W_Textwidth / 2),
                             dy - W_Textheight / 2, playerColor (j),
                             (j->p_mapchars)+1, 1, shipFont (j));
            else
                W_WriteText (mapw, dx - W_Textwidth,
                             dy - W_Textheight / 2, playerColor (j),
                             j->p_mapchars, 2, shipFont (j));
        }

#ifdef BEEPLITE
        if ((useLite && emph_player_seq_n[i] > 0)
	  && (liteflag & LITE_PLAYERS_MAP))
	{
	    int seq_n = (emph_player_seq_n[i] * 10 / server_ups) % emph_player_seq_frames;

	    W_WriteBitmap(dx - (emph_player_seq_width / 2 - 1),
			  dy - (emph_player_seq_height / 2 + 1),
			  emph_player_seq[seq_n],
			  W_White, mapw);
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

	/* Draw range circle */
	if (viewRange && (myPlayer(j) || isObsLockPlayer(j)))
        {
          /* Orbitting any non-owned planet gets you seen,
             so don't draw the circle */
          if ((j->p_flags & PFORBIT) &&
          (planets[j->p_planet].pl_owner != j->p_team)) ;
          /* Don't draw if uncloaked and viewRange is 1 */
          else if (!(j->p_flags & PFCLOAK) && viewRange == 1) ;
          /* Don't draw if not carrying and viewRange is 2 */
          else if (j->p_armies == 0 && viewRange == 2) ;
          else
          {
            struct planet *pl;
            int pRadius;
            int planx, plany;
            int distx, disty;
            int rad;
            int color;

#ifdef BEEPLITE
            pRadius = 3 * BMP_MPLANET_WIDTH / 5;
#else
            pRadius = BMP_MPLANET_WIDTH / 2;
#endif
            if (j->p_flags & PFCLOAK)
            {
                rad = MAXDISTCLOAK;
                color = W_Red;
            }
            else
            {
                rad = MAXDISTVIS;
                color = W_White;
            }
            W_WriteCircle(mapw, dx, dy, rad, 0, 1, color);
            mclearx = dx;
            mcleary = dy;
            mclearr = rad;
            mclearccount++;
            for (pl = planets + MAXPLANETS - 1; pl >= planets; --pl)
            {
                /* Redraw check - redraw all planets in range.  Have to
                   adjust distance to account for planet radius and text
                   of planet name */
                planx = pl->pl_x * WINSIDE / GWIDTH;
                plany = pl->pl_y * WINSIDE / GWIDTH;
                distx = ABS(planx - dx) - pRadius - W_Textwidth;
                disty = ABS(plany - dy) - pRadius - W_Textheight;
                if (distx*distx + disty*disty < (rad*rad))
                    pl->pl_flags |= PLREDRAW;
            }
          }
        }
    }

    /* Draw weapons */
    if (weaponsOnMap)
    {
        register int h;
        register struct phaser *ph;
        register struct torp *k;
        register struct plasmatorp *pt;
        int tx, ty;

        for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++)
        {
            if (j->p_status == PFREE)
                continue;

            dx = j->p_x * WINSIDE / GWIDTH;
            dy = j->p_y * WINSIDE / GWIDTH;
            
            /* phasers */
            ph = &phasers[j->p_no];
            if (ph->ph_status != PHFREE &&
               (j->p_status == PALIVE || j->p_status == PEXPLODE || j->p_status == PDEAD) &&
               !(j->p_x < 0 || j->p_x >= GWIDTH))
            {
                switch(ph->ph_status)
                {
                    case PHMISS:
                        /* Here I will have to compute end coordinate */
                        tx = (int) (j->p_x + PHASEDIST * j->p_ship.s_phaserdamage / 100
                            * Cos[ph->ph_dir]) * WINSIDE / GWIDTH;
                        ty = (int) (j->p_y + PHASEDIST * j->p_ship.s_phaserdamage / 100
                            * Sin[ph->ph_dir]) * WINSIDE / GWIDTH;
                        break;
                    case PHHIT2:
                        tx = ph->ph_x * WINSIDE / GWIDTH;
                        ty = ph->ph_y * WINSIDE / GWIDTH;
                        break;
                    default:
                        /* Don't draw phasers to ships out of galactic bounds */
                        if (players[ph->ph_target].p_x < 0 || players[ph->ph_target].p_y < 0)
                        {
                            tx = dx;
                            ty = dy;
                        }
                        else
                        {
                            tx = players[ph->ph_target].p_x * WINSIDE / GWIDTH;
                            ty = players[ph->ph_target].p_y * WINSIDE / GWIDTH;
                        }
                        break;
                }
                W_MakeLine(mapw, dx, dy, tx, ty, phaserColor(ph));
                mclearline[0][mclearlcount] = dx;
                mclearline[1][mclearlcount] = dy;
                mclearline[2][mclearlcount] = tx;
                mclearline[3][mclearlcount] = ty;
                mclearlcount++;
                /* Check for overwriting planets, corrected from view scale*/
                checkRedraw(j->p_x, j->p_y);
                checkRedraw(tx * (GWIDTH / WINSIDE), ty * (GWIDTH / WINSIDE));
            }

            if (!j->p_ntorp && !j->p_nplasmatorp)
                continue;

            /* torps */
            for (h = 0, k = &torps[MAXTORP * i + h]; h < MAXTORP; h++, k++)
            {
                if (!k->t_status)
                    continue;
                if (k->t_x < 0 || k->t_y < 0)
                    continue;
                dx = k->t_x * WINSIDE / GWIDTH;
                dy = k->t_y * WINSIDE / GWIDTH;

                if (k->t_status == TEXPLODE)
                {
                    /* Use frame 0 (smallest) to limit size of torp explosions 
                       on the galactic */
                    W_WriteBitmap (dx - (BMP_TORPDET_WIDTH / 2), dy - (BMP_TORPDET_HEIGHT / 2),
                                   cloud[0], torpColor (k), mapw);
                    mcleararea[0][mclearacount] = dx - (BMP_TORPDET_WIDTH / 2);
                    mcleararea[1][mclearacount] = dy - (BMP_TORPDET_HEIGHT / 2);
                    mcleararea[2][mclearacount] = BMP_TORPDET_WIDTH;
                    mcleararea[3][mclearacount] = BMP_TORPDET_HEIGHT;
                    mclearacount++;
                }
                else
                {
                    W_MakePoint(mapw, dx, dy, torpColor (k));
                    mclearpoint[0][mclearpcount] = dx;
                    mclearpoint[1][mclearpcount] = dy;
                    mclearpcount++;
                }
                /* Check for overwriting planets */
                checkRedraw(k->t_x, k->t_y);
            }

            /* plasmas */
            for (h = 0, pt = &plasmatorps[MAXPLASMA * i + h]; h < MAXPLASMA; h++, pt++)
            {
                if (!pt->pt_status)
                    continue;
                if (pt->pt_x < 0 || pt->pt_y < 0)
                    continue;
                dx = pt->pt_x * WINSIDE / GWIDTH;
                dy = pt->pt_y * WINSIDE / GWIDTH;

                if (pt->pt_status == TEXPLODE)
                {
                    /* Use frame 0 (smallest) of the TORP animations to limit size of plasma
                       explosions galactic.  Plasma explosion bitmaps are just too big */
                    W_WriteBitmap (dx - (BMP_TORPDET_WIDTH / 2), dy - (BMP_TORPDET_HEIGHT / 2),
                                   cloud[0], plasmatorpColor (pt), mapw);
                    mcleararea[0][mclearacount] = dx - (BMP_TORPDET_WIDTH / 2);
                    mcleararea[1][mclearacount] = dy - (BMP_TORPDET_HEIGHT / 2);
                    mcleararea[2][mclearacount] = BMP_TORPDET_WIDTH;
                    mcleararea[3][mclearacount] = BMP_TORPDET_HEIGHT;
                    mclearacount++;
                }
                else
                {
                    /* Draw plasmas as a 2x2 pixel torp */
                    W_MakeLine(mapw, dx, dy, dx + 1, dy, plasmatorpColor (pt));
                    W_MakeLine(mapw, dx, dy + 1, dx + 1, dy + 1, plasmatorpColor (pt));
                    mcleararea[0][mclearacount] = dx;
                    mcleararea[1][mclearacount] = dy;
                    mcleararea[2][mclearacount] = 2;
                    mcleararea[3][mclearacount] = 2;
                    mclearacount++;
                }
                /* Check for overwriting planets */
                checkRedraw(pt->pt_x, pt->pt_y);
            }
        }
    }

    /* Draw the lock symbol (if needed) */

    if ((me->p_flags & PFPLOCK) && (showLock & 1))
    {
        j = &players[me->p_playerl];

        dx = j->p_x * WINSIDE / GWIDTH;
        dy = j->p_y * WINSIDE / GWIDTH;

        if (j->p_status == PALIVE && !(j->p_flags & PFCLOAK))
        {
            W_WriteTriangle (mapw, dx, dy + 6, 4, 1, foreColor);

            clearlmark[0] = dx;
            clearlmark[1] = dy + 6;
            clearlmark[2] = 4;
            clearlmark[3] = 1;
            clearlock = 1;
        }
        if (lockLine && (dx <= view && dx >= 0 && dy <= view && dy >= 0))
        {
            int mydx, mydy;
            
            mydx = me->p_x * WINSIDE / GWIDTH;
            mydy = me->p_y * WINSIDE / GWIDTH;
            W_MakeTractLine (mapw,
                             mydx,
                             mydy,
                             dx, dy, W_Green);
            maplockline[0] = mydx;
            maplockline[1] = mydy;
            maplockline[2] = dx;
            maplockline[3] = dy;
            clearlockline = 1;
            /* Check 10 points in between start and end of line for planet overlap */
            for (i = 0; i < 10; i++)
                checkRedraw(me->p_x + i * (j->p_x - me->p_x)/10,
                            me->p_y + i * (j->p_y - me->p_y)/10);
        }
    }
    else if ((me->p_flags & PFPLLOCK) && (showLock & 1))
    {
        struct planet *l = &planets[me->p_planet];

        dx = l->pl_x * WINSIDE / GWIDTH;
        dy = l->pl_y * WINSIDE / GWIDTH;
        W_WriteTriangle (mapw, dx, dy - (BMP_MPLANET_HEIGHT) / 2 - 4, 4, 0, foreColor);

        clearlmark[0] = dx;
        clearlmark[1] = dy - (BMP_MPLANET_HEIGHT) / 2 - 4;
        clearlmark[2] = 4;
        clearlmark[3] = 0;
        clearlock = 1;
        
        if (lockLine)
        {
            int mydx, mydy;
            
            mydx = me->p_x * WINSIDE / GWIDTH;
            mydy = me->p_y * WINSIDE / GWIDTH;
            W_MakeTractLine (mapw,
                             mydx,
                             mydy,
                             dx, dy, W_Green);
            maplockline[0] = mydx;
            maplockline[1] = mydy;
            maplockline[2] = dx;
            maplockline[3] = dy;
            clearlockline = 1;
            /* Check 10 points in between start and end of line for planet overlap */
            for (i = 0; i < 10; i++)
                checkRedraw(me->p_x + i * (l->pl_x - me->p_x)/10,
                            me->p_y + i * (l->pl_y - me->p_y)/10);
        }
    }

#ifdef RECORDGAME
    if (doubleBuffering && !inplayback)
        W_Mem2Win (mapSDB);
#else
    if (doubleBuffering)
        W_Mem2Win (mapSDB);
#endif
}


inline void
clearMap (void)
/*
   Clear the weapons fire on the galactic map (intelligently rather than
   just simply wiping the map).  Similar to clearLocal().
*/
{
    int i;
    
    W_ClearAreas (mapw, mcleararea[0], mcleararea[1], mcleararea[2],
                      mcleararea[3], mclearacount);
    mclearacount = 0;

    for (i = 0; i < mclearpcount; i++)
        W_MakePoint (mapw, mclearpoint[0][i], mclearpoint[1][i], backColor);
    mclearpcount = 0;

    W_MakeLines (mapw, mclearline[0], mclearline[1], mclearline[2],
                     mclearline[3], mclearlcount, backColor);
    mclearlcount = 0;
    
    if (mclearccount)
    {
        W_WriteCircle(mapw, mclearx, mcleary, mclearr, 0, 0, backColor);
        mclearccount--;
    }
}

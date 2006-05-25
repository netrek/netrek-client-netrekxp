/******************************************************************************/
/***  File:  local.c
/***
/***  Function: Functions to maintain the local map.
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/


#include "config.h"
#include "copyright2.h"

#include <stdio.h>
#include <math.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "local.h"
#include "proto.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "bitmaps.h"

/* Local Variables */
#define XPI     3.1415926
static int clearcount = 0;
static int clearzone[4][(MAXTORP + 1) * MAXPLAYER +
                        (MAXPLASMA + 1) * MAXPLAYER + MAXPLANETS];
static int clearlcount = 0;
#ifdef HOCKEY_LINES
static int clearline[4][MAXPLAYER + 2 * MAXPLAYER + NUM_HOCKEY_LINES];
#else
static int clearline[4][MAXPLAYER + 2 * MAXPLAYER];
#endif

#ifdef SOUND
static int sound_phaser = 0;
static int sound_other_phaser = 0;
static int sound_torps = 0;
static int sound_other_torps = 0;
static int num_other_torps = 0;
static int sound_plasma = 0;
static int sound_other_plasmas = 0;
static int num_other_plasmas = 0;
static unsigned int sound_flags = 0;
static int other_torp_dist = 0;
static int new_other_torp_dist = 0;
static int other_torp_angle = 0;
static int other_plasma_dist = 0;
static int new_other_plasma_dist = 0;
static int other_plasma_angle = 0;
static int warpchange = 0;
static unsigned int twarpflag = 0; 
#endif

/* Background Stars Definitions */
struct _star {
    int s_x, s_y;
    int s_color;
};

static void redrawStarSector(int sectorx, int sectory);

#define scaleLocal(pt)           ((pt)/SCALE + (WINSIDE/2))

int fullview = WINSIDE * SCALE;
int view = WINSIDE * SCALE / 2;

static struct _star stars[10][10][16];

/* Function Defininitions */

int
randcolor()
{
    switch (RANDOM() % 10) {
    case 0:
        return W_Yellow;
    case 1:
        return W_Red;
    case 2:
        return W_Green;
    case 3:
        return W_Cyan;
    default:
        return W_White;
    }
}

void
initStars()
{
    register int i, j, k;

    for (i = 0; i < 10; i++) {
       for (j = 0; j < 10; j++) {
            for (k = 0; k < 16; k++) {
                stars[i][j][k].s_x = RANDOM() % 20000;
                stars[i][j][k].s_y = RANDOM() % 20000;
                stars[i][j][k].s_color = randcolor();
            }
        }
    }
}


void
DrawStars()
{
    /*
       note: cpp symbols in expressions (WINSIDE*SCALE) will be precalculated
       by any C optimizer
    */ 
    int sectorx = me->p_x / (fullview);
    int sectory = me->p_y / (fullview);
    int sector_offx = me->p_x - sectorx * (fullview);
    int sector_offy = me->p_y - sectory * (fullview);
    int l = 0, r = 0, t = 0, b = 0;

    if (sector_offx < 0)
    {  /* goddamn rounding towards 0 */
        sectorx--;
        sector_offx += fullview; 
    }
    if (sector_offy < 0) 
    {  /* goddamn rounding towards 0 */
        sectory--;
        sector_offy += fullview; 
    }   

#define MAXSECTOR   (GWIDTH/(fullview))
  
    /* at worst we have to redraw 4 star sectors */

    /* draw the one we're in */
    /*
       check first to make sure it's valid.  This is mainly important for if
       it tries to redraw and we're already dead
    */
    if (sectorx < 0 || sectory < 0)
        return;

    l = sector_offx < view && sectorx > 0;
    r = sector_offx > view && sectorx + 1 < MAXSECTOR;
    t = sector_offy < view && sectory > 0;
    b = sector_offy > view && sectory + 1 < MAXSECTOR;

    if (t) 
    {
        if (l)
            /* redraw upper-left sector */
            redrawStarSector (sectorx-1, sectory-1);

        /* redraw upper sector */
        redrawStarSector (sectorx, sectory-1);

        if (r)          
            /* redraw upper-right sector */
            redrawStarSector (sectorx+1, sectory-1);
    }
    if (l)          
        /* redraw left sector */
        redrawStarSector (sectorx - 1, sectory);

    /* redraw center sector */
    redrawStarSector (sectorx, sectory);

    if (r)          
        /* redraw right sector */
        redrawStarSector (sectorx + 1, sectory);

    if (b) 
    {
        if (l)          
            /* redraw bottom-left sector */
            redrawStarSector (sectorx - 1, sectory + 1);

        /* redraw bottom sector */
        redrawStarSector (sectorx, sectory + 1);

        if (r)          
            /* redraw bottom-right sector */
            redrawStarSector (sectorx + 1, sectory + 1);
    }
}


static void
redrawStarSector (int sectorx, int sectory)
{
    register int i, dx, dy, dxx, dyy;
    register int xbase = sectorx * fullview;
    register int ybase = sectory * fullview;
    register struct _star *s;
    static unsigned int warpflag = 0;    /* assume starting out not in warp */
    static int streaksOn = 0, lastspeed = 0, lastsubspeed = 0, updates = 0;
    static int streaklength = 1;
    static int mydir = -1;
    struct _star *star_sector = stars[sectorx][sectory];

    if (warpStreaks)
    {
        if (warpflag != (me->p_flags & PFTWARP))
        {   /* change in warp state */
            streaksOn = 1;
            warpflag = (me->p_flags & PFTWARP);
        }

        if (warpflag)
            mydir = me->p_dir;

        if (streaksOn)
        {
            if (warpflag && (me->p_speed < lastspeed ||
                (me->p_speed == lastspeed && me->p_subspeed <= lastsubspeed)))
            {
                /* finished accelerating */
                updates++;
                if (updates > 5)
                {
                    lastspeed = me->p_speed;
                    lastsubspeed = me->p_subspeed;
                    updates = 0;
                    streaksOn = 0;
                    redrawStarSector (sectorx, sectory);
                    return;
                }
            }
            else if (streaklength == 1 || (!warpflag && ((me->p_speed > lastspeed) ||
                    (me->p_speed == lastspeed && me->p_subspeed >= lastsubspeed))))
            {
                /* finished decelerating */
                updates++;
                if (updates > 5)
                {
                    lastspeed = me->p_speed;
                    lastsubspeed = me->p_subspeed;
                    updates = 0;
                    streaksOn = 0;
                    streaklength = 1;
                    redrawStarSector (sectorx, sectory);
                    return;
                }
            }
            else
                updates = 0;
            lastspeed = me->p_speed;
            lastsubspeed = me->p_subspeed;
            /* draw the streaks */
            if (warpflag)
                streaklength += 3;
            else
                streaklength--;
            dxx = (int) (Cos[mydir] * streaklength);
            dyy = (int) (Sin[mydir] * streaklength);
            for (i = 0, s = star_sector; i < 16; i++, s++)
            {
                dx = (s->s_x + xbase) - (me->p_x - (me->p_x % SCALE));
                dy = (s->s_y + ybase) - (me->p_y - (me->p_y % SCALE));
                if (ABS(dx) > (view) || ABS(dy) > (view))
                    continue;

                dx = scaleLocal (dx);
                dy = scaleLocal (dy);
                W_CacheLine (w, dx, dy, dx - dxx, dy - dyy, s->s_color);

                clearline[0][clearlcount] = dx;
                clearline[1][clearlcount] = dy;
                clearline[2][clearlcount] = dx - dxx;
                clearline[3][clearlcount] = dy - dyy;
                clearlcount++;
            }
            return;
        }
    }
    for (i = 0, s = star_sector; i < 16; i++, s++)
    {
        dx = (s->s_x + xbase) - (me->p_x - (me->p_x % SCALE));
        dy = (s->s_y + ybase) - (me->p_y - (me->p_y % SCALE));
        if (ABS(dx) > (view) || ABS(dy) > (view))
            continue;

        dx = scaleLocal (dx);
        dy = scaleLocal (dy);
        W_CachePoint (w, dx, dy, s->s_color);

        clearline[0][clearlcount] = dx;
        clearline[1][clearlcount] = dy;
        clearline[2][clearlcount] = dx;
        clearline[3][clearlcount] = dy;
        clearlcount++;
    }
}


#define NORMALIZE(d) (((d) + 256) % 256)


static inline W_Icon
getPlanetBitmap (register struct planet *p)
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
        return (bplanets[i]);
    }
    else
    {
        return (bplanets[8]);
    }
}

/******************************************************************************/
/***  planetBitmapC()
/******************************************************************************/
static inline W_Icon
planetBitmapC (register struct planet *p)
/*
 *  Choose the color bitmap for a planet.
 */
{
    int i;
    W_Icon (*planet_bits);
    
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
    	        planet_bits = planet_earth;
    	    else if (!strcmp(p->pl_name, "Klingus"))
    	        planet_bits = planet_klingus;
    	    else if (!strcmp(p->pl_name, "Romulus"))
    	        planet_bits = planet_romulus;
    	    else if (!strcmp(p->pl_name, "Orion"))
    	        planet_bits = planet_orion;
    	    else // This should never be called
    	        planet_bits = planet_rock1;
    	}
    	else if (p->pl_flags & PLCORE) // Not functional yet due to server
    	{
    	    if (p->pl_flags & PLAGRI)
    	        planet_bits = planet_agri1;
    	    else
    	        planet_bits = planet_rock1;
    	}
    	else if (p->pl_flags & PLAGRI)
            planet_bits = planet_agri2;
        else
            planet_bits = planet_rock2;

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
        return planet_bits[i];
    }
    else
    {
        return planet_unknown;
    }
}

/******************************************************************************/
/***  planetResourcesC()
/******************************************************************************/		           
static inline void
planetResourcesC (register struct planet *p, int destwidth, int destheight,
                  int dx, int dy, W_Window window)
/*
 *  Draw the resources for a colorized planet.
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
            W_WriteScaleBitmap(dx - 7 * destwidth / 8,
                               dy - (destheight / 2),
                               destwidth/3,
                               destheight,
                               BMP_ARMY_WIDTH,
                               BMP_ARMY_HEIGHT,
                               0,
                               army_bitmap, planetColor(p),
                               window);       
        if (p->pl_flags & PLREPAIR)
            W_WriteScaleBitmap(dx - (destwidth / 2),
                               dy - (5 * destheight / 6),
                               destwidth,
                               destheight/3,
                               BMP_WRENCH_WIDTH,
                               BMP_WRENCH_HEIGHT,
                               0,
                               wrench_bitmap, planetColor(p),
                               window);
        if (p->pl_flags & PLFUEL)
            W_WriteScaleBitmap(dx + 3 * destwidth / 5,
                               dy - (destheight / 2),
                               destwidth/3,
                               destheight,
                               BMP_FUEL_WIDTH,
                               BMP_FUEL_HEIGHT,
                               0,
                               fuel_bitmap, planetColor(p),
                               window);
    }
    return;
}

static void
DrawPlanets (void)
/*
   Draw the planets on the local map.
*/
{
    register int dx, dy;
    register struct planet *l;
    const int view = SCALE * WINSIDE / 2 + BMP_PLANET_WIDTH * SCALE / 2;

    for (l = planets + MAXPLANETS - 1; l >= planets; --l)
    {
        dx = l->pl_x - me->p_x;
        dy = l->pl_y - me->p_y;
        if (dx > view || dx < -view || dy > view || dy < -view)
            continue;

        dx = dx / SCALE + WINSIDE / 2;
        dy = dy / SCALE + WINSIDE / 2;
        
        if (planetBitmap == 3)
        {
            W_WriteScaleBitmap (dx - (BMP_PLANET_WIDTH / 2),
                                dy - (BMP_PLANET_HEIGHT / 2),
                                BMP_PLANET_WIDTH,
                                BMP_PLANET_HEIGHT,
                                BMP_CPLANET_WIDTH,
			        BMP_CPLANET_HEIGHT,
			        0,
                                planetBitmapC (l),
                                planetColor (l),
                                w);
             /* Draw planet resources */
             planetResourcesC(l, BMP_PLANET_WIDTH, BMP_PLANET_HEIGHT, dx, dy, w);
        }
        else
        {
            W_WriteBitmap (dx - (BMP_PLANET_WIDTH / 2),
                           dy - (BMP_PLANET_HEIGHT / 2), getPlanetBitmap (l),
                           planetColor (l), w);
        }
        if (showIND && ((l->pl_info & me->p_team)
#ifdef RECORDGAME
                        || playback
#endif
            ) && (l->pl_owner == NOBODY))
        {
            W_CacheLine (w, dx - (BMP_PLANET_WIDTH / 2),
                         dy - (BMP_PLANET_HEIGHT / 2),
                         dx + (BMP_PLANET_WIDTH / 2 - 1),
                         dy + (BMP_PLANET_HEIGHT / 2 - 1), W_White);
            W_CacheLine (w, dx + (BMP_PLANET_WIDTH / 2 - 1),
                         dy - (BMP_PLANET_HEIGHT / 2),
                         dx - (BMP_PLANET_WIDTH / 2),
                         dy + (BMP_PLANET_HEIGHT / 2 - 1), W_White);
        }

        if (showPlanetNames)
        {
            W_MaskText (w, dx - (BMP_PLANET_WIDTH / 2),
                        dy + (BMP_PLANET_HEIGHT / 2), planetColor (l),
                        l->pl_name, l->pl_namelen, planetFont (l));
            clearzone[0][clearcount] = dx - (BMP_PLANET_WIDTH / 2);
            clearzone[1][clearcount] = dy + (BMP_PLANET_HEIGHT / 2);
            clearzone[2][clearcount] = W_Textwidth * l->pl_namelen;
            clearzone[3][clearcount] = W_Textheight;
            clearcount++;
        }
        
        if (showArmy && (me->p_flags & PFORBIT)
        && (me->p_planet = get_closest_planet(me->p_x, me->p_y)) == l->pl_no)
        {
            char armbuf[4];
            int armbuflen;
            
            if (l->pl_armies < 10)
            {
                armbuf[0] = (char) (l->pl_armies + '0');
                armbuf[1] = '\0';
                armbuflen = 2;
            }
            else if (l->pl_armies < 100)
            {
                armbuf[0] = (char) (l->pl_armies / 10 + '0');
                armbuf[1] = (char) (l->pl_armies % 10 + '0');
                armbuf[2] = '\0';
                armbuflen = 3;
            }
            else
            {
                armbuf[0] = (char) (l->pl_armies / 100 + '0');
                armbuf[1] = (char) (l->pl_armies / 10 + '0');
                armbuf[2] = (char) (l->pl_armies % 10 + '0');
                armbuf[3] = '\0';
                armbuflen = 4;
            }
            W_MaskText (w, dx - (7 * BMP_PLANET_WIDTH / 8),
                        dy - (5 * BMP_PLANET_HEIGHT / 6), planetColor (l),
                        armbuf, armbuflen, planetFont (l));
            clearzone[0][clearcount] = dx - (7 * BMP_PLANET_WIDTH / 8);
            clearzone[1][clearcount] = dy - (5 * BMP_PLANET_HEIGHT / 6);
            clearzone[2][clearcount] = W_Textwidth * armbuflen;
            clearzone[3][clearcount] = W_Textheight;
            clearcount++;
        }
        if (planetBitmap == 3) // Needs adjusting
        {
            clearzone[0][clearcount] = dx - (7 * BMP_PLANET_WIDTH / 8);
            clearzone[1][clearcount] = dy - (5 * BMP_PLANET_HEIGHT / 6);
            clearzone[2][clearcount] = 7 * BMP_PLANET_WIDTH / 4 + 1;
            clearzone[3][clearcount] = 4 * BMP_PLANET_HEIGHT / 3 + 1;
        }
        else
        {
            clearzone[0][clearcount] = dx - (BMP_PLANET_WIDTH / 2);
            clearzone[1][clearcount] = dy - (BMP_PLANET_HEIGHT / 2);
            clearzone[2][clearcount] = BMP_PLANET_WIDTH;
            clearzone[3][clearcount] = BMP_PLANET_HEIGHT;
        }
        clearcount++;
    }
}


static void
get_shrink_phaser_coords(int *rx, int *ry, int dx, int dy, int tx,
                         int ty, int curr_fuse, int max_fuse)
{
   register int ph_dir, range, phaser_shrink;

   /* no negative lines */
   if (curr_fuse > max_fuse)
      curr_fuse = max_fuse;

   /*
    * Calculate direction (we don't get this from the server each tick)
    */
   ph_dir = (int) rint(atan2((double) (tx - dx),
                 (double) (dy - ty)) / 3.14159 * 128.);

   /*
    * Calculate current drawing range of phaser based on distance and phaser
    * fuse
    */
   phaser_shrink = 16 - phaserShrink;
   if (phaser_shrink < 5)
       phaser_shrink = 5;

   range = (int) (5 * curr_fuse * hypot((double) dx - tx, (double) dy - ty)) /
      (phaser_shrink * max_fuse);

   /* Calculate phaser origin */
   *rx = (int) (dx + range * Cos[(unsigned char) ph_dir]);
   *ry = (int) (dy + range * Sin[(unsigned char) ph_dir]);

}

static void
DrawShips (void)
{
    register struct player *j;
    register struct phaser *php;

    char idbuf[10];
    int buflen = 1;
    static int ph_col = 0;
    const int view = SCALE * WINSIDE / 2 + BMP_SHIELD_WIDTH * SCALE / 2;
    int dx, dy, px, py, wx, wy, tx, ty, lx, ly;
    int new_dx, new_dy;
    int newdx, newdy, distance, angle; /* For newSound */

    W_Icon (*ship_bits)[SHIP_VIEWS];
    W_Icon (*ship_bitsHR);

    /* Kludge to try to fix missing ID chars on tactical (short range)
       display. */

    idbuf[0] = '0';
    idbuf[1] = '\0';

    for (j = players + MAXPLAYER - 1; j >= players; --j)
    {
        if ((j->p_status != PALIVE) && (j->p_status != PEXPLODE))
            continue;

/* Twarp sounds put up here so observers can hear them */            
#ifdef SOUND

       /* Have to use me->p_flags because server doesn't send us twarp flag info
        on other players.  Current Vanilla does not let observer change players
        if the player they are observing is transwarping. */
        if (twarpflag != (me->p_flags & PFTWARP))
        {
            /* change in warp state */
            warpchange = 1;
            twarpflag = (me->p_flags & PFTWARP);
        }
        
        if (myPlayer(j) || isObsLockPlayer(j))
        {
            if (warpchange && (j->p_flags & PFTWARP))
            {
            	if (newSound) // Kill any channels with ENTER_WARP_WAV or EXIT_WARP_WAV (group 3)
	    	{
	    	    Mix_HaltGroup(3);
	            Play_Sound(ENTER_WARP_WAV);
	        }
	        else
	            Play_Sound(ENTER_WARP_SOUND);
	                
	        warpchange = 0;
	    }
	    if (warpchange && !(j->p_flags & PFTWARP))
	    {
	    	if (newSound) // Kill any channels with ENTER_WARP_WAV or EXIT_WARP_WAV (group 3)
	    	{
	    	    Mix_HaltGroup(3);
	    	    Play_Sound(EXIT_WARP_WAV);
	    	}
	        else
	            Play_Sound(EXIT_WARP_SOUND);
	                
	            warpchange = 0;
	    }
	}
#endif

        if (j->p_flags & PFOBSERV)
        {
            /* observer and NOT locked onto a player (ie. locked onto planet or
             * vacuum) */

            if (!(j->p_flags & PFPLOCK))
                continue;

            /* observer and NOT cloaked - don't display ship but do tractors
             * phasers and torps are done for that ship already */

            if (!(j->p_flags & PFCLOAK))
                continue;
        }
        
        dx = j->p_x - me->p_x;
        dy = j->p_y - me->p_y;
        if (dx > view || dx < -view || dy > view || dy < -view)
            continue;

        dx = dx / SCALE + WINSIDE / 2;
        dy = dy / SCALE + WINSIDE / 2;
        
        if (j->p_flags & PFCLOAK)
        {
            if (j->p_cloakphase < (CLOAK_PHASES - 1))
            {

#ifdef SOUND
                if (j->p_cloakphase == 0)
                {
                    // To avoid hearing twarp cloak sounds as the twarper/observer
                    if ( (myPlayer(j) || isObsLockPlayer(j)) ? ((me->p_flags & PFTWARP) ? 0 : 1) : 1 )
                    {
                        if (newSound)
                        {               
                            newdx = dx - WINSIDE/2;
                            newdy = dy - WINSIDE/2;
                        
                            distance = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                            // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                            // which would be a diagonal from player to corner of tactical
                            // Reason for normalization is Mix_SetDistance requires that range        
                            distance = (int)((255 * distance)/CORNER_DIST);
                            // Calculate angle, then adjust as necessary for Mix_SetDistance
                            angle = (int)(atan2(newdy, newdx)*180/XPI);
                            angle = 270 - angle;
                            // At short distances, don't use angular sound
                            if (distance < SCALE/2)
                                Play_Sound(CLOAKED_WAV);
                            else
                                Play_Sound_Loc(CLOAKED_WAV, angle, distance);
                        }
                        else
                        {
                            if (myPlayer(j) || isObsLockPlayer(j))
                                Play_Sound(CLOAK_SOUND);
                        }
                    }
                }
#endif

                j->p_cloakphase++;
            }
        }
        else
        {
            if (j->p_cloakphase)
            {

#ifdef SOUND
                // To avoid twarp cloak sounds as the twarper/observer
                if ( (myPlayer(j) || isObsLockPlayer(j)) ? ((me->p_flags & PFTWARP) ? 0 : 1) : 1 )
                {
                    if (newSound)
                    {
                        if (j->p_cloakphase == CLOAK_PHASES - 1)
                            {
                            newdx = dx - WINSIDE/2;
                            newdy = dy - WINSIDE/2;
                        
                            distance = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                            // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                            // which would be a diagonal from player to corner of tactical
                            // Reason for normalization is Mix_SetDistance requires that range        
                            distance = (int)((255 * distance)/CORNER_DIST);
                            // Calculate angle, then adjust as necessary for Mix_SetDistance
                            angle = (int)(atan2(newdy, newdx)*180/XPI);
                            angle = 270 - angle;
                             // At short distances, don't use angular sound
                            if (distance < SCALE/2)
                                Play_Sound(UNCLOAK_WAV);
                            else
                                Play_Sound_Loc(UNCLOAK_WAV, angle, distance);
                        }
                        else    // Kill any channels with CLOAKED_WAV on them (group 1)
                            Mix_HaltGroup(1);
                    }
                    else
                    {
                        if (myPlayer(j) || isObsLockPlayer(j))
                        {
                            if (j->p_cloakphase == CLOAK_PHASES - 1)
                                Play_Sound(UNCLOAK_SOUND);
                            else
                                Abort_Sound(CLOAK_SOUND);
                        }
                    }
                }            
#endif

                j->p_cloakphase--;
            }
        }

        if (j->p_flags & PFCLOAK && (j->p_cloakphase == (CLOAK_PHASES - 1)))
        {
            if (myPlayer (j)
#ifdef RECORDGAME
                || playback
#endif
                )
            {
                W_WriteBitmap (dx - (cloak_width / 2),
                               dy - (cloak_height / 2), cloakicon, myColor, w);
                clearzone[0][clearcount] = dx - (shield_width / 2);
                clearzone[1][clearcount] = dy - (shield_height / 2);
                clearzone[2][clearcount] = shield_width;
                clearzone[3][clearcount] = shield_height;
                clearcount++;

                goto shieldlabel;       /* draw the shield even when
                                         * cloaked */
            }
            continue;
        }
        if (j->p_status == PALIVE)
        {
#if defined (BEEPLITE)
	    clearzone[0][clearcount] = dx - (shield_width / 2 + 6);
	    clearzone[1][clearcount] = dy - (shield_height / 2 + 6);
	    clearzone[2][clearcount] = shield_width + 12;
	    clearzone[3][clearcount] = shield_height + 12;
	    clearcount++;
#else
	    clearzone[0][clearcount] = dx - (shield_width / 2);
	    clearzone[1][clearcount] = dy - (shield_height / 2);
	    clearzone[2][clearcount] = shield_width;
	    clearzone[3][clearcount] = shield_height;
	    clearcount++;
#endif

	/* Logic of color scheme is as follows:
	   1) Mono bitmaps (colorClient 0) and new bitmaps (colorClient 1)
	      and high-res bitmaps (colorClient 4) are the same for both player
	      and everyone else.
	   2) Single color bitmaps (colorClient 2) and shaded single color
	      bitmaps (colorClient 3) have a monochrome version (bitmap set
	      G) that will be used for the player's ship, to set it apart in
	      color from the rest of the team
	 */
	    if (colorClient <= 0 || colorClient > 4)
            {
            	switch (j->p_team)
            	{
                case FED:
                    ship_bits = fed_bitmapsM;
                    break;
                case ROM:
                    ship_bits = rom_bitmapsM;
                    break;
                case KLI:
                    ship_bits = kli_bitmapsM;
                    break;
                case ORI:
                    ship_bits = ori_bitmapsM;
                    break;
                default:
                    ship_bits = ind_bitmapsM;
                    break;
           	}
            }
            else if (colorClient == 1)
            {
                switch (j->p_team)
                {
                case FED:
                    ship_bits = fed_bitmaps1;
                    break;
                case ROM:
                    ship_bits = rom_bitmaps1;
                    break;
                case KLI:
                    ship_bits = kli_bitmaps1;
                    break;
                case ORI:
                    ship_bits = ori_bitmaps1;
                    break;
                default:
                    ship_bits = ind_bitmaps1;
                    break;
                }
            }
            else if (colorClient == 2 && !myPlayer(j))
            {
                switch (j->p_team)
                {
                case FED:
                    ship_bits = fed_bitmaps;
                    break;
                case ROM:
                    ship_bits = rom_bitmaps;
                    break;
                case KLI:
                    ship_bits = kli_bitmaps;
                    break;
                case ORI:
                    ship_bits = ori_bitmaps;
                    break;
                default:
                    ship_bits = ind_bitmaps;
                    break;
                }
            }
            else if (colorClient == 3 && !myPlayer(j))
            {
                switch (j->p_team)
                {
                case FED:
                    ship_bits = fed_bitmapsT;
                    break;
                case ROM:
                    ship_bits = rom_bitmapsT;
                    break;
                case KLI:
                    ship_bits = kli_bitmapsT;
                    break;
                case ORI:
                    ship_bits = ori_bitmapsT;
                    break;
                default:
                    ship_bits = ind_bitmapsT;
                    break;
                }
            }
            else if (colorClient == 4)
            {
                switch (j->p_team)
                {
                case FED:
                    ship_bitsHR = fed_bitmapsHR;
                    break;
                case ROM:
                    ship_bitsHR = rom_bitmapsHR;
                    break;
                case KLI:
                    ship_bitsHR = kli_bitmapsHR;
                    break;
                case ORI:
                    ship_bitsHR = ori_bitmapsHR;
                    break;
                default:
                    ship_bitsHR = ind_bitmapsHR;
                    break;
                }
            }
            else /* Default to bitmap set G (greyscale) for player's ship */
            {
            	switch (j->p_team)
                {
                case FED:
                    ship_bits = fed_bitmapsG;
                    break;
                case ROM:
                    ship_bits = rom_bitmapsG;
                    break;
                case KLI:
                    ship_bits = kli_bitmapsG;
                    break;
                case ORI:
                    ship_bits = ori_bitmapsG;
                    break;
                default:
                    ship_bits = ind_bitmapsG;
                    break;
                }
            }
            
            if (colorClient != 4)
            {
                W_WriteBitmap (dx - (j->p_ship.s_width / 2),
                               dy - (j->p_ship.s_height / 2),
                               ship_bits[j->p_ship.s_type][rosette (j->p_dir)],
                               playerColor (j), w);
            }
            else
            {
                W_WriteScaleBitmap (dx - (j->p_ship.s_width / 2),
                                    dy - (j->p_ship.s_height / 2),
                                    j->p_ship.s_width,
                                    j->p_ship.s_height,
                                    BMP_SHIP_WIDTH_HR,
                                    BMP_SHIP_HEIGHT_HR,
                                    (360 * j->p_dir/255), // Converted to angle
                                    ship_bitsHR[j->p_ship.s_type],
                                    playerColor (j),
                                    w);
            }

            if (j->p_cloakphase > 0)
            {
                W_WriteBitmap (dx - (cloak_width / 2),
                               dy - (cloak_height / 2), cloakicon,
                               playerColor (j), w);
                if (!myPlayer (j))      /* if myplayer draw the
                                         * shield */
                    continue;
            }

          shieldlabel:

#ifdef BEEPLITE
	    if ((useLite && emph_player_seq_n[j->p_no] > 0)
	      && (liteflag & LITE_PLAYERS_LOCAL))
	    {
	        int     seq_n = emph_player_seq_n[j->p_no] % emph_player_seql_frames;
	        W_WriteBitmap (dx - (emph_player_seql_width / 2),
			       dy - (emph_player_seql_height / 2),
			       emph_player_seql[seq_n],
			       W_White, w);
	    }
#endif

#ifdef SOUND
            if (myPlayer(j) || isObsLockPlayer(j))
            {
                if ((sound_flags & PFSHIELD) && !(j->p_flags & PFSHIELD))
                {
                    if (newSound)
                    {
                        // Kill any channels with SHIELD_UP/DOWN_WAV on them (group 4)
                        Mix_HaltGroup(4);
                        Play_Sound(SHIELD_DOWN_WAV);
                    }
                    else
                        Play_Sound(SHIELD_DOWN_SOUND);
                }
                if (!(sound_flags & PFSHIELD) && (j->p_flags & PFSHIELD))
                {
                    if (newSound)
                    {
                        // Kill any channels with SHIELD_UP/DOWN_WAV on them (group 4)
                        Mix_HaltGroup(4);
                        Play_Sound(SHIELD_UP_WAV);
                    }
                    else
                        Play_Sound(SHIELD_UP_SOUND);
                }
            }
#endif

            /* It used to be that if "showShields" was false, shields were not
             * shown.  COW has already stopped accepting showShields flags from
             * the server, now stop using showShields altogether. James Soutter
             * (Zork)     4 Jan 95 */

            if (j->p_flags & PFSHIELD)
            {
                int color = playerColor (j);

#ifdef VSHIELD_BITMAPS
                int shieldnum;

                if (j == me && varyShields)
                {
                    shieldnum =
                        SHIELD_FRAMES * me->p_shield / me->p_ship.s_maxshield;
                    if (shieldnum >= SHIELD_FRAMES)
                        shieldnum = SHIELD_FRAMES - 1;
                    color = gColor;
                    if (shieldnum < SHIELD_FRAMES * 2 / 3)
                    {
                        color = yColor;
                        if (shieldnum < SHIELD_FRAMES * 2 / 3)
                        {
                            color = rColor;
                        }
                    }
                }
                else
                {
                    color = playerColor (j);
                    shieldnum = 2;
                }
#endif

                if (warnShields && j == me)
                {
                    switch (me->p_flags & (PFGREEN | PFYELLOW | PFRED))
                    {
                    case PFGREEN:
                        color = gColor;
                        break;
                    case PFYELLOW:
                        color = yColor;
                        break;
                    case PFRED:
                        color = rColor;
                        break;
                    }
                }

#ifdef VSHIELD_BITMAPS
                W_WriteBitmap (dx - (shield_width / 2),
                               dy - (shield_height / 2), shield[shieldnum],
                               color, w);
#else
                W_WriteBitmap (dx - (shield_width / 2),
                               dy - (shield_height / 2), shield, color, w);
#endif
            }
            /* Det circle */
            if (detCircle)
            {
            	if (myPlayer(j) || isObsLockPlayer(j))
            	{
                    W_WriteCircle(w, WINSIDE/2, WINSIDE/2, DETDIST/SCALE, W_Red);         
                    clearzone[0][clearcount] = WINSIDE/2 - (DETDIST/SCALE);
                    clearzone[1][clearcount] = WINSIDE/2 - (DETDIST/SCALE);
                    clearzone[2][clearcount] = 2*DETDIST/SCALE;
                    clearzone[3][clearcount] = 2*DETDIST/SCALE;
                    clearcount++;
                }
            }
            
            if (j->p_flags & PFCLOAK)   /* when cloaked stop here */
                continue;

            {
                int color = playerColor (j);

#ifdef HOCKEY_LINES
                /* Do we want to see puck's letter ? Nah. */
                if (hockey_mode () && 
                    strcmp(j->p_name, "Puck") == 0 &&
                    strcmp(j->p_login, "Robot") == 0 &&
                    j->p_team == NOBODY &&
                    j->p_ship.s_type == SCOUT)
                {
                    idbuf[0] = '\0';
                    buflen = 1;
                }
                else
#endif
                {
                    idbuf[0] = *(shipnos + j->p_no);
                    buflen = 1;
                }

                if (myPlayer(j) || isObsLockPlayer(j))
                {
                    switch (me->p_flags & (PFGREEN | PFYELLOW | PFRED))
                    {
                    case PFGREEN:
                        color = gColor;
                        break;
                    case PFYELLOW:
                        color = yColor;
                        break;
                    case PFRED:
                        color = rColor;
                        break;
                    }

                    if (showMySpeed)
                    {
                        if (j->p_speed < 10)
                        {
                            idbuf[1] = ',';
                            idbuf[2] = (char) (j->p_speed + '0');
                            idbuf[3] = '\0';
                            buflen = 3;
                        }
                        else
                        {
                            idbuf[1] = ',';
                            idbuf[2] = (char) (j->p_speed / 10 + '0');
                            idbuf[3] = (char) (j->p_speed % 10 + '0');
                            idbuf[4] = '\0';
                            buflen = 4;
                        }
                    }
                }
                W_MaskText (w, dx + (j->p_ship.s_width / 2),
                            dy - (j->p_ship.s_height / 2), color,
                            idbuf, buflen, shipFont (j));
                clearzone[0][clearcount] = dx + (j->p_ship.s_width / 2);
                clearzone[1][clearcount] = dy - (j->p_ship.s_height / 2);
                clearzone[2][clearcount] = buflen * W_Textwidth;
                clearzone[3][clearcount] = W_Textheight;
                clearcount++;
            }

        }
        else if (j->p_status == PEXPLODE)
        {
            int i;

            i = j->p_explode;

#ifdef SOUND
            if (i == 1)
            {
                if (newSound)
                {
                    if (myPlayer(j) || isObsLockPlayer(j))
                    {
                    	if (j->p_ship.s_type == STARBASE)
                    	    Play_Sound(BASE_EXPLOSION_WAV);
                    	else
                            Play_Sound(EXPLOSION_WAV);
                    }
                    else
                    {
                        int newdx, newdy, distance, angle;
                    
                        newdx = dx - WINSIDE/2;
                        newdy = dy - WINSIDE/2;
                        
                        distance = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                        // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                        // which would be a diagonal from player to corner of tactical
                        // Reason for normalization is Mix_SetDistance requires that range        
                        distance = (int)((255 * distance)/CORNER_DIST);
                        // Calculate angle, then adjust as necessary for Mix_SetDistance
                        angle = (int)(atan2(newdy, newdx)*180/XPI);
                        angle = 270 - angle;
                        // At short distances, don't use angular sound
                        if (distance < SCALE/2)
                        {
                            if (j->p_ship.s_type == STARBASE)
                    	        Play_Sound(BASE_EXPLOSION_WAV);
                    	    else
                                Play_Sound(EXPLOSION_OTHER_WAV);
                        }
                        else
                        {
                            if (j->p_ship.s_type == STARBASE)
                    	        Play_Sound_Loc(BASE_EXPLOSION_WAV, angle, distance);
                    	    else
                                Play_Sound_Loc(EXPLOSION_OTHER_WAV, angle, distance);
                        }
                    }
                }
                else
                {
                    if (j->p_ship.s_type == STARBASE)
                        Play_Sound(BASE_EXPLOSION_SOUND);
                    else if (myPlayer(j) || isObsLockPlayer(j))
                        Play_Sound(EXPLOSION_SOUND);
                    else
                        Play_Sound(OTHER_EXPLOSION_SOUND);
                }
            }
#endif

            if (i < BMP_SHIPEXPL_FRAMES ||
                (i < BMP_SBEXPL_FRAMES && j->p_ship.s_type == STARBASE))
            {

                if (j->p_ship.s_type == STARBASE)
                {
                    W_WriteBitmap (dx - (BMP_SBEXPL_WIDTH / 2),
                                   dy - (BMP_SBEXPL_HEIGHT / 2), sbexpview[i],
                                   playerColor (j), w);
                    clearzone[0][clearcount] = dx - (BMP_SBEXPL_WIDTH / 2);
                    clearzone[1][clearcount] = dy - (BMP_SBEXPL_HEIGHT / 2);
                    clearzone[2][clearcount] = BMP_SBEXPL_WIDTH;
                    clearzone[3][clearcount] = BMP_SBEXPL_HEIGHT;
                }
                else
                {
                    W_WriteBitmap (dx - (BMP_SHIPEXPL_WIDTH / 2),
                                   dy - (BMP_SHIPEXPL_HEIGHT / 2), expview[i],
                                   playerColor (j), w);
                    clearzone[0][clearcount] = dx - (BMP_SHIPEXPL_WIDTH / 2);
                    clearzone[1][clearcount] = dy - (BMP_SHIPEXPL_HEIGHT / 2);
                    clearzone[2][clearcount] = BMP_SHIPEXPL_WIDTH;
                    clearzone[3][clearcount] = BMP_SHIPEXPL_HEIGHT;
                }
                clearcount++;
                j->p_explode++;
            }
        }

        /* Now draw his phaser (if it exists) */
        php = &phasers[j->p_no];
        
        /* Reset colorful phasers here */
#ifdef JUBILEE_PHASERS
        if (j == me && colorfulPhasers 
            && (php->ph_status == PHFREE || php->ph_updateFuse == 0))
            ph_col = 0;
#endif

        if (php->ph_status != PHFREE)
        {

#ifdef SOUND
            if (!sound_phaser)
            {           
                if (newSound)
                {
                    if (myPlayer(j) || isObsLockPlayer(j))
                        Play_Sound(PHASER_WAV);
                    else
                    {
                        int newdx, newdy, distance, angle;
                    
                        newdx = dx - WINSIDE/2;
                        newdy = dy - WINSIDE/2;
                        
                        distance = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                        // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                        // which would be a diagonal from player to corner of tactical
                        // Reason for normalization is Mix_SetDistance requires that range        
                        distance = (int)((255 * distance)/CORNER_DIST);
                        // Calculate angle, then adjust as necessary for Mix_SetDistance
                        angle = (int)(atan2(newdy, newdx)*180/XPI);
                        angle = 270 - angle;
                        // At short distances, don't use angular sound
                        if (distance < SCALE/2)
                            Play_Sound(PHASER_OTHER_WAV);
                        else
                            Play_Sound_Loc(PHASER_OTHER_WAV, angle, distance);
                    }
                }
                else
                    Play_Sound((myPlayer(j) || isObsLockPlayer(j))
                               ? PHASER_SOUND : OTHER_PHASER_SOUND);
                
                sound_phaser++;
            }
#endif

            if ((php->ph_updateFuse -= weaponUpdate) == 0)
            {
                /* Expire the phaser */
                php->ph_status = PHFREE;
            }
            else
            {
                if (php->ph_status == PHMISS)
                {
                    /* Here I will have to compute end coordinate */
                    tx = (int) (PHASEDIST * j->p_ship.s_phaserdamage / 100 *
                                Cos[php->ph_dir]);

                    ty = (int) (PHASEDIST * j->p_ship.s_phaserdamage / 100 *
                                Sin[php->ph_dir]);

                    tx = (j->p_x + tx - me->p_x) / SCALE + WINSIDE / 2;
                    ty = (j->p_y + ty - me->p_y) / SCALE + WINSIDE / 2;
                    php->ph_fuse = 0;

                }
                else if (php->ph_status == PHHIT2)
                {
                    tx = (php->ph_x - me->p_x) / SCALE + WINSIDE / 2;
                    ty = (php->ph_y - me->p_y) / SCALE + WINSIDE / 2;
                }
                else
                {               /* Start point is dx, dy */
                    tx = (players[php->ph_target].p_x - me->p_x) /
                        SCALE + WINSIDE / 2;
                    ty = (players[php->ph_target].p_y - me->p_y) /
                        SCALE + WINSIDE / 2;
                }


                /* Shrink the phasers if necessary:
                 *
                 * Measure length in 16ths to make the maths a little
                 * easier for the computer (div 16 is a 4 bit shift).
                 * 
                 * Add 8 to each sum to round properly. */

                if (shrinkPhaserOnMiss || php->ph_status != PHMISS)
                {
                    if (j == me)
                    {
                        if (phaserShrinkStyle == 1)
                        {
                            px = (dx * 16 + 8) / 16;
                            py = (dy * 16 + 8) / 16;
                        }
                        else
                        {
                            px = (dx * (16 - phaserShrink) + tx * phaserShrink +
                                    8) / 16;
                            py = (dy * (16 - phaserShrink) + ty * phaserShrink +
                                    8) / 16;
                        }
                    }
                    else
                    {
                        px = (dx * (16 - theirPhaserShrink) +
                              tx * theirPhaserShrink + 8) / 16;

                        py = (dy * (16 - theirPhaserShrink) +
                              ty * theirPhaserShrink + 8) / 16;
                    }
                }
                else
                {
                    px = dx;
                    py = dy;
                }


                /* Now draw the phasers */

                if (friendlyPlayer (j))
                {
#ifdef JUBILEE_PHASERS
                    if (j == me && php->ph_status == PHHIT && colorfulPhasers)
                    {
                        int col;

                        switch (ph_col)
                        {
                        case 1:
                        case 2:
                            col = W_Red;
                            break;
                        case 3:
                        case 4:
                            col = W_Green;
                            break;
                        case 5:
                        case 6:
                            col = W_Yellow;
                            break;
                        case 7:
                        case 8:
                            col = W_Cyan;
                            break;
                        default:
                            col = shipCol[remap[j->p_team]];
                            ph_col = 0;
                            break;
                        }
                        ph_col += (100/j->p_ship.s_phaserfuse/updatesPerSec);
                        if (phaserShrinkStyle == 1)
                        {
                            get_shrink_phaser_coords(&new_dx, &new_dy,
                                                    px, py, tx, ty, 
                                                    php->ph_fuse, php->ph_maxfuse);
                            px = new_dx;
                            py = new_dy;
                        }
                        W_CacheLine (w, px, py, tx, ty, col);
                    }
                    else
                    {
                        if (php->ph_status != PHMISS)
                        {
                            if (phaserShrinkStyle == 1 && j == me)
                            {
                                get_shrink_phaser_coords(&new_dx, &new_dy,
                                                        px, py, tx, ty, 
                                                        php->ph_fuse, php->ph_maxfuse);
                                px = new_dx;
                                py = new_dy;
                            }
                            if (highlightFriendlyPhasers)
                                W_CacheLine (w, px, py, tx, ty, foreColor);
                            else
                            {
	                        if ((php->ph_fuse % 2) == 1)
                                    W_CacheLine (w, px, py, tx, ty, foreColor);
	                        else
                                    W_CacheLine (w, px, py, tx, ty, shipCol[remap[j->p_team]]);
                            }
                        }
                        else
                            W_CacheLine (w, px, py, tx, ty, shipCol[remap[j->p_team]]);
                    }
#else
                    if (highlightFriendlyPhasers && (php->ph_status == PHHIT))
                        W_CacheLine (w, px, py, tx, ty, foreColor);
                    else
                    {
                        if ((php->ph_fuse % 2) == 1)
                            W_CacheLine (w, px, py, tx, ty, foreColor);
                        else
                            W_CacheLine (w, px, py, tx, ty, shipCol[remap[j->p_team]]);
                    }
#endif
                    php->ph_fuse++;

                    clearline[0][clearlcount] = px;
                    clearline[1][clearlcount] = py;
                    clearline[2][clearlcount] = tx;
                    clearline[3][clearlcount] = ty;
                    clearlcount++;
                }
                else
                {
                    if ((enemyPhasers > 0) && (enemyPhasers <= 10))
                    {
                        unsigned char dir;

                        if (tx == px && ty == py)
                            continue;
#ifdef SHORT_PACKETS
                        if (php->ph_status != PHMISS)   /* KOC 10/20/95  */
                        {       /* hack for SP_2 */
                            dir = (unsigned char) 
                                  nint (atan2((double) (ty - py),
                                       (double) (tx - px)) / XPI * 128.0);
                        }
                        else
#endif
                        {
                            dir = (unsigned char) (NORMALIZE (php->ph_dir + 64));
                        }

                        wx = (int) (px + enemyPhasers * Cos[dir]);
                        wy = (int) (py + enemyPhasers * Sin[dir]);
                        lx = (int) (px - enemyPhasers * Cos[dir]);
                        ly = (int) (py - enemyPhasers * Sin[dir]);

                        W_MakePhaserLine (w, wx, wy, tx, ty, shipCol[remap[j->p_team]]);
                        W_MakePhaserLine (w, lx, ly, tx, ty, shipCol[remap[j->p_team]]);

                        php->ph_fuse++;

                        clearline[0][clearlcount] = wx;
                        clearline[1][clearlcount] = wy;
                        clearline[2][clearlcount] = tx;
                        clearline[3][clearlcount] = ty;
                        clearlcount++;

                        clearline[0][clearlcount] = lx;
                        clearline[1][clearlcount] = ly;
                        clearline[2][clearlcount] = tx;
                        clearline[3][clearlcount] = ty;
                        clearlcount++;
                    }
                    else
                    {
                        W_MakePhaserLine (w, px, py, tx, ty, shipCol[remap[j->p_team]]);

                        php->ph_fuse++;

                        clearline[0][clearlcount] = px;
                        clearline[1][clearlcount] = py;
                        clearline[2][clearlcount] = tx;
                        clearline[3][clearlcount] = ty;
                        clearlcount++;
                    }
                }
            }
        }

#ifdef SOUND
        else if (myPlayer(j) || isObsLockPlayer(j))
            sound_phaser = 0;
#endif

        /* ATM - show tractor/pressor beams (modified by James Collins) */
        /* showTractorPressor is a variable set by xtrekrc. */

        if (showTractorPressor)
        {
            if ((myPlayer (j) || (F_show_all_tractors && showAllTractorPressor)) && isAlive (me)
                && isAlive(j) && (j->p_flags & PFTRACT || j->p_flags & PFPRESS))
            {
                double theta;
                unsigned char dir;
                int lx[2], ly[2], target_width;

                struct player *tractee;

                if (j->p_tractor < 0 || j->p_tractor >= MAXPLAYER)
                    continue;
                tractee = &players[j->p_tractor];

                if (tractee->p_status != PALIVE ||
                    ((tractee->p_flags & PFCLOAK) &&
                     (tractee->p_cloakphase == (CLOAK_PHASES - 1))))
                {
                    continue;
                }

                if (tcounter >= 2)
                {               /* continue tractor stuff */
                    if (!continueTractor)
                        tcounter--;
                    px = (tractee->p_x - me->p_x);
                    py = (tractee->p_y - me->p_y);

                    px = px / SCALE + WINSIDE / 2;
                    py = py / SCALE + WINSIDE / 2;

                    if (px == dx && py == dy)
                        continue;       /* this had better be last
                                         * in for(..) */

                    theta = atan2 ((double) (px - dx),
                                   (double) (dy - py)) + XPI / 2.0;
                    dir = (unsigned char) nint (theta / XPI * 128.0);
                    if (tractee->p_flags & PFSHIELD)
                        target_width = shield_width;
                    else
                    {
                        target_width = tractee->p_ship.s_width / 2;
                    }
                    lx[0] = (int) (px + (Cos[dir] * (target_width / 2)));
                    ly[0] = (int) (py + (Sin[dir] * (target_width / 2)));
                    lx[1] = (int) (px - (Cos[dir] * (target_width / 2)));
                    ly[1] = (int) (py - (Sin[dir] * (target_width / 2)));
                    if (j->p_flags & PFPRESS)
                    {
                        W_MakeTractLine (w, dx, dy, lx[0], ly[0], W_Yellow);
                        W_MakeTractLine (w, dx, dy, lx[1], ly[1], W_Yellow);
                    }
                    else
                    {
                        W_MakeTractLine (w, dx, dy, lx[0], ly[0], W_Green);
                        W_MakeTractLine (w, dx, dy, lx[1], ly[1], W_Green);
                    }

                    /*
                        keeping track of tractors seperately from other lines allows
                        clearing them diffently.  Clearing them the same as other
                        solid lines caused occasional bits to be left on the screen.
                        This is the fix.  [BDyess]
                    */
                    if (tractcurrent == NULL)
                    { 
                        /* just starting */
                        tractcurrent = (struct tractor *) malloc (sizeof (struct tractor));
                        tracthead = tractcurrent;
                        tracthead->next = NULL;
                    }
                    tractcurrent->sx = dx;
                    tractcurrent->sy = dy;
                    tractcurrent->d1x = lx[0];
                    tractcurrent->d1y = ly[0];
                    tractcurrent->d2x = lx[1];
                    tractcurrent->d2y = ly[1];
                    /* get ready for the next run through */
                    if (tractcurrent->next)
                    {   
                        /* already malloc'd before */
                        tractcurrent = tractcurrent->next;
                    } 
                    else 
                    {        
                        /* new maximum, create a new struct */
                        tractcurrent->next = (struct tractor *) malloc (sizeof (struct tractor));
                        tractcurrent = tractcurrent->next;
                        tractcurrent->next = NULL;
                    }
                }
            }
            else if (!(j->p_flags & PFPRESS || j->p_flags & PFTRACT))
                tcounter = 2;
        }
    }
}


static void
DrawTorps (void)
{
    register struct torp *k, *t;
    register int dx, dy;
    struct player *j;
    int torpCount;
    int torpTeam;
    const int view = SCALE * WINSIDE / 2;

    for (t = torps, j = players; j != players + MAXPLAYER; t += MAXTORP, ++j)
    {
#ifdef SOUND
        if (j != me)
            num_other_torps += j->p_ntorp;
#endif
        torpCount = j->p_ntorp;

        for (k = t; torpCount > 0; ++k)
        {
            /* Work until all the torps for a given player have been examined.
               In the current INL server torps are allocated from low to high
               so this loop must work so that k is incrimented rather than
               decrimented. */

            if (!k->t_status)
                continue;

            --torpCount;


            /* Age a torp only if some weapon has been updated
             * (eg this is not a pause). */
            if ((k->t_updateFuse -= weaponUpdate) == 0)
            {
                if (k->t_status != TEXPLODE)
                {
                    /* Expire the torp */
#if 0
                    fputs ("[torp]", stderr);
                    fflush (stderr);
#endif
                    k->t_status = TFREE;
                    j->p_ntorp--;
                    continue;
                }
                else
                {
                    /* Leave the torp to explode on its own */

                    k->t_updateFuse = 100;
                }
            }

            dx = k->t_x - me->p_x;
            dy = k->t_y - me->p_y;

            if (dx > view || dx < -view || dy > view || dy < -view)
            {
                /* Call any torps off screen "free" (if owned by other) */
                if (k->t_status == TEXPLODE && j != me)
                {
                    k->t_status = TFREE;
                    j->p_ntorp--;
                }
                continue;
            }

#ifdef SOUND
            if (j != me && newSound)
            {
            	int new_angle;
		int newdy, newdx;

		newdy = dy / SCALE;
		newdx = dx / SCALE;
                // Store location of torp.
                new_other_torp_dist = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                // which would be a diagonal from player to corner of tactical
                // Reason for normalization is Mix_SetDistance requires that range        
                new_other_torp_dist = (int)((255 * new_other_torp_dist)/CORNER_DIST);
                // Find how long till torp expires...BROKEN!
                // Calculate angle, then adjust as necessary for Mix_SetDistance
                new_angle = (int)(atan2(newdy, newdx)*180/XPI);
                new_angle = 270 - new_angle;
                // Choose closest torp - yes this sucks sometimes, but better than nothing
                if (new_other_torp_dist < other_torp_dist)
                {
                    other_torp_dist = new_other_torp_dist;
                    other_torp_angle = new_angle;
                }	
            }
#endif         
            dx = dx / SCALE + WINSIDE / 2;
            dy = dy / SCALE + WINSIDE / 2;


            if (k->t_status == TEXPLODE)
            {

                k->t_fuse--;
                if (k->t_fuse <= 0)
                {
                    k->t_status = TFREE;
                    j->p_ntorp--;
                    continue;
                }
                if (k->t_fuse >= BMP_TORPDET_FRAMES)
                {
                    k->t_fuse = BMP_TORPDET_FRAMES - 1;
                }

#ifdef SOUND
                if (k->t_fuse == BMP_TORPDET_FRAMES - 1)
                {
                    if (newSound)
                    {
                        int newdx, newdy, distance, angle;
                    
                        newdx = dx - WINSIDE/2;
                        newdy = dy - WINSIDE/2;
                        
                        distance = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                        // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                        // which would be a diagonal from player to corner of tactical
                        // Reason for normalization is Mix_SetDistance requires that range        
                        distance = (int)((255 * distance)/CORNER_DIST);
                        // Calculate angle, then adjust as necessary for Mix_SetDistance
                        angle = (int)(atan2(newdy, newdx)*180/XPI);
                        angle = 270 - angle;
                        // At short distances, don't use angular sound
                        if (distance < SCALE/2)
                            Play_Sound(TORP_HIT_WAV);
                        else
                            Play_Sound_Loc(TORP_HIT_WAV, angle, distance);
                    }
                    else
                        Play_Sound(TORP_HIT_SOUND);
                }
#endif

                if (colorWeapons)
                {
                    if (myPlayer(j))
                        torpTeam = 0;
                    else
                    {
                        switch (j->p_team)
                        {
                        case FED:
                            torpTeam = 1;
                            break;
                        case KLI:
                            torpTeam = 3;
                            break;
                        case ORI:
                            torpTeam = 4;
                            break;
                        case ROM:
                            torpTeam = 5;
                            break;
                        default: // IND
                            torpTeam = 2;
                        }
                    }
                    W_WriteBitmap (dx - (BMP_CTORPDET_WIDTH / 2),
                                   dy - (BMP_CTORPDET_HEIGHT / 2),
                                   cloudC[torpTeam][k->t_fuse], torpColor (k), w);
                    clearzone[0][clearcount] = dx - (BMP_CTORPDET_WIDTH / 2);
                    clearzone[1][clearcount] = dy - (BMP_CTORPDET_HEIGHT / 2);
                    clearzone[2][clearcount] = BMP_CTORPDET_WIDTH;
                    clearzone[3][clearcount] = BMP_CTORPDET_HEIGHT;
                    clearcount++;
                }
                else
                {
                    W_WriteBitmap (dx - (BMP_TORPDET_WIDTH / 2),
                                   dy - (BMP_TORPDET_HEIGHT / 2),
                                   cloud[k->t_fuse], torpColor (k), w);
                    clearzone[0][clearcount] = dx - (BMP_TORPDET_WIDTH / 2);
                    clearzone[1][clearcount] = dy - (BMP_TORPDET_HEIGHT / 2);
                    clearzone[2][clearcount] = BMP_TORPDET_WIDTH;
                    clearzone[3][clearcount] = BMP_TORPDET_HEIGHT;
                    clearcount++;
                }
            }
            else
            {
            	if (colorWeapons)
            	{
                    if ((k->t_fuse++ >= BMP_TORP_FRAMES - 1) || (k->t_fuse < 0))
                        k->t_fuse = 0;
        
                    if (myPlayer(j))
                        torpTeam = 0;
                    else
                    {
                        switch (j->p_team)
                        {
                        case FED:
                            torpTeam = 1;
                            break;
                        case KLI:
                            torpTeam = 3;
                            break;
                        case ORI:
                            torpTeam = 4;
                            break;
                        case ROM:
                            torpTeam = 5;
                            break;
                        default: // IND
                            torpTeam = 2;
                        }
                    }
                    
                    if (j != me && ((k->t_war & me->p_team) ||
                                        (j->p_team & (me->p_hostile | me->p_swar))))
                    {
                        W_WriteBitmap (dx - (BMP_CTORP_WIDTH / 2),
                                       dy - (BMP_CTORP_HEIGHT / 2),
                                       torpC[torpTeam][k->t_fuse], torpColor (k), w);
                    }
                    else
                    {
                        W_WriteBitmap (dx - (BMP_CTORP_WIDTH / 2),
                                       dy - (BMP_CTORP_HEIGHT / 2),
                                       mtorpC[torpTeam][k->t_fuse], torpColor (k), w);
                    }
                    
                    clearzone[0][clearcount] = dx - (BMP_CTORP_WIDTH / 2);
                    clearzone[1][clearcount] = dy - (BMP_CTORP_HEIGHT / 2);
                    clearzone[2][clearcount] = BMP_CTORP_WIDTH;
                    clearzone[3][clearcount] = BMP_CTORP_HEIGHT;
                    clearcount++;
                }
                else
                {
                    if (j != me && ((k->t_war & me->p_team) ||
                                        (j->p_team & (me->p_hostile | me->p_swar))))
                    {
                        /* solid.  Looks strange. W_FillArea(w, dx - (etorp_width/2),
                         * dy - (etorp_height/2), etorp_width, etorp_height,
                         * torpColor(k)); */
        
                        /* XFIX */
                        W_WriteBitmap (dx - (etorp_width / 2),
                                       dy - (etorp_height / 2), etorp, torpColor (k), w);
                        clearzone[0][clearcount] = dx - (etorp_width / 2);
                        clearzone[1][clearcount] = dy - (etorp_height / 2);
                        clearzone[2][clearcount] = etorp_width;
                        clearzone[3][clearcount] = etorp_height;
                        clearcount++;
                    }
                    else
                    {
                        W_WriteBitmap (dx - (mtorp_width / 2),
                                       dy - (mtorp_height / 2), mtorp, torpColor (k), w);
                        clearzone[0][clearcount] = dx - (mtorp_width / 2);
                        clearzone[1][clearcount] = dy - (mtorp_height / 2);
                        clearzone[2][clearcount] = mtorp_width;
                        clearzone[3][clearcount] = mtorp_height;
                        clearcount++;
                    }
                }
            }
        }
    }
}


void
DrawPlasmaTorps (void)
{
    register struct plasmatorp *pt;
    register int dx, dy;
    const int view = SCALE * WINSIDE / 2;
    int ptorpTeam;

    /* MAXPLASMA is small so work through all the plasmas rather than
       look at the number of outstanding plasma torps for each player. */
    for (pt = plasmatorps + (MAXPLASMA * MAXPLAYER) - 1; pt >= plasmatorps;
         --pt)
    {
        if (!pt->pt_status)
            continue;
#ifdef SOUND
        if (pt->pt_owner != me->p_no)
            num_other_plasmas++;
#endif
        if ((pt->pt_updateFuse -= weaponUpdate) == 0)
        {
            if (pt->pt_status != PTEXPLODE)
            {
                /* Expire the torp */
#ifdef DEBUG
                fputs ("[plasma]", stderr);
                fflush (stderr);
#endif
                pt->pt_status = PTFREE;
                players[pt->pt_owner].p_nplasmatorp--;
                continue;
            }
            else
            {
                /* Leave the torp to explode on its own */

                pt->pt_updateFuse = 100;
            }
        }

        dx = pt->pt_x - me->p_x;
        dy = pt->pt_y - me->p_y;

        if (dx > view || dx < -view || dy > view || dy < -view)
            continue;


#ifdef SOUND
        if (pt->pt_owner != me->p_no && newSound)
        {
            int new_angle;
            int newdy, newdx;
		
	    newdy = dy / SCALE;
	    newdx = dx / SCALE;
            // Store location of plasma.
            new_other_plasma_dist = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
            // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
            // which would be a diagonal from player to corner of tactical
            // Reason for normalization is Mix_SetDistance requires that range        
            new_other_plasma_dist = (int)((255 * new_other_plasma_dist)/CORNER_DIST);
            // Find how long till torp expires...BROKEN!
            // Calculate angle, then adjust as necessary for Mix_SetDistance
            new_angle = (int)(atan2(newdy, newdx)*180/XPI);
            new_angle = 270 - new_angle;
            // Choose closest plasma - yes this sucks sometimes, but better than nothing
            if (new_other_plasma_dist < other_plasma_dist)
            {
                other_plasma_dist = new_other_plasma_dist;
                other_plasma_angle = new_angle;
            }   	
        }
#endif   

        dx = dx / SCALE + WINSIDE / 2;
        dy = dy / SCALE + WINSIDE / 2;

        if (pt->pt_status == PTEXPLODE)
        {
            pt->pt_fuse--;
            if (pt->pt_fuse <= 0)
            {
                pt->pt_status = PTFREE;
                players[pt->pt_owner].p_nplasmatorp--;
                continue;
            }

            if (pt->pt_fuse >= BMP_TORPDET_FRAMES)
            {
                pt->pt_fuse = BMP_TORPDET_FRAMES - 1;
            }

#ifdef SOUND
            if (pt->pt_fuse == BMP_TORPDET_FRAMES - 1)
            {
                if (newSound)
                {
                    int newdx, newdy, distance, angle;
                    
                    newdx = dx - WINSIDE/2;
                    newdy = dy - WINSIDE/2;
                        
                    distance = (int) sqrt((newdx)*(newdx) + (newdy)*(newdy));
                    // Normalize from 0 to 255, 0 being on top of player, 255 being max distance
                    // which would be a diagonal from player to corner of tactical
                    // Reason for normalization is Mix_SetDistance requires that range        
                    distance = (int)((255 * distance)/CORNER_DIST);
                    // Calculate angle, then adjust as necessary for Mix_SetDistance
                    angle = (int)(atan2(newdy, newdx)*180/XPI);
                    angle = 270 - angle;
                    // At short distances, don't use angular sound
                    if (distance < SCALE/2)
                        Play_Sound(PLASMA_HIT_WAV);
                    else
                        Play_Sound_Loc(PLASMA_HIT_WAV, angle, distance);
                }
                else
                    Play_Sound(PLASMA_HIT_SOUND);
            }
#endif

            if (colorWeapons)
            {
                if (pt->pt_owner == me->p_no)
                    ptorpTeam = 0;
                else
                {
                    switch (players[pt->pt_owner].p_team)
                    {
                    case FED:
                        ptorpTeam = 1;
                        break;
                    case KLI:
                        ptorpTeam = 3;
                        break;
                    case ORI:
                        ptorpTeam = 4;
                        break;
                    case ROM:
                        ptorpTeam = 5;
                        break;
                    default:  // IND
                        ptorpTeam = 2;
                    }
                }
                W_WriteBitmap (dx - (BMP_CPLASMATORPDET_WIDTH / 2),
                               dy - (BMP_CPLASMATORPDET_HEIGHT / 2),
                               plcloudC[ptorpTeam][pt->pt_fuse],
                               plasmatorpColor (pt), w);
                clearzone[0][clearcount] = dx - (BMP_CPLASMATORPDET_WIDTH / 2);
                clearzone[1][clearcount] = dy - (BMP_CPLASMATORPDET_HEIGHT / 2);
                clearzone[2][clearcount] = BMP_CPLASMATORPDET_WIDTH;
                clearzone[3][clearcount] = BMP_CPLASMATORPDET_HEIGHT;
                clearcount++;
            }
            else
            {
                W_WriteBitmap (dx - (BMP_PLASMATORPDET_WIDTH / 2),
                               dy - (BMP_PLASMATORPDET_HEIGHT / 2),
                               plasmacloud[pt->pt_fuse], plasmatorpColor (pt), w);
                clearzone[0][clearcount] = dx - (BMP_PLASMATORPDET_WIDTH / 2);
                clearzone[1][clearcount] = dy - (BMP_PLASMATORPDET_HEIGHT / 2);
                clearzone[2][clearcount] = BMP_PLASMATORPDET_WIDTH;
                clearzone[3][clearcount] = BMP_PLASMATORPDET_HEIGHT;
                clearcount++;
            }
        }
        else
        {
            if (colorWeapons)
            {
                if ((pt->pt_fuse++ >= BMP_TORP_FRAMES - 1) || (pt->pt_fuse < 0))
                    pt->pt_fuse = 0;
        
                if (pt->pt_owner == me->p_no)
                    ptorpTeam = 0;
                else
                {
                    switch (players[pt->pt_owner].p_team)
                    {
                    case FED:
                        ptorpTeam = 1;
                        break;
                    case KLI:
                        ptorpTeam = 3;
                        break;
                    case ORI:
                        ptorpTeam = 4;
                        break;
                    case ROM:
                        ptorpTeam = 5;
                        break;
                    default: // IND
                        ptorpTeam = 2;
                    }
                }
                if (pt->pt_owner != me->p_no && ((pt->pt_war & me->p_team) ||
                                                      (players[pt->pt_owner].
                                                       p_team & (me->p_hostile | me->
                                                                 p_swar))))
                {
                    W_WriteBitmap (dx - (BMP_CPLASMATORP_WIDTH / 2),
                                   dy - (BMP_CPLASMATORP_HEIGHT / 2),
                                   plasmaC[ptorpTeam][pt->pt_fuse],
                                   plasmatorpColor (pt), w);
                }
                else
                {
                    W_WriteBitmap (dx - (BMP_CPLASMATORP_WIDTH / 2),
                                   dy - (BMP_CPLASMATORP_HEIGHT / 2),
                                   mplasmaC[ptorpTeam][pt->pt_fuse],
                                   plasmatorpColor (pt), w);
                }
                clearzone[0][clearcount] = dx - (BMP_CPLASMATORP_WIDTH / 2);
                clearzone[1][clearcount] = dy - (BMP_CPLASMATORP_HEIGHT / 2);
                clearzone[2][clearcount] = BMP_CPLASMATORP_WIDTH;
                clearzone[3][clearcount] = BMP_CPLASMATORP_HEIGHT;
                clearcount++;
             }
             else
             {   
                /* needmore: if(pt->pt_war & me->p_team) */
                if (pt->pt_owner != me->p_no && ((pt->pt_war & me->p_team) ||
                                                      (players[pt->pt_owner].
                                                       p_team & (me->p_hostile | me->
                                                                 p_swar))))
                {
                    W_WriteBitmap (dx - (eplasmatorp_width / 2),
                                   dy - (eplasmatorp_height / 2),
                                   eplasmatorp, plasmatorpColor (pt), w);
                    clearzone[0][clearcount] = dx - (eplasmatorp_width / 2);
                    clearzone[1][clearcount] = dy - (eplasmatorp_height / 2);
                    clearzone[2][clearcount] = eplasmatorp_width;
                    clearzone[3][clearcount] = eplasmatorp_height;
                    clearcount++;
                }
                else
                {
                    W_WriteBitmap (dx - (mplasmatorp_width / 2),
                                   dy - (mplasmatorp_height / 2),
                                   mplasmatorp, plasmatorpColor (pt), w);
                    clearzone[0][clearcount] = dx - (mplasmatorp_width / 2);
                    clearzone[1][clearcount] = dy - (mplasmatorp_height / 2);
                    clearzone[2][clearcount] = mplasmatorp_width;
                    clearzone[3][clearcount] = mplasmatorp_height;
                    clearcount++;
                }
            }
        }
    }
}


static void
DrawMisc (void)
{
    register struct player *j;
    register int dx, dy;
    const int view = SCALE * WINSIDE / 2;

#ifdef HOCKEY_LINES
    register struct s_line *sl;
    const int HALF_WINSIDE = WINSIDE / 2;
    int ex, ey, sx, sy;
#endif


#ifdef HOCKEY_LINES
    if (showHockeyLinesLocal && hockey_mode ())
        for (sl = local_hockey_lines + NUM_HOCKEY_LINES - 1;
             sl >= local_hockey_lines; --sl)
        {
            /* Treat the line differently based on the orientation */
            if (sl->orientation == S_LINE_VERTICAL)
            {
                if (((sx = (sl->begin_x - me->p_x) / SCALE) < HALF_WINSIDE)
                    && (sx > -HALF_WINSIDE))
                {
                    sx += HALF_WINSIDE;
                    ex = sx;
                    if ((sy =
                         HALF_WINSIDE - (me->p_y - sl->begin_y) / SCALE) < 0)
                        sy = 0;
                    if (sy > (WINSIDE - 1))
                        sy = WINSIDE - 1;
                    if ((ey =
                         HALF_WINSIDE - (me->p_y - sl->end_y) / SCALE) < 0)
                        ey = 0;
                    if (ey > (WINSIDE - 1))
                        ey = WINSIDE - 1;
                    if (sy == ey)
                        continue;
                }
                else
                    continue;
            }

            else if (sl->orientation == S_LINE_HORIZONTAL)
            {
                if (((sy = (sl->begin_y - me->p_y) / SCALE) < HALF_WINSIDE)
                    && (sy > -HALF_WINSIDE))
                {
                    sy += HALF_WINSIDE;
                    ey = sy;
                    if ((sx =
                         HALF_WINSIDE - (me->p_x - sl->begin_x) / SCALE) < 0)
                        sx = 0;
                    if (sx > (WINSIDE - 1))
                        sx = WINSIDE - 1;
                    if ((ex =
                         HALF_WINSIDE - (me->p_x - sl->end_x) / SCALE) < 0)
                        ex = 0;
                    if (ex > (WINSIDE - 1))
                        ex = WINSIDE - 1;
                    if (sx == ex)
                        continue;
                }
                else
                    continue;
            }
            else
                continue;

            W_CacheLine (w, sx, sy, ex, ey, sl->color);
            clearline[0][clearlcount] = sx;
            clearline[1][clearlcount] = sy;
            clearline[2][clearlcount] = ex;
            clearline[3][clearlcount] = ey;
            clearlcount++;
        }                       /* End for Hockey Lines *
                                 * Ends the if, too */
#endif /* HOCKEY_LINES */

    /* Draw Edges */
    if (me->p_x < (WINSIDE / 2) * SCALE)
    {
        dx = (WINSIDE / 2) - (me->p_x) / SCALE;
        sy = (WINSIDE / 2) + (0 - me->p_y) / SCALE;
        ey = (WINSIDE / 2) + (GWIDTH - me->p_y) / SCALE;
        if (sy < 0)
            sy = 0;
        if (ey > WINSIDE - 1)
            ey = WINSIDE - 1;
        /* XFIX */
        W_CacheLine (w, dx, sy, dx, ey, warningColor);
        clearline[0][clearlcount] = dx;
        clearline[1][clearlcount] = sy;
        clearline[2][clearlcount] = dx;
        clearline[3][clearlcount] = ey;
        clearlcount++;
    }

    if ((GWIDTH - me->p_x) < (WINSIDE / 2) * SCALE)
    {
        dx = (WINSIDE / 2) + (GWIDTH - me->p_x) / SCALE;
        sy = (WINSIDE / 2) + (0 - me->p_y) / SCALE;
        ey = (WINSIDE / 2) + (GWIDTH - me->p_y) / SCALE;
        if (sy < 0)
            sy = 0;
        if (ey > WINSIDE - 1)
            ey = WINSIDE - 1;
        /* XFIX */
        W_CacheLine (w, dx, sy, dx, ey, warningColor);
        clearline[0][clearlcount] = dx;
        clearline[1][clearlcount] = sy;
        clearline[2][clearlcount] = dx;
        clearline[3][clearlcount] = ey;
        clearlcount++;
    }

    if (me->p_y < (WINSIDE / 2) * SCALE)
    {
        dy = (WINSIDE / 2) - (me->p_y) / SCALE;
        sx = (WINSIDE / 2) + (0 - me->p_x) / SCALE;
        ex = (WINSIDE / 2) + (GWIDTH - me->p_x) / SCALE;
        if (sx < 0)
            sx = 0;
        if (ex > WINSIDE - 1)
            ex = WINSIDE - 1;
        /* XFIX */
        W_CacheLine (w, sx, dy, ex, dy, warningColor);
        clearline[0][clearlcount] = sx;
        clearline[1][clearlcount] = dy;
        clearline[2][clearlcount] = ex;
        clearline[3][clearlcount] = dy;
        clearlcount++;
    }

    if ((GWIDTH - me->p_y) < (WINSIDE / 2) * SCALE)
    {
        dy = (WINSIDE / 2) + (GWIDTH - me->p_y) / SCALE;
        sx = (WINSIDE / 2) + (0 - me->p_x) / SCALE;
        ex = (WINSIDE / 2) + (GWIDTH - me->p_x) / SCALE;
        if (sx < 0)
            sx = 0;
        if (ex > WINSIDE - 1)
            ex = WINSIDE - 1;
        /* XFIX */
        W_CacheLine (w, sx, dy, ex, dy, warningColor);
        clearline[0][clearlcount] = sx;
        clearline[1][clearlcount] = dy;
        clearline[2][clearlcount] = ex;
        clearline[3][clearlcount] = dy;
        clearlcount++;
    }

    /* Change border color to signify alert status */

    if (oldalert != (me->p_flags & (PFGREEN | PFYELLOW | PFRED)))
    {
        oldalert = (me->p_flags & (PFGREEN | PFYELLOW | PFRED));
        switch (oldalert)
        {
        case PFGREEN:
            if (extraAlertBorder)
            {
#ifndef DOUBLE_BUFFERING
                W_ChangeBorder (w, gColor);
                W_ChangeBorder (mapw, gColor);
#else
                W_ChangeBorderDB (localSDB, gColor);
                W_ChangeBorderDB (mapSDB, gColor);
#endif
            }
            W_ChangeBorder (baseWin, gColor);

#if defined(SOUND)
            if (newSound) // Kill any channels with WARNING_WAV or RED_ALERT_WAV (group 2)
                Mix_HaltGroup(2);
            else
            {
                Abort_Sound(WARNING_SOUND);
                Abort_Sound(RED_ALERT_SOUND);
            }
                
#endif

            break;
        case PFYELLOW:
            if (extraAlertBorder)
            {
#ifndef DOUBLE_BUFFERING
                W_ChangeBorder (w, yColor);
                W_ChangeBorder (mapw, yColor);
#else
                W_ChangeBorderDB (localSDB, yColor);
                W_ChangeBorderDB (mapSDB, yColor);
#endif
            }
            W_ChangeBorder (baseWin, yColor);

#if defined(SOUND)

            if (newSound) // Kill any channels with RED_ALERT_WAV (group 2)
            {
            	Mix_HaltGroup(2);
                Play_Sound(WARNING_WAV);
            }
            else
            {
            	Abort_Sound(RED_ALERT_SOUND);
                Play_Sound(WARNING_SOUND);
            }

#endif

            break;
        case PFRED:
            if (extraAlertBorder)
            {
#ifndef DOUBLE_BUFFERING
                W_ChangeBorder (w, rColor);
                W_ChangeBorder (mapw, rColor);
#else
                W_ChangeBorderDB (localSDB, rColor);
                W_ChangeBorderDB (mapSDB, rColor);
#endif
            }
            W_ChangeBorder (baseWin, rColor);
            
#if defined(SOUND)
            if (newSound)
                Play_Sound(RED_ALERT_WAV);
            else
                Play_Sound(RED_ALERT_SOUND);
#endif

            break;
        }
    }

#if defined(SOUND)
    if (newSound)
    {
        if (sound_torps < me->p_ntorp)
            Play_Sound(FIRE_TORP_WAV);
        if (sound_other_torps < num_other_torps)
        {
            if (other_torp_dist < SCALE/2)
                Play_Sound(OTHER_FIRE_TORP_WAV);
            else
                Play_Sound_Loc(OTHER_FIRE_TORP_WAV, other_torp_angle, other_torp_dist);
        }
        if (sound_plasma < me->p_nplasmatorp)
            Play_Sound(FIRE_PLASMA_WAV);
        if (sound_other_plasmas < num_other_plasmas)
        {
            if (other_plasma_dist < SCALE/2)
                Play_Sound(OTHER_FIRE_PLASMA_WAV);
            else
                Play_Sound_Loc(OTHER_FIRE_PLASMA_WAV, other_plasma_angle, other_plasma_dist);
        }
        // Reset locations and fuses of other's closest torps and plasmas
        other_torp_dist = CORNER_DIST;
        new_other_torp_dist = 0;
        other_torp_angle = 0;
        other_plasma_dist = CORNER_DIST;
        new_other_plasma_dist = 0;
        other_plasma_angle = 0;
    }
    else
    {
        if (sound_torps < me->p_ntorp)
            Play_Sound(FIRE_TORP_SOUND);
        if (sound_other_torps < num_other_torps)
            Play_Sound(OTHER_FIRE_TORP_SOUND);
        if (sound_plasma < me->p_nplasmatorp)
            Play_Sound(FIRE_PLASMA_SOUND);
        if (sound_other_plasmas < num_other_plasmas)
            Play_Sound(OTHER_FIRE_PLASMA_SOUND);
    }

    sound_flags = me->p_flags;
    sound_torps = me->p_ntorp;
    sound_other_torps = num_other_torps;
    num_other_torps = 0;
    sound_plasma = me->p_nplasmatorp;
    sound_other_plasmas = num_other_plasmas;
    num_other_plasmas = 0;
#endif

    /* show 'lock' icon on local map (Actually an EM hack ) */
    if (showLock & 2)
    {
        int tri_x = -1, tri_y = -1, facing = 0;
        int tri_size = 4;

        if (me->p_flags & PFPLOCK)
        {
            /* locked onto a ship */
            j = &players[me->p_playerl];
            if (!(j->p_flags & PFCLOAK))
            {
                dx = j->p_x - me->p_x;
                dy = j->p_y - me->p_y;
                if (ABS (dx) < view && ABS (dy) < view)
                {
                    dx = dx / SCALE + WINSIDE / 2;
                    dy = dy / SCALE + WINSIDE / 2;
                    tri_x = dx + 0;
                    tri_y = dy + 20;    /* below ship */
                    facing = 1;
                }
            }
        }
        else if (me->p_flags & PFPLLOCK)
        {
            /* locked onto a planet */
            struct planet *l = &planets[me->p_planet];

            dx = l->pl_x - me->p_x;
            dy = l->pl_y - me->p_y;
            if (ABS (dx) < view && ABS (dy) < view)
            {
                dx = dx / SCALE + WINSIDE / 2;
                dy = dy / SCALE + WINSIDE / 2;
                tri_x = dx;
                tri_y = dy - 20;        /* below planet */
                facing = 0;
            }
        }
        if (tri_x != -1)
        {
            W_WriteTriangle (w, tri_x, tri_y, 4, facing, foreColor);
            clearzone[0][clearcount] = tri_x - tri_size - 1;
            clearzone[1][clearcount] = tri_y - 1 + (facing ? 0 : -tri_size);
            clearzone[2][clearcount] = tri_size * 2 + 2;
            clearzone[3][clearcount] = tri_size + 2;
            clearcount++;
        }
    }
}


inline void
local (void)
/*
   Draw out the 'tactical' map
*/
{
    if (doubleBuffering)
        W_Win2Mem (localSDB);

    clearLocal ();

    /* Keep redrawing for double buffered observers who get set out of normal gameplay bounds,
       whether due to locking onto an ineligible planet, or observing a player who dies -
       otherwise screen doesn't refresh*/
    if (me->p_x < 0 && !(doubleBuffering && (me->p_flags & PFOBSERV)))
        return;

    DrawPlanets ();

    if (showStars)
        DrawStars();

    DrawShips ();

    DrawTorps ();
    DrawPlasmaTorps ();

    weaponUpdate = 0;
    DrawMisc ();

    if (doubleBuffering)
        W_Mem2Win (localSDB);
}


// Place this into local()
inline void
clearLocal (void)
/*
   Clear the local map (intelligently rather than just simply wiping
   the map).
*/
{
    if (W_FastClear)
    {
        W_ClearWindow (w);
        clearcount = 0;
        clearlcount = 0;
        tractcurrent = tracthead;
    }
    else
    {
        // Much more efficient way of clearing -- X programmers take note!
        W_ClearAreas (w, clearzone[0], clearzone[1], clearzone[2],
                      clearzone[3], clearcount);
        clearcount = 0;

      /* erase the tractor lines [BDyess] */
        for (tractrunner = tracthead; tractrunner != tractcurrent;
             tractrunner = tractrunner->next)
        {
            W_MakeTractLine (w, tractrunner->sx, tractrunner->sy,
                             tractrunner->d1x, tractrunner->d1y,
                             backColor);
            W_MakeTractLine (w, tractrunner->sx, tractrunner->sy,
                             tractrunner->d2x, tractrunner->d2y,
                             backColor);
        }
        tractcurrent = tracthead;

        W_MakeLines (w, clearline[0], clearline[1], clearline[2],
                     clearline[3], clearlcount, backColor);
        clearlcount = 0;
    }
}

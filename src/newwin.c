/******************************************************************************/
/***  File:  newwin.c
/***
/***  Function:
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <winsock.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "playerlist.h"
#include "bitmaps.h"
#include "parsemeta.h"
#include "packets.h"
#include "spopt.h"
#include "proto.h"

extern HINSTANCE MyInstance;
extern int metaHeight;   /* height of metaserver window */

static int line = 0;
int MaxMotdLine = 0;

#ifdef PARADISE
#define LINESPERPAGE        38
#else
#define LINESPERPAGE        28
#endif

#ifdef PARADISE
#define S_MOTD 0
#define S_SYSDEF 1
#define S_CREDITS 2
#define S_MAX 3
static void showPics (W_Window win, int atline);
int newMotdStuff = 0;	/* set to 1 when new motd packets arrive */
static struct piclist *motdPics = NULL;
static struct piclist **motd_buftail = &motdPics;
#endif

/* if a motd line from the server is this, the client will junk all motd *
 * data it currently has.  New data may be received */
#define MOTDCLEARLINE  "\033\030CLEAR_MOTD\000"

#define SIZEOF(a)       (sizeof (a) / sizeof (*(a)))

#define BOXSIDE         (TWINSIDE / 5)
#define MENU_PAD 4
#define TILESIDE        16
#define YOFF            0

/* Local function prototypes */
void loadbitmaps (void);
void loadbitmaps1 (void);
void loadbitmapsT (void);
void loadbitmapsM (void);
void loadbitmapsG (void);
void loadbitmapsHR (void);
#ifdef PARADISE
void loadbitmapsparadise (void);
void loadparadisethings (void);
#endif
void loadweaponsC (void);
void loadplanetsC (void);

/******************************************************************************/
/***   loadbitmaps(), loadbitmaps1(), loadbitmapsT(), loadbitmapsG(),
       loadbitmapsM()
/***   Called by savebitmaps(). */
/***
    If any bitmap files in a set are missing, default to the monoscale bitmaps
    for that set.
    If the monoscale bitmaps are missing, it's the users fault for deleting his/her
    bitmap files.
    This purpose of this code is preventative, as older versions of the client had
    different bitmap directories names and different bitmap sets.  If someone were
    to use a newer netrek executable without updating the bitmap libraries, this
    should keep the client working.
***/
/******************************************************************************/
void loadbitmaps()
{    
    int j,k;
    
    if ( access("bitmaps/shiplib/fedship.bmp", R_OK) == 0
     && access("bitmaps/shiplib/indship.bmp", R_OK) == 0
     && access("bitmaps/shiplib/kliship.bmp", R_OK) == 0
     && access("bitmaps/shiplib/oriship.bmp", R_OK) == 0
     && access("bitmaps/shiplib/romship.bmp", R_OK) == 0 )
    {
	    ship_bitmaps[0] =
	        W_StoreBitmap3 (fed_ship_bmp, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps[1] =
	        W_StoreBitmap3 (ind_ship_bmp, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps[2] =
	        W_StoreBitmap3 (kli_ship_bmp, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps[3] =
	        W_StoreBitmap3 (ori_ship_bmp, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps[4] =
	        W_StoreBitmap3 (rom_ship_bmp, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_DEFAULTCOLOR);
    }
    else /* default to mono */
    {
	    ship_bitmaps[0] =
	        W_StoreBitmap3 (fed_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps[1] =
	        W_StoreBitmap3 (ind_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps[2] =
	        W_StoreBitmap3 (kli_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps[3] =
	        W_StoreBitmap3 (ori_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps[4] =
	        W_StoreBitmap3 (rom_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_MONOCHROME);
    }
    
    for (j = 0; j < NUM_TYPES; j++)
    {
        for (k = 0; k < SHIP_VIEWS; k++)
        {
            fed_bitmaps[j][k] =
                W_PointBitmap2 (ship_bitmaps[0], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ind_bitmaps[j][k] =
                W_PointBitmap2 (ship_bitmaps[1], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            kli_bitmaps[j][k] =
                W_PointBitmap2 (ship_bitmaps[2], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ori_bitmaps[j][k] =
                W_PointBitmap2 (ship_bitmaps[3], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            rom_bitmaps[j][k] =
                W_PointBitmap2 (ship_bitmaps[4], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
        }
    }
}

void loadbitmaps1()
{
    int j,k;
    
    if ( access("bitmaps/shiplib/fedship1.bmp", R_OK) == 0
     && access("bitmaps/shiplib/indship1.bmp", R_OK) == 0
     && access("bitmaps/shiplib/kliship1.bmp", R_OK) == 0
     && access("bitmaps/shiplib/oriship1.bmp", R_OK) == 0
     && access("bitmaps/shiplib/romship1.bmp", R_OK) == 0 )
    {
	    ship_bitmaps1[0] =
	        W_StoreBitmap3 (fed_ship_bmp_1, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps1[1] =
	        W_StoreBitmap3 (ind_ship_bmp_1, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps1[2] =
	        W_StoreBitmap3 (kli_ship_bmp_1, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps1[3] =
	        W_StoreBitmap3 (ori_ship_bmp_1, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmaps1[4] =
	        W_StoreBitmap3 (rom_ship_bmp_1, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_DEFAULTCOLOR);
    }
    else /* default to mono */
    {
	    ship_bitmaps1[0] =
	        W_StoreBitmap3 (fed_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps1[1] =
	        W_StoreBitmap3 (ind_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps1[2] =
	        W_StoreBitmap3 (kli_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps1[3] =
	        W_StoreBitmap3 (ori_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmaps1[4] =
	        W_StoreBitmap3 (rom_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_MONOCHROME);
    }
    for (j = 0; j < NUM_TYPES; j++)
    {
        for (k = 0; k < SHIP_VIEWS; k++)
        {
            fed_bitmaps1[j][k] =
                W_PointBitmap2 (ship_bitmaps1[0], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ind_bitmaps1[j][k] =
                W_PointBitmap2 (ship_bitmaps1[1], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            kli_bitmaps1[j][k] =
                W_PointBitmap2 (ship_bitmaps1[2], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ori_bitmaps1[j][k] =
                W_PointBitmap2 (ship_bitmaps1[3], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            rom_bitmaps1[j][k] =
                W_PointBitmap2 (ship_bitmaps1[4], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
        }
    }
}
	
void loadbitmapsT()
{
    int j,k;
    
    if ( access("bitmaps/shiplib/fedshipT.bmp", R_OK) == 0
     && access("bitmaps/shiplib/indshipT.bmp", R_OK) == 0
     && access("bitmaps/shiplib/klishipT.bmp", R_OK) == 0
     && access("bitmaps/shiplib/orishipT.bmp", R_OK) == 0
     && access("bitmaps/shiplib/romshipT.bmp", R_OK) == 0 )
    {             
	    ship_bitmapsT[0] =
	        W_StoreBitmap3 (fed_ship_bmp_T, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsT[1] =
	        W_StoreBitmap3 (ind_ship_bmp_T, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsT[2] =
	        W_StoreBitmap3 (kli_ship_bmp_T, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsT[3] =
	        W_StoreBitmap3 (ori_ship_bmp_T, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsT[4] =
	        W_StoreBitmap3 (rom_ship_bmp_T, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_DEFAULTCOLOR);
    }
    else /* default to mono */
    {
	    ship_bitmapsT[0] =
	        W_StoreBitmap3 (fed_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsT[1] =
	        W_StoreBitmap3 (ind_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsT[2] =
	        W_StoreBitmap3 (kli_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsT[3] =
	        W_StoreBitmap3 (ori_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsT[4] =
	        W_StoreBitmap3 (rom_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_MONOCHROME);
    }
    for (j = 0; j < NUM_TYPES; j++)
    {
        for (k = 0; k < SHIP_VIEWS; k++)
        {
            fed_bitmapsT[j][k] =
                W_PointBitmap2 (ship_bitmapsT[0], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ind_bitmapsT[j][k] =
                W_PointBitmap2 (ship_bitmapsT[1], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            kli_bitmapsT[j][k] =
                W_PointBitmap2 (ship_bitmapsT[2], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ori_bitmapsT[j][k] =
                W_PointBitmap2 (ship_bitmapsT[3], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            rom_bitmapsT[j][k] =
                W_PointBitmap2 (ship_bitmapsT[4], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
        }
    }
}

void loadbitmapsG()
{
    int j,k;
    
    if ( access("bitmaps/shiplib/fedshipG.bmp", R_OK) == 0
     && access("bitmaps/shiplib/indshipG.bmp", R_OK) == 0
     && access("bitmaps/shiplib/klishipG.bmp", R_OK) == 0
     && access("bitmaps/shiplib/orishipG.bmp", R_OK) == 0
     && access("bitmaps/shiplib/romshipG.bmp", R_OK) == 0 )
    {
	    ship_bitmapsG[0] =
	        W_StoreBitmap3 (fed_ship_bmp_G, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsG[1] =
	        W_StoreBitmap3 (ind_ship_bmp_G, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsG[2] =
	        W_StoreBitmap3 (kli_ship_bmp_G, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsG[3] =
	        W_StoreBitmap3 (ori_ship_bmp_G, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsG[4] =
	        W_StoreBitmap3 (rom_ship_bmp_G, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_DEFAULTCOLOR);
    }
    else /* default to mono */
    {
	    ship_bitmapsG[0] =
	        W_StoreBitmap3 (fed_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsG[1] =
	        W_StoreBitmap3 (ind_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsG[2] =
	        W_StoreBitmap3 (kli_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsG[3] =
	        W_StoreBitmap3 (ori_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
	                        LR_MONOCHROME);
	    ship_bitmapsG[4] =
	        W_StoreBitmap3 (rom_ship_bmp_M, BMP_SHIP_WIDTH * 8,
	                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
	                        LR_MONOCHROME);
    }
    for (j = 0; j < NUM_TYPES; j++)
    {
        for (k = 0; k < SHIP_VIEWS; k++)
        {
            fed_bitmapsG[j][k] =
                W_PointBitmap2 (ship_bitmapsG[0], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ind_bitmapsG[j][k] =
                W_PointBitmap2 (ship_bitmapsG[1], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            kli_bitmapsG[j][k] =
                W_PointBitmap2 (ship_bitmapsG[2], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ori_bitmapsG[j][k] =
                W_PointBitmap2 (ship_bitmapsG[3], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            rom_bitmapsG[j][k] =
                W_PointBitmap2 (ship_bitmapsG[4], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
        }
    }
}
	
void loadbitmapsM()
{    
    int j,k;
    
    ship_bitmapsM[0] =
        W_StoreBitmap3 (fed_ship_bmp_M, BMP_SHIP_WIDTH * 8,
                        BMP_SHIP_HEIGHT * 32, BMP_FED_SHIP, w,
                        LR_MONOCHROME);
    ship_bitmapsM[1] =
        W_StoreBitmap3 (ind_ship_bmp_M, BMP_SHIP_WIDTH * 8,
                        BMP_SHIP_HEIGHT * 32, BMP_IND_SHIP, w,
                        LR_MONOCHROME);
    ship_bitmapsM[2] =
        W_StoreBitmap3 (kli_ship_bmp_M, BMP_SHIP_WIDTH * 8,
                        BMP_SHIP_HEIGHT * 32, BMP_KLI_SHIP, w,
                        LR_MONOCHROME);
    ship_bitmapsM[3] =
        W_StoreBitmap3 (ori_ship_bmp_M, BMP_SHIP_WIDTH * 8,
                        BMP_SHIP_HEIGHT * 32, BMP_ORI_SHIP, w,
                        LR_MONOCHROME);
    ship_bitmapsM[4] =
        W_StoreBitmap3 (rom_ship_bmp_M, BMP_SHIP_WIDTH * 8,
                        BMP_SHIP_HEIGHT * 32, BMP_ROM_SHIP, w,
                        LR_MONOCHROME);
                        
    for (j = 0; j < NUM_TYPES; j++)
    {
        for (k = 0; k < SHIP_VIEWS; k++)
        {
            fed_bitmapsM[j][k] =
                W_PointBitmap2 (ship_bitmapsM[0], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ind_bitmapsM[j][k] =
                W_PointBitmap2 (ship_bitmapsM[1], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            kli_bitmapsM[j][k] =
                W_PointBitmap2 (ship_bitmapsM[2], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            ori_bitmapsM[j][k] =
                W_PointBitmap2 (ship_bitmapsM[3], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
            rom_bitmapsM[j][k] =
                W_PointBitmap2 (ship_bitmapsM[4], j, k, BMP_SHIP_WIDTH,
                                BMP_SHIP_HEIGHT);
        }
    }               
}

#ifdef PARADISE
/******************************************************************************/
/***  loadbitmapsParadise()
/******************************************************************************/
void loadbitmapsParadise()
{
    int j, k;
                        
    paradise_ship_bitmaps =
        W_StoreBitmap3 ("bitmaps/paradise/paradise_ships.bmp", BMP_SHIP_WIDTH * NUM_PSHIP_TYPES,
                        BMP_SHIP_HEIGHT * NUMTEAMS, BMP_PSHIP, w, LR_MONOCHROME);
    paradise_cship_self_bitmaps =
        W_StoreBitmap3 ("bitmaps/paradise/paradise_cships_self.bmp", BMP_SHIP_WIDTH * NUM_PSHIP_TYPES,
                        BMP_SHIP_HEIGHT * NUMTEAMS, BMP_PSHIP, w, LR_DEFAULTCOLOR);
    paradise_cship_bitmaps =
        W_StoreBitmap3 ("bitmaps/paradise/paradise_cships.bmp", BMP_SHIP_WIDTH * NUM_PSHIP_TYPES,
                        BMP_SHIP_HEIGHT * NUMTEAMS, BMP_PSHIP, w, LR_DEFAULTCOLOR);

    for (j = 0; j < NUM_PSHIP_TYPES; j++)
    {
        for (k = 0; k < NUMTEAMS; k++)
        {
            paradise_ships[j][k] =
                W_PointBitmap2 (paradise_ship_bitmaps, j, k, BMP_SHIP_WIDTH,
                                    BMP_SHIP_HEIGHT);
            paradise_cships_self[j][k] =
                W_PointBitmap2 (paradise_cship_self_bitmaps, j, k, BMP_SHIP_WIDTH,
                                    BMP_SHIP_HEIGHT);
            paradise_cships[j][k] =
                W_PointBitmap2 (paradise_cship_bitmaps, j, k, BMP_SHIP_WIDTH,
                                    BMP_SHIP_HEIGHT);
        }
    }
}

/******************************************************************************/
/***  loadparadisethings() - misc paradise art like stars, wormholes, etc
/******************************************************************************/
void loadparadisethings (void)
{
    int i;

    base_star_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/star.bmp", BMP_STAR_WIDTH,
                        BMP_STAR_HEIGHT * STAR_VIEWS, BMP_STAR, w,
                        LR_DEFAULTCOLOR);
    for (i = 0; i < STAR_VIEWS; i++)
        star_bitmap[i] =
            W_PointBitmap2 (base_star_bitmap, 0, i, BMP_STAR_WIDTH, BMP_STAR_HEIGHT);

    star_mbitmap =
        W_StoreBitmap3 ("bitmaps/paradise/starm.bmp", BMP_MSTAR_WIDTH,
                        BMP_MSTAR_HEIGHT, BMP_MSTAR, mapw,
                        LR_DEFAULTCOLOR);

    drone_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/drone.bmp", BMP_DRONE_WIDTH,
                        BMP_DRONE_HEIGHT, BMP_DRONE, w, LR_MONOCHROME);
    base_dronec_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/droneC.bmp", BMP_DRONE_WIDTH * NUM_CTORP_TYPES,
                        BMP_DRONE_HEIGHT, BMP_DRONEC, w, LR_DEFAULTCOLOR);
    for (i = 0; i < NUM_CTORP_TYPES; i++)
        dronec_bitmap[i] =
            W_PointBitmap2 (base_dronec_bitmap, i, 0, BMP_DRONE_WIDTH, BMP_DRONE_HEIGHT);

    base_drone_explosion_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/dronecloud.bmp", BMP_DRONEDET_WIDTH,
                        BMP_DRONEDET_HEIGHT * BMP_DRONEDET_FRAMES, BMP_DRONECLOUD, w,
                        LR_MONOCHROME);
    for (i = 0; i < BMP_DRONEDET_FRAMES; i++)
        drone_explosion_bitmap[i] =
            W_PointBitmap2 (base_drone_explosion_bitmap, 0, i, BMP_DRONEDET_WIDTH, BMP_DRONEDET_HEIGHT);

    base_dronec_explosion_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/dronecloudC.bmp", BMP_DRONEDET_WIDTH,
                        BMP_DRONEDET_HEIGHT * BMP_DRONEDET_FRAMES, BMP_DRONECLOUD, w,
                        LR_DEFAULTCOLOR);
    for (i = 0; i < BMP_DRONEDET_FRAMES; i++)
        dronec_explosion_bitmap[i] =
            W_PointBitmap2 (base_dronec_explosion_bitmap, 0, i, BMP_DRONEDET_WIDTH, BMP_DRONEDET_HEIGHT);
}
#endif

/******************************************************************************/
/***  loadbitmapsHR() - high quality ship bitmaps, 80x80
/******************************************************************************/
void loadbitmapsHR()
{
    int j;

    if ( access("bitmaps/shiplib/fedshipHR.bmp", R_OK) == 0
     && access("bitmaps/shiplib/indshipHR.bmp", R_OK) == 0
     && access("bitmaps/shiplib/klishipHR.bmp", R_OK) == 0
     && access("bitmaps/shiplib/orishipHR.bmp", R_OK) == 0
     && access("bitmaps/shiplib/romshipHR.bmp", R_OK) == 0 )
    {
	    ship_bitmapsHR[0] =
	        W_StoreBitmap3 (fed_ship_bmp_HR, BMP_SHIP_WIDTH_HR * 8,
	                        BMP_SHIP_HEIGHT_HR, BMP_FED_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsHR[1] =
	        W_StoreBitmap3 (ind_ship_bmp_HR, BMP_SHIP_WIDTH_HR * 8,
	                        BMP_SHIP_HEIGHT_HR, BMP_IND_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsHR[2] =
	        W_StoreBitmap3 (kli_ship_bmp_HR, BMP_SHIP_WIDTH_HR * 8,
	                        BMP_SHIP_HEIGHT_HR, BMP_KLI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsHR[3] =
	        W_StoreBitmap3 (ori_ship_bmp_HR, BMP_SHIP_WIDTH_HR * 8,
	                        BMP_SHIP_HEIGHT_HR, BMP_ORI_SHIP, w,
	                        LR_DEFAULTCOLOR);
	    ship_bitmapsHR[4] =
	        W_StoreBitmap3 (rom_ship_bmp_HR, BMP_SHIP_WIDTH_HR * 8,
	                        BMP_SHIP_HEIGHT_HR, BMP_ROM_SHIP, w,
	                        LR_DEFAULTCOLOR);
    }
    else  // No bitmaps, doh!  Force to mono and don't allow swapping
    {
        LineToConsole("Failed to load high res ship bitmaps, defaulting to mono.\n");
        dynamicBitmaps = 0;
        colorClient = 0;
        return;
    }
    
    for (j = 0; j < NUM_TYPES; j++)
    {
        fed_bitmapsHR[j] =
            W_PointBitmap2 (ship_bitmapsHR[0], j, 0, BMP_SHIP_WIDTH_HR,
                                BMP_SHIP_HEIGHT_HR);
        ind_bitmapsHR[j] =
            W_PointBitmap2 (ship_bitmapsHR[1], j, 0, BMP_SHIP_WIDTH_HR,
                                BMP_SHIP_HEIGHT_HR);
        kli_bitmapsHR[j] =
            W_PointBitmap2 (ship_bitmapsHR[2], j, 0, BMP_SHIP_WIDTH_HR,
                                BMP_SHIP_HEIGHT_HR);
        ori_bitmapsHR[j] =
            W_PointBitmap2 (ship_bitmapsHR[3], j, 0, BMP_SHIP_WIDTH_HR,
                                BMP_SHIP_HEIGHT_HR);
        rom_bitmapsHR[j] =
            W_PointBitmap2 (ship_bitmapsHR[4], j, 0, BMP_SHIP_WIDTH_HR,
                                BMP_SHIP_HEIGHT_HR);
    }
}

/******************************************************************************/
/***  loadweaponsC() - colorized weapons
/******************************************************************************/
void loadweaponsC()
{    
    int j,k;

    /* Load the four 6x8 weapon bitmaps */
    torp_bitmaps =
        W_StoreBitmap3 ("bitmaps/weaplibm/color/torpC.bmp",
                        BMP_CTORP_WIDTH * NUM_CTORP_TYPES,
                        BMP_CTORP_HEIGHT * BMP_TORP_FRAMES, BMP_TORPC, w,
                        LR_DEFAULTCOLOR);
    mtorp_bitmaps =
        W_StoreBitmap3 ("bitmaps/weaplibm/color/mtorpC.bmp",
                        BMP_CTORP_WIDTH * NUM_CTORP_TYPES,
                        BMP_CTORP_HEIGHT * BMP_TORP_FRAMES, BMP_MTORPC, w,
                        LR_DEFAULTCOLOR);
    plasma_bitmaps =
        W_StoreBitmap3 ("bitmaps/weaplibm/color/plasmaC.bmp",
                        BMP_CPLASMATORP_WIDTH * NUM_CTORP_TYPES,
                        BMP_CPLASMATORP_HEIGHT * BMP_TORP_FRAMES, BMP_PLASMAC, w,
                        LR_DEFAULTCOLOR);
    mplasma_bitmaps =
        W_StoreBitmap3 ("bitmaps/weaplibm/color/mplasmaC.bmp",
                        BMP_CPLASMATORP_WIDTH * NUM_CTORP_TYPES,
                        BMP_CPLASMATORP_HEIGHT * BMP_TORP_FRAMES, BMP_MPLASMAC, w,
                        LR_DEFAULTCOLOR);
                        
    /* Make pointers to the four 6x8 weapon bitmaps */
    for (j = 0; j < NUM_CTORP_TYPES; j++)
    {        
    	for (k = 0; k < BMP_TORP_FRAMES; k++)
        {
            torpC[j][k] =
                W_PointBitmap2 (torp_bitmaps, j, k, BMP_CTORP_WIDTH,
                                BMP_CTORP_HEIGHT);
                            
            mtorpC[j][k] =
                W_PointBitmap2 (mtorp_bitmaps, j, k, BMP_CTORP_WIDTH,
                                BMP_CTORP_HEIGHT);
                            
            plasmaC[j][k] =
                 W_PointBitmap2 (plasma_bitmaps, j, k, BMP_CPLASMATORP_WIDTH,
                                BMP_CPLASMATORP_HEIGHT);
                            
            mplasmaC[j][k] =
                W_PointBitmap2 (mplasma_bitmaps, j, k, BMP_CPLASMATORP_WIDTH,
                                BMP_CPLASMATORP_HEIGHT);
        }
    }
        
           
    /* Load the two 6x5 cloud bitmaps */
    torp_cloud_bitmaps =
         W_StoreBitmap3 ("bitmaps/weaplibm/color/cloudC.bmp",
                        BMP_CTORPDET_WIDTH * NUM_CTORP_TYPES,
                        BMP_CTORPDET_HEIGHT * BMP_TORPDET_FRAMES, BMP_CLOUDC, w,
                        LR_DEFAULTCOLOR);
    plasma_cloud_bitmaps = 
         W_StoreBitmap3 ("bitmaps/weaplibm/color/plcloudC.bmp",
                        BMP_CPLASMATORPDET_WIDTH * NUM_CTORP_TYPES,
                        BMP_CPLASMATORPDET_HEIGHT * BMP_TORPDET_FRAMES, BMP_PLCLOUDC, w,
                        LR_DEFAULTCOLOR);
                        
    /* Make pointers to the two 6x5 cloud bitmaps */
    for (j = 0; j < NUM_CTORP_TYPES; j++)
    {
        for (k = 0; k < BMP_TORPDET_FRAMES; k++)
        {
            cloudC[j][k] =
                W_PointBitmap2 (torp_cloud_bitmaps, j, k, BMP_CTORPDET_WIDTH,
                                BMP_CTORPDET_HEIGHT);
            plcloudC[j][k] =
                W_PointBitmap2 (plasma_cloud_bitmaps, j, k, BMP_CPLASMATORPDET_WIDTH,
                                BMP_CPLASMATORPDET_HEIGHT);
        }
    }            
}

/******************************************************************************/
/***  loadplanetsC() - colorized planets
/******************************************************************************/
void loadplanetsC()
{    
    int j;

    /* Load the eight 5x1 planet bitmaps */
    planet_bitmaps[0] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/earth.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_EARTH, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[1] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/klingus.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_KLINGUS, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[2] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/orion.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ORION, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[3] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/romulus.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ROMULUS, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[4] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/agri1.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_AGRI1, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[5] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/agri2.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_AGRI2, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[6] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/rock1.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ROCK1, w,
                        LR_DEFAULTCOLOR);
    planet_bitmaps[7] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/rock2.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ROCK2, w,
                        LR_DEFAULTCOLOR);  
                                          
    /* Make pointers to the bitmaps */
    for (j = 0; j < NUMTEAMS; j++)
    {
    	planet_earth[j] =
    	    W_PointBitmap2 (planet_bitmaps[0], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_klingus[j]=
    	    W_PointBitmap2 (planet_bitmaps[1], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_orion[j]=
    	    W_PointBitmap2 (planet_bitmaps[2], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_romulus[j]=
    	    W_PointBitmap2 (planet_bitmaps[3], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_agri1[j]=
    	    W_PointBitmap2 (planet_bitmaps[4], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_agri2[j]=
    	    W_PointBitmap2 (planet_bitmaps[5], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_rock1[j]=
    	    W_PointBitmap2 (planet_bitmaps[6], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	planet_rock2[j]=
    	    W_PointBitmap2 (planet_bitmaps[7], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    }
                        
    /* Load the rotating unknown planet bitmap */
    planet_unknown_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/unknown.bmp",
                        BMP_CPLANET_WIDTH, BMP_CPLANET_HEIGHT * CPLANET_VIEWS,
                        BMP_PLANET_UNKNOWN, w, LR_DEFAULTCOLOR);
    
    /* Make pointers to the rotating unknown bitmaps */
    for (j = 0; j < CPLANET_VIEWS; j++)
    {
    	planet_unknown[j] =
    	    W_PointBitmap2 (planet_unknown_bitmap, 0, j, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    }
    /* Load the non-rotating unknown planet bitmap - use map version of bitmap */
    planet_unknown_NR =
        W_StoreBitmap3 ("bitmaps/planlibm/color/munknown.bmp",
                        BMP_CPLANET_WIDTH, BMP_CPLANET_HEIGHT, BMP_PLANET_UNKNOWN_NR, w,
                        LR_DEFAULTCOLOR);

    /* Load the resource bitmaps */
    army_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/army.bmp",
                        BMP_ARMY_WIDTH, BMP_ARMY_HEIGHT, BMP_ARMYBMP, w,
                        LR_DEFAULTCOLOR);
    wrench_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/wrench.bmp",
                        BMP_WRENCH_WIDTH, BMP_WRENCH_HEIGHT, BMP_WRENCHBMP, w,
                        LR_DEFAULTCOLOR);
#ifdef PARADISE
    gear_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/gear.bmp",
                        BMP_GEAR_WIDTH, BMP_GEAR_HEIGHT, BMP_GEARBMP, w,
                        LR_DEFAULTCOLOR);
#endif
    fuel_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/fuel.bmp",
                        BMP_FUEL_WIDTH, BMP_FUEL_HEIGHT, BMP_FUELBMP, w,
                        LR_DEFAULTCOLOR);
}

/******************************************************************************/
/***  loadmplanetsC() - colorized map planets, using same bitmaps but more efficient
                        drawing if we assign another set to map HWND
/******************************************************************************/
void loadmplanetsC()
{    
    int j;

    /* Load the eight 5x1 mplanet bitmaps */
    mplanet_bitmaps[0] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/earth.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_EARTH, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[1] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/klingus.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_KLINGUS, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[2] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/orion.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ORION, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[3] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/romulus.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ROMULUS, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[4] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/agri1.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_AGRI1, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[5] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/agri2.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_AGRI2, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[6] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/rock1.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ROCK1, mapw,
                        LR_DEFAULTCOLOR);
    mplanet_bitmaps[7] =
        W_StoreBitmap3 ("bitmaps/planlibm/color/rock2.bmp",
                        BMP_CPLANET_WIDTH * NUMTEAMS, BMP_CPLANET_HEIGHT, BMP_ROCK2, mapw,
                        LR_DEFAULTCOLOR);  
                                          
    /* Make pointers to the bitmaps */
    for (j = 0; j < NUMTEAMS; j++)
    {
    	mplanet_earth[j] =
    	    W_PointBitmap2 (mplanet_bitmaps[0], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_klingus[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[1], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_orion[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[2], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_romulus[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[3], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_agri1[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[4], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_agri2[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[5], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_rock1[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[6], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    	mplanet_rock2[j]=
    	    W_PointBitmap2 (mplanet_bitmaps[7], j, 0, BMP_CPLANET_WIDTH,
                            BMP_CPLANET_HEIGHT);
    }

    /* Load the unknown planet bitmap */
    mplanet_unknown =
        W_StoreBitmap3 ("bitmaps/planlibm/color/munknown.bmp",
                        BMP_CPLANET_WIDTH, BMP_CPLANET_HEIGHT, BMP_PLANET_MUNKNOWN, mapw,
                        LR_DEFAULTCOLOR);
    
    /* Load the resource bitmaps */
    marmy_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/army.bmp",
                        BMP_ARMY_WIDTH, BMP_ARMY_HEIGHT, BMP_ARMYBMP, mapw,
                        LR_DEFAULTCOLOR);
    mwrench_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/wrench.bmp",
                        BMP_WRENCH_WIDTH, BMP_WRENCH_HEIGHT, BMP_WRENCHBMP, mapw,
                        LR_DEFAULTCOLOR);
#ifdef PARADISE
   mgear_bitmap =
        W_StoreBitmap3 ("bitmaps/paradise/gear.bmp",
                        BMP_GEAR_WIDTH, BMP_GEAR_HEIGHT, BMP_GEARBMP, w,
                        LR_DEFAULTCOLOR);
#endif
    mfuel_bitmap =
        W_StoreBitmap3 ("bitmaps/planlibm/color/fuel.bmp",
                        BMP_FUEL_WIDTH, BMP_FUEL_HEIGHT, BMP_FUELBMP, mapw,
                        LR_DEFAULTCOLOR);
}

/******************************************************************************/
/***  handleMessageWindowKeyDown()
/******************************************************************************/
static void
handleMessageWindowKeyDown (W_Event * event)
{
    if (messageKeyOnly && messageon == 0)
    {
    	if (mystats->st_keymap[(event->key)-32] != 109) /* Not a message key (Key109)*/
    	    return;
    	else /* It is a message key, allow them to start a message */
    	    Key109();
    }
    else  /* Attempt to send message to whatever key was pressed */
        smessage (event->key);
}

/******************************************************************************/
/***  handleMessageWindowButton()
/******************************************************************************/
static void
handleMessageWindowButton (W_Event * event)
{
    switch (event->key)
    {
    case W_LBUTTON:
        if (messageon == 0)
        {
#ifdef SOUND
            Play_Sound(MESSAGE_WAV, SF_MESSAGE);
#endif
            message_on ();
            if (messpend == 0)
                smessage ('A');
        }
        return;
    default:
        return;
    }
}

/******************************************************************************/
/***  newwin()
/******************************************************************************/
void
newwin (char *hostmon,
        char *progname)
{
    int MESSAGESIZE, STATSIZE;
    int i;

    W_Initialize (hostmon);

    // We now know the font size
    MESSAGESIZE = 2 * W_Textheight;
    STATSIZE = (MESSAGESIZE * 2 + BORDER * 2);

    baseWin = W_MakeWindow ("netrek", 0, 0, 1024, 768, NULL, BORDER, gColor);

    w = W_MakeWindow ("local", 0, 0, TWINSIDE, TWINSIDE, baseWin, THICKBORDER, foreColor);

    localSDB = W_InitSDB (w);

    mapw = W_MakeWindow ("map", TWINSIDE + 2 * THICKBORDER, 0, GWINSIDE, GWINSIDE, baseWin,
                         THICKBORDER, foreColor);

    mapSDB = W_InitSDB (mapw);

    tstatw = W_MakeWindow ("tstat", 0, TWINSIDE + 2 * THICKBORDER, DashMaxWidth(),
                            STATSIZE, baseWin, BORDER, foreColor);

    W_SetWindowExposeHandler (tstatw, redrawTstats);

    messagew = W_MakeWindow ("message", TWINSIDE + 2 * THICKBORDER, GWINSIDE + 2 * THICKBORDER,
                             GWINSIDE + (2 * THICKBORDER - 2 * BORDER), MESSAGESIZE, baseWin, BORDER, foreColor);

    W_SetWindowKeyDownHandler (messagew, handleMessageWindowKeyDown);
    W_SetWindowButtonHandler (messagew, handleMessageWindowButton);
    W_SetWindowExposeHandler (messagew, DisplayMessage);
    
    warnw = W_MakeWindow ("warn", TWINSIDE + 2 * THICKBORDER, GWINSIDE + 2 * THICKBORDER + MESSAGESIZE + 2 * BORDER,
                          GWINSIDE + (2 * THICKBORDER - 2 * BORDER), MESSAGESIZE, baseWin, BORDER, foreColor);

    W_SetWindowKeyDownHandler (warnw, handleMessageWindowKeyDown);

#ifdef PARADISE
    planetw = W_MakeTextWindow ("planet", TWINSIDE + 2 * THICKBORDER + 10, 10, 70, MAXPLANETS + 8, baseWin, 2);
#else
    planetw = W_MakeTextWindow ("planet", TWINSIDE + 2 * THICKBORDER + 10, 10, 57, MAXPLANETS + 3, baseWin, 2);
#endif
    W_SetWindowExposeHandler (planetw, planetlist);

#ifdef PARADISE
    rankw = W_MakeTextWindow ("rank", 10, 100, 80, nranks2 + 9, baseWin, 2);
#else
    rankw = W_MakeTextWindow ("rank", 10, 300, 80, NUMRANKS + 9, baseWin, 2);
#endif
    W_SetWindowExposeHandler (rankw, ranklist);

    playerw = W_MakeTextWindow ("player", 0, TWINSIDE + 2 * THICKBORDER + STATSIZE + 2 * BORDER,
                                PlistMaxWidth (), MAXPLAYER + 3, baseWin, 2);
    W_SetWindowExposeHandler (playerw, RedrawPlayerList);

    playerw2 = W_MakeTextWindow ("player2", 140, 100, PlistMaxWidth2 (), MAXPLAYER + 3, baseWin, 2);
    W_SetWindowExposeHandler (playerw2, RedrawPlayerList);

#ifdef RECORDGAME
    if (playback)
        helpWin = W_MakeTextWindow ("help", 286,
                          TWINSIDE + 2 * THICKBORDER + STATSIZE + 2 * BORDER,
                          72, 15, NULL, BORDER);
    else
#endif
        helpWin = W_MakeTextWindow ("help", 20,
                          TWINSIDE + 2 * THICKBORDER + STATSIZE + 2 * BORDER - 5,
#ifdef PARADISE
                          160, 23, NULL, BORDER);
#else
                          160, 21, NULL, BORDER);
#endif

#ifdef RECORDGAME
    if (playback)
        W_SetWindowExposeHandler (helpWin, pbfillhelp);
    else
#endif
        W_SetWindowExposeHandler (helpWin, fillhelp);
    W_SetWindowKeyDownHandler (helpWin, helpaction);
    W_SetWindowButtonHandler (helpWin, helpaction);

#ifdef META
    metaWin = W_MakeMenu ("MetaServer List", 0, 0, 80, metaHeight,
                          NULL, 2);
    W_SetWindowKeyDownHandler (metaWin, metaaction);
    W_SetWindowButtonHandler (metaWin, metaaction);
#endif

    /* Message windows */
    if (richText
#ifdef RECORDGAME
        && !playback
#endif
    )
    {
        messwa = W_MakeScrollingRichTextWindow ("review_all", TWINSIDE + 2 * THICKBORDER,
                                                GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 8 * BORDER + 4 * MENU_PAD + 9 * W_Textheight,
                                                81, 5, baseWin, BORDER);
        wam_windows[0] = messwa;
        W_SetWindowKeyDownHandler (messwa, handleMessageWindowKeyDown);

        messwt = W_MakeScrollingRichTextWindow ("review_team", TWINSIDE + 2 * THICKBORDER,
                                                GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 6 * BORDER + 2 * MENU_PAD + 3 * W_Textheight,
                                                81, 6, baseWin, BORDER);
        wam_windows[1] = messwt;
        W_SetWindowKeyDownHandler (messwt, handleMessageWindowKeyDown);
   
        messwi = W_MakeScrollingRichTextWindow ("review_your", TWINSIDE + 2 * THICKBORDER,
                                                GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 4 * BORDER,
                                                81, 3, baseWin, BORDER);
        wam_windows[2] = messwi;
        W_SetWindowKeyDownHandler (messwi, handleMessageWindowKeyDown);
    
        messwk = W_MakeScrollingRichTextWindow ("review_kill", TWINSIDE + 2 * THICKBORDER,
                                                GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 10 * BORDER + 6 * MENU_PAD + 14 * W_Textheight,
                                                81, 3, baseWin, BORDER);
        wam_windows[3] = messwk;

        phaserwin = W_MakeScrollingRichTextWindow ("review_phaser", TWINSIDE + 2 * THICKBORDER,
                                                    GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 12 * BORDER + 8 * MENU_PAD + 17 * W_Textheight,
                                                    81, 4, baseWin, BORDER);
        wam_windows[4] = phaserwin;

        reviewWin = W_MakeScrollingRichTextWindow ("review", TWINSIDE + 2 * THICKBORDER,
                                                   GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 4 * BORDER,
                                                   81, 20, baseWin, BORDER);
        wam_windows[5] = reviewWin;
        W_SetWindowKeyDownHandler (reviewWin, handleMessageWindowKeyDown);
    }
    else
    {
        messwa = W_MakeScrollingWindow ("review_all", TWINSIDE + 2 * THICKBORDER,
                                        GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 8 * BORDER + 4 * MENU_PAD + 9 * W_Textheight,
                                        81, 5, baseWin, BORDER);
        wam_windows[0] = messwa;
        W_SetWindowKeyDownHandler (messwa, handleMessageWindowKeyDown);

        messwt = W_MakeScrollingWindow ("review_team", TWINSIDE + 2 * THICKBORDER,
                                        GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 6 * BORDER + 2 * MENU_PAD + 3 * W_Textheight,
                                        81, 6, baseWin, BORDER);
        wam_windows[1] = messwt;
        W_SetWindowKeyDownHandler (messwt, handleMessageWindowKeyDown);
   
        messwi = W_MakeScrollingWindow ("review_your", TWINSIDE + 2 * THICKBORDER,
                                        GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 4 * BORDER,
                                        81, 3, baseWin, BORDER);
        wam_windows[2] = messwi;
        W_SetWindowKeyDownHandler (messwi, handleMessageWindowKeyDown);
    
        messwk = W_MakeScrollingWindow ("review_kill", TWINSIDE + 2 * THICKBORDER,
                                        GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 10 * BORDER + 6 * MENU_PAD + 14 * W_Textheight,
                                        81, 3, baseWin, BORDER);
        wam_windows[3] = messwk;

        phaserwin = W_MakeScrollingWindow ("review_phaser", TWINSIDE + 2 * THICKBORDER,
                                           GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 12 * BORDER + 8 * MENU_PAD + 17 * W_Textheight,
                                           81, 4, baseWin, BORDER);
        wam_windows[4] = phaserwin;

        reviewWin = W_MakeScrollingWindow ("review", TWINSIDE + 2 * THICKBORDER,
                                           GWINSIDE + 2 * THICKBORDER + 2 * MESSAGESIZE + 4 * BORDER,
                                           81, 20, baseWin, BORDER);
        wam_windows[5] = reviewWin;
        W_SetWindowKeyDownHandler (reviewWin, handleMessageWindowKeyDown);
    }
    /* End of Message windows */
    /* Now let's set Window Allowed Messages for all message windows */
    for (i = 0; i < 6; i++)
        W_SetWAM (wam_windows[i]);

    pStats = W_MakeWindow ("pingStats", TWINSIDE + 10, 4, pStatsWidth (), pStatsHeight (),
                            baseWin, 1, foreColor);
    W_SetWindowExposeHandler (pStats, redrawPStats);

    udpWin = W_MakeMenu ("UDP", TWINSIDE + 10, -BORDER + 10, 40, UDP_NUMOPTS, NULL, 2);
    W_SetWindowButtonHandler (udpWin, udpaction);

#ifdef SHORT_PACKETS
    spWin = W_MakeMenu ("network", TWINSIDE + 10, -BORDER + 10, 40, SPK_NUMFIELDS, NULL, 2);
    W_SetWindowKeyDownHandler (spWin, spaction);
    W_SetWindowButtonHandler (spWin, spaction);
#endif

#if defined(SOUND)
    soundWin = W_MakeMenu("sound", TWINSIDE + 20, -BORDER + 10, 33,
                        sound_window_height(), NULL, 2);
    W_SetWindowKeyDownHandler(soundWin, soundaction);
    W_SetWindowButtonHandler(soundWin, soundaction);
    W_DefineArrowCursor(soundWin);
#endif

#ifdef TOOLS
    toolsWin = W_MakeScrollingWindow ("tools", TWINSIDE + BORDER, BORDER,
                                        80, TOOLSWINLEN, NULL, BORDER);
    W_DefineTrekCursor (toolsWin);
#endif

#ifdef XTREKRC_HELP
    defWin = W_MakeTextWindow ("xtrekrc_help", 1, 100, 174, 41, NULL, BORDER);
#endif

#ifdef DOC_WIN
    docwin = W_MakeWindow ("DocWin", 0, 181,
                           TWINSIDE * W_Textwidth / 6,
                           TWINSIDE,
                           0, 2, foreColor);
    xtrekrcwin = W_MakeWindow ("xtrekrcWin", 0, 200,
                                TWINSIDE * W_Textwidth / 6,
                                TWINSIDE,
                                0, 2, foreColor);
#endif

    for (i = 0; i < 4; i++)
    {
        teamWin[i] = W_MakeWindow (teamshort[1 << i], i * BOXSIDE, TWINSIDE - BOXSIDE,
                                    BOXSIDE, BOXSIDE, w, 1, foreColor);
    }
    qwin = W_MakeWindow ("quit", 4 * BOXSIDE, TWINSIDE - BOXSIDE, BOXSIDE,
                         BOXSIDE, w, 1, foreColor);

    statwin = W_MakeWindow ("stats", TWINSIDE + 2 * THICKBORDER - 100, TWINSIDE + 2 * THICKBORDER,
                            StatsWidth(), StatsHeight(), baseWin, BORDER, foreColor);
    W_SetWindowExposeHandler (statwin, redrawStats);

    W_DefineTrekCursor (baseWin);
    W_DefineLocalcursor (w);
    W_DefineMapcursor (mapw);
    W_DefineTrekCursor (pStats);
    W_DefineTextCursor (warnw);
    W_DefineTextCursor (messagew);
    W_DefineTrekCursor (messwa);
    W_DefineTrekCursor (messwt);
    W_DefineTrekCursor (messwi);
    W_DefineTrekCursor (messwk);
    W_DefineTrekCursor (reviewWin);
    W_DefineTrekCursor (phaserwin);
    W_DefineTrekCursor (helpWin);

#ifdef META
    W_DefineArrowCursor (metaWin);
#endif

    W_DefineTrekCursor (playerw);
    W_DefineTrekCursor (playerw2);
    W_DefineTrekCursor (rankw);
    W_DefineTrekCursor (statwin);
    W_DefineTrekCursor (tstatw);
    W_DefineWarningCursor (qwin);
    W_DefineArrowCursor (udpWin);

#ifdef SHORT_PACKETS
    W_DefineArrowCursor (spWin);
#endif

/* SRS these are not defined? Oh, because it's a inline def... grrr*/
    W_DefineFedCursor (teamWin[0]);
    W_DefineRomCursor (teamWin[1]);
    W_DefineKliCursor (teamWin[2]);
    W_DefineOriCursor (teamWin[3]);

#define WARHEIGHT 2
#define WARWIDTH 20
#define WARBORDER 2

    war = W_MakeMenu ("war", TWINSIDE + 10, -BORDER + 10, WARWIDTH, 6, baseWin, WARBORDER);
    W_SetWindowButtonHandler (war, waraction);
    W_DefineArrowCursor (war);

    fillhint ();

    getResources (progname);
}

/******************************************************************************/
/***  mapAll()
/******************************************************************************/
void
mapAll (void)
{
    initinput ();
    W_MapWindow (mapw);
    W_MapWindow (tstatw);
    W_MapWindow (warnw);
#ifdef XTRA_MESSAGE_UI
    if (checkMappedPref ("message", 1))
#endif
        W_MapWindow (messagew);
    W_MapWindow (w);
    W_MapWindow (baseWin);
    /* since we aren't mapping windows that have root as parent in x11window.c
     * (since that messes up the TransientFor feature) we have to map them
     * here. (If already mapped, W_MapWindow returns) */

    if (checkMapped ("planet"))
        W_MapWindow (planetw);
    if (checkMapped ("rank"))
        W_MapWindow (rankw);
    if (checkMapped ("help"))
        W_MapWindow (helpWin);
    if (checkMapped ("review_all"))
        W_MapWindow (messwa);
    if (checkMapped ("review_team"))
        W_MapWindow (messwt);
    if (checkMapped ("review_your"))
        W_MapWindow (messwi);
    if (checkMapped ("review_kill"))
        W_MapWindow (messwk);
    if (checkMapped ("review_phaser"))
    {
        W_MapWindow (phaserwin);
        phaserWindow = 1;
    }
    if (checkMapped ("player2"))
        W_MapWindow (playerw2);
    if (checkMappedPref ("player", 1))
        W_MapWindow (playerw);
    if (checkMappedPref ("review", 1))
        W_MapWindow (reviewWin);
    if (checkMapped ("UDP"))
        udpwindow ();

#ifdef SHORT_PACKETS
    if (checkMapped ("network"))
        spwindow ();
#endif
    if (checkMapped ("stats"))
        W_MapWindow (statwin);
}


/******************************************************************************/
/***  savebitmaps()
/******************************************************************************/
void
savebitmaps (void)
{
    int i, k;
    char *Planlib;
    char *MPlanlib;

    planetBitmap = intDefault ("planetBitmap", planetBitmap);
    planetBitmapGalaxy = intDefault ("planetBitmapGalaxy", planetBitmapGalaxy);
    rotatePlanets = booleanDefault ("rotatePlanets", rotatePlanets);
    loadplanetsC();  // Always load new color planet bitmaps..for now
    loadmplanetsC();
#ifdef PARADISE
    loadparadisethings();
#endif
#ifdef PARADISE
    Planlib = "bitmaps/paradise/paradise_plan.bmp";
    MPlanlib = "bitmaps/paradise/paradise_mplan.bmp";
#else
    switch (planetBitmap) // Case 3 = new color, but we never use Planlib
    {
    case 1:
        Planlib = "bitmaps/planlibm/planM.bmp";
        break;
    case 2:
        Planlib = "bitmaps/planlibm/planR.bmp";
        break;
    default:
        Planlib = "bitmaps/planlibm/plan.bmp";
        break;
    }
    switch (planetBitmapGalaxy) // Case 3 = new color, but we never use MPlanlib
    {
    case 1:
        MPlanlib = "bitmaps/planlibm/mplanM.bmp";
        break;
    case 2:
        MPlanlib = "bitmaps/planlibm/mplanR.bmp";
        break;
    default:
        MPlanlib = "bitmaps/planlibm/mplan.bmp";
        break;
    }
#endif
 
    if (!dynamicBitmaps) /* Only load needed bitmaps */
    {
    	switch (colorClient)
    	{
    		case 1: /* New color bitmaps */
    			loadbitmaps1();
    			break;
    		case 2: /* Old color bitmaps + greyscale for player's ship */
    			loadbitmaps();
    			loadbitmapsG();
    			break;
    		case 3: /* Shaded color bitmaps + greyscale for player's ship */
    			loadbitmapsT();
    			loadbitmapsG();
    			break;
                case 4: /* HR bitmaps */
                    	loadbitmapsHR();
    			break;
    		default: /* mono */
    			loadbitmapsM();
    			break;	
	}
#ifdef PARADISE
        loadbitmapsParadise();
#endif
    }
    else /* Load all bitmaps */
    {
    	loadbitmaps();
    	loadbitmaps1();
    	loadbitmapsG();
    	loadbitmapsT();
    	loadbitmapsM();
    	loadbitmapsHR();
#ifdef PARADISE
        loadbitmapsParadise();
#endif
    }
    
#ifdef BEEPLITE
            
    beeplite1 = 
        W_StoreBitmap3 ("bitmaps/misclib/beepliteplayerm.bmp", emph_player_seq_width,
                        emph_player_seq_height * emph_player_seq_frames, BMP_BEEPLITE1,
                        w, LR_MONOCHROME);       

    for (i = 0; i < emph_player_seq_frames; i++)
        emph_player_seq[emph_player_seq_frames - (i + 1)] =
            W_PointBitmap2 (beeplite1, 0, i, emph_player_seq_width, emph_player_seq_height);

    beeplite2 = 
        W_StoreBitmap3 ("bitmaps/misclib/beepliteplayerl.bmp", emph_player_seql_width,
                        emph_player_seql_height * emph_player_seql_frames, BMP_BEEPLITE2,
                        w, LR_MONOCHROME);      
 
    for (i = 0; i < emph_player_seql_frames; i++)
        emph_player_seql[emph_player_seql_frames - (i + 1)] =
            W_PointBitmap2 (beeplite2, 0, i, emph_player_seql_width, emph_player_seql_height);

    beeplite3 = 
        W_StoreBitmap3 ("bitmaps/misclib/beepliteplanet.bmp", emph_planet_seq_width,
                        emph_planet_seq_height * emph_planet_seq_frames, BMP_BEEPLITE3,
                        w, LR_MONOCHROME);

    for (i = 0; i < emph_planet_seq_frames; i++)
        emph_planet_seq[emph_planet_seq_frames - (i + 1)] =
            W_PointBitmap2 (beeplite3, 0, i, emph_planet_seq_width, emph_planet_seq_height);

#endif


    loadweaponsC();

    cloud[0] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/cloud1.bmp", BMP_TORPDET_WIDTH,
                        BMP_TORPDET_HEIGHT, BMP_CLOUD, w, LR_MONOCHROME);
    cloud[1] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/cloud2.bmp", BMP_TORPDET_WIDTH,
                        BMP_TORPDET_HEIGHT, BMP_CLOUD, w, LR_MONOCHROME);
    cloud[2] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/cloud3.bmp", BMP_TORPDET_WIDTH,
                        BMP_TORPDET_HEIGHT, BMP_CLOUD, w, LR_MONOCHROME);
    cloud[3] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/cloud4.bmp", BMP_TORPDET_WIDTH,
                        BMP_TORPDET_HEIGHT, BMP_CLOUD, w, LR_MONOCHROME);
    cloud[4] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/cloud5.bmp", BMP_TORPDET_WIDTH,
                        BMP_TORPDET_HEIGHT, BMP_CLOUD, w, LR_MONOCHROME);

    plasmacloud[0] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/plcloud1.bmp",
                        BMP_PLASMATORPDET_WIDTH, BMP_PLASMATORPDET_HEIGHT,
                        BMP_PLCLOUD, w, LR_MONOCHROME);
    plasmacloud[1] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/plcloud2.bmp",
                        BMP_PLASMATORPDET_WIDTH, BMP_PLASMATORPDET_HEIGHT,
                        BMP_PLCLOUD, w, LR_MONOCHROME);
    plasmacloud[2] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/plcloud3.bmp",
                        BMP_PLASMATORPDET_WIDTH, BMP_PLASMATORPDET_HEIGHT,
                        BMP_PLCLOUD, w, LR_MONOCHROME);
    plasmacloud[3] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/plcloud4.bmp",
                        BMP_PLASMATORPDET_WIDTH, BMP_PLASMATORPDET_HEIGHT,
                        BMP_PLCLOUD, w, LR_MONOCHROME);
    plasmacloud[4] =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/plcloud5.bmp",
                        BMP_PLASMATORPDET_WIDTH, BMP_PLASMATORPDET_HEIGHT,
                        BMP_PLCLOUD, w, LR_MONOCHROME);

    etorp =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/etorp.bmp", BMP_TORP_WIDTH,
                        BMP_TORP_HEIGHT, BMP_ETORP, w, LR_MONOCHROME);
    mtorp =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/mtorp.bmp", BMP_TORP_WIDTH,
                        BMP_TORP_HEIGHT, BMP_MTORP, w, LR_MONOCHROME);
    eplasmatorp =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/eplasma.bmp", BMP_EPLASMA_WIDTH,
                        BMP_EPLASMA_HEIGHT, BMP_EPLASMA, w, LR_MONOCHROME);
    mplasmatorp =
        W_StoreBitmap3 ("bitmaps/weaplibm/mono/mplasma.bmp", BMP_MPLASMA_WIDTH,
                        BMP_MPLASMA_HEIGHT, BMP_MPLASMA, w, LR_MONOCHROME);
                        
    base_planets =
        W_StoreBitmap3 (Planlib, BMP_PLANET_WIDTH, BMP_PLANET_HEIGHT * PLANET_VIEWS,
                        BMP_PLANET000, w, LR_MONOCHROME);
    base_mplanets =
        W_StoreBitmap3 (MPlanlib, BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT * PLANET_VIEWS,
                        BMP_MPLANET000, mapw, LR_MONOCHROME);

    for (k = 0; k < PLANET_VIEWS; k++)
    {
        bplanets[k] =
            W_PointBitmap2 (base_planets, 0, k, BMP_PLANET_WIDTH,
                            BMP_PLANET_HEIGHT);
        bmplanets[k] =
            W_PointBitmap2 (base_mplanets, 0, k, BMP_MPLANET_WIDTH,
                            BMP_MPLANET_HEIGHT);
    }

    if (colorClient > 0)
    {
        base_expview =
            W_StoreBitmap3 ("bitmaps/misclib/shexpl.bmp", BMP_SHIPEXPL_WIDTH,
                            BMP_SHIPEXPL_HEIGHT * BMP_SHIPEXPL_FRAMES,
                            BMP_SHIP_EXPLOSION, w, LR_DEFAULTCOLOR);
        base_sbexpview =
            W_StoreBitmap3 ("bitmaps/misclib/sbexpl.bmp", BMP_SBEXPL_WIDTH,
                            BMP_SBEXPL_HEIGHT * BMP_SBEXPL_FRAMES,
                            BMP_SB_EXPLOSION, w, LR_DEFAULTCOLOR);
    }
    else
    {
        base_expview =
            W_StoreBitmap3 ("bitmaps/misclib/shexplM.bmp", BMP_SHIPEXPL_WIDTH,
                            BMP_SHIPEXPL_HEIGHT * BMP_SHIPEXPL_FRAMES,
                            BMP_SHIP_EXPLOSION, w, LR_DEFAULTCOLOR);
        base_sbexpview =
            W_StoreBitmap3 ("bitmaps/misclib/sbexplM.bmp", BMP_SBEXPL_WIDTH,
                            BMP_SBEXPL_HEIGHT * BMP_SBEXPL_FRAMES,
                            BMP_SB_EXPLOSION, w, LR_DEFAULTCOLOR);
    }

    for (i = 0; i < BMP_SHIPEXPL_FRAMES; i++)
    {
        expview[i] =
            W_PointBitmap2 (base_expview, 0, i, BMP_SHIPEXPL_WIDTH,
                            BMP_SHIPEXPL_HEIGHT);
    }
    for (i = 0; i < BMP_SBEXPL_FRAMES; i++)
    {
        sbexpview[i] =
            W_PointBitmap2 (base_sbexpview, 0, i, BMP_SBEXPL_WIDTH,
                            BMP_SBEXPL_HEIGHT);
    }

#ifndef VSHIELD_BITMAPS
    shield =
        W_StoreBitmap3 ("bitmaps/misclib/shield.bmp", BMP_SHIELD_WIDTH,
                        BMP_SHIELD_HEIGHT, BMP_SHIELD, w, LR_MONOCHROME);
#else
    base_vshield =
        W_StoreBitmap3 ("bitmaps/misclib/vshield.bmp", BMP_SHIELD_WIDTH,
                        BMP_SHIELD_HEIGHT * SHIELD_FRAMES, BMP_SHIELD, w,
                        LR_MONOCHROME);
    for (i = 0; i < SHIELD_FRAMES; i++)
        shield[i] =
            W_PointBitmap2 (base_vshield, 0, i, BMP_SHIELD_WIDTH, BMP_SHIELD_HEIGHT);
#endif

    base_hull =
        W_StoreBitmap3 ("bitmaps/misclib/varyhull.bmp", hull_width,
                        hull_height * BMP_HULL_FRAMES, BMP_HULL, w,
                        LR_MONOCHROME);
    for (i = 0; i < BMP_HULL_FRAMES; i++)
        hull[i] =
            W_PointBitmap2 (base_hull, 0, i, hull_width, hull_height);

    cloakicon =
        W_StoreBitmap3 ("bitmaps/misclib/cloak.bmp", BMP_CLOAK_WIDTH,
                        BMP_CLOAK_HEIGHT, BMP_CLOAK, w, LR_MONOCHROME);
    stipple =
        W_StoreBitmap3 ("bitmaps/misclib/stipple.bmp", BMP_STIPPLE_WIDTH,
                        BMP_STIPPLE_HEIGHT, BMP_STIPPLE, w, LR_MONOCHROME);
    if (newQuit)
    {
        clockpic =
            W_StoreBitmap3 ("bitmaps/misclib/color/clock.bmp", BMP_CCLOCK_WIDTH,
                            BMP_CCLOCK_HEIGHT, BMP_CLOCK, qwin, LR_DEFAULTCOLOR);
        clockhandpic =
            W_StoreBitmap3 ("bitmaps/misclib/color/clockhand.bmp", BMP_CHAND_WIDTH,
                            BMP_CHAND_HEIGHT, BMP_CLOCKHAND, qwin, LR_DEFAULTCOLOR);                          
    }
    else
    {
        clockpic =
            W_StoreBitmap3 ("bitmaps/misclib/clock.bmp", BMP_CLOCK_WIDTH,
                            BMP_CLOCK_HEIGHT, BMP_CLOCK, qwin, LR_MONOCHROME);
    }

    fedteam =
        W_StoreBitmap3 ("bitmaps/misclib/color/fedteam.bmp", BMP_TEAM_SELECT_WIDTH,
                        BMP_TEAM_SELECT_HEIGHT, BMP_FEDTEAM, w, LR_DEFAULTCOLOR);
    romteam =
        W_StoreBitmap3 ("bitmaps/misclib/color/romteam.bmp", BMP_TEAM_SELECT_WIDTH,
                        BMP_TEAM_SELECT_HEIGHT, BMP_ROMTEAM, w, LR_DEFAULTCOLOR);
    kliteam =
        W_StoreBitmap3 ("bitmaps/misclib/color/kliteam.bmp", BMP_TEAM_SELECT_WIDTH,
                        BMP_TEAM_SELECT_HEIGHT, BMP_KLITEAM, w, LR_DEFAULTCOLOR);
    oriteam =
        W_StoreBitmap3 ("bitmaps/misclib/color/oriteam.bmp", BMP_TEAM_SELECT_WIDTH,
                        BMP_TEAM_SELECT_HEIGHT, BMP_ORITEAM, w, LR_DEFAULTCOLOR);
    genopic =
        W_StoreBitmap3 ("bitmaps/misclib/color/genocide.bmp", BMP_GENO_WIDTH,
                        BMP_GENO_HEIGHT, BMP_GENO, w, LR_DEFAULTCOLOR);

    genopic2 =
        W_StoreBitmap3 ("bitmaps/misclib/color/genocide2.bmp", BMP_GENO_WIDTH,
                        BMP_GENO_HEIGHT, BMP_GENO2, w, LR_DEFAULTCOLOR);                      
}


/******************************************************************************/
/***  entrywindow()
/***   This routine throws up an entry window for the player. */
/******************************************************************************/
void
entrywindow (int *team,
             int *s_type)
{
    int typeok = 0, i = 0;
    time_t startTime;
    W_Event event;
    int lastplayercount[4];
    int okayMask, lastOkayMask;
    int resetting = 0;
    int tiled = 0;
    time_t lasttime = -1;
    time_t spareTime = 1000;    /* Allow them an extra 240
                                 * seconds, as LONG */

    /* fast quit? - jn */
    if (fastQuit)
    {
        *team = -1;
        return;
    }


    /* The following allows quick choosing of teams */

    lastOkayMask = okayMask = tournMask;

    for (i = 0; i < 4; i++)
    {
        if (okayMask & (1 << i))
        {
            tiled = 0;
        }
        else
        {
            tiled = 1;
        }

        if (tiled)
        {
            W_TileWindow (teamWin[i], stipple);
        }
        else
        {
            W_UnTileWindow (teamWin[i]);
        }
        W_MapWindow (teamWin[i]);
        lastplayercount[i] = -1;        /* force redraw first time
                                         * through */
    }

    light_erase();

    W_MapWindow (qwin);

    *team = -1;
    startTime = time (0);
    if (me->p_whydead != KWINNER && me->p_whydead != KGENOCIDE)
        showMotdWin (w, line);

    updatedeath ();

    if (remap[me->p_team] == NOBODY)
    {
    	/* When you first login */
        char buf[80];

        RedrawPlayerList ();
        sprintf (buf, "Netrek - %s(%c)@%s", me->p_name, me->p_mapchars[1], serverName);
        W_RenameWindow (baseWin, buf);
    }
    else
        UpdatePlayerList ();    /* Otherwise */


    autoQuit = intDefault ("autoQuit", autoQuit);      /* allow extra */
    /* quit time -RW */

    do
    {
        while (!W_EventsPending ())
        {
            time_t elapsed;
            fd_set rfds;
            struct timeval tv;

            me->p_ghostbuster = 0;
            /* Since we don't have a socket to check on Win32
               for windowing system events, we set the timeout to zero and
               effectively poll. Yes, I could do the correct thing
               and call WaitForMultipleObjects() etc. but I don't feel like it */
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            FD_ZERO (&rfds);
            FD_SET (sock, &rfds);
            if (udpSock >= 0)
                FD_SET (udpSock, &rfds);
            /* Select() causes infinite CPU usage, can't use WaitForMultipleObjects
               due to UI working on same thread (no blocking), so a simple Sleep
               is the best solution. */
            Sleep(10);
            select (32, &rfds, 0, 0, &tv);      /* hmm,  32 might be too
                                                 * small */
            if (FD_ISSET (sock, &rfds) ||
                (udpSock >= 0 && FD_ISSET (udpSock, &rfds)))
            {
                readFromServer (&rfds);
                light_erase();
            }
            elapsed = time (0) - startTime;
            if (elapsed > (time_t)(autoQuit))
            {
                LineToConsole ("Auto-Quit.\n");
                *team = 4;
                break;
            }
            if (lasttime != time (0))
            {
                updatedeath ();
                if (W_IsMapped (playerw))
                    UpdatePlayerList ();
#ifdef PARADISE
                if (newMotdStuff)
                {
                    showMotdWin (w, line);
                    //showValues(mapw);
                }
#endif
                showTimeLeft (elapsed, autoQuit);
                lasttime = time (0);
            }

            okayMask = tournMask;

            for (i = 0; i < 4; i++)
            {
                if ((okayMask ^ lastOkayMask) & (1 << i))
                {
                    if (okayMask & (1 << i))
                    {
                        W_UnTileWindow (teamWin[i]);
                    }
                    else
                    {
                        W_TileWindow (teamWin[i], stipple);
                    }
                    lastplayercount[i] = -1;    /* force update */
                }
                redrawTeam (teamWin[i], i, &lastplayercount[i]);
            }
            lastOkayMask = okayMask;
        }
        if (*team == 4)
            break;

        if (time (0) - startTime <= spareTime)
        {
            spareTime -= time (0) - startTime;
            startTime = time (0);
        }
        else
        {
            startTime += spareTime;
            spareTime = 0;
        }
        if (!W_EventsPending ())
            continue;
        W_NextEvent (&event);
        typeok = 1;
        switch ((int) event.type)
        {
        case W_EV_KEY:
            switch (event.key)
            {
            case 's':
                *s_type = SCOUT;
                break;
            case 'd':
                *s_type = DESTROYER;
                break;
            case 'c':
                *s_type = CRUISER;
                break;
            case 'b':
                *s_type = BATTLESHIP;
                break;
            case 'g':
                *s_type = SGALAXY;
                break;
            case '*':
                *s_type = ATT;
                break;
            case 'a':
                *s_type = ASSAULT;
                break;
            case 'o':
                *s_type = STARBASE;
                break;
#ifdef PARADISE
            case 'j':
                *s_type = JUMPSHIP;
                break;
            case 'f':
                *s_type = FLAGSHIP;
                break;    
            case 'w':
                *s_type = WARBASE;
                break;
            case 'l':
                *s_type = LIGHTCRUISER;
                break;        
            case 'v':
                *s_type = CARRIER;
                break;
            case 'u':
                *s_type = UTILITY;
                break;
            case 'p':
                *s_type = PATROL;
                break;
#endif
            case ' ':
                switch (me->p_team)
                {
                case FED:
                    *team = 0;
                    break;
                case ROM:
                    *team = 1;
                    break;
                case KLI:
                    *team = 2;
                    break;
                case ORI:
                    *team = 3;
                    break;
                default:
                    break;
                }
                break;
            default:
                typeok = 0;
                break;
            }
            if (event.Window == w)
            {
                switch (event.key)
                {
                case 'y':
                    if (resetting)
                    {
                        sendResetStatsReq ('Y');
                        warning
                            ("OK, your reset request has been sent to the server.");
                        resetting = 0;
                    }
                    break;
                case 'n':
                    if (resetting)
                    {
                        warning ("Yeah, WHATever.");
                        resetting = 0;
                    }
                    break;
                case 'R':
                    warning ("Please confirm reset request. (y/n)");
                    resetting = 1;
                    break;
                case 'f':      /* Scroll motd forward */
                    line = line + LINESPERPAGE;
                    if (line > MaxMotdLine)
                    {
                        line = line - LINESPERPAGE;
                        break;
                    }
                    W_ClearWindow (w);
                    showMotdWin (w, line);
                    break;
                case 'b':      /* Scroll motd backward */
                    if (line == 0)
                        break;
                    line = line - LINESPERPAGE;
                    if (line < 0)
                        line = 0;
                    W_ClearWindow (w);
                    showMotdWin (w, line);
                    break;
#ifndef PARADISE	/* Paradise MOTD requires paging */
                case 'F':      /* Scroll motd a bit forwards */
                    line = line + 4;
                    if (line > MaxMotdLine)
                    {
                        line = line - 4;
                        break;
                    }
                    W_ClearWindow (w);
                    showMotdWin (w, line);
                    break;
                case 'B':      /* Scroll motd a bit backwards */
                    if (line == 0)
                        break;
                    line = line - 4;
                    if (line < 0)
                        line = 0;
                    W_ClearWindow (w);
                    showMotdWin (w, line);
                    break;
#endif
                }
            }
            /* No break, we just fall through */
        case W_EV_BUTTON:
            if (typeok == 0)
                break;
            for (i = 0; i < 4; i++)
                if (event.Window == teamWin[i])
                {
                    *team = i;
                    break;
                }
            if (event.Window == qwin /* new */  &&
                event.type == W_EV_BUTTON)
            {
                *team = 4;
                break;
            }
            if (*team != -1 && !teamRequest (*team, *s_type))
            {
                *team = -1;
            }
            break;
        case W_EV_EXPOSE:
            for (i = 0; i < 4; i++)
                if (event.Window == teamWin[i])
                {
                    lastplayercount[i] = -1;    /* force update */
                    redrawTeam (teamWin[i], i, &lastplayercount[i]);
                    break;
                }
            if (event.Window == qwin && !newQuit)
            {
                redrawQuit ();
            }
            else if (event.Window == tstatw)
            {
                redrawTstats ();
            }
            else if (event.Window == w)
            {
            	if (me->p_whydead != KWINNER && me->p_whydead != KGENOCIDE)
                    showMotdWin (w, line);
            }
            else if (event.Window == helpWin)
#ifdef RECORDGAME
                if (playback)
                    pbfillhelp();
                else
#endif
                    fillhelp ();

#ifdef NBT
            else if (event.Window == macroWin)
                fillmacro ();
#endif

            else if (event.Window == playerw)
                RedrawPlayerList ();
            else if (event.Window == warnw)
                W_ClearWindow (warnw);
            else if (event.Window == messagew)
                W_ClearWindow (messagew);
            break;
        }
    }
    while (*team < 0);
    if (event.Window != qwin)
    {
        char buf[80];

#ifdef PARADISE
        sprintf (buf, "Welcome aboard %s!",
        	(me->p_stats2.st_royal == 0 ? ranks2[me->p_stats2.st_rank].name : royal[me->p_stats2.st_royal].name));
#else
        sprintf (buf, "Welcome aboard %s!", ranks[me->p_stats.st_rank].name);
#endif
        warning (buf);
    }

    if (*team == 4)
    {
        *team = -1;
        return;
    }

    for (i = 0; i < 4; i++)
        W_UnmapWindow (teamWin[i]);
    W_UnmapWindow (qwin);
    W_UnTileWindow (mapw);
}


/******************************************************************************/
/***  teamRequest()
/***   Attempt to pick specified team & ship
/******************************************************************************/
int
teamRequest (int team,
             int ship)
{
    time_t lastTime;

    pickOk = -1;
    sendTeamReq (team, ship);
    lastTime = time (NULL);
    while (pickOk == -1)
    {
        if (lastTime + 3 < time (NULL))
        {
            sendTeamReq (team, ship);
        }
        socketPause ();
        readFromServer (NULL);
        if (isServerDead ())
        {
            LineToConsole ("Oh SHIT,  We've been ghostbusted!\n");
            LineToConsole ("hope you weren't in a base\n");
            /* UDP fail-safe */
            commMode = commModeReq = COMM_TCP;
            commSwitchTimeout = 0;
            if (udpSock >= 0)
                closeUdpConn ();
            if (udpWin)
            {
                udprefresh (UDP_CURRENT);
                udprefresh (UDP_STATUS);
            }
            connectToServer (nextSocket);
            LineToConsole (" We've been resurrected!\n");
            pickOk = 0;
            break;
        }
    }
    return (pickOk);
}


/******************************************************************************/
/***  numShips()
/******************************************************************************/
int
numShips (int owner)
{
    int i, num = 0;
    struct player *p;

    for (i = 0, p = players; i < MAXPLAYER; i++, p++)
        if (p->p_status == PALIVE && p->p_team == owner)
            num++;
    return (num);
}


/******************************************************************************/
/***  realNumShips()
/******************************************************************************/
int
realNumShips (int owner)
{
    int i, num = 0;
    struct player *p;

    for (i = 0, p = players; i < MAXPLAYER; i++, p++)
        if (p->p_status != PFREE && p->p_team == owner)
            num++;
    return (num);
}


/******************************************************************************/
/***  deadTeam()
/******************************************************************************/
int
deadTeam (int owner)
/* The team is dead if it has no planets and cannot coup it's home planet */
{
    int i, num = 0;
    struct planet *p;

    if (planets[remap[owner] * 10 - 10].pl_couptime == 0)
        return (0);
    for (i = 0, p = planets; i < MAXPLANETS; i++, p++)
    {
        if (p->pl_owner & owner)
        {
            num++;
        }
    }
    if (num != 0)
        return (0);
    return (1);
}


/******************************************************************************/
/***  checkBold()
/******************************************************************************/
int
checkBold (char *line)
/* Determine if that line should be highlighted on sign-on screen */
/* Which is done when it is the players own score being displayed */
{
    char *s, *t;
    int i;
    int end = 0;

    if (strlen (line) < 60)
        return (0);
    s = line + 4;
    t = me->p_name;

    if (me == NULL)
        return (0);

    for (i = 0; i < 16; i++)
    {
        if (!end)
        {
            if (*t == '\0')
                end = 1;
            else if (*t != *s)
                return (0);
        }
        if (end)
        {
            if (*s != ' ')
                return (0);
        }
        s++;
        t++;
    }
    return (1);
}

struct list
{
    char bold;
    struct list *next;
    char *data;
};
static struct list *motddata = NULL;    /* pointer to first bit of
                                         * motddata */
static int first = 1;


/******************************************************************************/
/***  showMotdWin()
/******************************************************************************/
void
showMotdWin (W_Window motdwin, int atline)
{
    int i, length, top, center;
    struct list *data;
    int count;
    char buf[128];

#ifdef PARADISE
    newMotdStuff = 0;		/* clear the flag */
#endif

    sprintf (buf, "---  %s  ---", (char *) query_cowid ());
    length = strlen (buf);
    center = TWINSIDE / 2 - (length * W_Textwidth) / 2;
#ifndef PARADISE		/* no space for client header in paradise */
    W_WriteText (motdwin, center, W_Textheight, textColor,
                 buf, length, W_BoldFont);
#endif
    sprintf (buf, CBUGS);
    length = strlen (buf);
    center = TWINSIDE / 2 - (length * W_Textwidth) / 2;
#ifndef PARADISE		/* no space for client header in paradise */
    W_WriteText (motdwin, center, 3 * W_Textheight, textColor,
                 buf, length, W_RegularFont);
#endif

#ifdef PARADISE
    top = 0;
#else
    top = 10;
#endif

    if (first)
    {
        first = 0;
        data = motddata;
        while (data != NULL)
        {
            data->bold = (char) (checkBold (data->data));
            data = data->next;
        }
    }

    data = motddata;
    for (i = 0; i < atline; i++)
    {
        if (data == NULL)
        {
            atline = 0;
            data = motddata;
            break;
        }
        data = data->next;
    }

    count = LINESPERPAGE;       /* Magical # of lines to
                                 * display */
    for (i = top; i < 50; i++)
    {
        if (data == NULL)
            break;
        if (!strcmp (data->data, "\t@@@"))      /* ATM */
            break;
        if (data->bold)
        {
            W_WriteText (motdwin, 20, i * W_Textheight, textColor, data->data,
                         strlen (data->data), W_BoldFont);
        }
        else
        {
            W_WriteText (motdwin, 20, i * W_Textheight, textColor, data->data,
                         strlen (data->data), W_RegularFont);
        }
        data = data->next;
        count--;
        if (count <= 0)
            break;
    }
#ifdef PARADISE
    if (motdwin == w) {
	W_WriteText(mapw, GWINSIDE/2 - W_Textwidth * strlen(blk_refitstring) / 2, GWINSIDE - 20, textColor, blk_refitstring,
		    strlen(blk_refitstring), W_RegularFont);
    }
    showPics(motdwin, atline);
#endif
    showValues (data);
}

/******************************************************************************/
/***  showPics()                                                              */
/******************************************************************************/
#ifdef PARADISE
static void
showPics(W_Window win, int atline)
{
    struct piclist *temp;

    temp = motdPics;

    while (temp != NULL) {
	if (atline/LINESPERPAGE == temp->page) { /* hack for paging not working */
	    if (temp->thepic)
		W_WriteBitmap(temp->x, temp->y, temp->thepic, foreColor, win);
	    else {
		W_MakeLine(win, temp->x, temp->y,
			   temp->x + temp->width - 1, temp->y + temp->height - 1, W_Grey);
		W_MakeLine(win, temp->x, temp->y + temp->height - 1,
			   temp->x + temp->width - 1, temp->y, W_Grey);
		W_MakeLine(win, temp->x, temp->y,
			   temp->x + temp->width - 1, temp->y, W_Grey);
		W_MakeLine(win, temp->x, temp->y,
			   temp->x, temp->y + temp->height - 1, W_Grey);
		W_MakeLine(win, temp->x, temp->y + temp->height - 1,
			   temp->x + temp->width - 1, temp->y + temp->height - 1, W_Grey);
		W_MakeLine(win, temp->x + temp->width - 1, temp->y + temp->height - 1,
			   temp->x + temp->width - 1, temp->y, W_Grey);
	    }
	}
	temp = temp->next;
    }
}
#endif

/******************************************************************************/
/***  showValues()
/***   ATM: show the current values of the .sysdef parameters. */
/******************************************************************************/
void
showValues (struct list *data)
{
    int i;
    static char *msg = "OPTIONS SET WHEN YOU STARTED WERE:";

    /* try to find the start of the info */
    while (1)
    {
        if (data == NULL)
            return;
        if (!strcmp (data->data, STATUS_TOKEN))
            break;
        data = data->next;
    }
    data = data->next;

    W_WriteText (mapw, 20, 14 * W_Textheight, textColor, msg,
                 strlen (msg), W_RegularFont);
#ifdef PARADISE
    for (i = 16; i < 50; i++)
#else
    for (i = 16; i < 50; i += 2)
#endif
    {
        if (data == NULL)
            break;
        if (data->data[0] == '+')       /* quick boldface hack */
            W_WriteText (mapw, 20, i * W_Textheight, textColor,
                         data->data + 1, strlen (data->data) - 1, W_BoldFont);
        else
            W_WriteText (mapw, 20, i * W_Textheight, textColor, data->data,
                         strlen (data->data), W_RegularFont);
        data = data->next;
    }
}

/******************************************************************************/
/***  ClearMotd()
/***   Free the current motdData
/******************************************************************************/
void ClearMotd (void)
{
    struct list *temp, *temp2;

    temp = motddata;            /* start of motd information */
    while (temp != NULL)
    {
        temp2 = temp;
        temp = temp->next;
        free (temp2->data);
        free (temp2);
    }
#ifdef PARADISE
    // Probably need some stuff here from erase_motd()
#endif

    first = 1;                  /* so that it'll check bold
                                 * next time around */
}

/******************************************************************************/
/***  newMotdLine()
/******************************************************************************/
void
newMotdLine (char *line)
{
    static struct list **temp = &motddata;
    static int statmode = 0;    /* ATM */

#ifdef PARADISE
    /* Inlined blk_parsemotd() paradise client function */
    if (strncmp("BLK: ", line, 5) == 0) {
        /* See if it's a refit string.*/
        if (strncmp(&line[5], "REFIT", 5) == 0) {
    	    strncpy(blk_refitstring, &line[10], 79);
    	    blk_refitstring[79] = '\0';
        }
        /* Check to see if it's a borgish feature being enabled. */
        else if (strncmp(&line[5], "BORGISH ", 8) == 0) {
    	    if (strncmp(&line[13], "FRCLOAK", 7) == 0)
    	        blk_friendlycloak = 1;
        }
	return;
    }
    if ( strncmp("\t@@b", line, 4) == 0) // Between pages
	return;
    /*if (!currpage ||
	(pagecount - 1) == currpage->page ||
	motdlinestate == IN_SYSDEF) */
	newMotdStuff = 1;	/* set flag for event loop */
	first = 1;		/* check for bold again */
#endif
    if (!statmode && !strcmp (line, STATUS_TOKEN))
        statmode = 1;
    if (!statmode)
        MaxMotdLine++;          /* ATM - don't show on left */
    (*temp) = (struct list *) malloc (sizeof (struct list));

    if ((*temp) == NULL)
    {                           /* malloc error checking --
                                 * 10/30/92 EM */
        LineToConsole ("Warning:  Couldn't malloc space for a new motd line!");
        return;
    }
    /* Motd clearing code */
    if (strcmp (line, MOTDCLEARLINE) == 0)
    {
        ClearMotd ();
        return;
    }

    (*temp)->next = NULL;
    (*temp)->data = malloc (strlen (line) + 1);
    strcpy ((*temp)->data, line);
    temp = &((*temp)->next);
}

#ifdef PARADISE
/******************************************************************************/
/***  newMotdPic()
/******************************************************************************/
void
newMotdPic(int x, int y, int width, int height, char *bits, int page)
{
    struct piclist *tmp;
    struct motd_pic_spacket dummy;

    if ((width + 7) / 8 * height > sizeof(dummy.bits) && bits)
    {
        LineToConsole("MOTD picture from server is too big!  %dx%d couldn't possibly fit in the %d data bytes of the packet\n",
                      width, height, (int) sizeof(dummy.bits));
        return;
    }
 
    if ((currpage && page == currpage->page) || page == 0)
    {
	newMotdStuff = 1;	/* set flag for event loop */
	first = 1;		/* check for bold again */
    }

    tmp = (*motd_buftail) = (struct piclist *) malloc(sizeof(struct piclist));
    tmp->next = NULL;
    tmp->x = x;
    tmp->y = y - 112; /* 112 is magic number of height of the paradise header
                         image which we do NOT have */
    tmp->width = width;
    tmp->height = height;
    tmp->thepic = bits ? W_StoreBitmap(width, height, bits, w) : 0;
    tmp->page = page;
    motd_buftail = &(tmp->next);
}
#endif

/******************************************************************************/
/***  getResources()
/******************************************************************************/
void
getResources (char *prog)
{
    getColorDefs ();
}


/******************************************************************************/
/***  redrawTeam()
/******************************************************************************/
void
redrawTeam (W_Window win,
            int teamNo,
            int *lastnum)
{
    char buf[BUFSIZ];
    static char *teams[] = { "Federation", "Romulan", "Klingon", "Orion" };
    int num = numShips (1 << teamNo);

    /* Only redraw if number of players has changed */
    if (*lastnum == num)
        return;

    W_ClearWindow (win);
    if (newTeams)
    {
        switch (teamNo)
        {
            case 0:
                W_WriteScaleBitmap ( 0, 0, BOXSIDE, BOXSIDE,
                                     BMP_TEAM_SELECT_WIDTH, BMP_TEAM_SELECT_HEIGHT,
                                     0, fedteam, foreColor, win);
                break;
            case 1:
                W_WriteScaleBitmap ( 0, 0, BOXSIDE, BOXSIDE,
                                     BMP_TEAM_SELECT_WIDTH, BMP_TEAM_SELECT_HEIGHT,
                                     0, romteam, foreColor, win);
                break;
            case 2:
                W_WriteScaleBitmap ( 0, 0, BOXSIDE, BOXSIDE,
                                     BMP_TEAM_SELECT_WIDTH, BMP_TEAM_SELECT_HEIGHT,
                                     0, kliteam, foreColor, win);
                break;
            case 3:
                W_WriteScaleBitmap ( 0, 0, BOXSIDE, BOXSIDE,
                                     BMP_TEAM_SELECT_WIDTH, BMP_TEAM_SELECT_HEIGHT,
                                     0, oriteam, foreColor, win);
                break;
        }
    }
    else
        W_WriteText (win, 5, 5, shipCol[teamNo + 1], teams[teamNo],
                 strlen (teams[teamNo]), W_RegularFont);

    (void) sprintf (buf, "%d", num);
    W_MaskText (win, 5, 46, shipCol[teamNo + 1], buf, strlen (buf),
                W_BigFont);
    *lastnum = num;
}


/******************************************************************************/
/***  redrawQuit()
/******************************************************************************/
void
redrawQuit (void)
{
    int xoff;
    
    xoff = (BOXSIDE / 2) - 7 * W_Textwidth;
    if (xoff < 1)
        xoff = 5;

    W_WriteText (qwin, xoff, 5, textColor,
                 "Quit Netrek XP", 14, W_RegularFont);
}

#define CLOCK_BDR       0
#define CLOCK_X         (BOXSIDE / 2 - CLOCK_WID / 2)
#define CLOCK_Y         (BOXSIDE / 2 - CLOCK_HEI / 2)

#define XPI             3.141592654

/******************************************************************************/
/***  showTimeLeft()
/******************************************************************************/
void
showTimeLeft (time_t time, time_t max)
{
    register struct player *j = me;
    char buf[BUFSIZ], *cp;
    int cx, cy, tx, ty;
    int ex = 0, ey = 0;
    int CLOCK_WID, CLOCK_HEI;
    int angle;

    if (newQuit)
    {
    	CLOCK_WID = BOXSIDE;
    	CLOCK_HEI = BOXSIDE;
    }
    else
    {
    	CLOCK_WID = (BOXSIDE * 9 / 10);
    	CLOCK_HEI = (BOXSIDE * 2 / 3);
    }
    if ((max - time) < 10 && time & 1)
    {
        W_Beep ();
    }
    /* XFIX */
    W_ClearArea (qwin, CLOCK_X, CLOCK_Y, CLOCK_WID, CLOCK_HEI);

    cx = CLOCK_X + CLOCK_WID / 2;
    if (newQuit)
    {
        cy = CLOCK_Y + CLOCK_HEI / 2;
        
        angle = (int)(-360 * time / max);
        W_WriteScaleBitmap (CLOCK_BDR, CLOCK_BDR, BOXSIDE, BOXSIDE,
                            BMP_CCLOCK_WIDTH, BMP_CCLOCK_HEIGHT,
                            0, clockpic, foreColor, qwin);
        W_OverlayScaleBitmap (CLOCK_BDR, CLOCK_BDR, BOXSIDE, BOXSIDE,
                              BMP_CHAND_WIDTH, BMP_CHAND_HEIGHT,
                              angle, clockhandpic, foreColor, qwin);
        sprintf (buf, "%d", max - time);
        tx = cx - W_Textwidth * strlen (buf) / 2;
        ty = 2*(cy - W_Textheight / 2)/3;
        W_MaskText (qwin, tx, ty, W_Black, buf, strlen (buf), W_RegularFont);

        cp = "Quit Netrek XP";
    }
    else
    {
        cy = CLOCK_Y + (CLOCK_HEI - W_Textheight) / 2;
        ex = cx - BMP_CLOCK_WIDTH / 2;
        ey = cy - BMP_CLOCK_HEIGHT / 2;
        W_WriteBitmap (ex, ey, clockpic, foreColor, qwin);
        
        ex = (int) (cx - BMP_CLOCK_WIDTH * sin (2 * XPI * time / max) / 2);
        ey = (int) (cy - BMP_CLOCK_HEIGHT * cos (2 * XPI * time / max) / 2);
        W_MakeLine (qwin, cx, cy, ex, ey, foreColor);

        sprintf (buf, "%d", max - time);
        tx = cx - W_Textwidth * strlen (buf) / 2;
        ty = cy - W_Textheight / 2;
        W_WriteText (qwin, tx, ty, textColor, buf, strlen (buf), W_RegularFont);

        cp = "Auto Quit";
    }
    tx = CLOCK_X + cx - W_Textwidth * strlen (cp) / 2;
    ty = CLOCK_Y + CLOCK_HEI - W_Textheight;
    W_WriteText (qwin, tx, ty, textColor, cp, strlen (cp), W_RegularFont);
}

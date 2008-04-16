/******************************************************************************/
/***  File:  enter.c                                                        ***/
/***                                                                        ***/
/***  Function: This version modified to work as the client in a socket     ***/
/***            based protocol.                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "proto.h"

/******************************************************************************/
/***  enter()                                                               ***/
/***  Enter the game                                                        ***/
/******************************************************************************/
void
enter (void)
{
#ifdef ROTATERACE
    if (autoRotate)   
        rotateTeams();
#endif
    drawTstats ();
    delay = 0;
}

/******************************************************************************/
/***  openmem()                                                             ***/
/******************************************************************************/
void
openmem (void)
{
    /* Used to be struct memory universe, but leaving room for flexible struct
       sizes is better, and necessary for paradise - BB */
    initialize_players();
    initialize_torps();
    initialize_plasmas();
    initialize_phasers();
    load_default_teams();
    initialize_thingies();
    status2 = (struct status2 *) malloc(sizeof(*status2));
    status = (struct status *) malloc(sizeof(*status));
    initialize_planets();
    mctl = (struct mctl *) malloc(sizeof(*mctl));
    messages = (struct message *) malloc(sizeof(*messages) * MAXMESSAGE);

    mctl->mc_current = 0;
    status->time = 1;
    status->timeprod = 1;
    status->kills = 1;
    status->losses = 1;
    status->time = 1;
    status->planets = 1;
    status->armsbomb = 1;

    /* initialize pointers if ghost start */
    if (ghoststart)
    {
        me = &players[ghost_pno];
        myship = &(me->p_ship);
        mystats = &(me->p_stats);
    }
}

/******************************************************************************/
/***  do_autosetwar(int team)                                               ***/
/***  Set war declares on first entry into game.  Passes int team as        ***/
/***  player's p_team is not set at the time this function is called        ***/
/******************************************************************************/
void
do_autosetwar (int team)
{
    int i, newhostile;
    int playercount = 0;
    int largestenemy = -1;
    int largestenemycount = 0;

    /* Get my current hostile settings.  Should be hostile with all teams,
       set earlier in cowmain() */
    newhostile = me->p_hostile;

    /* Loop through all teams, and set war as appropriate*/
    for (i = 0; i < 4; i++)
    {
    	/* Don't change war settings with own team */
    	if (team == i)
    	    continue;

    	/* If team has 0 players ... */
        if ((playercount = realNumShips(1 << i)) == 0)
        {
            /* Declare peace?  Yes for non-zero autoSetWar */
            if (autoSetWar)
                newhostile ^= (1 << i);
            continue;
        }
        
        /* Keep track of largest enemy team */
        if (playercount > largestenemycount)
        {
            largestenemy = i;
            largestenemycount = playercount;
        }
    }
    /* No enemy team?  Don't change war settings. */
    if (largestenemy < 0)
        return;

    /* If autoSetWar is 2, keep hostile with largest enemy team, but
       peace with all other teams even if they have 1 or more players */
    if (autoSetWar == 2)
    {
        for (i = 0; i < 4; i++)
        {
            /* Skip own team and zero enemy teams, these were handled earlier */
    	    if (team == i || (realNumShips(1 << i)) == 0)
                continue;

            /* Non-zero players team that isn't largest opponent? */
            if (i != largestenemy)
                newhostile ^= (1 << i);
        }
    }

    sendWarReq (newhostile);
    return;
}

#ifdef ROTATERACE
/******************************************************************************/
/***  rotateTeams()                                                         ***/
/***  Place 2 largest teams on left hand side of galaxy                     ***/
/******************************************************************************/
void
rotateTeams (void)
{
    int i;
    int playercount = 0;
    int largestteam = -1, nextlargestteam= -1;
    int largestteamcount = 0, nextlargestteamcount = 0;

    /* Find the 2 largest teams (hopefully) */
    for (i = 0; i < 4; i++)
    {
        if ((playercount = realNumShips(1 << i)) == 0)
            continue;
        if (playercount > largestteamcount)
        {
            nextlargestteam = largestteam;
            nextlargestteamcount = largestteamcount;
            largestteam = i;
            largestteamcount = playercount;
        }
        else if (playercount > nextlargestteamcount)
        {
            nextlargestteam = i;
            nextlargestteamcount = playercount;
        }
    }
    /* Server empty or only 1 team?  Don't rotate. */
    if (largestteam < 0 || nextlargestteam < 0)
        return;

    /* 2 largest teams are diagonal (possible before T mode), i.e. FED-KLI
       or ROM-ORI?  Don't rotate.  Team order is FED ROM KLI ORI */
    if ((largestteam - nextlargestteam) == 2 || (largestteam - nextlargestteam) == -2)
        return;
    
    /* Determine how many degrees needed to rotate galaxy so both teams
       on left side of the map, and set global rotate value accordingly */
    switch (largestteam)
    {
        case 0: /* FED */
            if (nextlargestteam == 3) /* FED-ORI */
               rotate = 1;
            break;
        case 1: /* ROM */
            if (nextlargestteam == 2) /* ROM-KLI */
                rotate = 3;
            break;
        case 2: /* KLI */
            if (nextlargestteam == 1) /* KLI-ROM */
                rotate = 3;
            else if (nextlargestteam == 3) /* KLI-ORI */
                rotate = 2;
            break;
        case 3: /* ORI */
            if (nextlargestteam == 1) /* ORI-FED */
                rotate = 1;
            else if (nextlargestteam == 2) /* ORI-KLI */
                rotate = 2;
            break;
        default:
            break;
    }
    
    /* Perform rotation if necessary */
    if (rotate != old_rotate)
        rotateGalaxy();
    return;
}
#endif

/******************************************************************************/
/***  drawTstats()                                                          ***/
/******************************************************************************/
void
drawTstats (void)
{
    char buf[BUFSIZ];

    if (newDashboard)
        return;
    sprintf (buf,
             "Flags        Warp Dam Shd Torps  Kills Armies   Fuel  Wtemp Etemp  Time");
    W_WriteText (tstatw, 50, 5, textColor, buf, strlen (buf), W_RegularFont);
    sprintf (buf,
             "Maximum:      %2d  %3d %3d               %3d   %6d   %3d   %3d",
             me->p_ship.s_maxspeed, me->p_ship.s_maxdamage,
             me->p_ship.s_maxshield, me->p_ship.s_maxarmies,
             me->p_ship.s_maxfuel, me->p_ship.s_maxwpntemp / 10,
             me->p_ship.s_maxegntemp / 10);
    W_WriteText (tstatw, 50, 27, textColor, buf, strlen (buf), W_RegularFont);
}

#ifdef HOCKEY_LINES
/******************************************************************************/
/***  check_hockey_mode() - checked on initial entry into game              ***/
/******************************************************************************/
void check_hockey_mode (void)
{
    int i;

    for (i = 0; i < nplayers; i++)
    {
        if (strcmp(players[i].p_name, "Puck") == 0 &&
            strcmp(players[i].p_login, "Robot") == 0 &&
            players[i].p_team == NOBODY &&
            players[i].p_ship.s_type == SCOUT)
        {
            playing_hockey = 1;
        }
    }
}

/******************************************************************************/
/***  init_hockey_lines()                                                   ***/
/******************************************************************************/
void
init_hockey_lines (void)
{
    int i = 0;                  /* This is incremented for
                                 * each line added */

    /* For speed, the normal netrek walls are not done this way */

    /* Defines for Hockey lines and the Hockey lines themselves */
#define RINK_TOP 0
#define RINK_BOTTOM (GWIDTH)
#define TENTH (((RINK_BOTTOM - RINK_TOP)/10))
#define R_MID (((RINK_BOTTOM - RINK_TOP)/2))    /* center (red) line */
#define RINK_LENGTH ((RINK_BOTTOM - RINK_TOP))
#define RINK_WIDTH ((GWIDTH*2/3))
#define G_MID ((GWIDTH/2))      /* center of goal */
#define RINK_LEFT ((G_MID-(RINK_WIDTH/2)))
#define RINK_RIGHT ((G_MID+(RINK_WIDTH/2)))
#define G_LFT (R_MID-TENTH)     /* left edge of goal */
#define G_RGT (R_MID+TENTH)     /* right edge of goal */
#define RED_1 (RINK_LEFT + (1*RINK_WIDTH/5))
#define RED_2 (RINK_LEFT + (2*RINK_WIDTH/5))
#define RED_3 (RINK_LEFT + (3*RINK_WIDTH/5))
#define RED_4 (RINK_LEFT + (4*RINK_WIDTH/5))
#define ORI_G (RINK_BOTTOM - /*2* */TENTH)      /* Ori goal line */
#define ORI_E (RINK_BOTTOM -   TENTH/2) /* end of Ori goal */
#define ORI_B (RINK_BOTTOM - (RINK_LENGTH/3))   /* Ori blue line */
#define KLI_G (RINK_TOP    + /*2* */TENTH)      /* Kli goal line */
#define KLI_E (RINK_TOP    +   TENTH/2) /* end of Kli goal */
#define KLI_B (RINK_TOP    + (RINK_LENGTH/3))   /* Kli blue line */

    /* This part defines galaxy lines */
#define G_RINK_TOP 0
#define G_RINK_BOTTOM (GWINSIDE)
#define G_TENTH (((G_RINK_BOTTOM - G_RINK_TOP)/10))
#define G_R_MID (((G_RINK_BOTTOM - G_RINK_TOP)/2)-1)    /* center (red) line */
#define G_RINK_LENGTH ((G_RINK_BOTTOM - G_RINK_TOP))
#define G_RINK_WIDTH ((GWINSIDE*2/3))
#define G_G_MID ((GWINSIDE/2))   /* center of goal */
#define G_RINK_LEFT ((G_G_MID-(G_RINK_WIDTH/2))-2)
#define G_RINK_RIGHT ((G_G_MID+(G_RINK_WIDTH/2))-1)
#define G_G_LFT (G_R_MID-G_TENTH)       /* left edge of goal */
#define G_G_RGT (G_R_MID+G_TENTH)       /* right edge of goal */
#define G_RED_1 (G_RINK_LEFT + (1*G_RINK_WIDTH/5) - 1)
#define G_RED_2 (G_RINK_LEFT + (2*G_RINK_WIDTH/5) - 1)
#define G_RED_3 (G_RINK_LEFT + (3*G_RINK_WIDTH/5) - 1)
#define G_RED_4 (G_RINK_LEFT + (4*G_RINK_WIDTH/5) - 1)
#define G_ORI_G (G_RINK_BOTTOM - G_TENTH - 1)   /* Ori goal line */
#define G_ORI_E (G_RINK_BOTTOM - G_TENTH/2 - 1) /* end of Ori goal */
#define G_ORI_B (G_RINK_BOTTOM - (G_RINK_LENGTH/3) - 2) /* Ori blue line */
#define G_KLI_G (G_RINK_TOP + G_TENTH - 1)      /* Kli goal line */
#define G_KLI_E (G_RINK_TOP + G_TENTH/2 - 1)    /* end of Kli goal */
#define G_KLI_B (G_RINK_TOP + (G_RINK_LENGTH/3) - 1)    /* Kli blue line */

    /* local window hockey lines */

    i = 0;
    /* The Kli goal line */
    local_hockey_lines[i].begin_x = G_LFT;
    local_hockey_lines[i].end_x = G_RGT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = KLI_G;
    local_hockey_lines[i].color = W_Red;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"Kli Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The left side goal line */
    local_hockey_lines[i].begin_x = local_hockey_lines[i].end_x = G_LFT;
    local_hockey_lines[i].begin_y = KLI_G;
    local_hockey_lines[i].end_y = KLI_E;
    local_hockey_lines[i].color = W_Green;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_VERTICAL;
    /* fprintf(stderr,"L K Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The right side goal line */
    local_hockey_lines[i].begin_x = local_hockey_lines[i].end_x = G_RGT;
    local_hockey_lines[i].begin_y = KLI_G;
    local_hockey_lines[i].end_y = KLI_E;
    local_hockey_lines[i].color = W_Green;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_VERTICAL;
    /* fprintf(stderr,"K R Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The End of kli goal line */
    local_hockey_lines[i].begin_x = G_LFT;
    local_hockey_lines[i].end_x = G_RGT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = KLI_E;
    local_hockey_lines[i].color = W_Green;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"K B Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The Kli blue line */
    local_hockey_lines[i].begin_x = RINK_LEFT;
    local_hockey_lines[i].end_x = RINK_RIGHT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = KLI_B;
    local_hockey_lines[i].color = W_Cyan;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"Kli Blue: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The Ori goal line */
    local_hockey_lines[i].begin_x = G_LFT;
    local_hockey_lines[i].end_x = G_RGT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = ORI_G;
    local_hockey_lines[i].color = W_Red;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"Ori Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The left side goal line */
    local_hockey_lines[i].begin_x = local_hockey_lines[i].end_x = G_LFT;
    local_hockey_lines[i].begin_y = ORI_G;
    local_hockey_lines[i].end_y = ORI_E;
    local_hockey_lines[i].color = W_Cyan;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_VERTICAL;
    /* fprintf(stderr,"O L Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The right side goal line */
    local_hockey_lines[i].begin_x = local_hockey_lines[i].end_x = G_RGT;
    local_hockey_lines[i].begin_y = ORI_G;
    local_hockey_lines[i].end_y = ORI_E;
    local_hockey_lines[i].color = W_Cyan;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_VERTICAL;
    /* fprintf(stderr,"O R Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The End of ori goal line */
    local_hockey_lines[i].begin_x = G_LFT;
    local_hockey_lines[i].end_x = G_RGT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = ORI_E;
    local_hockey_lines[i].color = W_Cyan;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"O B Goal: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The Ori blue line */
    local_hockey_lines[i].begin_x = RINK_LEFT;
    local_hockey_lines[i].end_x = RINK_RIGHT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = ORI_B;
    local_hockey_lines[i].color = W_Cyan;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"Ori Blue: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* The red line */
    local_hockey_lines[i].begin_x = RINK_LEFT;
    local_hockey_lines[i].end_x = RINK_RIGHT;
    local_hockey_lines[i].begin_y = local_hockey_lines[i].end_y = R_MID;
    local_hockey_lines[i].color = W_Red;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;
    /* fprintf(stderr,"Red Line: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* Right rink boundary */
    local_hockey_lines[i].begin_x = local_hockey_lines[i].end_x = RINK_RIGHT;
    local_hockey_lines[i].begin_y = 0;
    local_hockey_lines[i].end_y = GWIDTH - 1;
    local_hockey_lines[i].color = W_Grey;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_VERTICAL;
    /* fprintf(stderr,"Rt. Line: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* Left rink boundary */
    local_hockey_lines[i].begin_x = local_hockey_lines[i].end_x = RINK_LEFT;
    local_hockey_lines[i].begin_y = 0;
    local_hockey_lines[i].end_y = GWIDTH - 1;
    local_hockey_lines[i].color = W_Grey;
    local_hockey_lines[i].flag = &showHockeyLinesLocal;
    local_hockey_lines[i++].orientation = S_LINE_VERTICAL;
    /* fprintf(stderr,"Lef Line: x: %i to %i, y: %i to
     * %i\n",local_hockey_lines[i-1].begin_x,
     * local_hockey_lines[i-1].end_x,local_hockey_lines[i-1].begin_y,local_hockey_lines[i-1].end_y); */

    /* NOTE:  The number of lines must EXACTLY match the NUM_HOCKEY_LINES */
    /* in defs.h for it to run properly.                           */

    /* galaxy window hockey lines */

    i = 0;
    /* The Kli goal line */
    map_hockey_lines[i].begin_x = G_G_LFT;
    map_hockey_lines[i].end_x = G_G_RGT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_KLI_G;
    map_hockey_lines[i].color = W_Red;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* The left side goal line */
    map_hockey_lines[i].begin_x = map_hockey_lines[i].end_x = G_G_LFT;
    map_hockey_lines[i].begin_y = G_KLI_G;
    map_hockey_lines[i].end_y = G_KLI_E;
    map_hockey_lines[i].color = W_Green;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_VERTICAL;

    /* The right side goal line */
    map_hockey_lines[i].begin_x = map_hockey_lines[i].end_x = G_G_RGT;
    map_hockey_lines[i].begin_y = G_KLI_G;
    map_hockey_lines[i].end_y = G_KLI_E;
    map_hockey_lines[i].color = W_Green;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_VERTICAL;

    /* The End of kli goal line */
    map_hockey_lines[i].begin_x = G_G_LFT;
    map_hockey_lines[i].end_x = G_G_RGT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_KLI_E;
    map_hockey_lines[i].color = W_Green;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* The Kli blue line */
    map_hockey_lines[i].begin_x = G_RINK_LEFT;
    map_hockey_lines[i].end_x = G_RINK_RIGHT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_KLI_B;
    map_hockey_lines[i].color = W_Cyan;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* The Ori goal line */
    map_hockey_lines[i].begin_x = G_G_LFT;
    map_hockey_lines[i].end_x = G_G_RGT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_ORI_G;
    map_hockey_lines[i].color = W_Red;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* The left side goal line */
    map_hockey_lines[i].begin_x = map_hockey_lines[i].end_x = G_G_LFT;
    map_hockey_lines[i].begin_y = G_ORI_G;
    map_hockey_lines[i].end_y = G_ORI_E;
    map_hockey_lines[i].color = W_Cyan;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_VERTICAL;

    /* The right side goal line */
    map_hockey_lines[i].begin_x = map_hockey_lines[i].end_x = G_G_RGT;
    map_hockey_lines[i].begin_y = G_ORI_G;
    map_hockey_lines[i].end_y = G_ORI_E;
    map_hockey_lines[i].color = W_Cyan;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_VERTICAL;

    /* The End of ori goal line */
    map_hockey_lines[i].begin_x = G_G_LFT;
    map_hockey_lines[i].end_x = G_G_RGT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_ORI_E;
    map_hockey_lines[i].color = W_Cyan;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* The Ori blue line */
    map_hockey_lines[i].begin_x = G_RINK_LEFT;
    map_hockey_lines[i].end_x = G_RINK_RIGHT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_ORI_B;
    map_hockey_lines[i].color = W_Cyan;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* The red line */
    map_hockey_lines[i].begin_x = G_RINK_LEFT;
    map_hockey_lines[i].end_x = G_RINK_RIGHT;
    map_hockey_lines[i].begin_y = map_hockey_lines[i].end_y = G_R_MID;
    map_hockey_lines[i].color = W_Red;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_HORIZONTAL;

    /* Right rink boundary */
    map_hockey_lines[i].begin_x = map_hockey_lines[i].end_x = G_RINK_RIGHT;
    map_hockey_lines[i].begin_y = 0;
    map_hockey_lines[i].end_y = GWINSIDE - 1;
    map_hockey_lines[i].color = W_Grey;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_VERTICAL;

    /* Left rink boundary */
    map_hockey_lines[i].begin_x = map_hockey_lines[i].end_x = G_RINK_LEFT;
    map_hockey_lines[i].begin_y = 0;
    map_hockey_lines[i].end_y = GWINSIDE - 1;
    map_hockey_lines[i].color = W_Grey;
    map_hockey_lines[i].flag = &showHockeyLinesMap;
    map_hockey_lines[i++].orientation = S_LINE_VERTICAL;

    /* NOTE:  The number of lines must EXACTLY match the NUM_HOCKEY_LINES */
    /* in defs.h for it to run properly.                           */

}

#endif

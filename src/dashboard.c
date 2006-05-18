/******************************************************************************/
/***  File:  dashboard.c                                                    ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/*** dashboard.c - graphic tstatw - 6/2/93                                  ***/
/***                                                                        ***/
/*** copyright 1993 Lars Bernhardsson (lab@mtek.chalmers.se) Free to use    ***/
/*** as long as this notice is left here.                                   ***/
/***                                                                        ***/
/*** Color by Nick Trown.                                                   ***/
/***                                                                        ***/
/******************************************************************************/

/******************************************************************************/
/***                             Include Files                              ***/
/******************************************************************************/

#include <stdio.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "string_util.h"
#include "proto.h"

#define DB_NOFILL 0
#define DB_LINE 1
#define DB_FILL 2

#define DB_3DIGITS 0
#define DB_5DIGITS 1

#define BAR_LENGTH 56

#define SPACING 4

/******************************************************************************/
/***  timerString()                                                         ***/
/******************************************************************************/
char * 
timeString(time_t t)
{
    static char *s = NULL;
    struct tm *tm;

    if (!s)
        s = (char *) malloc(9);
    if (t > 24 * 60 * 60)
    {
        tm = localtime(&t);
        sprintf(s, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
    } 
    else
        sprintf(s, "%02d:%02d:%02d", (int) (t / (60 * 60)), 
                (int) ((t % (60 * 60)) / 60), (int) (t % 60));
    return s;
}

/******************************************************************************/
/***  db_timer()                                                            ***/
/******************************************************************************/
void
db_timer (int fr, int xloc, int yloc)
{
    static time_t oldtime = -1;
    static int lastTimerType = -1;
    time_t  now = 0;
    static char lasttimer[9], *timer;
    int left, right, x, pos;

#ifdef RECORDGAME
    if(playback)
    {
        run_clock (time (0));
        //pb_framectr (xloc, yloc);   /* show frame counter for playback */
        return;
    }
#endif

    if (timerType != lastTimerType || fr)
    {
        char *s = NULL;

        fr = 1;
        lastTimerType = timerType;
        switch (timerType)
        {
        case T_NONE:
            if (newDashboard)
                W_ClearArea(tstatw, xloc, yloc, 12 * W_Textwidth, W_Textheight);
            else
                W_ClearArea(tstatw, xloc + 4 * W_Textwidth, yloc - W_Textheight, 8 * W_Textwidth, 2 * W_Textheight);
            strcpy(lasttimer, "        ");
            oldtime = now;
            break;
        case T_DAY:
            s = "NOW";
            break;
        case T_SERVER:
            s = "SRV";
            break;
        case T_SHIP:
            s = "SHP";
            break;
        case T_USER:
            s = "TMR";
            break;
        }
        if(s) 
        {
            if (newDashboard)
                W_WriteText(tstatw, xloc, yloc, textColor, s, 3, W_RegularFont);
            else
                W_WriteText(tstatw, xloc + 4 * W_Textwidth, yloc - W_Textheight, textColor, s, 3, W_RegularFont);
        }
    }
    if (!timerType)
        return;
    now = time(NULL);
    if (now != oldtime || fr) 
    {
        /*
            get the timer string and start comparing it with the old one. Only
            print the differences
        */
        timer = timeString(now - timeBank[timerType]);
        x = xloc + 4 * W_Textwidth;
        left = 0;
        right = -1;
        pos = 0;

        /*
            run through the string to find any differences.  Print any
            continuous differences with one W_WriteText call.
        */
        if (fr) 
        {
            W_WriteText(tstatw, x, yloc, textColor, timer, 8, W_RegularFont);
        } 
        else 
        {
            while (pos < 8) 
            {
                if (timer[pos] == lasttimer[pos]) 
                {
                    if (left <= right)
                        W_WriteText(tstatw, x + left * W_Textwidth, yloc, textColor,
                                    timer + left, right - left + 1, W_RegularFont);
                    left = pos + 1;
                    right = pos;
                } 
                else
                    right++;
                pos++;
            }
            if (left <= right)
                W_WriteText(tstatw, x + left * W_Textwidth, yloc, textColor,
                            timer + left, right - left + 1, W_RegularFont);
        }
        oldtime = now;
        strcpy(lasttimer, timer);
    }
    return;
}

/******************************************************************************/
/***  db_box()                                                              ***/
/******************************************************************************/
static void
db_box (int x,
        int y,
        int w,
        int h,
        int f,
        int color)
{
    int border = W_White;

    if (color == W_Red)
        border = color;

    if (w == 0 || h == 0)
        return;

    switch (f)
    {
    case DB_FILL:
        W_FillArea (tstatw, x, y, w + 1, h + 1, color);
        break;
    case DB_LINE:
        W_MakeLine (tstatw, x + w, y, x + w, y + h, border);
        W_MakeLine (tstatw, x + w, y + 4, x + BAR_LENGTH, y + 4, border);
        break;
    case DB_NOFILL:
        W_MakeLine (tstatw, x, y, x + w, y, border);
        W_MakeLine (tstatw, x + w, y, x + w, y + h, border);
        W_MakeLine (tstatw, x + w, y + h, x, y + h, border);
        W_MakeLine (tstatw, x, y + h, x, y, border);
        break;
    }
}

/******************************************************************************/
/***  db_bar()                                                              ***/
/******************************************************************************/
static void
db_bar (char *lab,
        int x,
        int y,
        int value,
        int tmpmax,
        int max,
        int digits,
        int color)
{
    register int wt, wv;
    register int tc = 11;
    register int tw = W_Textwidth * tc;
    char valstr[32];

    switch (digits)
    {
    case DB_3DIGITS:
        tc = 11;
        tw = W_Textwidth * tc;
        valstr[0] = lab[0];
        valstr[1] = lab[1];
        valstr[2] = '[';
        itoapad (value, &(valstr[3]), 0, 3);
        valstr[6] = '/';
        itoapad (tmpmax, &(valstr[7]), 0, 3);
        valstr[10] = ']';
        W_ClearArea (tstatw, x, y, tw + BAR_LENGTH, 10);
        break;
    case DB_5DIGITS:
        tc = 15;
        tw = W_Textwidth * tc;
        valstr[0] = lab[0];
        valstr[1] = lab[1];
        valstr[2] = '[';
        itoapad (value, &(valstr[3]), 0, 5);
        valstr[8] = '/';
        itoapad (tmpmax, &(valstr[9]), 0, 5);
        valstr[14] = ']';
        W_ClearArea (tstatw, x, y, tw + BAR_LENGTH, 10);
        break;
    }

    wt = (int) ((float) BAR_LENGTH * ((float) tmpmax / (float) max));
    wv = (int) ((float) BAR_LENGTH * ((float) value / (float) max));
    if (wt > BAR_LENGTH)
        wt = BAR_LENGTH;
    if (wv > BAR_LENGTH)
        wv = BAR_LENGTH;

    W_WriteText (tstatw, x, y, textColor, valstr, 3, W_RegularFont);
    W_WriteText (tstatw, x + 3 * W_Textwidth, y, textColor, (&valstr[3]),
                 tc / 2 + 1, W_BoldFont);
    W_WriteText (tstatw, x + (tc / 2 + 1) * W_Textwidth, y, textColor,
                 (&valstr[tc / 2 + 1]), tc / 2, W_RegularFont);

    db_box (x + tw, y, BAR_LENGTH, 9, DB_NOFILL, color);
    if (wt >= wv && wt > 0)
        db_box (x + tw, y, wt, 9, DB_LINE, color);

    if (wv > 0)
        db_box (x + tw, y, wv, 9, DB_FILL, color);
}

/******************************************************************************/
/***  db_special() - for showing prioritized timer info in dashboard        ***/
/******************************************************************************/
static void
db_special (void)
{
    char buf[16];
    struct player *plr;
    int repairtime;

    if ((me->p_flags & (PFPLOCK | PFOBSERV)) == (PFPLOCK | PFOBSERV))
        plr = players + me->p_playerl;
    else
        plr = me;
    
    /* Start with low priority messages, clear as necessary for higher
       priority ones */
       
    /* Default impulse text */
    W_ClearArea (tstatw, 38, 3, W_Textwidth * 8, W_Textheight);
    W_WriteText (tstatw, 38, 3, W_Yellow, "Impulse", 7, W_BoldFont);
    
    /* Transwarp text */
    if (me->p_flags & PFTWARP)
    {
        W_ClearArea (tstatw, 38, 3, W_Textwidth * 8, W_Textheight);
        W_WriteText (tstatw, 38, 3, W_White, "Twarp", 5, W_BoldFont);
    }
    /* Tournament extension text */
    if (tdelay)
    {
        if (time (0) > tdelay)
            tdelay = 0;
	else
	{
	    W_ClearArea (tstatw, 38, 3, W_Textwidth * 8, W_Textheight);
            W_WriteText (tstatw, 38, 3, W_Grey, "Tmod", 4, W_BoldFont);
	    sprintf(buf, "%d", tdelay - time (0));
	    W_WriteText (tstatw, 68, 3, textColor, buf, strlen (buf), W_RegularFont);
	}
    }
    
    /* Repair text */
    if ((me->p_flags & PFREPAIR) && plr->p_speed == 0)
    {
        repairtime = repair_time();
        W_ClearArea (tstatw, 38, 3, W_Textwidth * 8, W_Textheight);
        W_WriteText (tstatw, 38, 3, W_Cyan, "Fix", 3, W_BoldFont);
        sprintf(buf, "%d", repairtime);
        W_WriteText (tstatw, 62, 3, textColor, buf, strlen (buf), W_RegularFont);
    }
    
    /* Refit text */
    if (rdelay)
    {
        if (time (0) > rdelay)
            rdelay = 0;
        else
        {
            W_ClearArea (tstatw, 38, 3, W_Textwidth * 8, W_Textheight);
            W_WriteText (tstatw, 38, 3, W_Green, "Refit", 5, W_BoldFont);
            sprintf(buf, "%d", rdelay - time (0));
            W_WriteText (tstatw, 74, 3, textColor, buf, strlen (buf), W_RegularFont);
        }
    }
    
    /* Declare War text */
    if (delay)
    {
        if (time (0) > delay)
            delay = 0;
        else
        {
            W_ClearArea (tstatw, 38, 3, W_Textwidth * 8, W_Textheight);
            W_WriteText (tstatw, 38, 3, W_Red, "War", 3, W_BoldFont);
            sprintf(buf, "%d", delay - time (0));
            W_WriteText (tstatw, 62, 3, textColor, buf, strlen (buf), W_RegularFont);
        }
    }
}


/******************************************************************************/
/***  db_flags()                                                            ***/
/******************************************************************************/
static void
db_flags (int fr)
{
    static float old_kills = -1.0;
    static int old_torp = -1;
    static unsigned int old_flags = (unsigned int) -1;
    static int old_tourn = 0;
    char buf[16];
    struct player *plr;

    if ((me->p_flags & (PFPLOCK | PFOBSERV)) == (PFPLOCK | PFOBSERV))
        plr = players + me->p_playerl;
    else
        plr = me;

    if (fr || me->p_flags != old_flags)
    {
        buf[0] = (char) (me->p_flags & PFSHIELD ? 'S' : ' ');
        if (me->p_flags & PFGREEN)
            buf[1] = 'G';
        else if (me->p_flags & PFYELLOW)
            buf[1] = 'Y';
        else
            buf[1] = 'R';
        buf[2] = (char) (me->p_flags & (PFPLLOCK | PFPLOCK) ? 'L' : ' ');
        buf[3] = (char) (me->p_flags & PFREPAIR ? 'R' : ' ');
        buf[4] = (char) (me->p_flags & PFBOMB ? 'B' : ' ');
        buf[5] = (char) (me->p_flags & PFORBIT ? 'O' : ' ');
        if (me->p_ship.s_type == STARBASE)
            buf[6] = (char) (me->p_flags & PFDOCKOK ? 'D' : ' ');
        else
            buf[6] = (char) (me->p_flags & PFDOCK ? 'D' : ' ');
        buf[7] = (char) (me->p_flags & PFCLOAK ? 'C' : ' ');
        buf[8] = (char) (me->p_flags & PFWEP ? 'W' : ' ');
        buf[9] = (char) (me->p_flags & PFENG ? 'E' : ' ');
        if (me->p_flags & PFPRESS)
            buf[10] = 'P';
        else if (me->p_flags & PFTRACT)
            buf[10] = 'T';
        else
            buf[10] = ' ';
        if (me->p_flags & PFBEAMUP)
            buf[11] = 'u';
        else if (me->p_flags & PFBEAMDOWN)
            buf[11] = 'd';
        else
            buf[11] = ' ';
            
        W_WriteText (tstatw, 2, 3, textColor, "Flags", 5, W_RegularFont);
        W_WriteText (tstatw, 2, 17, textColor, buf, 12, W_RegularFont);
        old_flags = me->p_flags;
    }
    
    if (fr || status->tourn != old_tourn)
    {
        if (status->tourn)
        {
            W_WriteText (tstatw, 74, 17, textColor, "T", 1, W_BoldFont);
            tdelay = 0;
        }
        else
        {
            if (status->tourn != old_tourn && !tdelay)
                tdelay = time (0) + TOURNEXTENDTIME;
            W_WriteText (tstatw, 74, 17, textColor, " ", 1, W_BoldFont);
        }
        old_tourn = status->tourn;
    }

    if (fr || plr->p_kills != old_kills)
    {
        if (plr->p_kills > 0.0)
        {
            W_WriteText (tstatw, 346, 17, textColor, "Kills:", 6,
                         W_RegularFont);
            ftoa (plr->p_kills, buf, 0, 3, 2);
            W_WriteText (tstatw, 386, 17, textColor, buf, strlen (buf),
                         W_RegularFont);
        }
        else
        {
            W_ClearArea (tstatw, 346, 17, 96, 10);
        }
        old_kills = plr->p_kills;
    }

    if (fr || plr->p_ntorp != old_torp)
    {
        if (plr->p_ntorp > 0)
        {
            W_WriteText (tstatw, 346, 30, textColor, "Torps:", 6,
                         W_RegularFont);
            buf[0] = (char) (plr->p_ntorp % 10 + '0');
            W_WriteText (tstatw, 386, 30, textColor, buf, 1, W_RegularFont);
        }
        else
        {
            W_ClearArea (tstatw, 346, 30, 96, 10);
        }
        old_torp = plr->p_ntorp;
    }
}


/******************************************************************************/
/***  db_redraw_krp()                                                       ***/
/******************************************************************************/
void
db_redraw_krp (int fr)
{
    static int old_spd = -1, old_cur_spd = -1;
    static int old_shl = -1, old_dam = -1;
    static int old_arm = -1, old_cur_arm = -1;
    static int old_wpn = -1, old_egn = -1;
    static int old_ful = -1;
    register int cur_max;
    register int value;
    int color;
    float kills;

    if ((me->p_flags & (PFPLOCK | PFOBSERV)) == (PFPLOCK | PFOBSERV))
        kills = players[me->p_playerl].p_kills;
    else
        kills = me->p_kills;

    if (fr)
        W_ClearWindow (tstatw);

    db_flags (fr);
    db_special ();

    /* TIMER */
    db_timer (fr, 2, 3 + 2 * (W_Textheight + SPACING));

    // SRS - inserted some additional casts to clear up compiler warnings
    cur_max = (int) ((me->p_ship.s_maxspeed + 2) -
                     ((me->p_ship.s_maxspeed + 1) *
                      ((float) me->p_damage /
                       (float) (me->p_ship.s_maxdamage))));

    if (cur_max > me->p_ship.s_maxspeed)
        cur_max = me->p_ship.s_maxspeed;
    if (cur_max < 0)
        cur_max = 0;

    if (fr || me->p_speed != old_spd || cur_max != old_cur_spd)
    {
        if (me->p_speed >= me->p_ship.s_maxspeed - 2)
            color = W_Red;
        else
            color = W_Green;
        db_bar ("Sp", 90, 3,
                me->p_speed, cur_max, me->p_ship.s_maxspeed, DB_3DIGITS,
                color);
        old_spd = me->p_speed;
        old_cur_spd = cur_max;
    }

    if (fr || me->p_shield != old_shl)
    {
        value = (100 * me->p_shield) / me->p_ship.s_maxshield;
        if (value <= 16)
            color = W_Red;
        else if (value <= 66)
            color = W_Yellow;
        else
            color = W_Green;
        db_bar ("Sh", 90, 17,
                me->p_shield, me->p_ship.s_maxshield, me->p_ship.s_maxshield,
                DB_3DIGITS, color);
        old_shl = me->p_shield;
    }

    if (fr || me->p_damage != old_dam)
    {
        value =
            (100 * (me->p_ship.s_maxdamage - me->p_damage)) /
            me->p_ship.s_maxdamage;
        if (value <= 16)
            color = W_Red;
        else if (value <= 66)
            color = W_Yellow;
        else
            color = W_Green;
        db_bar ("Hu", 90, 31,
                (me->p_ship.s_maxdamage - me->p_damage),
                me->p_ship.s_maxdamage, me->p_ship.s_maxdamage, DB_3DIGITS,
                color);
        old_dam = me->p_damage;
    }

    if (me->p_ship.s_type == ASSAULT)
        cur_max = (((kills * 3) > me->p_ship.s_maxarmies) ?
                   me->p_ship.s_maxarmies : (int) (kills * 3));
    else if (me->p_ship.s_type == STARBASE)
        cur_max = me->p_ship.s_maxarmies;
    else
        cur_max = (((kills * 2) > me->p_ship.s_maxarmies) ?
                   me->p_ship.s_maxarmies : (int) (kills * 2));

    if (fr || me->p_armies != old_arm || cur_max != old_cur_arm)
    {
        value = me->p_armies;

        if (value <= 0)
            color = W_Green;
        else
            color = W_Red;

        db_bar ("Ar", 218, 3,
                me->p_armies, cur_max, me->p_ship.s_maxarmies, DB_3DIGITS,
                color);
        old_arm = me->p_armies;
        old_cur_arm = cur_max;
    }

    if (fr || me->p_wtemp != old_wpn)
    {
        value = (100 * me->p_wtemp) / me->p_ship.s_maxwpntemp;

        if (value <= 16)
            color = W_Green;
        else if (value <= 66)
            color = W_Yellow;
        else
            color = W_Red;
        db_bar ("Wt", 218, 17,
                me->p_wtemp / 10, me->p_ship.s_maxwpntemp / 10,
                me->p_ship.s_maxwpntemp / 10, DB_3DIGITS, color);
        old_wpn = me->p_wtemp;
    }

    if (fr || me->p_etemp != old_egn)
    {
        value = (100 * me->p_etemp) / me->p_ship.s_maxegntemp;
        if (value <= 16)
            color = W_Green;
        else if (value <= 66)
            color = W_Yellow;
        else
            color = W_Red;
        db_bar ("Et", 218, 31,
                me->p_etemp / 10, me->p_ship.s_maxegntemp / 10,
                me->p_ship.s_maxegntemp / 10, DB_3DIGITS, color);
        old_egn = me->p_etemp;
    }

    if (fr || me->p_fuel != old_ful)
    {
        value = ((100 * me->p_fuel) / me->p_ship.s_maxfuel);
        if (value <= 16)
            color = W_Red;
        else if (value <= 66)
            color = W_Yellow;
        else
            color = W_Green;
        db_bar ("Fu", 346, 3,
                me->p_fuel, me->p_ship.s_maxfuel, me->p_ship.s_maxfuel,
                DB_5DIGITS, color);
        old_ful = me->p_fuel;
    }
}

/******************************************************************************/
/***  db_redraw_COW()                                                       ***/
/******************************************************************************/
void
db_redraw_COW (int fr)
{
    static int old_spd = -1, old_cur_spd = -1;
    static int old_shl = -1, old_dam = -1;
    static int old_arm = -1, old_cur_arm = -1;
    static int old_wpn = -1, old_egn = -1;
    static int old_ful = -1;
    register int cur_max;
    register int value;
    int color;
    float kills;

    if ((me->p_flags & (PFPLOCK | PFOBSERV)) == (PFPLOCK | PFOBSERV))
        kills = players[me->p_playerl].p_kills;
    else
        kills = me->p_kills;


    if (fr)
        W_ClearWindow (tstatw);

    db_flags (fr);
    db_special ();

    db_timer (fr, 2, 3 + 2 * (W_Textheight + SPACING));

    // SRS - inserted some additional casts to clear up compiler warnings
    cur_max = (int) ((me->p_ship.s_maxspeed + 2) -
                     ((me->p_ship.s_maxspeed + 1) *
                      ((float) me->p_damage /
                       (float) (me->p_ship.s_maxdamage))));

    if (cur_max > me->p_ship.s_maxspeed)
        cur_max = me->p_ship.s_maxspeed;
    if (cur_max < 0)
        cur_max = 0;

    if (fr || me->p_speed != old_spd || cur_max != old_cur_spd)
    {
        if (me->p_speed >= me->p_ship.s_maxspeed - 2)
            color = W_Yellow;
        else
            color = W_White;
        db_bar ("Sp", 90, 3,
                me->p_speed, cur_max, me->p_ship.s_maxspeed, DB_3DIGITS,
                color);
        old_spd = me->p_speed;
        old_cur_spd = cur_max;
    }

    if (fr || me->p_shield != old_shl)
    {
        value = (100 * me->p_shield) / me->p_ship.s_maxshield;
        if (value <= 50)
            color = W_Red;
        else if (value < 90)
            color = W_Yellow;
        else
            color = W_White;
        db_bar ("Sh", 90, 17,
                me->p_ship.s_maxshield - me->p_shield, me->p_ship.s_maxshield,
                me->p_ship.s_maxshield, DB_3DIGITS, color);
        old_shl = me->p_shield;
    }

    if (fr || me->p_damage != old_dam)
    {
        value = (100 * me->p_damage) / me->p_ship.s_maxdamage;
        if (value <= 10)
            color = W_White;
        else if (value > 50)
            color = W_Red;
        else
            color = W_Yellow;
        db_bar ("Da", 90, 31,
                me->p_damage, me->p_ship.s_maxdamage, me->p_ship.s_maxdamage,
                DB_3DIGITS, color);
        old_dam = me->p_damage;
    }

    if (me->p_ship.s_type == ASSAULT)
        cur_max = (((kills * 3) > me->p_ship.s_maxarmies) ?
                   me->p_ship.s_maxarmies : (int) (kills * 3));
    else if (me->p_ship.s_type == STARBASE)
        cur_max = me->p_ship.s_maxarmies;
    else
        cur_max = (((kills * 2) > me->p_ship.s_maxarmies) ?
                   me->p_ship.s_maxarmies : (int) (kills * 2));

    if (fr || me->p_armies != old_arm || cur_max != old_cur_arm)
    {
        value = me->p_armies;

        if (value <= 3)
            color = W_White;
        else if (value > 5)
            color = W_Red;
        else
            color = W_Yellow;
        db_bar ("Ar", 218, 3,
                me->p_armies, cur_max, me->p_ship.s_maxarmies, DB_3DIGITS,
                color);
        old_arm = me->p_armies;
        old_cur_arm = cur_max;
    }

    if (fr || me->p_wtemp != old_wpn)
    {
        value = (100 * me->p_wtemp) / me->p_ship.s_maxwpntemp;

        if (value > 50)
            color = W_Red;
        else if (value <= 20)
            color = W_White;
        else
            color = W_Yellow;
        db_bar ("Wt", 218, 17,
                me->p_wtemp / 10, me->p_ship.s_maxwpntemp / 10,
                me->p_ship.s_maxwpntemp / 10, DB_3DIGITS, color);
        old_wpn = me->p_wtemp;
    }

    if (fr || me->p_etemp != old_egn)
    {
        value = (100 * me->p_etemp) / me->p_ship.s_maxegntemp;

        if (value <= 25)
            color = W_White;
        else if (value < 75)
            color = W_Yellow;
        else
            color = W_Red;
        db_bar ("Et", 218, 31,
                me->p_etemp / 10, me->p_ship.s_maxegntemp / 10,
                me->p_ship.s_maxegntemp / 10, DB_3DIGITS, color);
        old_egn = me->p_etemp;
    }

    if (fr || me->p_fuel != old_ful)
    {
        value = ((100 * me->p_fuel) / me->p_ship.s_maxfuel);

        if (value <= 50)
            color = W_Red;
        else if (value > 90)
            color = W_White;
        else
            color = W_Yellow;
        db_bar ("Fu", 346, 3,
                me->p_fuel, me->p_ship.s_maxfuel, me->p_ship.s_maxfuel,
                DB_5DIGITS, color);
        old_ful = me->p_fuel;
    }
}

/******************************************************************************/
/***  db_redraw()                                                           ***/
/******************************************************************************/
void
db_redraw (int fr)
{
    switch (newDashboard)
    {
    case 2:
        db_redraw_krp (fr);
        break;
    case 3:
        db_redraw_lab2 (fr);
        break;
    default:
        db_redraw_COW (fr);
    }
}

/******************************************************************************/
/***  repair_time() - calculate time left till ship is fully repaired
                      using server defined repair rates.  Only called when
                      ship is at warp 0 and under repair                    ***/
/******************************************************************************/
int
repair_time (void)
{
    int shieldtime = 0;
    int hulltime = 0;
    int shieldneeded, hullneeded;
    float shieldrate = 0.0, hullrate = 0.0;
    
    /* 100 subshield or subdamage =  1 shield or hull repaired 
       This routine is used by server every update (and 10 updates/sec) */
    /* calculate shield repair rate */
    if ((shieldneeded = (me->p_ship.s_maxshield - me->p_shield)) > 0)
    {
        me->p_subshield = me->p_ship.s_repair * 4;
        if ((me->p_flags & PFORBIT)
        && (planets[me->p_planet].pl_flags & PLREPAIR)
        &&(!(planets[me->p_planet].pl_owner & (me->p_swar | me->p_hostile))))
        {
            me->p_subshield += me->p_ship.s_repair * 4;
        }
        if (me->p_flags & PFDOCK)
        {
    	    me->p_subshield += me->p_ship.s_repair * 6;
        }
        /* Calculate time needed to repair shields */
        shieldrate = (float)(me->p_subshield)/(float)100.0;
        shieldtime = (int)(shieldneeded/shieldrate);
    }

    /* calculate hull repair rate */
    if (((hullneeded = me->p_damage) > 0) && !(me->p_flags & PFSHIELD))
    {
    	me->p_subdamage = me->p_ship.s_repair * 2;
    	if ((me->p_flags & PFORBIT)
    	&& (planets[me->p_planet].pl_flags & PLREPAIR)
    	&& (!(planets[me->p_planet].pl_owner & (me->p_swar | me->p_hostile))))
    	{
    	    me->p_subdamage += me->p_ship.s_repair * 2;
    	}
    	if (me->p_flags & PFDOCK)
    	{
    	    me->p_subdamage += me->p_ship.s_repair * 3;
    	}
    	/* Calculate time needed to repair hull */
    	hullrate = (float)(me->p_subdamage)/(float)100.0;
        hulltime = (int)(hullneeded/hullrate);
    }
    
    return MAX(shieldtime, hulltime);
};
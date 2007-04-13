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
#include <math.h>

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

#define SPACING 4

/* COW/KRP dashboard width spacing is as follows:

1/3 blank char, 14 text chars for flags/dash message/clock, 2/3 blank char,
11 text chars for shield/speed/hull labels, BAR_LENGTH for shield/speed/hull bars,
1 blank char, 11 text chars for army/wtemp/etemp labels, BAR_LENGTH for army/
wtemp/etemp bars, 1 blank char, 15 text chars for fuel label, BAR_LENGTH for fuel
bar.  For the normal text width (6 pixels) this works out to a dashboard width of:

(1/3)*6 + 14*6 + (2/3)*6 + 11*6 + (9 1/3)*6 + 1*6 + 11*6 + (9 1/3)*6
        + 1*6 + 15*6 + (9 1/3)*6 = 482 pixels

To be safe, dash width should be defined as at least 82 chars wide, which it is
in DashMaxWidth().

Height spacing is as follows:
3 blank pixels, first line of text, 4 blank pixels (SPACING), second line of text, 4
blank pixels (SPACING), 3rd line of text.  For the normal text height (10 pixels)
this works out to a dashboard height of:

3 + 1*10 + 4 + 1*10 + 4 + 1*10 = 41 pixels

To be safe, dash height should be defined as 4 times text height, which it is
in newwin().

*/

/* code to draw and erase packet lights 2/5/94 [BDyess] */

#define SENDX 		7
#define SENDY		1
#define RECEIVEX	3
#define RECEIVEY	1

static int send_lit = 0, receive_lit = 0;

void
light_send()
{
    if (send_lit)
        return;
    send_lit = MAX(1, server_ups / 10);
    W_MakePoint(tstatw, SENDX, SENDY, W_Green);
    W_MakePoint(tstatw, SENDX + 1, SENDY, W_Green);
    W_MakePoint(tstatw, SENDX, SENDY + 1, W_Green);
    W_MakePoint(tstatw, SENDX + 1, SENDY + 1, W_Green);
}

void
light_receive()
{
    if (receive_lit)
        return;
    receive_lit = MAX(2, 2 * server_ups / 10);
    W_MakePoint(tstatw, RECEIVEX, RECEIVEY, W_Yellow);
    W_MakePoint(tstatw, RECEIVEX + 1, RECEIVEY, W_Yellow);
    W_MakePoint(tstatw, RECEIVEX, RECEIVEY + 1, W_Yellow);
    W_MakePoint(tstatw, RECEIVEX + 1, RECEIVEY + 1, W_Yellow);
}

void
light_erase()
{
    if (receive_lit == MAX(1, server_ups / 10))
    {
        W_MakePoint(tstatw, RECEIVEX, RECEIVEY, backColor);
        W_MakePoint(tstatw, RECEIVEX + 1, RECEIVEY, backColor);
        W_MakePoint(tstatw, RECEIVEX, RECEIVEY + 1, backColor);
        W_MakePoint(tstatw, RECEIVEX + 1, RECEIVEY + 1, backColor);
    }
    if (receive_lit)
        receive_lit--;
    if (send_lit == MAX(1, server_ups / 20))
    {
        W_MakePoint(tstatw, SENDX, SENDY, backColor);
        W_MakePoint(tstatw, SENDX + 1, SENDY, backColor);
        W_MakePoint(tstatw, SENDX, SENDY + 1, backColor);
        W_MakePoint(tstatw, SENDX + 1, SENDY + 1, backColor);
    }
    if (send_lit)
        send_lit--;
}

/******************************************************************************/
/***  DashMaxWidth() - return width adjusted for text size (if font is      ***/
/***  large), or if font is small, return the size of tactical window       ***/
/******************************************************************************/
int
DashMaxWidth(void)
{
    /* Text dashboard uses slightly less space due to having a larger fixed
       entry spacing of 50 pixels, but we handle the largest case here, which
       is the COW/KRP dashboards */
    return MAX(TWINSIDE + (2 * THICKBORDER - 2 * BORDER), 83 * W_Textwidth);
}

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
    /* Was 56, too bad it wasn't defined as an even multiple of text width. */
    register int BAR_LENGTH = W_Textwidth/3 + 9 * W_Textwidth;
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
        W_MakeLine (tstatw, x + w, y + 2 * W_Textheight / 3,
                    x + BAR_LENGTH, y + 2 * W_Textheight / 3, border);
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
    /* Was 56, too bad it wasn't defined as an even multiple of text width. */
    register int BAR_LENGTH = W_Textwidth/3 + 9 * W_Textwidth;
    int sign_change = 0;
    char valstr[32];
    
    /* Take care of case where value is negative */
    if (value < 0)
    {
        value = abs(value);
        sign_change = 1;
    }

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
        W_ClearArea (tstatw, x, y, tw + BAR_LENGTH, W_Textheight);
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
        W_ClearArea (tstatw, x, y, tw + BAR_LENGTH, W_Textheight);
        break;
    }

    wt = (int) ((float) BAR_LENGTH * ((float) tmpmax / (float) max));
    wv = (int) ((float) BAR_LENGTH * ((float) value / (float) max));
    if (wt > BAR_LENGTH)
        wt = BAR_LENGTH;
    if (wv > BAR_LENGTH)
        wv = BAR_LENGTH;

    W_WriteText (tstatw, x, y, textColor, valstr, 3, W_RegularFont);
    W_WriteText (tstatw, x + 3 * W_Textwidth, y, sign_change ? W_Red : textColor,
                (&valstr[3]), tc / 2 + 1, W_BoldFont);
    W_WriteText (tstatw, x + (tc / 2 + 1) * W_Textwidth, y, textColor,
                 (&valstr[tc / 2 + 1]), tc / 2, W_RegularFont);

    db_box (x + tw, y, BAR_LENGTH, W_Textheight - 1, DB_NOFILL, color);
    if (wt >= wv && wt > 0)
        db_box (x + tw, y, wt, W_Textheight - 1, DB_LINE, color);

    if (wv > 0)
        db_box (x + tw, y, wv, W_Textheight - 1, DB_FILL, color);
}

/******************************************************************************/
/***  db_special() - for showing prioritized timer info in dashboard        ***/
/******************************************************************************/
void
db_special (int fr, int x, int y)
{
    char buf[8];
    char buf2[4];
    char tmp[8];
    struct player *plr;
    int msgtype;
    static char oldmsg[8];
    static int oldmsgtype;
    W_Color color;
    int left, right, pos;

    if ((me->p_flags & (PFPLOCK | PFOBSERV)) == (PFPLOCK | PFOBSERV))
        plr = players + me->p_playerl;
    else
        plr = me;

    /* Check if any delays expired */
    if (delay && time (0) > delay)
        delay = 0;
    if (rdelay && time (0) > rdelay)
        rdelay = 0;
    if (tdelay && time (0) > tdelay)
        tdelay = 0;

    /* Start with highest priority message, then go down in descending order
       of importance */

    /* Declare War text */
    if (me->p_flags & PFWAR)
    {
        sprintf (buf, "War ");
        if (delay)
        {
            sprintf(buf2, "%d", delay - time (0));
            strcat (buf, buf2);
        }
        msgtype = 0;
        color = W_Red;
    }
    /* Refit text */
    else if (me->p_flags & PFREFITTING)
    {
        sprintf (buf, "Refit ");
        if (rdelay)
        {
            sprintf(buf2, "%d", rdelay - time (0));
            strcat (buf, buf2);
        }
        msgtype = 1;
        color = W_Green;
    }
    /* Repair text */
    else if ((me->p_flags & PFREPAIR) && plr->p_speed == 0)
    {
        sprintf (buf, "Fix ");
        sprintf(buf2, "%d", (F_sp_generic_32 ? me->p_repair_time : repair_time()));
        strcat (buf, buf2);
        msgtype = 2;
        color = W_Cyan;
    }
    /* Tournament extension text */
    else if (tdelay)
    {
        sprintf (buf, "Tmod ");
        sprintf(buf2, "%d", tdelay - time (0));
        strcat (buf, buf2);
        msgtype = 3;
        color = W_Grey;
    }    
    /* Transwarp text */
    else if (me->p_flags & PFTWARP)
    {
        sprintf (buf, "Twarp");
        msgtype = 4;
        color = W_White;
    }
    /* Ship stopped */
    else if (me->p_speed == 0)
    {
        sprintf (buf, "Stopped");
        msgtype = 5;
        color = W_Grey;
    }
    /* Default impulse text */
    else
    {
        sprintf (buf, "Impulse");
        msgtype = 6;
        color = W_Yellow;
    }

    if (fr || msgtype != oldmsgtype)
    {
    	W_ClearArea (tstatw, x, y, W_Textwidth * 8, W_Textheight);
    	W_WriteText (tstatw, x, y, color, buf, strlen (buf), W_BoldFont);
    }
    else
    {
    	/*
            Run through the string to find any differences.  Print any
            continuous differences with one W_WriteText call.
        */
        left = 0;
        right = -1;
        pos = 0;
        strncpy (tmp, buf, 7);
        while (pos < 7) 
        {
            if (tmp[pos] == oldmsg[pos]) 
            {
                if (left <= right)
                    W_WriteText(tstatw, x + left * W_Textwidth, y, color,
                                tmp + left, right - left + 1, W_BoldFont);
                left = pos + 1;
                right = pos;
            } 
            else
                right++;
            pos++;
        }
        if (left <= right)
            W_WriteText(tstatw, x + left * W_Textwidth, y, color,
                        tmp + left, right - left + 1, W_BoldFont);
    }
    
    oldmsgtype = msgtype;
    strcpy(oldmsg, buf);

    return;
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
    register int BAR_LENGTH = W_Textwidth/3 + 9 * W_Textwidth;
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
   
        if (me->p_flags & (PFWEP | PFENG))
            W_WriteText (tstatw, W_Textwidth/3, 3, W_Red, "Flags", 5, W_RegularFont);
        else
            W_WriteText (tstatw, W_Textwidth/3, 3, textColor, "Flags", 5, W_RegularFont);
        W_WriteText (tstatw, W_Textwidth/3, 3 + W_Textheight + SPACING, textColor, buf, 12, W_RegularFont);
        old_flags = me->p_flags;
#ifdef SOUND
        if (ingame && oldengflag != (me->p_flags & PFENG))
        {
            if (me->p_flags & PFENG)
                Play_Sound(ENGINE_MELT_WAV, SF_INFO);
            else
                Play_Sound(ENGINE_OK_WAV, SF_INFO);
        }
        oldengflag = (me->p_flags & PFENG);
#endif
    }

    if (fr || status->tourn != old_tourn)
    {
        if (status->tourn)
        {
            W_WriteText (tstatw, W_Textwidth/3 + 12 * W_Textwidth, 3 + W_Textheight + SPACING, textColor, "T", 1, W_BoldFont);
            tdelay = 0;
        }
        else
        {
            if (status->tourn != old_tourn && !tdelay)
                tdelay = time (0) + TOURNEXTENDTIME;
            W_WriteText (tstatw, W_Textwidth/3 + 12 * W_Textwidth, 3 + W_Textheight + SPACING, textColor, " ", 1, W_BoldFont);
        }
        old_tourn = status->tourn;
    }

    if (fr || plr->p_kills != old_kills)
    {
        if (plr->p_kills > 0.0)
        {
            W_WriteText (tstatw, 173 * W_Textwidth / 3, 3 + W_Textheight + SPACING, textColor, "Kills:", 6,
                         W_RegularFont);
            ftoa (plr->p_kills, buf, 0, 3, 2);
            W_WriteText (tstatw, 173 * W_Textwidth / 3 + 6 * W_Textwidth + 2 * W_Textwidth / 3,
                         3 + W_Textheight + SPACING, textColor, buf, strlen (buf), W_RegularFont);
        }
        else
        {
            W_ClearArea (tstatw, 173 * W_Textwidth / 3, 3 + W_Textheight + SPACING,
                         12 * W_Textwidth + 2 * W_Textwidth / 3, W_Textheight);
        }
        old_kills = plr->p_kills;
    }

    if (fr || plr->p_ntorp != old_torp)
    {
        if (plr->p_ntorp > 0)
        {
            W_WriteText (tstatw, 173 * W_Textwidth / 3, 3 + 2 * (W_Textheight + SPACING), textColor, "Torps:", 6,
                         W_RegularFont);
            buf[0] = (char) (plr->p_ntorp % 10 + '0');
            W_WriteText (tstatw, 173 * W_Textwidth / 3 + 6 * W_Textwidth + 2 * W_Textwidth / 3,
                         3 + 2 * (W_Textheight + SPACING), textColor, buf, 1, W_RegularFont);
        }
        else
        {
            W_ClearArea (tstatw, 173 * W_Textwidth / 3, 3 + 2 * (W_Textheight + SPACING), 
                         8 * W_Textwidth + 2 * W_Textwidth / 3, W_Textheight);
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
    db_special (fr, W_Textwidth/3 + 6 * W_Textwidth, 3);

    /* TIMER */
    db_timer (fr, W_Textwidth/3, 3 + 2 * (W_Textheight + SPACING));

    light_erase();

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
        db_bar ("Sp", 15 * W_Textwidth, 3,
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
        if (me->p_ship.s_type == ATT)
            db_bar ("Sh", 15 * W_Textwidth, 3 + W_Textheight + SPACING,
                    me->p_shield, me->p_ship.s_maxshield, me->p_ship.s_maxshield,
                    DB_5DIGITS, color);
        else
            db_bar ("Sh", 15 * W_Textwidth, 3 + W_Textheight + SPACING,
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
        if (me->p_ship.s_type == ATT)
            db_bar ("Hu", 15 * W_Textwidth, 3 + 2 * (W_Textheight + SPACING),
                    (me->p_ship.s_maxdamage - me->p_damage),
                    me->p_ship.s_maxdamage, me->p_ship.s_maxdamage, DB_5DIGITS,
                    color);
        else
            db_bar ("Hu", 15 * W_Textwidth, 3 + 2 * (W_Textheight + SPACING),
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

        db_bar ("Ar", 109 * W_Textwidth / 3, 3,
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
        if (me->p_ship.s_type != ATT)
            db_bar ("Wt", 109 * W_Textwidth / 3, 3 + W_Textheight + SPACING,
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
        if (me->p_ship.s_type != ATT)
            db_bar ("Et", 109 * W_Textwidth / 3, 3 + 2 * (W_Textheight + SPACING),
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
        db_bar ("Fu", 173 * W_Textwidth / 3, 3,
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
    db_special (fr, W_Textwidth/3 + 6 * W_Textwidth, 3);

    /* TIMER */
    db_timer (fr, W_Textwidth/3, 3 + 2 * (W_Textheight + SPACING));

    light_erase();

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
        db_bar ("Sp", 15 * W_Textwidth, 3,
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
        if (me->p_ship.s_type == ATT)
            db_bar ("Sh", 15 * W_Textwidth, 3 + W_Textheight + SPACING,
                    me->p_ship.s_maxshield - me->p_shield, me->p_ship.s_maxshield,
                    me->p_ship.s_maxshield, DB_5DIGITS, color);
        else
            db_bar ("Sh", 15 * W_Textwidth, 3 + W_Textheight + SPACING,
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
        if (me->p_ship.s_type == ATT)
            db_bar ("Da", 15 * W_Textwidth, 3 + 2 * (W_Textheight + SPACING),
                    me->p_damage, me->p_ship.s_maxdamage, me->p_ship.s_maxdamage,
                    DB_5DIGITS, color);
        else
            db_bar ("Da", 15 * W_Textwidth, 3 + 2 * (W_Textheight + SPACING),
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
        db_bar ("Ar", 109 * W_Textwidth / 3, 3,
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
        if (me->p_ship.s_type != ATT)
            db_bar ("Wt", 109 * W_Textwidth / 3, 3 + W_Textheight + SPACING,
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
        if (me->p_ship.s_type != ATT)
            db_bar ("Et", 109 * W_Textwidth / 3, 3 + 2 * (W_Textheight + SPACING),
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
        db_bar ("Fu", 173 * W_Textwidth / 3, 3,
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
                      ship is at warp 0 and under repair.  Made obsolete by
                      F_sp_generic_32 feature packet.                       ***/
/******************************************************************************/
int
repair_time (void)
{
    int obs = 0;
    int shieldtime = 0;
    int hulltime = 0;
    int shieldneeded, hullneeded;
    float shieldrate = 0.0, hullrate = 0.0;
    short planet;
    struct player *plr;

    if ((me->p_flags & (PFPLOCK | PFOBSERV)) == (PFPLOCK | PFOBSERV))
    {
        plr = players + me->p_playerl;
        obs = 1;
    }
    else
        plr = me;
        
    /* 100 subshield or subdamage =  1 shield or hull repaired 
       This routine is used by server every update (and 10 updates/sec) */
    /* calculate shield repair rate */
    if ((shieldneeded = (me->p_ship.s_maxshield - me->p_shield)) > 0)
    {
        me->p_subshield = me->p_ship.s_repair * 4;
        if (me->p_flags & PFORBIT)
        {
            /* Damn server doesn't send us p_planet info, have to calculate it ourselves! */
            /* And since observers don't get war flags, assume that if ship is orbiting a repair
    	       planet in repair mode, the planet is friendly - best we can do until server
    	       sends us more info */
            planet = get_closest_planet(me->p_x, me->p_y);
            if ((planets[planet].pl_flags & PLREPAIR)
            &&(obs || !(planets[planet].pl_owner & (plr->p_swar | plr->p_hostile))))
                me->p_subshield += me->p_ship.s_repair * 4;
    	}
    	if (me->p_flags & PFDOCK)
    	    me->p_subshield += me->p_ship.s_repair * 6;
        /* Calculate time needed to repair shields */
        shieldrate = (float)(me->p_subshield)/(float)100.0;
        shieldtime = (int)(shieldneeded/shieldrate);
    }

    /* calculate hull repair rate */
    if (((hullneeded = me->p_damage) > 0) && !(me->p_flags & PFSHIELD))
    {
    	me->p_subdamage = me->p_ship.s_repair * 2;
    	if (me->p_flags & PFORBIT)
    	{
    	    /* Damn server doesn't send us p_planet info, have to calculate it ourselves! */
    	    /* And since observers don't get war flags, assume that if ship is orbiting a repair
    	       planet in repair mode, the planet is friendly - best we can do until server
    	       sends us more info */
            planet = get_closest_planet(me->p_x, me->p_y);
    	    if ((planets[planet].pl_flags & PLREPAIR)
    	    && (obs || !(planets[planet].pl_owner & (plr->p_swar | plr->p_hostile))))
    	        me->p_subdamage += me->p_ship.s_repair * 2;
    	}
    	if (me->p_flags & PFDOCK)
    	    me->p_subdamage += me->p_ship.s_repair * 3;
	/* Calculate time needed to repair hull */
    	hullrate = (float)(me->p_subdamage)/(float)100.0;
        hulltime = (int)(hullneeded/hullrate);
    }
    
    return MAX(shieldtime, hulltime);
}

/******************************************************************************/
/***  get_closest_planet() - find closest planet to given location
      Used to determine which planet you are orbiting, when the server
      does not provide the SP_GENERIC_32 packet.                            ***/
/******************************************************************************/
int get_closest_planet(int x, int y)
{
    register int i;
    register struct planet *k;
    double dist, closedist;
    int target;

    closedist = GWIDTH;
    for (i = 0, k = &planets[i]; i < MAXPLANETS; i++, k++)
    {
        dist = hypot ((double) (x - k->pl_x), (double) (y - k->pl_y));
        if (dist < closedist)
        {
                target = i;
                closedist = dist;
        }
    }
    return (short)(target);
}

/******************************************************************************/
/***  File:  stats.c
/***
/***  Function:
/***
/***  Revisions:
/***
/******************************************************************************/

/*
 * Took Stats box code from SWINE SRS 2/23/98
 * Why?  Why ask why?
 * Because it was better, it was cool!
 */

#include "config.h"
#include "copyright.h"

#include <stdio.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

#define BX_OFF()        ((textWidth + 1) * W_Textwidth + S_IBORDER)
#define BY_OFF(line)    ((line) * (W_Textheight + S_IBORDER) + S_IBORDER)
#define TX_OFF(len)     ((textWidth - len) * W_Textwidth + S_IBORDER)
#define TY_OFF(line)    BY_OFF(line)

#if 0
#define STAT_WIDTH              160
#else
#define STAT_WIDTH              st_width
#endif
#define STAT_HEIGHT             BY_OFF(NUM_SLIDERS)
#define STAT_BORDER             2
#define S_IBORDER               5
#define STAT_X                  422
#define STAT_Y                  13

#define SL_WID                  \
	(STAT_WIDTH - 2 * S_IBORDER - (textWidth + 1) * W_Textwidth)
#define SL_HEI                  (W_Textheight)

#define NUM_ELS(a)              (sizeof (a) / sizeof (*(a)))
#define NUM_SLIDERS             NUM_ELS(sliders)

typedef struct slider
{
    char *label;
    int min, max;
    int red;
    int yellow;
    int label_length;
    int diff;
    int *var;
    int lastVal;
}
SLIDER;

typedef struct record
{
    int *data;
    int last_value;
}
RECORD;

static SLIDER sliders[] = {
    {"Sh", 0, 100, 20, 100},
    {"Da", 0, 100, 0, 0},
    {"Fu", 0, 10000, 2000, 10000},
    {"WT", 0, 1200, 0, 800},
    {"ET", 0, 1200, 0, 800},
};

static int textWidth = 0;
static int initialized = 0;

static int st_width = -1;

static void
box (int filled,
     int x,
     int y,
     int wid,
     int hei,
     W_Color color)
{
    if (wid == 0)
        return;

    if (filled)
    {
        /* XFIX */
        if (color == -1)
        {
            int Lines;

            for (Lines = y; Lines <= (y + hei - 1); Lines++)
            {
                W_MakeLine (statwin, x, Lines, x + wid, Lines, W_Cyan);
                Lines++;
                W_MakeLine (statwin, x, Lines, x + wid, Lines, W_Black);
            }
        }
        else
        {
            W_FillArea (statwin, x, y, wid + 1, hei + 1, color);
        }

        return;
    }
    W_MakeLine (statwin, x, y, x + wid, y, color);
    W_MakeLine (statwin, x + wid, y, x + wid, y + hei, color);
    W_MakeLine (statwin, x + wid, y + hei, x, y + hei, color);
    W_MakeLine (statwin, x, y + hei, x, y, color);
}


void
initStats ()
{
    int i;

    if (initialized
#ifdef RECORDGAME
        && !playback
#endif
       )
        return;
    initialized = 1;
    sliders[0].var = &(me->p_shield);
    sliders[1].var = &(me->p_damage);
    sliders[2].var = &(me->p_fuel);
    sliders[3].var = &(me->p_wtemp);
    sliders[4].var = &(me->p_etemp);
    for (i = 0; i < NUM_SLIDERS; i++)
    {
        sliders[i].label_length = strlen (sliders[i].label);
        textWidth = MAX (textWidth, sliders[i].label_length);
        sliders[i].diff = sliders[i].max - sliders[i].min;
        sliders[i].lastVal = 0;
    }
    st_width = W_WindowWidth (statwin);
}

void
redrawStats ()
{
    int i;

    W_ClearWindow (statwin);
    initStats ();
    for (i = 0; i < NUM_SLIDERS; i++)
    {
        sliders[i].lastVal = 0;
    }
    for (i = 0; i < NUM_SLIDERS; i++)
    {
        W_WriteText (statwin, TX_OFF (sliders[i].label_length), TY_OFF (i),
                     textColor, sliders[i].label, sliders[i].label_length,
                     W_RegularFont);
        box (0, BX_OFF () - 1, BY_OFF (i) - 1, SL_WID + 2, SL_HEI + 2,
             borderColor);
        sliders[i].lastVal = 0;
    }
}

void
updateStats ()
{
    int i, value, colvalue, new_x;
    int r, y, t;
    SLIDER *s;

    initStats ();
    for (i = 0; i < NUM_SLIDERS; i++)
    {
        s = &sliders[i];
        value = *(s->var);
        colvalue = value;
        if ((i == 0) || (i == 2))
            colvalue = s->max - value;
        if (value < s->min)
            value = s->min;
        else if (value > s->max)
            value = s->max;
#if 0
        if (value == s->lastVal)
            continue;
#endif
        new_x = value * SL_WID / s->diff;
        y = s->yellow * SL_WID / s->diff;
        r = s->red * SL_WID / s->diff;
        t = s->max * SL_WID / s->diff;
        if (((me->p_flags & PFWEP) && (i == 3)) ||
            ((me->p_flags & PFENG) && (i == 4)))
        {
            box (1, BX_OFF (), BY_OFF (i), new_x, SL_HEI, -1);
        }
        else if ((i == 1) && (colvalue > 0))
        {
            box (1, BX_OFF (), BY_OFF (i), new_x, SL_HEI, rColor);
        }
        else if (colvalue > s->red)
        {
            box (1, BX_OFF (), BY_OFF (i), new_x, SL_HEI, rColor);
/*
	    box(1, BX_OFF(), BY_OFF(i), y, SL_HEI, gColor);
	    box(1, BX_OFF() + y, BY_OFF(i), r - y, SL_HEI, yColor);
	    box(1, BX_OFF() + r, BY_OFF(i), new_x - r, SL_HEI, rColor);
*/

        }
        else if (colvalue > s->yellow)
        {
            box (1, BX_OFF (), BY_OFF (i), new_x, SL_HEI, yColor);
/*
	    box(1, BX_OFF(), BY_OFF(i), y, SL_HEI, gColor);
	    box(1, BX_OFF() + y, BY_OFF(i), new_x - y, SL_HEI, yColor);
*/
        }
        else
        {
            box (1, BX_OFF (), BY_OFF (i), new_x, SL_HEI, gColor);
        }
        box (1, BX_OFF () + new_x, BY_OFF (i), t - new_x, SL_HEI, backColor);
    }
}


void
calibrate_stats ()
{
    register int i;
    sliders[0].min = 0;
    sliders[0].max = me->p_ship.s_maxshield;
    sliders[0].yellow = (int) (.33 * ((double) sliders[0].max));
    sliders[0].red = (int) (.66 * ((double) sliders[0].max));

    sliders[1].min = 0;
    sliders[1].max = me->p_ship.s_maxdamage;
    sliders[1].yellow = (int) (.33 * ((double) sliders[1].max));
    sliders[1].red = (int) (.66 * ((double) sliders[1].max));

    sliders[2].min = 0;
    sliders[2].max = me->p_ship.s_maxfuel;
    sliders[2].yellow = (int) (.33 * ((double) sliders[2].max));
    sliders[2].red = (int) (.66 * ((double) sliders[2].max));

    sliders[3].min = 0;
    sliders[3].max = me->p_ship.s_maxwpntemp;
    sliders[3].yellow = (int) (.33 * ((double) sliders[3].max));
    sliders[3].red = (int) (.66 * ((double) sliders[3].max));

    sliders[4].min = 0;
    sliders[4].max = me->p_ship.s_maxegntemp;
    sliders[4].yellow = (int) (.33 * ((double) sliders[4].max));
    sliders[4].red = (int) (.66 * ((double) sliders[4].max));

    for (i = 0; i < NUM_SLIDERS; i++)
        sliders[i].diff = sliders[i].max - sliders[i].min;

}

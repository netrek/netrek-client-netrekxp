/******************************************************************************/
/***  File:  interface.c
/***
/***  Function: This file will include all the interfaces between the input 
/***            routines and the daemon.  They should be useful for writing 
/***            robots and the like
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "proto.h"


/******************************************************************************/
/***  set_speed()
/******************************************************************************/
void
set_speed (int speed)
{
    sendSpeedReq ((char) speed);
}


/******************************************************************************/
/***  set_course()
/******************************************************************************/
void
set_course (unsigned char dir)
{
    sendDirReq (dir);
}


/******************************************************************************/
/***  shield_up()
/******************************************************************************/
void
shield_up (void)
{
    if (!(me->p_flags & PFSHIELD))
    {
        sendShieldReq (1);
    }
}

/******************************************************************************/
/***  shield_down()
/******************************************************************************/
void
shield_down (void)
{
    if (me->p_flags & PFSHIELD)
    {
        sendShieldReq (0);
    }
}

/******************************************************************************/
/***  shield_tog()
/******************************************************************************/
void
shield_tog (void)
{
    if (me->p_flags & PFSHIELD)
    {
        sendShieldReq (0);
    }
    else
    {
        sendShieldReq (1);
    }
}

/******************************************************************************/
/***  bomb_planet()
/******************************************************************************/
void
bomb_planet (void)
{
    if (!(me->p_flags & PFBOMB))
    {
        sendBombReq (1);
    }
}

/******************************************************************************/
/***  beam_up()
/******************************************************************************/
void
beam_up (void)
{
    if (!(me->p_flags & PFBEAMUP))
    {
        sendBeamReq (1);        /* 1 means up... */
    }
}

/******************************************************************************/
/***  beam_down()
/******************************************************************************/
void
beam_down (void)
{
    if (!(me->p_flags & PFBEAMDOWN))
    {
        sendBeamReq (2);        /* 2 means down... */
    }
}

/******************************************************************************/
/***  repair()
/******************************************************************************/
void
repair (void)
{
    if (!(me->p_flags & PFREPAIR))
    {
        sendRepairReq (1);
    }
}

/******************************************************************************/
/***  repair_off()
/******************************************************************************/
void
repair_off (void)
{
    if (me->p_flags & PFREPAIR)
    {
        sendRepairReq (0);
    }
}

/******************************************************************************/
/***  repeat_message()
/******************************************************************************/
void
repeat_message (void)
{
    if (++lastm == MAXMESSAGE);
    lastm = 0;
}

/******************************************************************************/
/***  cloak()
/******************************************************************************/
void
cloak (void)
{
    if (me->p_flags & PFCLOAK)
    {
        sendCloakReq (0);
    }
    else
    {
        sendCloakReq (1);
    }
}

/******************************************************************************/
/***  cloak_on()
/******************************************************************************/
void
cloak_on (void)
{
    if (!(me->p_flags & PFCLOAK))
    {
        sendCloakReq (1);
    }
}

/******************************************************************************/
/***  cloak_off()
/******************************************************************************/
void
cloak_off (void)
{
    if (me->p_flags & PFCLOAK)
    {
        sendCloakReq (0);
    }
}

/******************************************************************************/
/***  mstime()
/******************************************************************************/
unsigned long
mstime (void)
{
    static unsigned long base;
    if (!base)
    {
        base = GetTickCount ();
        return (0);
    }
    return (GetTickCount () - base);
}

/******************************************************************************/
/***  msetime()
/******************************************************************************/
unsigned long
msetime (void)
{
    return GetTickCount ();
}

/******************************************************************************/
/***  run_clock()
/******************************************************************************/
void
run_clock (time_t curtime)
{
    char timebuf[9];
    struct tm *tm;

#ifdef BRMH
    static time_t tt;

    if ((curtime - tt) < 1)
        return;

    tm = localtime (&curtime);
    timebuf[0] = (char) (tm->tm_hour / 10 + '0');
    timebuf[1] = (char) ((tm->tm_hour % 10) + '0');
    timebuf[2] = ':';
    timebuf[3] = (char) (tm->tm_min / 10 + '0');
    timebuf[4] = (char) ((tm->tm_min % 10) + '0');
    timebuf[5] = ':';
    timebuf[6] = (char) (tm->tm_sec / 10 + '0');
    timebuf[7] = (char) ((tm->tm_sec % 10) + '0');
#else
    sprintf (timebuf, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
#endif

    tm = NULL;

    switch (newDashboard)
    {
    case 1:
    case 2:
        W_WriteText (tstatw, 2, 30, textColor, timebuf, 8, W_RegularFont);
        break;
    default:
        W_WriteText (tstatw, 446, 27, textColor, timebuf, 8, W_RegularFont);
    }
    tt = curtime;
}

/******************************************************************************/
/***  File:  defwin.c                                                       ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include "config.h"

#ifdef XTREKRC_HELP
/* taken from helpwin.c (copyright 1991 ERic mehlhaff Free to use, hack, etc.
 * Just keep these credits here. Use of this code may be dangerous to your
 * health and/or system. Its use is at your own risk. I assume no
 * responsibility for damages, real, potential, or imagined, resulting  from
 * the use of it.)
 * 
 * $Log: defwin.c,v $
 * Revision 1.5  2006/05/14 02:14:54  modemhero
 * New planet bitmaps!  Using Defcom's art.  Changeable via planets menu.
 * New netrekrc option, "planetBitmapGalaxy: (0-3)", same options as planetBitmap, but now you have
 * the choice to change map display planets too!  And have map and local planets use different
 * bitmap sets
 * Fixed bug where map window border wasn't being redrawn on death
 * Shortpackets is now off by default.  In the current state of internet connectivity, most people
 *  don't need the reduced packets, which don't send complete information and break certain features
 *  such as which direction other players are moving, robot shields, observer geno messages,
 *  shield/cloak status for warp 0 players, etc.
 * Fix to problem with bottom and right borders in certain windows (like map) getting overwritten - thanks Stas!
 * Client now recognizes planets that are flagged as "core", waiting on server patch to
 * actually get this information and do something with it
 *
 * Revision 1.4  2006/05/07 16:59:27  modemhero
 * Major features in this patch are:
 * Merge of Stas' latest source into client.
 *  - Lots of double buffering code
 *  - Cleanup of protoyping functions and proper variable initialization
 *  - Addition of working RSA key generator mkkey.exe (this necessitates another DLL in the source)
 *  - Updated compile instructions, and a new document on how to make a RSA key
 *  - Working version of winkey with BCC compiler
 *  - Bug fixes as per listed in his change log
 * Cleanup of changes list to remove bug fix/stuff only coders need to know.
 * Removal of buildexe script - Stas rewrote build to make this obsolete
 * Addition of HR bitmap set (including a few placeholder bitmaps until art is done)
 * Metablock patch accepted (not working proper yet though due to retrieving login issue)
 * Probably a few other things I forgot about!
 *
 * Revision 1.2  2004/08/11 00:12:56  stas_p
 * replaced printf by more generic console functions fro console.c
 *
 * Revision 1.2  1999/06/13 05:51:49  sheldon
 * Added code for Cambot playback
 *
 * Revision 1.1.1.1  1999/03/10 06:21:13  sheldon
 * Initial public release of Netrek:1999
 * */

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "math.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "playerlist.h"
#include "proto.h"

/* this is the number of help messages there are */

#define INT_DEF		0
#define BOOL_DEF	1
#define STR_DEF		2
#define SINT_DEF	3

#define NAME_WIDTH	18
#define VAL_WIDTH	8
#define INDENT		3
#define MAX_VLINES	42

#define DEFMESSAGES	(sizeof(def_messages)/ sizeof(struct def))

char *name = NULL, *cloak_chars = NULL, *bmap = NULL, *keymap = NULL,
    *plist = NULL, *ckeymap = NULL;

/* sure its a mess, but it gets the job done */

static struct def
{
    char *name;
    int type;
    char *desc;
    int *variable;

    struct
    {
        int i_value;            /* if int or bool */
        char *s_value;          /* if str */
        char *desc;
    }
    values[10];
}

def_messages[] =
{
    {
        "autoQuit", INT_DEF, "Autoquit timer", &autoQuit,
        {
            {60, NULL, NULL}
        ,}
    ,}
    ,
    {
        "buttonmap", STR_DEF, "Mouse button map", (int *) &(bmap),
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "ckeymap", STR_DEF, "Control keyboard map", (int *) &(ckeymap),
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "cloakChars", STR_DEF, "Cloak chars for map", (int *) &(cloak_chars),
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "continueTractor", BOOL_DEF, "Keep showing tract/press",
            &continueTractor,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#ifdef MOTION_MOUSE
    {
        "continuousMouse", BOOL_DEF, "Continuous mouse input", &continuousMouse,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#endif
    {
        "enemyPhasers", SINT_DEF, "Width of enemy phasers", &enemyPhasers,
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "extraAlertBorder", BOOL_DEF, "Show alert on local border",
            &extraAlertBorder,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "ignoreCaps", BOOL_DEF, "Ignore the Capslock key", &ignoreCaps,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "keepInfo", SINT_DEF, "No. of updates to keep info windows",
            &keepInfo,
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "keepPeace", BOOL_DEF, "Stay peaceful when reborn", &keepPeace,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "keymap", STR_DEF, "Keyboard map", (int *) &(keymap),
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
#ifdef MOTION_MOUSE
    {
        "motionThresh", SINT_DEF, "Threshold for continuousMouse", &motionThresh,
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
#endif
    {
        "name", STR_DEF, "Default player name", (int *) &(name),
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "newDashboard", INT_DEF, "Use new dashboard", &newDashboard,
        {
            {0, NULL, "Text dashboard"},
            {1, NULL, "COW style dashboard"},
            {2, NULL, "KRP style dashboard"},
            {3, NULL, "LABs new dashboard"},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "newDistress", BOOL_DEF, "Use new distress call", &newDistress,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "newPlist", BOOL_DEF, "Show new player list", &newPlist,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "partitionPlist", BOOL_DEF, "Use blank space in player list",
            &partitionPlist,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "playerlist", STR_DEF, "What to show on custom player list",
            (int *) &(plistCustomLayout),
        {
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "playerListStyle", INT_DEF, "The style for the playerlist",
            &playerListStyle,
        {
            {0, NULL, "Custom player list"},
            {1, NULL, "Old Player List"},
            {2, NULL, "COW Player List"},
            {3, NULL, "Kill Watch Player List"},
            {4, NULL, "BRMH Player List"},
            {0, NULL, NULL}
        ,}
    }
    ,
    {
        "rejectMacro", BOOL_DEF, "Reject macros", &rejectMacro,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "reportKills", BOOL_DEF, "Report kill messages", &reportKills,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#ifdef SHIFTED_MOUSE
    {
        "shiftedMouse", BOOL_DEF, "More mouse buttons with shift",
            &shiftedMouse,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#endif
    {
        "showIND", BOOL_DEF, "Show independant planets w/X", &showIND,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "showLock", INT_DEF, "Lock display for planets/players", &showLock,
        {
            {0, NULL, "don't show lock"},
            {1, NULL, "show lock on galactic only"},
            {2, NULL, "show lock on tactical only"},
            {3, NULL, "show lock on both"},
            {0, NULL, NULL}
        ,}
    }
    ,
    {
        "showPlanetNames", BOOL_DEF, "Show names on map/local", &showPlanetNames,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "showTractorPressor", BOOL_DEF, "Show my tract/press",
            &showTractorPressor,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#ifdef SHORT_PACKETS
    {
        "tryShort", BOOL_DEF, "Use short packets for communications", &tryShort1,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#endif
    {
        "tryUdp", BOOL_DEF, "Use UDP for communications", &tryUdp1,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "udpClientReceive", INT_DEF, "UDP receive mode", &udpClientReceive,
        {
            {0, NULL, "TCP only"},
            {1, NULL, "simple UDP"},
            {2, NULL, "fat UDP"},
            {3, NULL, "double UDP (obsolete)"},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "udpClientSend", INT_DEF, "UDP send mode", &udpClientSend,
        {
            {0, NULL, "TCP only"},
            {1, NULL, "simple UDP"},
            {2, NULL, "enforced UDP (state only)"},
            {3, NULL, "enforced UDP (state & weapon)"},
            {0, NULL, NULL}
        ,}
    ,}
    ,
    {
        "udpSequenceCheck", BOOL_DEF, "UDP sequence checking",
            &udpSequenceCheck,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#ifdef RSA
    {
        "useRSA", BOOL_DEF, "Use RSA checking", &useRsa,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#endif
#ifdef VSHIELD_BITMAPS
    {
        "varyShields", BOOL_DEF, "Vary shields base on damage",
            &varyShields,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
#endif
    {
        "warnShields", BOOL_DEF, "Shield color based on alert status",
            &warnShields,
        {
            {0, NULL, ""},
            {0, NULL, NULL}
        ,}
    ,}
    ,
};

/******************************************************************************/
/***  itos()                                                                ***/
/******************************************************************************/
static char *
itos (int v)
{
    static char value[10];

    sprintf (value, "%d", v);
    return value;
}

/******************************************************************************/
/***  btoa()                                                                ***/
/******************************************************************************/
static char *
btoa (int v)
{
    if (v)
        return "on";
    else
        return "off";
}

/******************************************************************************/
/***  showdef()                                                             ***/
/******************************************************************************/
void
showdef (void)
{
    register i, j, x = 0, y = 0, xo = 0, yo = 0, max_desc = 0, height = 1,
        width = 1;
    register struct def *d;
    char *val;

    name = stringDefault ("name");
    keymap = stringDefault ("keymap");
    ckeymap = stringDefault ("ckeymap");
    plist = stringDefault ("playerlist");
    cloak_chars = cloakChars;
    bmap = stringDefault ("buttonmap");

    if (!defWin)
        defWin =
            W_MakeTextWindow ("xtrekrc_help", 1, 100, 174, 41, NULL, BORDER);

    for (i = 0, d = def_messages; i < DEFMESSAGES; i++, d++)
    {
        x = xo;
        y = yo;

        W_WriteText (defWin, x, y, W_Yellow, d->name, strlen (d->name),
                     W_BoldFont);
        x += NAME_WIDTH;

        W_WriteText (defWin, x, y, textColor, d->desc, strlen (d->desc),
                     W_RegularFont);
        if (strlen (d->desc) > (unsigned int) max_desc)
        {
            max_desc = strlen (d->desc);
            width = MAX (width, x + max_desc);
        }
        y++;
        x = xo + INDENT;

        if (d->type != STR_DEF)
        {
            if (!d->values[0].desc && d->variable)
            {
                if (d->type == SINT_DEF)
                    val = itos (*d->variable);
                else
                    val = itos (d->values[0].i_value);

                W_WriteText (defWin, x, y, W_Green, val, strlen (val),
                             W_RegularFont);
                y++;
            }
            for (j = 0; d->values[j].desc; j++)
            {
                switch (d->type)
                {
                case INT_DEF:
                    val = itos (d->values[j].i_value);
                    if (d->values[j].i_value == *d->variable)
                    {
                        W_WriteText (defWin, x, y, W_Green, val, strlen (val),
                                     W_BoldFont);
                        if (W_Mono ())
                        {
                            W_WriteText (defWin, x + 1, y, W_Green, "*", 1,
                                         W_RegularFont);
                        }
                    }
                    else
                        W_WriteText (defWin, x, y, textColor, val,
                                     strlen (val), W_RegularFont);
                    x = xo + NAME_WIDTH;
                    W_WriteText (defWin, x, y, textColor, d->values[j].desc,
                                 strlen (d->values[j].desc), W_RegularFont);
                    y++;
                    x = xo + INDENT;
                    break;

                case BOOL_DEF:
                    val = btoa (*d->variable);
                    W_WriteText (defWin, x, y, W_Green, val, strlen (val),
                                 W_RegularFont);
                    y++;
                    x = xo + INDENT;
                    break;
                default:
                    LineToConsole ("Unknown type.\n");
                    break;
                }
            }
        }
        else if (d->variable && *d->variable)
        {
            W_WriteText (defWin, x, y, W_Green, *((char **) d->variable),
                         strlen (*((char **) d->variable)), W_RegularFont);
            y++;
        }

        height = MAX (height, y);
        if (y > MAX_VLINES)
        {
            yo = 0;
            xo += NAME_WIDTH + max_desc + 2;
            max_desc = 0;
        }
        else
        {
            yo = y + 1;
        }
    }

    W_ResizeTextWindow (defWin, width, height);
    W_MapWindow (defWin);
}

#endif

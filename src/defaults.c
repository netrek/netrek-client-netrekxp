/******************************************************************************/
/***  File:  defaults.c                                                     ***/
/***  Function:  This file reads the default parameters from .xtrekrc and   ***/
/***             sets appropriate flags.                                    ***/
/***                                                                        ***/
/***  Author:  Kevin P. Smith 6/11/89                                       ***/
/******************************************************************************/

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <limits.h>

#include "config.h"
#include "copyright2.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "playerlist.h"
#include "proto.h"

struct stringlist
{
    char *string;
    char *value;
    struct stringlist *next;
};

struct stringlist *defaults = NULL;

#define DEFAULTSHIP NUM_TYPES

struct shipdef shipdefaults[NUM_TYPES + 1] = {
    {"sc", NULL, NULL, NULL, NULL},
    {"dd", NULL, NULL, NULL, NULL},
    {"ca", NULL, NULL, NULL, NULL},
    {"bb", NULL, NULL, NULL, NULL},
    {"as", NULL, NULL, NULL, NULL},
    {"sb", NULL, NULL, NULL, NULL},
    {"ga", NULL, NULL, NULL, NULL},
    {"att", NULL, NULL, NULL, NULL},
    {"default", NULL, NULL, NULL, NULL}
};

int myshiptype = DEFAULTSHIP;
struct shipdef *myshipdef = &shipdefaults[DEFAULTSHIP];

struct save_options save_options[] = {
    {"autoQuit", &autoQuit, RC_INT,
        {
            "Autoquit timer (default 60)",
            NULL
        }
    },
    {"baseUdpLocalPort", &baseUdpLocalPort, RC_INT,
        {
            "Base UDP local port",
            NULL
        }
    },
    {"colorClient", &colorClient, RC_INT,
        {
            "What type of ship bitmaps to load",
            "0 - mono",
            "1 - color (default)",
            NULL
        }
    },
#ifdef JUBILEE_PHASERS
    {"colorfulPhasers", &colorfulPhasers, RC_BOOL,
        {
            "Display color phasers",
            NULL
        }
    },
#endif
    {"continueTractor", &continueTractor, RC_BOOL,
        {
            "Show tractor/pressor after lock",
            NULL
        }
    },
#ifdef MOTION_MOUSE
    {"continuousMouse", &continuousMouse, RC_BOOL,
        {
            "Use mouse for continuous steering",
            NULL
        }
    },
#endif
    {"enemyPhasers", &enemyPhasers, RC_INT,
        {
            "Angle between enemy phaser lines",
            "1-10 range",
            NULL
        }
    },
    {"extraAlertBorder", &extraAlertBorder, RC_BOOL,
        {
            "Extend alter borders",
            NULL
        }
    },
    {"forceDisplay", &forceDisplay, RC_INT,
        {
            "Number of colors the client will display",
            "0 - mono",
            "1 - 16 colors",
            "2 - 256 colors",
            "3 - true color (default)",
            NULL
        }
    },
    {"forceMono", &forceMono, RC_BOOL,
        {
            "Force old bitmaps",
            NULL
        }
    },
    {"highlightFriendlyPhasers", &highlightFriendlyPhasers, RC_BOOL,
        {
            "Highlight friendly phasers",
            NULL
        }
    },
    {"ignoreCaps", &ignoreCaps, RC_BOOL,
        {
            "Ignore Caps Lock key state",
            NULL
        }
    },
    {"keepInfo", &keepInfo, RC_INT,
        {
            "How much updates to keep info window on (default 15)",
            NULL
        }
    },
    {"keepPeace", &keepPeace, RC_BOOL,
        {
            "Keep peace with enemy team after death",
            NULL
        }
    },
    {"logging", &logging, RC_BOOL,
        {
            "Log all messages to file. Requires \"logfile\" option",
            NULL
        }
    },
#ifdef XTRA_MESSAGE_UI
    {"messageHoldThresh", &messageHoldThresh, RC_INT,
        {
            "Message hold threshold",
            NULL
        }
    },
    {"messageHUD", &messageHUD, RC_INT,
        {
            "Output message to local window while typing",
            "0 - nowhere",
            "1 - top of the window",
            "2 - bottom of the window",
            NULL
        }
    },
#endif
    {"metaPort", &metaPort, RC_INT,
        {
            "Metaserver port",
            NULL
        }
    },
    {"metaStatusLevel", &metaStatusLevel, RC_INT,
        {
            "What servers to get from metaserver",
            "0 - with players",
            "1 - as above + with queue",
            "2 - as above + with nobody playing",
            "3 - as above + timed out servers (default)",
            "4 - as above + servers that metaserver could not connect to",
            NULL
        }
    },
#ifdef MOTION_MOUSE
    {"motionThresh", &motionThresh, RC_INT,
        {
            "Mouse motion threshold",
            NULL
        }
    },
#endif
#ifdef MOUSE_AS_SHIFT
    {"mouseAsShift", &mouseAsShift, RC_BOOL,
        {
            "Use mouse buttons as Shift keys",
            NULL
        }
    },
#endif
    {"mungScrollbarColors", &mungScrollbarColors, RC_BOOL,
        {
            "Paint dark scrollbars",
            NULL
        }
    },
    {"newDashboard", &newDashboard, RC_INT,
        {
            "Type of dashboard",
            "0 - Text",
            "1 - COW",
            "2 - KRP (default)",
            "3 - LABs",
            NULL
        }
    },
    {"newDistress", &newDistress, RC_BOOL,
        {
            "Indent RCD messages",
            NULL
        }
    },
    {"partitionPlist", &partitionPlist, RC_BOOL,
        {
            "Add space between teams in player list",
            NULL
        }
    },
    {"phaserMsg", &phaserMsg, RC_INT,
        {
            "Window to write phaser messages to",
            "0 - none",
            "1 - all",
            "2 - team",
            "3 - indiv",
            "4 - kill",
            "5 - total (default)",
            NULL
        }
    },
    {"phaserShrink", &phaserShrink, RC_INT,
        {
            "Shrink our phaser by x/16 of its length",
            "0-16 range for COW style phaser shrink",
            "0-11 range for BRMH style phaser shrink",
            NULL
        }
    },
    {"phaserShrinkStyle", &phaserShrinkStyle, RC_INT,
        {
            "Style of phaser srhinkage",
            "0 - COW style",
            "1 - BRMH style",
            NULL
        }
    },
#ifdef PHASER_STATS
    {"phaserStats", &phaserStats, RC_BOOL,
        {
            "Log phaser statistics",
            NULL
        }
    },
#endif
    {"phaserWindow", &phaserWindow, RC_BOOL,
        {
            "Show phaser window",
            NULL
        }
    },
    {"planetBitmap", &planetBitmap, RC_INT,
        {
            "Type of planet bitmaps",
            "0 - Bronco (default)",
            "1 - Moo",
            "2 - Rabbitear",
            NULL
        }
    },
    {"playerListObserver", &playerListObserver, RC_INT,
        {
            "What kind of players to list",
            "0 - all (default)",
            "1 - players",
            "2 - observers",
            NULL
        }
    },
    {"playerListStyle", &playerListStyle, RC_INT,
        {
            "Predefined player list",
            "0 - use style from playerList",
            "1 - old style \"nTRNKWLr O D d\"",
            "2 - COW style \"nTR N  K lrSd\" (default)",
            "3 - kill watch style \"nTK  RNlr Sd\"",
            "4 - BRMH style \"nTR N  K l M\"",
            NULL
        }
    },
    {"port", &xtrekPort, RC_INT,
        {
            "Port to connect",
            NULL
        }
    },
#ifdef UDP_PORTSWAP
    {"portSwap", &portSwap, RC_BOOL,
        {
            "Use new UDP code",
            NULL
        }
    },
#endif
    {"redrawDelay", &redrawDelay, RC_INT,
        {
            "Number of updates before redraw",
            NULL
        }
    },
    {"rejectMacro", &rejectMacro, RC_BOOL,
        {
            "Reject macros",
            NULL
        }
    },
    {"reportKills", &reportKills, RC_BOOL,
        {
            "Report kills",
            NULL
        }
    },
    {"saveBig", &saveBig, RC_BOOL,
        {
            "Save options with comments",
            NULL
        }
    },
    {"saveMacro", &saveMacro, RC_BOOL,
        {
            "Save macros to saveFile",
            NULL
        }
    },
    {"saveRCD", &saveRCD, RC_BOOL,
        {
            "Save RCD to saveFile",
            NULL
        }
    },
#ifdef RCM
    {"saveRCM", &saveRCM, RC_BOOL,
        {
            "Save RCM to saveFile",
            NULL
        }
    },
#endif
#ifdef TOOLS
    {"shellTools", &shellTools, RC_BOOL,
        {
            "Enable shell tools",
            NULL
        }
    },
#endif
#ifdef SHIFTED_MOUSE
    {"shiftedMouse", &shiftedMouse, RC_BOOL,
        {
            "Shift+Mouse gives additional commands",
            NULL
        }
    },
#endif
    {"showAllTractorPressor", &showAllTractorPressor, RC_BOOL, 
        {
            "Show tractor/pressor for all players",
            "This option actually helps to turn the feature off",
            "Server god decides wether to enable it on server",
            NULL
        }
    },
#ifdef HOCKEY_LINES
    {"showHockeyLinesMap", &showHockeyLinesMap, RC_BOOL,
        {
            "Show hockey lines on map",
            "Works only on hockey server",
            NULL
        }
    },
    {"showHockeyLinesLocal", &showHockeyLinesLocal, RC_BOOL,
        {
            "Show hockey lines on local",
            "Works only on hockey server",
            NULL
        }
    },
#endif
    {"showIND", &showIND, RC_BOOL,
        {
            "Cross independent planets with X",
            NULL
        }
    },
    {"showLock", &showLock, RC_INT,
        {
            "Where to show lock triangle",
            "0 - nowhere",
            "1 - map",
            "2 - local",
            "3 - both (default)",
            NULL
        }
    },
    {"showMotd", &showMotd, RC_BOOL,
        {
            "Show MOTD before login",
            NULL
        }
    },
    {"showMySpeed", &showMySpeed, RC_BOOL,
        {
            "Show my speed on local",
            NULL
        }
    },
    {"sortMyTeamFirst", &sortMyTeamFirst, RC_BOOL,
        {
            "Put my team first in the player list",
            NULL
        }
    },
    {"showPlanetNames", &showPlanetNames, RC_BOOL,
        {
            "Show planet names on local",
            NULL
        }
    },
    {"showPlanetOwner", &showPlanetOwner, RC_BOOL,
        {
            "Show planet owner on galaxy",
            NULL
        }
    },
    {"showStats", &showStats, RC_BOOL,
        {
            "Show statistics window",
            NULL
        }
    },
    {"showTractorPressor", &showTractorPressor, RC_BOOL,
        {
            "Draw lines for tractor/pressor",
            NULL
        }
    },
    {"shrinkPhaserOnMiss", &shrinkPhaserOnMiss, RC_BOOL,
        {
            "Shrink phasers if missed",
            NULL
        }
    },
    {"sortPlayers", &sortPlayers, RC_BOOL,
        {
            "Sort players by team",
            NULL
        }
    },
    {"theirPhaserShrink", &theirPhaserShrink, RC_INT,
        {
            "Shrink enemy phaser by x/16 of its length",
            "1-16 range",
            NULL
        }
    },
    {"tpDotDist", &tpDotDist, RC_INT,
        {
            "Distance between tractor/pressor dots",
            NULL
        }
    },
#ifdef SHORT_PACKETS
    {"tryShort", &tryShort, RC_BOOL,
        {
            "Use short packets for communications",
            NULL
        }
    },
#endif
    {"tryUdp", &tryUdp, RC_BOOL,
        {
            "Use UDP for communications",
            NULL
        }
    },
    {"udpClientReceive", &udpClientReceive, RC_INT,
        {
            "Type of incoming UDP traffic",
            "0 - TCP",
            "1 - simple UDP (default)",
            "2 - fat UDP",
            NULL
        }
    },
    {"udpClientSend", &udpClientSend, RC_INT,
        {
            "Type of outgoing UDP traffic",
            "0 - TCP",
            "1 - simple UDP (default)",
            "2 - enforced UDP (state)",
            "3 - enfotrced UDP (state & weapons)",
            NULL
        }
    },
    {"udpDebug", &udpDebug, RC_INT,
        {
            "Print UDP debug information",
            NULL
        }
    },
    {"udpSequenceCheck", &udpSequenceCheck, RC_BOOL,
        {
            "Check sequence of UDP traffic",
            NULL
        }
    },
    {"updatesPerSec", &updatesPerSec, RC_INT,
        {
            "How much updates to send/receive",
            "1-10 range",
            NULL
        }
    },
#ifdef RSA
    {"useRsa", &useRsa, RC_BOOL,
        {
            "Use RSA verification",
            NULL
        }
    },
#endif
#ifdef VSHIELD_BITMAPS
    {"varyShields", &varyShields, RC_BOOL,
        {
            "Change shields color on ship damage",
            NULL
        }
    },
#endif
    {"warnShields", &warnShields, RC_BOOL,
        {
            "Change shields color on enemy approach",
            NULL
        }
    },
    {NULL, NULL, 0, { NULL }}
};

/*************************************************************************/
/***  prob_desc[] will allow us to have simpler code to handle parsing ***/
/***  for these key words.                                             ***/
/*************************************************************************/
static char *prob_desc[] = { "shld", "dam", "wtmp", "etmp", "arms", "fuel" };
static char *prob_severity[] = { "low", "mid", "high" };

int DefaultsLoaded;
char *GetExeDir ();

#define XTREKRC "xtrekrc"
#define NETREKRC "netrekrc"


/******************************************************************************/
/***  initDefaults()                                                        ***/
/******************************************************************************/
void
initDefaults (char *deffile)
{
    FILE *fp;
    char file[256];
// SRS not referenced char   *home;
    char *v;
    struct stringlist *new;
    struct dmacro_list *dm;
    struct dmacro_list *dm_def;
    int notdone;
    unsigned char c;
    char *str;

#ifdef MULTILINE_MACROS
    unsigned char keysused[256];

    MZERO (keysused, sizeof (keysused));
#endif

    DefaultsLoaded = 1;

#ifdef DEBUG
    printf ("Initdefaults\n");
#endif

    /* sizeof doesn't work if it isn't in the same source file, shoot me */
    MCOPY (dist_defaults, dist_prefered, sizedist);

    getshipdefaults ();

    if (!deffile)
        if (findDefaults (deffile, file))
            deffile = file;
        else
            return;             /* No defaults file! */

    fp = fopen (deffile, "r");
    if (!fp)
        return;
    printf ("Reading defaults file %s\n", deffile);

#ifdef NBT
    macrocnt = 0;               /* reset macros */
#endif

    STRNCPY (defaultsFile, deffile, sizeof (defaultsFile));
    while (fgets (file, 250, fp))
    {
        if (*file == '#')
            continue;
        if (*file != 0)
            file[strlen (file) - 1] = 0;
        v = file;
        while (*v != ':' && *v != 0)
        {
            v++;
        }
        if (*v == 0)
            continue;
        *v = 0;
        v++;
        while (*v == ' ' || *v == '\t')
        {
            v++;
        }

#ifdef NBT
        /* not very robust but if it breaks nothing will die horribly I think -
         * jmn */
        if (strncmpi (file, "macro.", 6) == 0)
        {
            if (macrocnt == MAX_MACRO)
            {
                fprintf (stderr, "Maximum number of macros is %d\n",
                         MAX_MACRO);
            }
            else
            {
                str = file + 6;
                c = getctrlkey ((unsigned char **) &str);
                if (c == '?')
                    fprintf (stderr, "Cannot use '?' for a macro\n");
                else
                {
                    macro[macrocnt].type = NBTM;
                    macro[macrocnt].key = c;
                    macro[macrocnt].who = str[1];
                    macro[macrocnt].string = strdup (v);

#ifdef MULTILINE_MACROS
                    if (keysused[macro[macrocnt].key])
                    {
                        macro[keysused[macro[macrocnt].key] - 1].type =
                            NEWMULTIM;
                        macro[macrocnt].type = NEWMULTIM;
                    }
                    else
                    {
                        keysused[macro[macrocnt].key] = macrocnt + 1;
                    }
#endif /* MULTILINE_MACROS */

                    macrocnt++;
                }
            }
        }
        else
#endif

        if (strncmpi (file, "mac.", 4) == 0)
        {
            if (macrocnt == MAX_MACRO)
            {
                fprintf (stderr, "Maximum number of macros is %d\n",
                         MAX_MACRO);
            }
            else
            {
                str = file + 4;
                c = getctrlkey ((unsigned char **) &str);
                if (c == '?')
                    fprintf (stderr, "Cannot use '?' for a macro\n");
                else
                {
                    macro[macrocnt].key = c;

                    if (str[0] == '.')
                    {
                        if (str[1] == '%')
                        {
                            switch (str[2])
                            {
                            case 'u':
                            case 'U':
                            case 'p':
                                macro[macrocnt].who = MACRO_PLAYER;
                                break;
                            case 't':
                            case 'z':
                            case 'Z':
                                macro[macrocnt].who = MACRO_TEAM;
                                break;
                            case 'g':
                                macro[macrocnt].who = MACRO_FRIEND;
                                break;
                            case 'h':
                                macro[macrocnt].who = MACRO_ENEMY;
                                break;
                            default:
                                macro[macrocnt].who = MACRO_ME;
                                break;
                            }
                            macro[macrocnt].type = NEWMMOUSE;
                        }
                        else
                        {
                            macro[macrocnt].who = str[1];
                            macro[macrocnt].type = NEWMSPEC;
                        }
                    }
                    else
                    {
                        macro[macrocnt].who = '\0';
                        macro[macrocnt].type = NEWM;

#ifdef MULTILINE_MACROS
                        if (keysused[macro[macrocnt].key])
                        {
                            printf
                                ("Multiline macros of nonstandard types are not recommended.\n");
                            printf
                                ("You might experience strange behaviour of macros.\n");
                            printf ("Type: unspecified macro, key: %c.\n",
                                    macro[macrocnt].key);
                        }
#endif /* MULTILINE_MACROS */
                    }

#ifdef MULTILINE_MACROS
                    if (keysused[macro[macrocnt].key])
                    {
                        macro[keysused[macro[macrocnt].key] - 1].type =
                            NEWMULTIM;
                        macro[macrocnt].type = NEWMULTIM;
                    }
                    else
                    {
                        keysused[macro[macrocnt].key] = macrocnt + 1;
                    }
#endif /* MULTILINE_MACROS */

                    macro[macrocnt].string = strdup (v);
                    macrocnt++;
                }
            }
        }

        else if (strncmpi (file, "dist.", 5) == 0)
        {
            str = file + 5;
            c = getctrlkey ((unsigned char **) &str);
            if (*str != '.')
            {
                str = file + 4;
                c = '\0';
            }
            str++;

            notdone = 1;
            for (dm = &dist_prefered[take], dm_def = &dist_defaults[take];
                 dm->name && notdone; dm++, dm_def++)

            {
                if (strcmpi (str, dm->name) == 0)
                {
                    dm->macro = strdup (v);

#ifdef DIST_KEY_NAME
                    if (c)
                    {
                        dm->c = c;
                        dm_def->c = c;
                    }
#endif /* DIST_KEY_NAME */

                    notdone = 0;
                }
            }
        }


#ifdef RCM
        else if (strncmpi (file, "msg.", 4) == 0)
        {
            str = file + 4;
            notdone = 1;

            for (dm = &rcm_msg[0]; dm->name && notdone; dm++)
            {
                if (strcmpi (str, dm->name) == 0)
                {
                    dm->macro = strdup (v);
                    notdone = 0;
                }
            }
        }
#endif /* RCM */

#ifdef TOOLS                    /* Free configurable macro keys */
        else if (strncmpi (file, "key.", 4) == 0)
        {
            int keycnt;

            if ((keycnt = strlen (keys)) == MAX_KEY - 1)
            {
                fprintf (stderr, "Maximum number of keys is %d\n",
                         MAX_KEY - 1);
            }
            else
            {
                str = file + 4;
                c = getctrlkey ((unsigned char **) &str);
                keys[keycnt] = c;
                keys[keycnt + 1] = '\0';
                if (*str != '.')
                {
                    c = 't';
                }
                else
                {
                    str++;
                    c = getctrlkey ((unsigned char **) &str);
                }
                macroKeys[keycnt].dest = c;
                macroKeys[keycnt].name = strdup (v);
            }
        }
#endif /* Macro Keys */

        else if (strncmpi (file, "singleMacro", 11) == 0)
        {
            int i;

            str = v;
            for (i = 0; *str; i++)
                singleMacro[i] = getctrlkey ((unsigned char **) &str);
            singleMacro[i] = '\0';
        }

        if (*v != 0)
        {
            new = (struct stringlist *) malloc (sizeof (struct stringlist));

            new->next = defaults;
            new->string = strdup (file);
            new->value = strdup (v);
            defaults = new;
        }
    }
    fclose (fp);
}


/******************************************************************************/
/***  getdefault()                                                          ***/
/******************************************************************************/
char *
getdefault (char *str)
{
    struct stringlist *sl;

    sl = defaults;
    while (sl != NULL)
    {
        if (strcmpi (sl->string, str) == 0)
        {
            return (sl->value);
        }
        sl = sl->next;
    }
    return (NULL);
}


#ifndef __BORLANDC__
/******************************************************************************/
/***  strncmpi()                                                            ***/
/***  grr... are you telling me this sort of function isn't in the std      ***/
/***  libraries somewhere?! sons of satan... - jn                           ***/
/******************************************************************************/
strncmpi (char *str1, char *str2, int max)
{
    char chr1, chr2;
    register int duh, stop;

    stop = strlen (str1);

    if (stop < max)
        return -1;

    if (stop > max)
        stop = max;

    if ((unsigned int) stop > strlen (str2))
        return 1;

    for (duh = 0; duh < stop; duh++)
    {
        chr1 = isupper (str1[duh]) ? str1[duh] : toupper (str1[duh]);
        chr2 = isupper (str2[duh]) ? str2[duh] : toupper (str2[duh]);
        if (chr1 == 0 || chr2 == 0)
        {
            return (0);
        }
        if (chr1 != chr2)
        {
            return (chr2 - chr1);
        }
    }
    return (0);
}
#endif

/******************************************************************************/
/***  stringDefault()                                                          ***/
/******************************************************************************/
char *
stringDefault (char *def)
{
    char *str;

    str = getdefault (def);

    return (str);
}


/******************************************************************************/
/***  booleanDefault()                                                      ***/
/******************************************************************************/
booleanDefault (char *def, int preferred)
{
    char *str;

    str = getdefault (def);

    if (!str)
        return (preferred);

    if ((strncmpi (str, "on", 2) == 0) ||
        (strncmpi (str, "true", 4) == 0) ||
        (strncmpi (str, "1", 1) == 0))
        return (1);
    else
        return (0);
}

/******************************************************************************/
/***  intDefault()                                                          ***/
/******************************************************************************/
intDefault (char *def, int preferred)
{
    char *str;

    str = getdefault (def);

    if (!str)
        return preferred;
    return atoi (str);
}

/* no default file given on command line. See if serverName is defined.  If
 * it exists we look for HOME/.xtrekrc-<serverName> and .xtrekrc-<serverName>
 * Otherwise we try DEFAULT_SERVER. */

/* since this is Find Defaults, I moved all the defaults file checking to *
 * it, and put in support for a system defaults file. * and it uses the
 * access() system call to determine if a defaults *  file exists. * note,
 * access() returns 0 if user can read file, -1 on error or if * they can't. *
 * -EM *
 * 
 * Is anyone else bothered by the fact that this writes to deffile * without
 * really knowing how much of deffile is allocated? *
 * 
 */

/******************************************************************************/
/***  findDefaults()                                                        ***/
/******************************************************************************/
// SRS - flag for later followup, what is the purpose of deffile?
int
findDefaults (char *deffile,
              char *file)
{

    /* Check base names */
    if (findfile (NETREKRC, file))
        return 1;

    if (findfile (XTREKRC, file))
        return 1;

#ifdef SYSTEM_DEFAULTFILE
    /* now try for a system default defaults file */
    if (findfile (SYSTEM_DEFAULTFILE, file))
        return 1;
#endif

    return 0;
}

/******************************************************************************/
/***  resetdefaults()                                                       ***/
/******************************************************************************/
void
resetdefaults (void)
{
    char *pek;
    char tmp[100];
    int i;

#ifdef HOCKEY_LINES
    showHockeyLinesLocal =
        booleanDefault ("showHockeyLinesLocal", showHockeyLinesLocal);
    showHockeyLinesMap =
        booleanDefault ("showHockeyLinesMap", showHockeyLinesMap);
#endif

    saveBig = booleanDefault ("saveBig", saveBig);
    saveMacro = booleanDefault ("saveMacro", saveMacro);
    saveRCD = booleanDefault ("saveRCD", saveRCD);
#ifdef RCM
    saveRCM = booleanDefault ("saveRCM", saveRCM);
#endif

    keepInfo = intDefault ("keepInfo", keepInfo);
    showPlanetOwner = booleanDefault ("showPlanetOwner", showPlanetOwner);
    newDashboard = intDefault ("newDashboard", newDashboard);
    updatesPerSec = intDefault ("updatesPerSec", updatesPerSec);
    redrawDelay = intDefault ("redrawDelay", redrawDelay);
    logging = booleanDefault ("logging", logging);

    phaserShrink = intDefault ("phaserShrink", phaserShrink);
    if (phaserShrink > 16)
        phaserShrink = 16;

    // 0 - COW phasers shrink, 1 - BRMH phasers shrink
    phaserShrinkStyle = intDefault ("phaserShrinkStyle", phaserShrinkStyle);

    theirPhaserShrink = intDefault ("theirPhaserShrink", theirPhaserShrink);
    if (phaserShrink > 16)
        phaserShrink = 16;

    shrinkPhaserOnMiss =
        booleanDefault ("shrinkPhaserOnMiss", shrinkPhaserOnMiss);


#ifdef VSHIELD_BITMAPS
    varyShields = booleanDefault ("varyShields", varyShields);
#endif

    warnShields = booleanDefault ("warnShields", warnShields);

#ifdef RSA
    if (useRsa >= 0)
    {
        useRsa = booleanDefault ("useRsa", useRsa);
        sprintf (tmp, "useRSA.%s", serverName);
        useRsa = booleanDefault (tmp, useRsa);

    }
    else
    {
        /* RSA mode was specified in the command line args */
        useRsa = (useRsa == -2) ? 1 : 0;
    }
#endif


    showLock = intDefault ("showLock", showLock);
    if (showLock > 3)
        showLock = 3;

    phaserMsg = intDefault ("phaserMsg", phaserMsg);
#ifdef XTRA_MESSAGE_UI
    messageHUD = intDefault ("messageHUD", messageHUD);
    messageHoldThresh = intDefault ("messageHoldThresh", messageHoldThresh);
#endif
    phaserMsg = intDefault ("phaserMsg", phaserMsg);
#ifdef PHASER_STATS
    phaserStats = booleanDefault ("phaserStats", phaserStats);
#endif
    showStats = booleanDefault ("showStats", showStats);
    keepPeace = booleanDefault ("keepPeace", keepPeace);
    continueTractor = booleanDefault ("continueTractor", continueTractor);
    showTractorPressor =
        booleanDefault ("showTractorPressor", showTractorPressor);
    showAllTractorPressor =
        booleanDefault ("showAllTractorPressor", showAllTractorPressor);
    extraAlertBorder = booleanDefault ("extraAlertBorder", extraAlertBorder);
    showPlanetNames = booleanDefault ("showPlanetNames", 1);
    reportKills = booleanDefault ("reportKills", reportKills);

    udpDebug = intDefault ("udpDebug", udpDebug);
    udpClientSend = intDefault ("udpClientSend", udpClientSend);
    /* note: requires send */
    udpClientReceive = intDefault ("udpClientReceive", udpClientReceive);
    tryUdp = booleanDefault ("tryUdp", tryUdp);
    tryUdp1 = tryUdp;
    udpSequenceCheck = booleanDefault ("udpSequenceCheck", udpSequenceCheck);
    baseUdpLocalPort = intDefault ("baseUdpLocalPort", baseUdpLocalPort);

#ifdef SHORT_PACKETS
    tryShort = booleanDefault ("tryShort", tryShort);
    tryShort1 = tryShort;
#endif

    newDistress = booleanDefault ("newDistress", newDistress);
    rejectMacro = booleanDefault ("rejectMacro", rejectMacro);
    enemyPhasers = intDefault ("enemyPhasers", enemyPhasers);
    pek = stringDefault ("cloakChars");
    if (pek != (char *) NULL)
        STRNCPY (cloakChars, pek, 3);
    showIND = booleanDefault ("showIND", showIND);
    InitPlayerList ();


    highlightFriendlyPhasers = booleanDefault ("highlightFriendlyPhasers",
                                               highlightFriendlyPhasers);

#ifdef MOUSE_AS_SHIFT
    mouseAsShift = booleanDefault ("mouseAsShift", mouseAsShift);
#endif

#ifdef MOTION_MOUSE
    continuousMouse = booleanDefault ("continuousMouse", continuousMouse);

    motionThresh = intDefault ("motionThresh", motionThresh);
#endif

#ifdef SHIFTED_MOUSE
    shiftedMouse = booleanDefault ("shiftedMouse", shiftedMouse);
#endif

    /* SRS 12/94, ignore the Capslock key */
    ignoreCaps = booleanDefault ("ignoreCaps", ignoreCaps);

    showMySpeed = booleanDefault ("showMySpeed", showMySpeed);

#ifdef JUBILEE_PHASERS
    colorfulPhasers = booleanDefault ("colorfulPhasers", colorfulPhasers);
#endif

#ifdef SOUND
    sound_init = booleanDefault ("sound", sound_init);
#endif

#ifdef TOOLS
    shellTools = booleanDefault ("shellTools", shellTools);
#endif

#ifdef UDP_PORTSWAP
    portSwap = booleanDefault ("portSwap", TRUE);
#endif


    shipdefaults[DEFAULTSHIP].keymap =
        (unsigned char *) stringDefault ("keymap");
    shipdefaults[DEFAULTSHIP].buttonmap =
        (unsigned char *) stringDefault ("buttonmap");
    shipdefaults[DEFAULTSHIP].ckeymap =
        (unsigned char *) stringDefault ("ckeymap");

    for (i = DEFAULTSHIP; i >= 0; i--)
    {
        STRNCPY (tmp, "rcfile-", 8);
        strcat (tmp, shipdefaults[i].name);
        if (pek = stringDefault (tmp))
            shipdefaults[i].rcfile = pek;
        else
            shipdefaults[i].rcfile = shipdefaults[DEFAULTSHIP].rcfile;

        STRNCPY (tmp, "keymap-", 8);
        strcat (tmp, shipdefaults[i].name);
        if (pek = stringDefault (tmp))
            shipdefaults[i].keymap = (unsigned char *) pek;
        else
            shipdefaults[i].keymap = shipdefaults[DEFAULTSHIP].keymap;

        STRNCPY (tmp, "ckeymap-", 9);
        strcat (tmp, shipdefaults[i].name);
        if (pek = stringDefault (tmp))
            shipdefaults[i].ckeymap = (unsigned char *) pek;
        else
            shipdefaults[i].ckeymap = shipdefaults[DEFAULTSHIP].ckeymap;

        STRNCPY (tmp, "buttonmap-", 11);
        strcat (tmp, shipdefaults[i].name);
        if (pek = stringDefault (tmp))
            shipdefaults[i].buttonmap = (unsigned char *) pek;
        else
            shipdefaults[i].buttonmap = shipdefaults[DEFAULTSHIP].buttonmap;
    }
    myshipdef = &shipdefaults[myshiptype];
}

/******************************************************************************/
/***  shipchange()                                                          ***/
/******************************************************************************/
void
shipchange (int type)
{
    if (type == myshiptype)
        return;
    myshiptype = type;
    myshipdef = &shipdefaults[type];
    if (shipdefaults[type].rcfile)
    {
        initDefaults (shipdefaults[type].rcfile);
        resetdefaults ();
    }
    initkeymap ();
}


/* Generally useful function that searches for a file
   in the current and home directories, also
   the executable directory on Win32 */

#ifdef DEBUG
#define CHECK_FILE \
      printf("Checking for file %s...\n", found); \
      accessible = access(found, R_OK); \
      if (accessible == 0)\
        return 1;
#else
#define CHECK_FILE \
      accessible = access(found, R_OK); \
      if (accessible == 0)\
        return 1;
#endif

/******************************************************************************/
/***  findfile()                                                            ***/
/******************************************************************************/
int
findfile (char *fname,
          char *found)
{
    int accessible;
    char *home;

    /* check current directory first */
#ifdef DEBUG
    printf ("Checking for file %s\n", fname);
#endif
    accessible = access (fname, R_OK);
    if (accessible == 0)
    {
        strcpy (found, fname);
        return 1;
    }

    /* Check home directory next */
    home = getenv ("HOME");
    if (home)
    {
        int len = strlen (home);
        if (home[len - 1] == '/' || home[len - 1] == '\\')
            sprintf (found, "%s%s", home, fname);
        else
            sprintf (found, "%s/%s", home, fname);
    }
    CHECK_FILE;

    /* On Windows also check executable directory */
    home = GetExeDir ();
    if (home)
    {
        int len = strlen (home);
        if (home[len - 1] == '/' || home[len - 1] == '\\')
            sprintf (found, "%s%s", home, fname);
        else
            sprintf (found, "%s/%s", home, fname);
    }
    CHECK_FILE;

    return 0;
}


/******************************************************************************/
/***  gethexfromdec()                                                       ***/
/******************************************************************************/
char
gethexfromdec (int dec)
{
    char str[2];

    if (dec > 12)
        return '\0';

    switch (dec)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        sprintf (str, "%d", dec);
        break;
    case 10:
        strcpy (str, "a");
        break;
    case 11:
        strcpy (str, "b");
        break;
    case 12:
        strcpy (str, "c");
        break;
    }

    return str[0];
}

/******************************************************************************/
/***  saveOptions()                                                         ***/
/******************************************************************************/
void
saveOptions ()
{
    FILE *fp;
    char str[250], str1[250];
    char *exe_dir, save_file[256];
    struct save_options *so;
    struct dmacro_list *dm;
    unsigned int i, num_buttons;
    unsigned char c;
    char macroKey[3] = "";

    if (!saveFile)
        saveFile = stringDefault ("saveFile");
    if (!saveFile)
    {
        saveFile = (char *) malloc (sizeof (char) * 12); 
        sprintf (saveFile, "%s", "xtrekrc.sav");
    }

    exe_dir = GetExeDir ();
    if (exe_dir)
    {
        int len = strlen (exe_dir);
        if (exe_dir[len - 1] == '/' || exe_dir[len - 1] == '\\')
            sprintf (save_file, "%s%s", exe_dir, saveFile);
        else
            sprintf (save_file, "%s/%s", exe_dir, saveFile);
    }

    fp = fopen (save_file, "w+");
    if (fp == NULL)
        return;

    so = save_options;
    while (so->name != NULL)
    {
        switch (so->type)
        {
        case RC_INT:
            sprintf (str, "%s: %d\n", so->name, *so->int_value);
            break;
        case RC_BOOL:
            if (*so->int_value)
                sprintf (str, "%s: on\n", so->name);
            else
                sprintf (str, "%s: off\n", so->name);
            break;
        }
        if (saveBig)
        {
            i = 0;
            while (so->desc[i] != NULL)
            {
                sprintf (str1, "# %s\n", so->desc[i]);
                fputs (str1, fp);
                i++;
            }
        }
        fputs (str, fp);
        if (saveBig)
            fputs ("\n", fp);
        so++;
    }

    // buttonmap
#ifdef SHIFTED_MOUSE
    num_buttons = 12;
#else
    num_buttons = 3;
#endif

    str[0] = '\0';
    str1[0] = '\0';
    for (i = 1; i <= num_buttons; i++)
    {
        if (buttonmap[i] != 0)
        {
            c = getkeyfromctrl (buttonmap[i]);
            if (c == '^')
                sprintf (str, "%c^^", gethexfromdec (i));
            else
                sprintf (str, "%c%c", gethexfromdec (i), getkeyfromctrl (buttonmap[i]));
            strcat (str1, str);
        }
    }
    if (saveBig && strlen (str1) != 0)
        fputs ("# Mouse button mapping\n", fp);
    if (strlen (str1) != 0)
    {
        strcpy (str, "buttonmap: ");
        strcat (str, str1);
        strcat (str, "\n");
        fputs (str, fp);
    }
    if (saveBig && strlen (str1) != 0)
        fputs ("\n", fp);

    // cloak chars
    if (saveBig)
        fputs ("# Two characters to show cloaked player on map window\n", fp);
    sprintf (str, "cloakChars: %c%c\n", cloakChars[0], cloakChars[1]);
    fputs (str, fp);
    if (saveBig)
        fputs ("\n", fp);

    // keymap part
    // we're going to print only keymap that differs from standard one
    // we have to start from second key, because first one is space
    str[0] = '\0';
    for (c = 1; c < 95; c++)
    {
        if (c + 32 != mystats->st_keymap[c])
        {
            if (mystats->st_keymap[c] != 'X' &&
               (mystats->st_keymap[c] > 32 &&
                mystats->st_keymap[c] < 127))
                sprintf (str, "%s%c%c", str, c + 32, mystats->st_keymap[c]);
            else
                sprintf (macroKey, "%c", c + 32);
        }
    }
    // space time
    if (mystats->st_keymap[0] != 32)
        sprintf (str, "%s %c", str, mystats->st_keymap[0]);
    if (saveBig && strlen (str) != 0)
        fputs ("# Key mapping\n", fp);
    if (strlen (str) != 0)
    {
        strcpy (str1, "keymap: ");
        strcat (str1, str);
        strcat (str1, "\n");
        fputs (str1, fp);
    }
    if (saveBig && strlen (str) != 0)
        fputs ("\n", fp);

    // macroKey
    if (strlen (macroKey) == 0)
    {
        if (mystats->st_keymap[169] == 'X')
            strcpy (macroKey, "TAB");
        else if (mystats->st_keymap[155] == 'X')
            strcpy (macroKey, "ESC");
    }

    if (strlen (macroKey) != 0)
    {
        if (saveBig)
                fputs ("# Key to switch to macro mode\n# TAB, ESC could be used too\n", fp);
        sprintf (str, "macroKey: %s\n", macroKey);
        fputs (str, fp);
        if (saveBig)
            fputs ("\n", fp);
    }

    // player list
    if (strlen (plistLayout) != 0)
    {
        if (saveBig)
        {
            fputs ("# Custom style for player list\n", fp);
            fputs ("# ' ' - White Space\n", fp);
            fputs ("# 'b' - Armies Bombed\n", fp);
            fputs ("# 'd' - Damage Inflicted (DI)\n", fp);
            fputs ("# 'k' - Max Kills\n", fp);
            fputs ("# 'l' - Login Name\n", fp);
            fputs ("# 'n' - Ship Number", fp);
            fputs ("# 'p' - Planets Taken\n", fp);
            fputs ("# 'r' - Ratio\n", fp);
            fputs ("# 's' - Speed\n", fp);
            fputs ("# 'v' - Deaths per hour\n", fp);
            fputs ("# 'w' - War staus\n", fp);
            fputs ("# 'B' - Bombing\n", fp);
            fputs ("# 'C' - Curt (short) rank\n", fp);
            fputs ("# 'D' - Defense\n", fp);
            fputs ("# 'H' - Hours Played\n", fp);
            fputs ("# 'K' - Kills\n", fp);
            fputs ("# 'L' - Losses\n", fp);
            fputs ("# 'M' - Display, Host Machine\n", fp);
            fputs ("# 'N' - Name\n", fp);
            fputs ("# 'O' - Offense\n", fp);
            fputs ("# 'P' - Planets\n", fp);
            fputs ("# 'R' - Rank\n", fp);
            fputs ("# 'S' - Total Rating (stats)\n", fp);
            fputs ("# 'T' - Ship Type\n", fp);
            fputs ("# 'V' - Kills per hour\n", fp);
            fputs ("# 'W' - Wins\n", fp);
        }
        sprintf (str, "playerList: %s\n", plistLayout);
        fputs (str, fp);

        if (saveBig)
            fputs ("\n", fp);
    }

    fputs ("\n", fp);

    // macros
    if (saveMacro)
    {
        if (saveBig)
        {
            fputs ("# Macros section\n", fp);
            fputs ("# Also '?' can still not be used as a macro key. The following definitions will\n", fp);
            fputs ("# work in a macro (where 'target x' = whatever x the mouse cursor is closest to\n", fp);
            fputs ("# when the macro is sent):\n", fp);
            fputs ("#\n", fp);
            fputs ("#      Standard:\n", fp);
            fputs ("#\n", fp);
            fputs ("#      %a   armies carried by sender\n", fp);
            fputs ("#      %d   sender damage percentage\n", fp);
            fputs ("#      %s   sender shield percentage\n", fp);
            fputs ("#      %f   sender fuel percentage\n", fp);
            fputs ("#      %w   sender wtemp percentage\n", fp);
            fputs ("#      %e   sender etemp percentage\n", fp);
            fputs ("#      %t   team id character of target planet\n", fp);
            fputs ("#      %T   team id character of sender team\n", fp);
            fputs ("#      %c   sender id character\n", fp);
            fputs ("#      %n   armies on target planet\n", fp);
            fputs ("#      %E   1 if etemped, 0 if not\n", fp);
            fputs ("#      %W   1 if wtemped, 0 if not\n", fp);
            fputs ("#      %S   sender two character ship type\n", fp);
            fputs ("#      %p   id character of target player\n", fp);
            fputs ("#      %g   id char of target friendly player\n", fp);
            fputs ("#      %h   id char of target enemy player\n", fp);
            fputs ("#      %P   id character of player nearest sender\n", fp);
            fputs ("#      %G   id char of friendly player nearest sender\n", fp);
            fputs ("#      %H   id char of enemy player nearest sender\n", fp);
            fputs ("#      %l   three character name of target planet\n", fp);
            fputs ("#      %i   sender full player name (16 character max)\n", fp);
            fputs ("#      %u   full name of target player (16 character max)\n", fp);
            fputs ("#      %z   3 letter team id of target planet\n", fp);
            fputs ("#      %b   planet nearest sender\n", fp);
            fputs ("#      %o   three letter team name of sender\n", fp);
            fputs ("#      %k   number of kills sender has\n", fp);
            fputs ("#      %K   number of kills target player has\n", fp);
            fputs ("#      %*   if this is encountered, the macro IS NOT PRINTED\n", fp);
            fputs ("#      %(SPACE)  this is replaced by a space, mainly useful for starting a message\n", fp);
            fputs ("#\n", fp);
            fputs ("#\n", fp);
            fputs ("#      FULLY CAPITALIZED:\n", fp);
            fputs ("#\n", fp);
            fputs ("#      %L   three character name of target planet\n", fp);
            fputs ("#      %I   sender full player name (16 character max)\n", fp);
            fputs ("#      %U   full name of target player (16 character max)\n", fp);
            fputs ("#      %Z   3 letter team id of target planet\n", fp);
            fputs ("#      %B   sender nearest planet\n", fp);
            fputs ("#      %O   three letter team name of sender\n", fp);
            fputs ("#\n", fp);
            fputs ("#      Ping stats: (may differ slightly from server '!' ping stats)\n", fp);
            fputs ("#\n", fp);
            fputs ("#      %v   average ping stat round trip time\n", fp);
            fputs ("#      %V   ping stat round trip standard deviation\n", fp);
            fputs ("#      %y   percent total packet loss as calculated by server formula\n", fp);
            fputs ("#\n", fp);
            fputs ("#      Miscellanous:\n", fp);
            fputs ("#\n", fp);
            fputs ("#      %m   the last message you sent\n", fp);
            fputs ("#      %M   the last message you sent in all caps\n", fp);
            fputs ("#\n", fp);
            fputs ("# As a further extension to NEWMACRO, a macro may now be sent to any of the following destinations:\n", fp);
            fputs ("#      %i %I %c  send message to self\n", fp);
            fputs ("#      %u %U %p  send message to player nearest mouse\n", fp);
            fputs ("#      %t %z %Z  send message to team of player nearest mouse\n", fp);
            fputs ("#      %g        send message to nearest friendly player to my ship\n", fp);
            fputs ("#      %h        send message to nearest enemy player to my ship\n", fp);
            fputs ("#\n", fp);
            fputs ("#      with a syntax like\n", fp);
            fputs ("#\n", fp);
            fputs ("#      #useful for INL...\n", fp);
            fputs ("#      mac.C.%i: CAPTAIN\n", fp);
            fputs ("#      mac.N.%i: NEWGALAXY\n", fp);
            fputs ("#      mac.S.%i: START\n", fp);
            fputs ("#      mac.T.%i: %Z\n", fp);
            fputs ("#\n", fp);
            fputs ("#      mac.W.%t: SHUT UP, TWINKS!!\n", fp);
            fputs ("#      mac.I.%u: %u: det when you escort!\n", fp);
            fputs ("#      mac.O.%u: (%i) ogging\n", fp);
            fputs ("#\n", fp);
            fputs ("# What this does is allows you to send a macro to a player or team specified by your mouse\n", fp);
            fputs ("# location instead of requiring a 3rd character to be input. Also, it allows you to send\n", fp);
            fputs ("# message to yourself without having to actual sit there and figure out who you are (they\n", fp);
            fputs ("# tried and failed back in the '60s).\n", fp);
            fputs ("#\n", fp);
            fputs ("# Further, tests may be done within the macro system, the syntax for these test is as follows.\n", fp);
            fputs ("#\n", fp);
            fputs ("#      %?   introduces a test\n", fp);
            fputs ("#      =    equivalence\n", fp);
            fputs ("#      >    greater\n", fp);
            fputs ("#      <    less\n", fp);
            fputs ("#\n", fp);
            fputs ("# Expressions are evaluated on a character by character basis until the test is resolved.\n", fp);
            fputs ("# The text of the test is then replaced in the macro by 1 or 0.\n", fp);
            fputs ("#\n", fp);
            fputs ("# Test are then fed to a syntax I call conditional text. The best way to demonstrate\n", fp);
            fputs ("# how this works is example.\n", fp);
            fputs ("#\n", fp);
            fputs ("#      \"1%{included if true%!included if false%}\"\n", fp);
            fputs ("#\n", fp);
            fputs ("#      This would print:\n", fp);
            fputs ("#\n", fp);
            fputs ("#      \"included if true\"\n", fp);
            fputs ("#\n", fp);
            fputs ("#      whereas\n", fp);
            fputs ("#\n", fp);
            fputs ("#      \"0%{included if true%!included if false%}\"\n", fp);
            fputs ("#\n", fp);
            fputs ("#      would print:\n", fp);
            fputs ("#\n", fp);
            fputs ("#      \"included if false\"\n", fp);
            fputs ("#\n", fp);
            fputs ("# Combining the whole package, a very robust macroing system is quickly generated.\n", fp);
            fputs ("# One can easily design or mimic Distress calls, including the variable NBT\n", fp);
            fputs ("# distress of the BRM client and all the hardcoded message macroing built into my\n", fp);
            fputs ("# own client but never released.\n", fp);
            fputs ("#\n", fp);
            fputs ("# Here are a few more samples to work from:\n", fp);
            fputs ("#\n", fp);
            fputs ("#      mac.F.T:   Help!  Carrying %a!!\n", fp);
            fputs ("#      mac.f:     Help!  Carrying %a!!\n", fp);
            fputs ("#      mac.c.T:   %p++ near %l\n", fp);
            fputs ("#      mac.b.T:   %?%n>4%{bomb %l at %n%!bomb%}\n", fp);
            fputs ("#      mac.d.T:   %E%{%!%W%{%!I'm fine.  How are you? %}%}%E%{ETEMPED!!!  %}\n", fp);
            fputs ("#                 %W%{WTEMPED!!!  %}Carrying %?%a>0%{%a armies!%!NO armies.%}\n", fp);
            fputs ("#      mac.a.T:   %E%{ETEMPED!!!  %}%W%{WTEMPED!!!  %}Carrying %?%a>0%{%a\n", fp);
            fputs ("#                 armies! %!NO armies.%}\n", fp);
            fputs ("#      mac.R.A:   I'm a %?%S=SB%{star base!%!twink!%}\n", fp);
            fputs ("#      mac.K.A:   KissMy%S\n", fp);
            fputs ("#      mac.t:     thanks\n", fp);
            fputs ("#      mac.y:     yes\n", fp);
            fputs ("#      mac.n:     no\n", fp);
            fputs ("#      mac.B:     bye, getting hungry/sleep/horny\n", fp);
            fputs ("#      mac.e.T:   need escort going to %l%?%a>0%{, carrying %a armies!%}\n", fp);
            fputs ("#      mac.v.%t   %T%c PING stats: Average: %v ms, Stdv: %V ms, Loss: %y%%\n", fp);
            fputs ("\n", fp);
        }

        // singleMacro
        if (strlen (singleMacro) != 0)
        {
            if (saveBig)
                fputs ("# Keys that will trigger macro without going to macro mode\n", fp);

            strcpy (str, "singleMacro: ");

            for (i = 0; i < strlen (singleMacro); i++)
            {
                if (singleMacro[i] > 127)
                    sprintf (str1, "^%c", getkeyfromctrl (singleMacro[i]));
                else
                    sprintf (str1, "%c", getkeyfromctrl (singleMacro[i]));
                strcat (str, str1);
            }

            strcat (str, "\n");
            fputs (str, fp);

            if (saveBig)
                fputs ("\n", fp);
        }

        // Macros
        for (i = 2; i < MAX_MACRO; i++)
        {
            switch (macro[i].type)
            {
#ifdef NBT
            case NBTM:
                if (macro[i].key != 0)
                {
                    if (macro[i].key > 127)
                        sprintf (str, "macro.^%c.%c:%s\n",
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);
                    else
                        sprintf (str, "macro.%c.%c:%s\n",
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);

                    fputs (str, fp);
                    if (macro[i+1].type != NBTM)
                        fputs ("\n", fp);
                }
                break;
#endif /* NBT */
            case NEWM:
                if (macro[i].key != 0)
                {
                    if (macro[i].key > 127)
                        sprintf (str, "mac.^%c:%s\n", 
                                getkeyfromctrl (macro[i].key),
                                macro[i].string);
                    else
                        sprintf (str, "mac.%c:%s\n", 
                                getkeyfromctrl (macro[i].key),
                                macro[i].string);

                    fputs (str, fp);
                    if (macro[i+1].type != NEWM)
                        fputs ("\n", fp);
                }
                break;

            case NEWMSPEC:
                if (macro[i].key != 0)
                {
                    if (macro[i].key > 127)
                        sprintf (str, "mac.^%c.%c:%s\n", 
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);
                    else
                        sprintf (str, "mac.%c.%c:%s\n", 
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);

                    fputs (str, fp);
                    if (macro[i+1].type != NEWMSPEC)
                        fputs ("\n", fp);
                }
                break;

            case NEWMMOUSE:
                if (macro[i].key != 0)
                {
                    if (macro[i].key > 127)
                        sprintf (str, "mac.^%c.", getkeyfromctrl (macro[i].key));
                    else
                        sprintf (str, "mac.%c.", getkeyfromctrl (macro[i].key));

                    switch (macro[i].who)
                    {
                    case MACRO_PLAYER:
                        strcat (str, "%p:");
                        break;
                    case MACRO_TEAM:
                        strcat (str, "%t:");
                        break;
                    case MACRO_FRIEND:
                        strcat (str, "%g:");
                        break;
                    case MACRO_ENEMY:
                        strcat (str, "%h:");
                        break;
                    case MACRO_ME:
                        strcat (str, "%i:");
                        break;
                    }
                    strcat (str, macro[i].string);
                    strcat (str, "\n");
                    if (macro[i+1].type != NEWMMOUSE)
                        fputs ("\n", fp);

                    fputs (str, fp);
                }
                break;
#ifdef NBT
#ifdef MULTILINE_MACROS
            case NEWMULTIM:
                if (macro[i].key != 0)
                {
                    if (macro[i].key > 127)
                        sprintf (str, "macro.^%c.%c:%s\n",
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);
                    else
                        sprintf (str, "macro.%c.%c:%s\n",
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);

                    if (macro[i].key != macro[i+1].key)
                        strcat (str, "\n");

                    fputs (str, fp);
                }
                break;
#endif /* MULTILINE_MACROS */
#endif /* NBT */
            }
        }
    }

    // RCD macros
    if (saveRCD)
    {
        if (saveBig)
            fputs ("# Receiver configurable distress macros\n", fp);

        for (dm = &dist_prefered[1]; dm->name; dm++)
        {
            if (dm->c != 0)
            {
                if (dm->c > 127)
                    sprintf (str, "dist.^%c.", getkeyfromctrl (dm->c));
                else
                    sprintf (str, "dist.%c.", getkeyfromctrl (dm->c));
            }
            else
                strcpy (str, "dist.");

            strcat (str, dm->name);
            strcat (str, ":");
            strcat (str, dm->macro);
            strcat (str, "\n");

            fputs (str, fp);
        }

        fputs ("\n", fp);

        if (saveBig)
            fputs ("\n", fp);
    }

#ifdef RCM
    // RCM macros
    if (saveRCM)
    {
        if (saveBig)
            fputs ("# Receiver configurable messages\n", fp);

        for (dm = &rcm_msg[1]; dm->name; dm++)
        {
            sprintf (str, "msg.%s:%s\n", dm->name, dm->macro);
            fputs (str, fp);
        }

        fputs ("\n", fp);

        if (saveBig)
            fputs ("\n", fp);
    }
#endif

    fclose (fp);
}
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
#include "version.h"

struct save_options save_options[] = {
    {"agriCAPS", &agriCAPS, RC_BOOL,
        {
            "Show AGRI planet names in caps on map",
            NULL
        }
    },
    {"agriColor", &agriColor, RC_INT,
        {
            "Color of AGRI planet name on the map",
            "0 - owner race color",
            "1 - white",
            "2 - gray",
            NULL
        }
    },
    {"allowWheelActions", &allowWheelActions, RC_BOOL,
        {
            "allow mouse wheel to produce action in non-scrollable windows",
            NULL
        }
    },
    {"autoPhaser", &autoPhaser, RC_INT,
        {
            "Phaser exact location of enemy target closest to cursor",
            "Enemy must be within phaser range of your ship",
            "Server god decides whether to enable it on server",
            NULL
       }
    },
    {"autoQuit", &autoQuit, RC_INT,
        {
            "Autoquit timer (default 60)",
            NULL
        }
    },
#ifdef ROTATERACE
    {"autoRotate", &autoRotate, RC_BOOL,
        {
            "Automatically rotate galaxy so teams are on left side of map",
            NULL
        }
    },
#endif
    {"autoSetWar", &autoSetWar, RC_INT,
        {
            "Declare war with enemy races when you first join",
            "0 - Don't set war",
            "1 - Set war with nonzero player teams, peace w/ 0 player teams",
            "2 - Set war with largest enemy team,  peace w/ everyone else",
            NULL
        }
    },
    {"baseUdpLocalPort", &baseUdpLocalPort, RC_INT,
        {
            "Base UDP local port",
            NULL
        }
    },
    {"beepOnPrivateMessage", &beepOnPrivateMessage, RC_BOOL,
        {
            "Beep if received private message",
            NULL
        }
    },
#ifdef MOTION_MOUSE
    {"clickDelay", &clickDelay, RC_INT,
        {
            "Delay before continuous mouse clicks, in updates (0 = no delay)",
            NULL
        }
    },
#endif
    {"colorClient", &colorClient, RC_INT,
        {
            "What type of ship bitmaps to use",
            "0 - mono",
            "1 - new color bitmaps (default)",
            "2 - old color bitmaps",
            "3 - shaded old color bitmaps",
            "4 - experimental high res bitmaps",
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
    {"colorWeapons", &colorWeapons, RC_BOOL,
        {
            "Use colored bitmaps for torps and plasmas",
            NULL
        }
    },
    {"continueTractor", &continueTractor, RC_BOOL,
        {
            "Show tractor/pressor after lock",
            NULL
        }
    },
#ifdef MOTION_MOUSE
    {"continuousMouse", &continuousMouse, RC_BOOL,
        {
            "Use mouse for continuous steering and firing",
            NULL
        }
    },
    {"continuousMouseFix", &continuousMouseFix, RC_BOOL,
        {
            "When in continuousMouse mode allow more than one button to be pressed",
            "to generate more actions",
            NULL
        }
    },
#endif
    {"detCircle", &detCircle, RC_BOOL,
        {
            "Show det circle around your ship",
            NULL
        }
    },
    {"disableWinkey", &disableWinkey, RC_BOOL,
        {
            "Disable Windows and Context Keys",
            NULL
        }
    },
    {"doubleBuffering", &doubleBuffering, RC_BOOL,
        {
            "Use double buffering to reduce screen flicker",
            NULL
        }
    },
    {"dynamicBitmaps", &dynamicBitmaps, RC_BOOL,
    	{
    	    "Allow switching of ship bitmaps in game.  With this option disabled",
    	    "not all bitmaps load into memory.  This option is thus not changeable",
    	    "in game.  Do not attempt to edit your netrekrc.txt to turn dynamic bitmaps",
    	    "on and then reload netrekrc.txt with the & key - restart the client instead.",
    	    NULL
    	}
    },
    {"enemyPhasers", &enemyPhasers, RC_INT,
        {
            "Angle between enemy phaser lines",
            "1-10 range",
            NULL
        }
    },
    {"extraAlertBorder", &extraAlertBorder, RC_BOOL,
        {
            "Change window border on enemy approach",
            NULL
        }
    },
    {"fontSize", &fontSize, RC_INT,
        {
            "Height in pixels of font, default 10",
            NULL
        }
    },
    {"forceDisplay", &forceDisplay, RC_INT,
        {
            "Number of colors the client will display",
            "0 - find best available color option (default)",
            "1 - 16 colors",
            "2 - 256 colors",
            "3 - true color",
            NULL
        }
    },
    {"forceMono", &forceMono, RC_BOOL,
        {
            "Outdated option.  Will force display to 16 colors",
            "(but only works if forceDisplay = 0).",
            NULL
        }
    },
    {"fullBitmapRotation", &fullBitmapRotation, RC_BOOL,
        {
            "Draw old bitmap sets to 256 angles instead of 32",
            NULL
        }
    },
    {"headingTic", &headingTic, RC_BOOL,
        {
            "Draw a tic mark indicating your ship's direction",
            NULL
        }
    },
    {"hideConsole", &hideConsole, RC_BOOL,
        {
            "Hide the DOS console window",
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
    {"infoRange", &infoRange, RC_BOOL,
        {
            "Show weapon's range boundary as a dashed white box if tactical",
            "size is increased beyond the standard 500x500",
            NULL
        }
    },
    {"keepInfo", &keepInfo, RC_INT,
        {
            "How many tenths of seconds to keep info window on (default 15)",
            NULL
        }
    },
    {"keepPeace", &keepPeace, RC_BOOL,
        {
            "Keep peace with enemy team after death",
            NULL
        }
    },
    {"lockLine", &lockLine, RC_BOOL,
        {
            "Draw dashed green line on map from your ship to lock target,",
            "requires showLock to be on for map",
            NULL
        }
    },
    {"logging", &logging, RC_BOOL,
        {
            "Log all messages to file. Requires \"logfile\" option",
            NULL
        }
    },
    {"mainMaximized", &mainMaximized, RC_BOOL,
        {
            "Make main window maximized on client start",
            NULL
        }
    },
    {"mainResizeable", &mainResizeable, RC_BOOL,
        {
            "Make main window + local/map windows resizeable",
            NULL
        }
    },
    {"mainTitleBar", &mainTitleBar, RC_BOOL,
        {
            "Start main window with title bar on (can be toggled by alt+enter)",
            NULL
        }
    },
    {"maxScrollLines", &maxScrollLines, RC_INT,
        {
            "Maximum number of scroll lines in a message window (range of values 50-500)",
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
    {"messageKeyOnly", &messageKeyOnly, RC_BOOL,
        {
            "Only start messages with the actual message key(s)",
            "if cursor is in the message windows",
            NULL
        }
    },
#endif
#ifdef META
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
            "2 - as above + with nobody playing (default)",
            "3 - as above + timed out servers",
            "4 - as above + servers that metaserver could not connect to",
            NULL
        }
    },
    {"metaType", &metaType, RC_INT,
        {
            "What type of metaserver to use",
            "1 - UDP multiple metaservers",
            "2 - cache, then TCP metaserver",
            "3 - TCP metaserver, then cache",
            NULL
        }
    },
#ifdef METAPING
    {"metaPing", &metaPing, RC_BOOL,
        {
            "Use ICMP to ping the metaserver list",
            NULL
        }
    },
#endif
    {"metaVerbose", &metaVerbose, RC_BOOL,
        {
            "Show detailed messages during connect to metaserver",
            NULL
        }
    },
#endif

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
    {"newQuit", &newQuit, RC_BOOL,
        {
            "Use new quit window",
            NULL
        }
    },
    {"newTeams", &newTeams, RC_BOOL,
        {
            "Use new team windows",
            NULL
        }
    },
    {"omitTeamLetter", &omitTeamLetter, RC_BOOL,
        {
            "Omit team letter on galaxy",
            NULL
        }
    },
    {"packetLights", &packetLights, RC_BOOL,
        {
            "Show packets sent and received by blinking dashboard lights",
            NULL
        }
    },
    {"partitionPlist", &partitionPlist, RC_BOOL,
        {
            "Add space between teams in player list",
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
            "Type of planet bitmaps on local map",
            "0 - Bronco (default)",
            "1 - Moo",
            "2 - Rabbitear",
            "3 - New color",
            NULL
        }
    },
    {"planetBitmapGalaxy", &planetBitmapGalaxy, RC_INT,
        {
            "Type of planet bitmaps on galactic map",
            "0 - Bronco (default)",
            "1 - Moo",
            "2 - Rabbitear",
            "3 - New color",
            NULL
        }
    },
    {"planetHighlighting", &planetHighlighting, RC_BOOL,
        {
            "Highlight galactic planets by race and army status",
            NULL
        }
    },
    {"playerListBlankZeroKills", &playerListBlankZeroKills, RC_BOOL,
        {
            "Don't display player's kills if they have zero kills",
            NULL
        }
    },
    {"playerListHack", &playerListHack, RC_BOOL,
        {
            "Suppose that players with letter greater than \'f\' are observers",
            NULL
        }
    },
    {"playerListMessaging", &playerListMessaging, RC_BOOL,
        {
            "Enable mouse clicks in player list to send messages",
            NULL
        }
    },
    {"playerListObserver", &playerListObserver, RC_INT,
        {
            "What kind of players to list",
            "0 - all (default)",
            "1 - players",
            "2 - observers",
            "3 - players, then observers",
            "4 - playerlist is off",
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
    {"puckArrow", &puckArrow, RC_BOOL,
        {
            "Put a small tic mark on the puck to indicate its direction",
            NULL
        }
    },
    {"puckCircle", &puckCircle, RC_BOOL,
        {
            "Show the puck's max shot range as a circle around your ship",
            NULL
        }
    },
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
    {"richText", &richText, RC_BOOL,
        {
            "Use rich text message windows",
            NULL
        }
    },
    {"richTextMove", &richTextMove, RC_BOOL,
        {
            "Enable rich text windows moving",
            NULL
        }
    },
    {"rotatePlanets", &rotatePlanets, RC_BOOL,
        {
            "Rotate planets (only works with new planet bitmaps)",
            NULL
        }
    },
    {"saveBig", &saveBig, RC_BOOL,
        {
            "Save options with comments",
            NULL
        }
    },
    {"saveWindow", &saveWindow, RC_BOOL,
        {
            "Save window placements to saveFile",
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
#ifdef BEEPLITE
    {"saveBeeplite", &saveBeeplite, RC_BOOL,
        {
            "Save beeplite macros to saveFile",
            NULL
        }
    },
#endif
#ifdef RCM
    {"saveRCM", &saveRCM, RC_BOOL,
        {
            "Save RCM to saveFile",
            NULL
        }
    },
#endif
    {"scaleFactor", &scaleFactor, RC_INT,
        {
            "Scale of local map graphics",
            "10-40 range",
            NULL
        }
    },
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
            "Server god decides whether to enable it on server",
            NULL
        }
    },
    {"showArmy", &showArmy, RC_INT,
        {
            "Where to show army counts next to planets",
            "Server god decides whether to fully enable this feature",
            "If server god has feature off, army counts will only show on",
            "planet you are orbiting (or locked onto as an observer)",
            "0 - don't show army counts",
            "1 - show army counts on local map only (default)",
            "2 - show army counts on galactic map only",
            "3 - show army counts on both maps",
            NULL
        }
    },
    {"showCloakers", &showCloakers, RC_BOOL, 
        {
            "Show other cloakers on local",
            "Server god decides whether to enable this feature",
            NULL
        }
    },
    {"showHints", &showHints, RC_BOOL,
        {
            "Show hints window",
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
    {"showHockeyScore", &showHockeyScore, RC_BOOL,
        {
            "Show hockey score on the galaxy top left corner",
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
            "Show MOTD if waiting on queue",
            NULL
        }
    },
    {"showMySpeed", &showMySpeed, RC_BOOL,
        {
            "Show my speed on local",
            NULL
        }
    },
    {"showOtherSpeed", &showOtherSpeed, RC_BOOL,
        {
            "Show other player's speed on local",
            "Server god decides whether to enable this feature",
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
    {"showStars", &showStars, RC_BOOL,
        {
            "Draw background stars",
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
    {"sortMyTeamFirst", &sortMyTeamFirst, RC_BOOL,
        {
            "Put my team first in the player list",
            NULL
        }
    },
    {"sortPlanets", &sortPlanets, RC_BOOL,
        {
            "Sort the planet list by team and army count",
            NULL
        }
    },
    {"sortPlayers", &sortPlayers, RC_BOOL,
        {
            "Sort players by team",
            NULL
        }
    },
#ifdef SOUND
    {"sound", &sound_init, RC_BOOL,
        {
            "Enable layered, stereo sound",
            NULL
        }
    },
    {"soundVolume", &soundVolume, RC_INT,
        {
            "Starting sound volume",
            "0-128 range",
            NULL
        }
    },
    {"soundEffects", &soundEffects, RC_BOOL,
        {
            "Play sound effects",
            NULL
        }
    },
    {"soundMusic", &soundMusic, RC_BOOL,
        {
            "Play music",
            NULL
        }
    },
    {"soundMusicBkgd", &soundMusicBkgd, RC_BOOL,
        {
            "Play theme music in background (requires soundMusic)",
            NULL
        }
    },
    {"soundAngles", &soundAngles, RC_BOOL,
        {
            "Use 3D sound effects",
            NULL
        }
    },
#endif
    {"theirPhaserShrink", &theirPhaserShrink, RC_INT,
        {
            "Shrink enemy phaser by x/16 of its length",
            "1-16 range",
            NULL
        }
    },
    {"timerType", &timerType, RC_INT,
        {
            "Type of dashboard timer to show",
            "0 - don't show timer",
            "1 - show current time",
            "2 - show time on server",
            "3 - show time in ship",
            "4 - show user-set time",
            "5 - show game-related timers",
            NULL
        }
    },
    {"tpDotDist", &tpDotDist, RC_INT,
        {
            "Distance between tractor/pressor dots",
            NULL
        }
    },
    {"tractorID", &tractorID, RC_BOOL,
        {
            "Show the ID of player you are tractoring as a number below your ship",
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
            "3 - enforced UDP (state & weapons)",
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
            "How many updates per second to request",
            "1-50 (or whatever server frame rate is)",
            "Value may be constrained by server to be evenly divisible",
            "by server frame rate",
            NULL
        }
    },
    {"useCheckPlanets", &useCheckPlanets, RC_BOOL,
        {
            "Crosscheck with server to make sure all planet information is",
            "correct.  Prevents against lost planet updates which can cause",
            "problems like phantom armies or not realizing a planet has",
            "popped.  Increases upstream (client to server) bandwidth usage",
            "by 100 bytes/second.",
            NULL
        }
    },
    {"useFullShipInfo", &useFullShipInfo, RC_BOOL,
        {
            "Display other ships to 256 directions instead of 16",
            "by using long ship packets instead of short ship packets,",
            "increases bandwidth usage by 1-2 kb/second.",
            NULL
        }
    },
    {"useFullWeapInfo", &useFullWeapInfo, RC_BOOL,
        {
            "Allows observers to see all weapons fire on galaxy map",
            "by using long torp packets instead of short torp packets,",
            "can increase bandwidth usage by 1 kb/second, more if obs.",
            "Also allows self torps to be seen beyond tactical range,",
            "again via the galaxy map.",
            NULL
        }
    },
    {"useGeneric32", &useGeneric32, RC_BOOL,
        {
            "Receive SP_GENERIC_32 packets, which are used to convey things",
            "like repair time and planet being orbited.  Increases bandwidth",
            "usage anywhere from 0 to 320 bytes/second for a server running",
            "at 10 UPS.",
            NULL
        }
    },
#ifdef BEEPLITE
    {"useLite", &useLite, RC_BOOL,
        {
            "Use beeplite",
            NULL
        }
    },
    {"defLite", &defLite, RC_BOOL,
        {
            "Use default beeplite settings",
            NULL
        }
    },
#endif
#ifdef VSHIELD_BITMAPS
    {"varyShields", &varyShields, RC_BOOL,
        {
            "Change shields graphic on shield damage",
            NULL
        }
    },
    {"varyShieldsColor", &varyShieldsColor, RC_BOOL,
        {
            "Change shields color on shield damage",
            NULL
        }
    },
#endif
    {"viewBox", &viewBox, RC_BOOL,
        {
            "Show limits of tactical display on galaxy",
            NULL
        }
    },
    {"viewRange", &viewRange, RC_INT,
        {
            "Show range at which enemies can see you as a circle on map",
            "Server god decides whether to enable this feature",
            "0 - don't show range circle",
            "1 - only show range circle when cloaked",
            "2 - only show range circle when carrying",
            "3 - show range circle at all times",
            NULL
        }
    },
    {"warnHull", &vary_hull, RC_BOOL,
        {
            "Warn hull state based on damage",
            NULL
        }
    },
    {"warnShields", &warnShields, RC_BOOL,
        {
            "Change shields color on enemy approach, overrides varyShieldsColor",
            NULL
        }
    },
    {"warpStreaks", &warpStreaks, RC_BOOL,
        {
            "Draw warp streaks while transwarping to starbase",
            NULL
        }
    },
    {"weaponsOnMap", &weaponsOnMap, RC_BOOL,
        {
            "Show phasers, torps and plasmas on galactic map",
            NULL
        }
    },
    {"windowMove", &windowMove, RC_BOOL,
        {
            "Enable internal windows moving",
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
#define XTREKRCTXT "xtrekrc.txt"
#define NETREKRCTXT "netrekrc.txt"


/******************************************************************************/
/***  initDefaults()                                                        ***/
/******************************************************************************/
void
initDefaults (char *deffile)
{
    FILE *fp;
    char file[256];
    char *v;
    struct stringlist *new;
    struct dmacro_list *dm;
    struct dmacro_list *dm_def;
    int notdone;
    unsigned char c;
    char *str;
    struct stringlist *sl;

#ifdef MULTILINE_MACROS
    unsigned char keysused[256];

    MZERO (keysused, sizeof (keysused));
#endif

    DefaultsLoaded = 1;

#ifdef DEBUG
    LineToConsole ("Initdefaults\n");
#endif

    /* Clear defaults if they exist */
	while (defaults != NULL)
	{
        sl = defaults;
        defaults = defaults->next;
        free (sl->string);
        free (sl->value);
        free (sl);
	}

    /* sizeof doesn't work if it isn't in the same source file, shoot me */
    MCOPY (dist_defaults, dist_prefered, sizedist);

    if (!deffile)
        if (findDefaults (deffile, file))
            deffile = file;
        else
            return;             /* No defaults file! */

    fp = fopen (deffile, "r");
    if (!fp)
        return;

    LineToConsole ("Reading defaults file %s\n", deffile);

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
                LineToConsole ("Maximum number of macros is %d\n", MAX_MACRO);
            }
            else
            {
                str = file + 6;
                c = getctrlkey ((unsigned char **) &str);
                if (c == '?')
                {
                    LineToConsole ("Cannot use '?' for a macro\n");
                }
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
                        keysused[macro[macrocnt].key] = (unsigned char) (macrocnt + 1);
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
                LineToConsole ("Maximum number of macros is %d\n", MAX_MACRO);
            }
            else
            {
                str = file + 4;
                c = getctrlkey ((unsigned char **) &str);
                if (c == '?')
                {
                    LineToConsole ("Cannot use '?' for a macro\n");
                }
                else
                {
                    macro[macrocnt].key = c;
                    macro[macrocnt].multi = 0;

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
                            LineToConsole ("Multiline macros of nonstandard types are not recommended.\n");
                            LineToConsole ("You might experience strange behaviour of macros.\n");
                            LineToConsole ("Type: unspecified macro, key: %c.\n", macro[macrocnt].key);
                        }
#endif /* MULTILINE_MACROS */
                    }

#ifdef MULTILINE_MACROS
                    if (keysused[macro[macrocnt].key])
                    {
                    	/* Don't switch mouse targeted multiline macros to type
                    	   multiline, just flag them as being multiline */
                    	if (macro[macrocnt].type == NEWMMOUSE)
                    	{
                            macro[keysused[macro[macrocnt].key] - 1].multi = 1;
                            macro[macrocnt].multi = 1;
                        }
                    	else
                    	{
                            macro[keysused[macro[macrocnt].key] - 1].type =
                               NEWMULTIM;
                            macro[macrocnt].type = NEWMULTIM;
                	}
                    }
                    else
                    {
                        keysused[macro[macrocnt].key] = (unsigned char) (macrocnt + 1);
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
                /* Allow dist.taking to work, as that was the old dist name prior to 2006 */
                if (strcmpi (str, dm->name) == 0
                   || (strcmpi (str, "taking") == 0 && strcmpi (dm->name, "take") == 0) )
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

#ifdef BEEPLITE
        else if (strncasecmp(file, "lite.", 5) == 0)
	{
	    int offset = 5;
	    char  **lt;

	    if (file[6] == '.')
	        offset = 7;

	    notdone = 1;

	    for (lt = &distlite[take], dm = &dist_prefered[take],
	       dm_def = &dist_defaults[take];
	       dm->name && notdone; dm++, dm_def++, lt++)
	    {
	        if (strcmpi(file + offset, dm->name) == 0)
		{
		    *lt = strdup(v);
		    notdone = 0;
		}
	    }
        }
#endif /* BEEPLITE */

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
                LineToConsole ("Maximum number of keys is %d\n", MAX_KEY - 1);
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
    char tmp[256];
    
    /* We want to be able to have option.observer or option.serverNick in rc */
    if (observerMode)
    {
    	strcpy (tmp, str);
    	strcat (tmp, ".observer");
    
    	sl = defaults;
    	while (sl != NULL)
    	{
    	    if (strcmpi (sl->string, tmp) == 0)
    	        return (sl->value);
    	    sl = sl->next;
    	}
    }
    
    if (serverNick)
    {
    	strcpy (tmp, str);
    	strcat (tmp, ".");
    	strcat (tmp, serverNick);
    
    	sl = defaults;
    	while (sl != NULL)
    	{
    	    if (strcmpi (sl->string, tmp) == 0)
    	        return (sl->value);
    	    sl = sl->next;
    	}
    }
    
    if (serverType != ST_UNKNOWN)
    {
    	strcpy (tmp, str);
    	switch (serverType)
    	{
    	case ST_PARADISE:
    	    strcat (tmp, ".paradise");
    	    break;
    	case ST_BRONCO:
    	    strcat (tmp, ".bronco");
    	    break;
    	case ST_CHAOS:
    	    strcat (tmp, ".chaos");
    	    break;
    	case ST_INL:
    	    strcat (tmp, ".inl");
    	    break;
    	case ST_STURGEON:
    	    strcat (tmp, ".sturgeon");
    	    break;
    	case ST_HOCKEY:
    	    strcat (tmp, ".hockey");
    	    break;
    	case ST_DOGFIGHT:
    	    strcat (tmp, ".dogfight");
    	    break;
    	default:
    	    strcat (tmp, ".unknown");
    	}
    
    	sl = defaults;
    	while (sl != NULL)
    	{
    	    if (strcmpi (sl->string, tmp) == 0)
    	        return (sl->value);
    	    sl = sl->next;
    	}
    }

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


/******************************************************************************/
/***  getServerNick()                                                       ***/
/******************************************************************************/
char *
getServerNick (char *srvName)
{
    struct stringlist *sl;
    char *tmpServerNick;

    sl = defaults;
    while (sl != NULL)
    {
        if (strcmpi (sl->value, srvName) == 0)
        {
            tmpServerNick = strtok (sl->string, ".");	/* Remove server. */
            tmpServerNick = strtok (NULL, ".");		/* Get actual server nick */
            return strdup (tmpServerNick);
        }
        sl = sl->next;
    }
    return (NULL);
}


/******************************************************************************/
/***  getServerType()                                                       ***/
/******************************************************************************/
int
getServerType (char *srvName)
{
    struct stringlist *sl;
    char tmpTypeStr[128];
    
    sprintf (tmpTypeStr, "servertype.%s", srvName);

    sl = defaults;
    while (sl != NULL)
    {
        if (strcmpi (sl->string, tmpTypeStr) == 0)
        {
	    if (strcmpi (sl->value, "paradise") == 0)
		return ST_PARADISE;
	    else if (strcmpi (sl->value, "bronco") == 0)
		return ST_BRONCO;
	    else if (strcmpi (sl->value, "chaos") == 0)
		return ST_CHAOS;
	    else if (strcmpi (sl->value, "inl") == 0)
		return ST_INL;
	    else if (strcmpi (sl->value, "sturgeon") == 0)
		return ST_STURGEON;
	    else if (strcmpi (sl->value, "hockey") == 0)
		return ST_HOCKEY;
	    else if (strcmpi (sl->value, "dogfight") == 0)
		return ST_DOGFIGHT;
	    else
		return ST_UNKNOWN;
        }
        sl = sl->next;
    }
    return ST_UNKNOWN;
}

#ifndef __BORLANDC__
#ifndef CYGWIN
/******************************************************************************/
/***  strncmpi()                                                            ***/
/***  grr... are you telling me this sort of function isn't in the std      ***/
/***  libraries somewhere?! sons of satan... - jn                           ***/
/******************************************************************************/
int
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
        chr1 = (char) (isupper (str1[duh]) ? str1[duh] : toupper (str1[duh]));
        chr2 = (char) (isupper (str2[duh]) ? str2[duh] : toupper (str2[duh]));
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
#endif

/******************************************************************************/
/***  stringDefault()                                                       ***/
/******************************************************************************/
char *
stringDefault (char *def)
{
    char *str;

    str = getdefault (def);

    if (str)
        return strdup (str);
    else
        return (NULL);
}

/******************************************************************************/
/***  geometryDefault()                                                     ***/
/******************************************************************************/
char *
geometryDefault (char *def)
{
    char *str;

    str = getdefault (def);

    if (str && strcmpi(str, "auto") != 0)
        return strdup (str);
    else
        return (NULL);
}

/******************************************************************************/
/***  booleanDefault()                                                      ***/
/******************************************************************************/
int
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
int
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
findDefaults (char *deffile, char *file)
{

    /* Check base names */
    if (findfile (NETREKRCTXT, file))
        return 1;

    if (findfile (NETREKRC, file))
        return 1;

    if (findfile (XTREKRCTXT, file))
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

    if (strlen (pigcall) == 0)
        sprintf (pigcall, "Netrek XP 2009 (%s) - the smarter netrek eXPerience!", mvers);

    timerType = intDefault ("timerType", timerType);
    if (timerType < T_NONE || timerType >= T_TOTAL)
        timerType = T_SHIP;

    doubleBuffering = booleanDefault ("doubleBuffering", doubleBuffering);
    mainResizeable = booleanDefault ("mainResizeable", mainResizeable);
    mainMaximized = booleanDefault ("mainMaximized", mainMaximized);
    mainTitleBar = booleanDefault ("mainTitleBar", mainTitleBar);
    allowWheelActions = booleanDefault ("allowWheelActions", allowWheelActions);
    richText = booleanDefault ("richText", richText);
    richTextMove = booleanDefault ("richTextMove", richTextMove);
    newQuit = booleanDefault ("newQuit", newQuit);
    newTeams = booleanDefault ("newTeams", newTeams);
    soundVolume= intDefault ("soundVolume", soundVolume);
    soundEffects= booleanDefault ("soundEffects", soundEffects);
    soundMusic = booleanDefault ("soundMusic", soundMusic);
    soundMusicBkgd = booleanDefault ("soundMusicBkgd", soundMusicBkgd);
    soundAngles = booleanDefault ("soundAngles", soundAngles);
    useCheckPlanets = booleanDefault ("useCheckPlanets", useCheckPlanets);
    useFullShipInfo = booleanDefault ("useFullShipInfo", useFullShipInfo);
    useFullWeapInfo = booleanDefault ("useFullWeapInfo", useFullWeapInfo);
    useGeneric32 = booleanDefault ("useGeneric32", useGeneric32);
    headingTic = booleanDefault ("headingTic", headingTic);
    tractorID = booleanDefault ("tractorID", tractorID);
    lockLine = booleanDefault ("lockLine", lockLine);
    weaponsOnMap = booleanDefault ("weaponsOnMap", weaponsOnMap);
    sortPlanets = booleanDefault ("sortPlanets", sortPlanets);
    packetLights = booleanDefault ("packetLights", packetLights);
    tpDotDist = intDefault ("tpDotDist", tpDotDist);
    omitTeamLetter = booleanDefault ("omitTeamLetter", omitTeamLetter);
    beepOnPrivateMessage = booleanDefault ("beepOnPrivateMessage", beepOnPrivateMessage);
    viewBox = booleanDefault ("viewBox", viewBox);
    viewRange = intDefault ("viewRange", viewRange);
    warpStreaks = booleanDefault ("warpStreaks", warpStreaks);
    showStars = booleanDefault ("showStars", showStars);
    scaleFactor = intDefault ("scaleFactor", scaleFactor);
    if (scaleFactor > 40)
        scaleFactor = 40;
    else if (scaleFactor < 10)
        scaleFactor = 10;

#ifdef HOCKEY_LINES
    showHockeyLinesLocal = booleanDefault ("showHockeyLinesLocal", showHockeyLinesLocal);
    showHockeyLinesMap = booleanDefault ("showHockeyLinesMap", showHockeyLinesMap);
    showHockeyScore = booleanDefault ("showHockeyScore", showHockeyScore);
#endif

    saveBig = booleanDefault ("saveBig", saveBig);
    saveMacro = booleanDefault ("saveMacro", saveMacro);
    saveRCD = booleanDefault ("saveRCD", saveRCD);
    saveWindow = booleanDefault ("saveWindow", saveWindow);
#ifdef RCM
    saveBeeplite = booleanDefault ("saveBeeplite", saveBeeplite);
#endif
#ifdef RCM
    saveRCM = booleanDefault ("saveRCM", saveRCM);
#endif

    playerListBlankZeroKills =
        booleanDefault ("playerListBlankZeroKills", playerListBlankZeroKills);
    playerListHack = booleanDefault ("playerListHack", playerListHack);
    keepInfo = intDefault ("keepInfo", keepInfo);
    showPlanetOwner = booleanDefault ("showPlanetOwner", showPlanetOwner);
    newDashboard = intDefault ("newDashboard", newDashboard);
    detCircle = booleanDefault ("detCircle", detCircle);
    puckCircle = booleanDefault ("puckCircle", puckCircle);
    puckArrow = booleanDefault ("puckArrow", puckArrow);
    showArmy = intDefault ("showArmy", showArmy);
    showCloakers = booleanDefault ("showCloakers", showCloakers);
    autoPhaser = booleanDefault ("autoPhaser", autoPhaser);
    redrawDelay = intDefault ("redrawDelay", redrawDelay);
    planetHighlighting = booleanDefault ("planetHighlighting", planetHighlighting);
    rotatePlanets = booleanDefault ("rotatePlanets", rotatePlanets);
    autoRotate = booleanDefault ("autoRotate", autoRotate);
    autoSetWar = intDefault ("autoSetWar", autoSetWar);
    logging = booleanDefault ("logging", logging);
    maxScrollLines = intDefault ("maxScrollLines", maxScrollLines);
    if (maxScrollLines > 500)
        maxScrollLines = 500;
    else if (maxScrollLines < 50)
        maxScrollLines = 50;

    phaserShrink = intDefault ("phaserShrink", phaserShrink);
    if (phaserShrink > 16)
        phaserShrink = 16;

    // 0 - COW phasers shrink, 1 - BRMH phasers shrink
    phaserShrinkStyle = intDefault ("phaserShrinkStyle", phaserShrinkStyle);

    theirPhaserShrink = intDefault ("theirPhaserShrink", theirPhaserShrink);
    if (phaserShrink > 16)
        phaserShrink = 16;

    shrinkPhaserOnMiss = booleanDefault ("shrinkPhaserOnMiss", shrinkPhaserOnMiss);

    windowMove = booleanDefault ("windowMove", windowMove);

#ifdef VSHIELD_BITMAPS
    varyShields = booleanDefault ("varyShields", varyShields);
    varyShieldsColor = booleanDefault ("varyShieldsColor", varyShieldsColor);
#endif

    warnShields = booleanDefault ("warnShields", warnShields);
    vary_hull = booleanDefault("warnHull", vary_hull);

#ifdef METAPING
    metaPing = booleanDefault ("metaPing", metaPing);
#endif

    showLock = intDefault ("showLock", showLock);
    if (showLock > 3)
        showLock = 3;

#ifdef PHASER_STATS
    phaserStats = booleanDefault ("phaserStats", phaserStats);
#endif

#ifdef XTRA_MESSAGE_UI
    messageHUD = intDefault ("messageHUD", messageHUD);
    messageHoldThresh = intDefault ("messageHoldThresh", messageHoldThresh);
#endif
    showStats = booleanDefault ("showStats", showStats);
    showHints = booleanDefault ("showHints", showHints);
    keepPeace = booleanDefault ("keepPeace", keepPeace);
    colorClient = intDefault ("colorClient", colorClient);
    dynamicBitmaps = booleanDefault ("dynamicBitmaps", dynamicBitmaps);
    colorWeapons = booleanDefault ("colorWeapons", colorWeapons);
    continueTractor = booleanDefault ("continueTractor", continueTractor);
    showTractorPressor = booleanDefault ("showTractorPressor", showTractorPressor);
    showAllTractorPressor = booleanDefault ("showAllTractorPressor", showAllTractorPressor);
    extraAlertBorder = booleanDefault ("extraAlertBorder", extraAlertBorder);
    showPlanetNames = booleanDefault ("showPlanetNames", 1);
    reportKills = booleanDefault ("reportKills", reportKills);
    fullBitmapRotation = booleanDefault ("fullBitmapRotation", fullBitmapRotation);
    hideConsole = booleanDefault ("hideConsole", hideConsole);

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

    agriCAPS = booleanDefault ("agriCAPS", agriCAPS);
    agriColor = intDefault ("agriColor", agriColor);

    playerListMessaging = booleanDefault ("playerListMessaging", playerListMessaging);
    messageKeyOnly = booleanDefault ("messageKeyOnly", messageKeyOnly);

    highlightFriendlyPhasers = booleanDefault ("highlightFriendlyPhasers",
                                               highlightFriendlyPhasers);

#ifdef MOUSE_AS_SHIFT
    mouseAsShift = booleanDefault ("mouseAsShift", mouseAsShift);
#endif

#ifdef MOTION_MOUSE
    continuousMouse = booleanDefault ("continuousMouse", continuousMouse);
    continuousMouseFix = booleanDefault ("continuousMouseFix", continuousMouseFix);

    motionThresh = intDefault ("motionThresh", motionThresh);
    clickDelay = intDefault ("clickDelay", clickDelay);
#endif

#ifdef SHIFTED_MOUSE
    shiftedMouse = booleanDefault ("shiftedMouse", shiftedMouse);
#endif

    /* SRS 12/94, ignore the Capslock key */
    ignoreCaps = booleanDefault ("ignoreCaps", ignoreCaps);

    infoRange = booleanDefault ("infoRange", infoRange);
    showMySpeed = booleanDefault ("showMySpeed", showMySpeed);
    showOtherSpeed = booleanDefault ("showOtherSpeed", showOtherSpeed);

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

#ifdef BEEPLITE
    defLite = booleanDefault("defLite", defLite);
    useLite = booleanDefault("useLite", useLite);

    if (defLite)
	litedefaults();

    tts_time = intDefault("tts_time", tts_time);
    tts_max_len = intDefault("tts_max_len", tts_max_len);
    tts_ypos = intDefault("tts_ypos", TWINSIDE / 2 - 16);
#endif /* BEEPLITE */

	/* Let's check whether windows settings had changed */
        /* Read in defaults was moved before window generation, thus this
           code is obsolete and not worth fixing - BB 04/07 */
/*
	updateWindowsGeometry (baseWin);
	updateWindowsGeometry (w);
	updateWindowsGeometry (mapw);
	updateWindowsGeometry (tstatw);
	updateWindowsGeometry (warnw);
	updateWindowsGeometry (messagew);
	updateWindowsGeometry (planetw);
	updateWindowsGeometry (rankw);
	updateWindowsGeometry (playerw);
	updateWindowsGeometry (playerw2);
	updateWindowsGeometry (helpWin);
	updateWindowsGeometry (messwa);
	updateWindowsGeometry (messwt);
	updateWindowsGeometry (messwi);
	updateWindowsGeometry (messwk);
	updateWindowsGeometry (phaserwin);
	updateWindowsGeometry (reviewWin);
	updateWindowsGeometry (pStats);
	updateWindowsGeometry (udpWin);
#ifdef SHORT_PACKETS
	updateWindowsGeometry (spWin);
#endif
#ifdef SOUND
	updateWindowsGeometry (soundWin);
#endif
#ifdef TOOLS
	updateWindowsGeometry (toolsWin);
#endif
#ifdef XTREKRC_HELP
	updateWindowsGeometry (defWin);
#endif
#ifdef DOC_WIN
	updateWindowsGeometry (DocWin);
	updateWindowsGeometry (xtrekrcWin);
#endif
	for (i = 0; i < 4; i++)
		updateWindowsGeometry (teamWin[i]);

	updateWindowsGeometry (qwin);
	updateWindowsGeometry (statwin);
	updateWindowsGeometry (war);
*/
}
 
/* Generally useful function that searches for a file
   in the current and home directories, also
   the executable directory on Win32 */

#ifdef DEBUG
#define CHECK_FILE \
      LineToConsole ("Checking for file %s...\n", found); \
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
findfile (char *fname, char *found)
{
    int accessible;
    char *home;

    /* check current directory first */
#ifdef DEBUG
    LineToConsole ("Checking for file %s\n", fname);
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
getcharfromdec (int dec)
{
    char str[2];

    if (dec < 1 || dec > 22)
        return '\0';

    if (dec < 10)
        sprintf (str, "%d", dec);
    else
        sprintf (str, "%c", dec + 87);

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
    unsigned int i;
    int j;
    unsigned char c;
    char *adefault;
    char macroKey[3] = "";
    struct stringlist *sl;

    if (!saveFile)
        saveFile = stringDefault ("saveFile");
    if (!saveFile)
    {
        saveFile = (char *) malloc (sizeof (char) * 13); 
        sprintf (saveFile, "%s", "netrekrc.txt");
    }

    exe_dir = GetExeDir ();
    if (exe_dir)
    {
        int len = strlen (exe_dir);
        if (exe_dir[len - 1] == '/' || exe_dir[len - 1] == '\\')
            sprintf (save_file, "%s%s", exe_dir, saveFile);
        else
            sprintf (save_file, "%s\\%s", exe_dir, saveFile);
    }

    fp = fopen (save_file, "w+");
    if (fp == NULL)
        return;

    sprintf (str, "Saving options to: %s", save_file);
    warning (str);
    
    // Default character name
    if (pseudo != NULL)
    {
    	if (saveBig)
    	    fputs ("# Default character name\n", fp);
    	sprintf (str, "name: %s\n", pseudo);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }

    // Password
    if (defpasswd != NULL)
    {
        if (saveBig)
    	    fputs ("# Default password\n", fp);
    	sprintf (str, "password: %s\n", defpasswd);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }

    // Login
    if (login != NULL)
    {
    	if (saveBig)
    	    fputs ("# Login name\n", fp);
    	sprintf (str, "login: %s\n", login);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }

    // Metaserver
    if ((adefault = stringDefault ("metaServer")) != 0)
    {
    	if (saveBig)
    	    fputs ("# Metaserver(s)\n", fp);
    	sprintf (str, "metaServer: %s\n", adefault);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }

    // keymap part
    // we're going to print only keymap that differs from standard one
    // we have to start from second key, because first one is space
    str[0] = '\0';
    str1[0] = '\0';

    for (c = 1; c < 95; c++)
    {
        if (c + 32 != default_keymap[c])
        {
            if (default_keymap[c] != 'X' &&
               (default_keymap[c] >= 32 &&
                default_keymap[c] < 127))
            {
                sprintf (str1, "%c%c", c + 32, default_keymap[c]);
                strcat (str, str1);
            }
            else
                sprintf (macroKey, "%c", c + 32);
        }
    }
    // space time
    if (default_keymap[0] != 32)
    {
        sprintf (str1, " %c", default_keymap[0]);
        strcat (str, str1);
    }
    if (saveBig && strlen (str) != 0)
    {
        fputs ("# Key mapping\n", fp);
        fputs ("# Define your keymap here.  The format is newkey defaultkey.  For example, the\n", fp);
        fputs ("# default key for shield toggle is 's', if you want to remap shield toggle to\n", fp);
        fputs ("# 'q', you would put 'qs' in your keymap.  Shields would still be mapped to\n", fp);
        fputs ("# 's' as well as now being on 'q'.  Adding a mapping doesn't delete the old\n", fp);
        fputs ("# one.  If you want shields on 'w' as well, put 'ws' in your keymap.  If you\n", fp);
        fputs ("# had instead put 'wq', it would have mapped quit (the default action of 'q')\n", fp);
        fputs ("# onto 'w'.\n", fp);
    }
    if (strlen (str) != 0)
    {
        strcpy (str1, "keymap: ");
        strcat (str1, str);
        strcat (str1, "\n");
        fputs (str1, fp);
    }
    if (saveBig && strlen (str) != 0)
        fputs ("\n", fp);

    // Ckeymap
    if ((adefault = stringDefault ("ckeymap")) != 0)
    {
        if (saveBig)
            fputs ("# Control-key key mapping\n", fp);
        sprintf (str, "ckeymap: %s\n", adefault);
        fputs (str, fp);
        if (saveBig)
            fputs ("\n", fp);
    }
        
    // Let's print buttonmap
    str[0] = '\0';
    str1[0] = '\0';

#ifdef SHIFTED_MOUSE
    for (i = W_LBUTTON; i <= W_XBUTTON2_4; i++)
#else
    for (i = W_LBUTTON; i <= W_WHEELDOWN; i++)
#endif
    {
        if (default_buttonmap[i] != 0)
        {
            c = getkeyfromctrl (default_buttonmap[i]);
            if (c == '^')
                sprintf (str, "%c^^", getcharfromdec (i));
            else
                sprintf (str, "%c%c", getcharfromdec (i), getkeyfromctrl (default_buttonmap[i]));
            strcat (str1, str);
        }
    }

    if (saveBig && strlen (str1) != 0)
    {
        fputs ("# Mouse button mapping\n", fp);
        fputs ("# Button map lets you map the mouse buttons.  Unfortunately, it works in a\n", fp);
        fputs ("# different way than keymap.  The format is <mousebutton> <key>.  To map\n", fp);
        fputs ("# shields onto button 1 (the left one), you would put '1s' in your keymap.\n", fp);
        fputs ("# The buttonmap works through the keymap, so if you have shields mapped to\n", fp);
        fputs ("# 'q', putting '1q' would make button 1 shields and not quit.  Buttons 4 and\n", fp);
        fputs ("# 5 are support for Microsoft Xbuttons.  If you have a wheel mouse, the up\n", fp);
        fputs ("# wheel is button 6 and the down wheel is button 7.  There is also support\n", fp);
        fputs ("# for shifted, controled, and control-shifted mouse actions, see the netrek\n", fp);
        fputs ("# help file for more details.\n", fp);
    }
    if (strlen (str1) != 0)
    {
        strcpy (str, "buttonmap: ");
        strcat (str, str1);
        strcat (str, "\n");
        fputs (str, fp);
    }
    if (saveBig && strlen (str1) != 0)
        fputs ("\n", fp);

    // macroKey
    if (strlen (macroKey) == 0)
    {
        if (default_keymap[169] == 'X')
            strcpy (macroKey, "TAB");
        else if (default_keymap[155] == 'X')
            strcpy (macroKey, "ESC");
        else if (default_keymap[56] == 'X')
            strcpy (macroKey, "X");
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

    // Large list of save options
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

    // cloak chars
    if (saveBig)
        fputs ("# Two characters to show cloaked player on map window\n", fp);
    sprintf (str, "cloakChars: %c%c\n", cloakChars[0], cloakChars[1]);
    fputs (str, fp);
    if (saveBig)
        fputs ("\n", fp);

#ifdef SOUND
    // excluded sound categories
    adefault = stringDefault ("soundExclude");
    if (saveBig)
    {
    	fputs ("# Sound categories to turn off\n", fp);
    	fputs ("# e=explosions, w=weapons, a=alerts,\n", fp);
    	fputs ("# m=messages, i=info, c=cloaking,\n", fp);
    	fputs ("# s=shield, o=other ships\n", fp);
    }
    if (adefault == NULL)
        sprintf (str, "soundExclude:\n");
    else
        sprintf (str, "soundExclude: %s\n", adefault);
    fputs (str, fp);
    if (saveBig)
        fputs ("\n", fp);

    // sound directory
    if (sounddir != NULL)
    {
    	if (saveBig)
    	    fputs ("# Sound directory\n", fp);
    	sprintf (str, "sounddir: %s\n", sounddir);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }
#endif

    // savefile
    if (saveFile != NULL)
    {
    	if (saveBig)
    	    fputs ("# Save file (for using in-game save feature)\n", fp);
    	sprintf (str, "saveFile: %s\n", saveFile);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }
    // metacache
    if (metaCache != NULL)
    {
    	if (saveBig)
    	    fputs ("# Metacache file\n", fp);
    	sprintf (str, "metaCache: %s\n", metaCache);
    	fputs (str, fp);
    	if (saveBig)
    	    fputs ("\n", fp);
    }
    // UDP metacache
    if (metaUDPCache != NULL)
    {
    	if (saveBig)
    	    fputs ("# UDP Metacache file\n", fp);
    	sprintf (str, "metaUDPCache: %s\n", metaUDPCache);
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
    }
    // player2 list
    if (strlen (plist2Layout) != 0)
    {   
        sprintf (str, "playerList2: %s\n", plist2Layout);
        fputs (str, fp);
    }
    fputs ("\n", fp);
    if (saveBig)
        fputs ("\n", fp);

    // Window placements
    if (saveWindow)
    {
    	if (saveBig)
    	{
    	    fputs ("# Window placements section\n", fp);
    	    fputs ("# Local and map windows MUST be square.  Size can be adjusted.\n", fp);
    	    fputs ("# Most windows will autoadjust to the right spot if local or\n", fp);
    	    fputs ("# map size are changed, so most window placements are autoset\n", fp);
    	    fputs ("# If you wish to customize, window geometry should be of the form\n", fp);
    	    fputs ("# 502x885+1+1, 502x885, or +1+1.  The +1+1 indicates (x,y) point of\n", fp);
    	    fputs ("# top left corner of window, the 502x885 indicates width x height.\n", fp);
    	    fputs ("\n", fp);
    	}

        // Main window - always mapped
        if ((adefault = stringDefault ("netrek.parent")) != NULL)
        {
            sprintf (str, "netrek.parent:          %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (baseWin);
        sprintf (str, "netrek.geometry:        %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Local window - always mapped
        if ((adefault = stringDefault ("local.parent")) != NULL)
        {
            sprintf (str, "local.parent:           %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (w);
        sprintf (str, "local.geometry:         %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Map window - always mapped
        if ((adefault = stringDefault ("map.parent")) != NULL)
        {
            sprintf (str, "map.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (mapw);
        sprintf (str, "map.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Dashboard window - always mapped
        if ((adefault = stringDefault ("tstat.parent")) != NULL)
        {
            sprintf (str, "tstat.parent:           %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (tstatw);
        sprintf (str, "tstat.geometry:         %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Message window
        if ((adefault = stringDefault ("message.parent")) != NULL)
        {
            sprintf (str, "message.parent:         %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (messagew);
        sprintf (str, "message.geometry:       %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "message.mapped:         %s\n",
                 W_IsMapped (messagew) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Warning window - always mapped
        if ((adefault = stringDefault ("warn.parent")) != NULL)
        {
            sprintf (str, "warn.parent:            %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (warnw);
        sprintf (str, "warn.geometry:          %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Planet window
        if ((adefault = stringDefault ("planet.parent")) != NULL)
        {
            sprintf (str, "planet.parent:          %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (planetw);
        sprintf (str, "planet.geometry:        %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "planet.mapped:          %s\n",
                 W_IsMapped (planetw) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Rank window
        if ((adefault = stringDefault ("rank.parent")) != NULL)
        {
            sprintf (str, "rank.parent:            %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (rankw);
        sprintf (str, "rank.geometry:          %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "rank.mapped:            %s\n",
                 W_IsMapped (rankw) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Playerlist window
        if ((adefault = stringDefault ("player.parent")) != NULL)
        {
            sprintf (str, "player.parent:          %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (playerw);
        sprintf (str, "player.geometry:        %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "player.mapped:          %s\n",
                  W_IsMapped (playerw) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Alternate playerlist window
        if ((adefault = stringDefault ("player2.parent")) != NULL)
        {
            sprintf (str, "player2.parent:         %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (playerw2);
        sprintf (str, "player2.geometry:       %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "player2.mapped:         %s\n",
                  W_IsMapped (playerw2) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Help window
        if ((adefault = stringDefault ("help.parent")) != NULL)
        {
            sprintf (str, "help.parent:            %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (helpWin);
        sprintf (str, "help.geometry:          %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "help.mapped:            %s\n",
                  W_IsMapped (helpWin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);
        
        // Review all window
        if ((adefault = stringDefault ("review_all.parent")) != NULL)
        {
            sprintf (str, "review_all.parent:      %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (messwa);
        sprintf (str, "review_all.geometry:    %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "review_all.mapped:      %s\n",
                  W_IsMapped (messwa) ? "on" : "off");
        fputs (str, fp);
        if ((adefault = stringDefault ("review_all.allow")) != NULL)
        {
            sprintf (str, "review_all.allow:       %s\n", adefault);
            fputs (str, fp);
        }
        fputs ("\n", fp);
        
        // Review team window
        if ((adefault = stringDefault ("review_team.parent")) != NULL)
        {
            sprintf (str, "review_team.parent:     %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (messwt);
        sprintf (str, "review_team.geometry:   %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "review_team.mapped:     %s\n",
                  W_IsMapped (messwt) ? "on" : "off");
        fputs (str, fp);
        if ((adefault = stringDefault ("review_team.allow")) != NULL)
        {
            sprintf (str, "review_team.allow:      %s\n", adefault);
            fputs (str, fp);
        }
        fputs ("\n", fp);
        
        // Review your window
        if ((adefault = stringDefault ("review_your.parent")) != NULL)
        {
            sprintf (str, "review_your.parent:     %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (messwi);
        sprintf (str, "review_your.geometry:   %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "review_your.mapped:     %s\n",
                  W_IsMapped (messwi) ? "on" : "off");
        fputs (str, fp);
        if ((adefault = stringDefault ("review_your.allow")) != NULL)
        {
            sprintf (str, "review_your.allow:      %s\n", adefault);
            fputs (str, fp);
        }
        fputs ("\n", fp);

        // Review kill window
        if ((adefault = stringDefault ("review_kill.parent")) != NULL)
        {
            sprintf (str, "review_kill.parent:     %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (messwk);
        sprintf (str, "review_kill.geometry:   %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "review_kill.mapped:     %s\n",
                  W_IsMapped (messwk) ? "on" : "off");
        fputs (str, fp);
        if ((adefault = stringDefault ("review_kill.allow")) != NULL)
        {
            sprintf (str, "review_kill.allow:      %s\n", adefault);
            fputs (str, fp);
        }
        fputs ("\n", fp);
        
        // Review phaser window
        if ((adefault = stringDefault ("review_phaser.parent")) != NULL)
        {
            sprintf (str, "review_phaser.parent:   %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (phaserwin);
        sprintf (str, "review_phaser.geometry: %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "review_phaser.mapped:   %s\n",
                  W_IsMapped (phaserwin) ? "on" : "off");
        fputs (str, fp);
        if ((adefault = stringDefault ("review_phaser.allow")) != NULL)
        {
            sprintf (str, "review_phaser.allow:    %s\n", adefault);
            fputs (str, fp);
        }
        fputs ("\n", fp);

        // Review window
        if ((adefault = stringDefault ("review.parent")) != NULL)
        {
            sprintf (str, "review.parent:          %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (reviewWin);
        sprintf (str, "review.geometry:        %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "review.mapped:          %s\n",
                  W_IsMapped (reviewWin) ? "on" : "off");
        fputs (str, fp);
        if ((adefault = stringDefault ("review.allow")) != NULL)
        {
            sprintf (str, "review.allow:           %s\n", adefault);
            fputs (str, fp);
        }
        fputs ("\n", fp);

        // Ping Stats window
        if ((adefault = stringDefault ("pingStats.parent")) != NULL)
        {
            sprintf (str, "pingStats.parent:       %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (pStats);
        sprintf (str, "pingStats.geometry:     %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "pingStats.mapped:       %s\n",
                  W_IsMapped (pStats) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // UDP window
        if ((adefault = stringDefault ("UDP.parent")) != NULL)
        {
            sprintf (str, "UDP.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (udpWin);
        sprintf (str, "UDP.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "UDP.mapped:             %s\n",
                  W_IsMapped (udpWin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

#ifdef SHORT_PACKETS
        // Short packets window
        if ((adefault = stringDefault ("network.parent")) != NULL)
        {
            sprintf (str, "network.parent:         %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (spWin);
        sprintf (str, "network.geometry:       %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "network.mapped:         %s\n",
                  W_IsMapped (spWin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);
#endif

#ifdef TOOLS
        // Tools window
        if ((adefault = stringDefault ("tools.parent")) != NULL)
        {
            sprintf (str, "tools.parent:           %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (toolsWin);
        sprintf (str, "tools.geometry:         %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "tools.mapped:           %s\n",
                  W_IsMapped (toolsWin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);
#endif

#ifdef XTREKRC_HELP
        // Xtrekrc help window
        if ((adefault = stringDefault ("xtrekrc_help.parent")) != NULL)
        {
            sprintf (str, "xtrekrc_help.parent:    %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (defWin);
        sprintf (str, "xtrekrc_help.geometry:  %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "xtrekrc_help.mapped:    %s\n",
                  W_IsMapped (defWin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);
#endif

#ifdef DOC_WIN
        // Documentation window
        if ((adefault = stringDefault ("DocWin.parent")) != NULL)
        {
            sprintf (str, "DocWin.parent:          %s\n", adefault);
            fputs (str, fp);
        }
         adefault = checkWindowsGeometry (docwin);
        sprintf (str, "DocWin.geometry:        %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "DocWin.mapped:          %s\n",
                  W_IsMapped (docwin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Xtrekrc window
        if ((adefault = stringDefault ("xtrekrcWin.parent")) != NULL)
        {
            sprintf (str, "xtrekrcWin.parent:      %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (xtrekrcwin);
        sprintf (str, "xtrekrcWin.geometry:    %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "xtrekrcWin.mapped:      %s\n",
                  W_IsMapped (xtrekrcwin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);
#endif

        // Fed team window - always mapped
        if ((adefault = stringDefault ("fed.parent")) != NULL)
        {
            sprintf (str, "fed.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (teamWin[0]);
        sprintf (str, "fed.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Kli team window - always mapped
        if ((adefault = stringDefault ("kli.parent")) != NULL)
        {
            sprintf (str, "kli.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (teamWin[2]);
        sprintf (str, "kli.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Ori team window - always mapped
        if ((adefault = stringDefault ("ori.parent")) != NULL)
        {
            sprintf (str, "ori.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (teamWin[3]);
        sprintf (str, "ori.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Rom team window - always mapped
        if ((adefault = stringDefault ("rom.parent")) != NULL)
        {
            sprintf (str, "rom.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (teamWin[1]);
        sprintf (str, "rom.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Quit window - always mapped
        if ((adefault = stringDefault ("quit.parent")) != NULL)
        {
            sprintf (str, "quit.parent:            %s\n", adefault);
            fputs (str, fp);
        }
         adefault = checkWindowsGeometry (qwin);
        sprintf (str, "quit.geometry:          %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        fputs ("\n", fp);

        // Stats window
        if ((adefault = stringDefault ("stats.parent")) != NULL)
        {
            sprintf (str, "stats.parent:           %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (statwin);
        sprintf (str, "stats.geometry:         %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "stats.mapped:           %s\n",
                  W_IsMapped (statwin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // War window
        if ((adefault = stringDefault ("war.parent")) != NULL)
        {
            sprintf (str, "war.parent:             %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (war);
        sprintf (str, "war.geometry:           %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "war.mapped:             %s\n",
                  W_IsMapped (war) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);

        // Sound window
        if ((adefault = stringDefault ("sound.parent")) != NULL)
        {
            sprintf (str, "sound.parent:           %s\n", adefault);
            fputs (str, fp);
        }
        adefault = checkWindowsGeometry (soundWin);
        sprintf (str, "sound.geometry:         %s\n",
                 (adefault != NULL) ? adefault : "auto");
        fputs (str, fp);
        sprintf (str, "sound.mapped:           %s\n",
                  W_IsMapped (soundWin) ? "on" : "off");
        fputs (str, fp);
        fputs ("\n", fp);
        
        if (saveBig)
            fputs ("\n", fp);
    }

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
        for (i = 0; i < MAX_MACRO; i++)
        {
            switch (macro[i].type)
            {
#ifdef NBT
            case NBTM:
                if (macro[i].key != 0)
                {
                    if (macro[i].key > 127)
                        sprintf (str, "macro.^%c.%c: %s\n",
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);
                    else
                        sprintf (str, "macro.%c.%c: %s\n",
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
                        sprintf (str, "mac.^%c: %s\n", 
                                getkeyfromctrl (macro[i].key),
                                macro[i].string);
                    else
                        sprintf (str, "mac.%c: %s\n", 
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
                        sprintf (str, "mac.^%c.%c: %s\n", 
                                getkeyfromctrl (macro[i].key),
                                macro[i].who,
                                macro[i].string);
                    else
                        sprintf (str, "mac.%c.%c: %s\n", 
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
                        strcat (str, "%p: ");
                        break;
                    case MACRO_TEAM:
                        strcat (str, "%t: ");
                        break;
                    case MACRO_FRIEND:
                        strcat (str, "%g: ");
                        break;
                    case MACRO_ENEMY:
                        strcat (str, "%h: ");
                        break;
                    case MACRO_ME:
                        strcat (str, "%i: ");
                        break;
                    }
                    strcat (str, macro[i].string);
                    strcat (str, "\n");
                    fputs (str, fp);
                    if (macro[i+1].type != NEWMMOUSE)
                        fputs ("\n", fp);
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
            strcat (str, ": ");
            strcat (str, dm->macro);
            strcat (str, "\n");
            fputs (str, fp);
        }

        fputs ("\n", fp);

        if (saveBig)
            fputs ("\n", fp);
    }

#ifdef BEEPLITE
    // Beeplite macros
    if (saveBeeplite)
    {
    	char **lt;
        if (saveBig)
            fputs ("# Beeplite macros\n", fp);

        for (lt = &distlite[1], dm = &dist_prefered[1]; dm->name; lt++, dm++)
        {
            if (*lt != NULL)  // distlite array defined as NULL, so only save the ones
                              // that have been set either through reading in netrekrc
                              // or from defLite settings
            {
                strcpy (str, "lite.");
                strcat (str, dm->name);
                strcat (str, ": ");
                strcat (str, *lt);
                strcat (str, "\n");
                fputs (str, fp);
            }
        }

        fputs ("\n", fp);

        if (saveBig)
            fputs ("\n", fp);
    }
#endif

#ifdef RCM
    // RCM macros
    if (saveRCM)
    {
        if (saveBig)
            fputs ("# Receiver configurable messages\n", fp);

        for (dm = &rcm_msg[1]; dm->name; dm++)
        {
            sprintf (str, "msg.%s: %s\n", dm->name, dm->macro);
            fputs (str, fp);
        }
        fputs ("\n", fp);

        if (saveBig)
            fputs ("\n", fp);
    }
#endif

    if (saveBig)
        fputs ("# Color settings\n", fp);

    for (j = 0; j < COLORS; j++)
    {
        sprintf (str1, "color.%s", colornames[j]);
        adefault = stringDefault (str1);
        if (adefault)
        {
            sprintf(str, "%s: %s\n", str1, adefault);
            fputs (str, fp);
        }
    }
    fputs ("\n", fp);
    
    if (saveBig)
        fputs ("\n", fp);

    if (saveBig)
    {
        fputs ("# Esoteric features such as individual ship keymaps\n", fp);
        fputs ("# /ckeymaps/buttonmaps (i.e. keymap-ca: <keymap>), observer\n", fp);
        fputs ("# /servertype options (i.e. keymap.bronco: <keymap>), and\n", fp);
        fputs ("# button keymaps (b1keymap through b5keymap)\n", fp);
    }
    // Individual ship type settings
    for (j = 0; j < nshiptypes; j++)
    {
    	struct ship *shipp;

        shipp = getship(j);
        sprintf(str1, "keymap-%c%c", shipp->s_desig[0], shipp->s_desig[1]);
        adefault = stringDefault (str1);
        if (adefault != NULL)
        {
            sprintf (str, "%s: %s\n", str1, adefault);
            fputs (str, fp);
        }

        sprintf(str1, "ckeymap-%c%c", shipp->s_desig[0], shipp->s_desig[1]);
        adefault = stringDefault (str1);
        if (adefault != NULL)
        {
            sprintf (str, "%s: %s\n", str1, adefault);
            fputs (str, fp);
        }

        sprintf(str1, "buttonmap-%c%c", shipp->s_desig[0], shipp->s_desig[1]);
        adefault = stringDefault (str1);
        if (adefault != NULL)
        {
            sprintf (str, "%s: %s\n", str1, adefault);
            fputs (str, fp);
        }
    }

    // Servername/nick/observer settings 
    sl = defaults;
    while (sl != NULL)
    {
        if (strstr (sl->string, ".observer")
            || strstr (sl->string, ".paradise")
            || strstr (sl->string, ".bronco")
            || strstr (sl->string, ".chaos")
            || strstr (sl->string, ".inl")
            || strstr (sl->string, ".sturgeon")
            || strstr (sl->string, ".hockey")
            || strstr (sl->string, ".dogfight")
            || strstr (sl->string, ".unknown"))
        {
            sprintf (str, "%s: %s\n", sl->string, sl->value);
            fputs (str, fp);
        }
        sl = sl->next;
    }
    
    // Button keymaps
    adefault = stringDefault ("b1keymap");
    if (adefault)
    {
        sprintf (str, "b1keymap: %s\n", adefault);
        fputs (str, fp);
    }
    adefault = stringDefault ("b2keymap");
    if (adefault)
    {
        sprintf (str, "b2keymap: %s\n", adefault);
        fputs (str, fp);
    }
    adefault = stringDefault ("b3keymap");
    if (adefault)
    {
        sprintf (str, "b3keymap: %s\n", adefault);
        fputs (str, fp);
    }
    adefault = stringDefault ("b4keymap");
    if (adefault)
    {
        sprintf (str, "b4keymap: %s\n", adefault);
        fputs (str, fp);
    }
    adefault = stringDefault ("b5keymap");
    if (adefault)
    {
        sprintf (str, "b5keymap: %s\n", adefault);
        fputs (str, fp);
    }

    fclose (fp);
}

/******************************************************************************/
/***  File:  data.c                                                         ***/
/***                                                                        ***/
/***  Function: declarations of global variables                            ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***                                                                        ***/
/***                                                                        ***/
/******************************************************************************/

#include "config.h"
#include "copyright.h"

#include <stdio.h>
#include <time.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

struct player *players;
struct player *me = NULL;
struct torp *torps;
struct plasmatorp *plasmatorps;
struct status *status;
struct ship *myship;
struct stats *mystats;
struct planet *planets;
struct phaser *phasers;
struct message *messages;
struct mctl *mctl;
struct memory universe;

int ingame = 0;              /* If player is in game - to distinguish between whether
                                   to use double buffering on the local and map window */
int ghoststart = 0;             /* is this a ghostbust
                                 * restart? */
int ghost_pno = 0;              /* is this a ghostbust
                                 * restart? */
int keepInfo = 15;              /* how many updates to keep
                                 * infowins 6/1/93 LAB */
int showPlanetOwner = 0;

int phaserShrink = 0;
int phaserShrinkStyle = 1;
int theirPhaserShrink = 0;
int shrinkPhaserOnMiss = 0;

int colorClient = 1;            /* Use new colorized bitmaps SRS 11/12/99 */
int dynamicBitmaps = 1;         /* in game switching of ship bitmaps, default to on */
int colorWeapons = 1;           /* Use color bitmaps for torps and plasmas */
int newDashboard = 2;           /* use new graphic
                                 * dashboard, 6/2/93 LAB */
int old_db = 0;                 /* should be same as
                                 * newDashboard */
int detCircle = 0;              /* Show det circle on tactical */
int puckCircle = 0;             /* Show maximum puck shooting distance on tactical */
int showArmy = 1;               /* Show army count of planet you are orbiting */
int fastQuit = 0;
int gen_distress = 0;           /* generic distress/macro
                                 * system support */
int niftyNewMessages = 1;
unsigned int oldalert = 0;
unsigned char oldtourn = (unsigned char) 2;  /* placeholder starting status */
int alertBorderColor = 0;
int remap[16] = { 0, 1, 2, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 };
int messpend = 0;
#ifdef XTRA_MESSAGE_UI
int messageHUD = 0;             /* Show message being typed on the local display           */
int messageHoldThresh = 0;      /* Threshold value for putting a message on hold (squared) */
int messMouseDelta = 0;         /* To keep track of mouse movement delta                   */
#endif
int lastcount = 0;
int mdisplayed = 0;
int udcounter = 0;
int showTractorPressor = 1;
int showAllTractorPressor = 1;  /* Enable SHOW_ALL_TRACTORS feature */
int showLock = 3;
int autoKey = 0;
int extraAlertBorder = 1;

/* udp options */
int tryUdp = 1;
int tryUdp1 = 1;
struct plupdate pl_update[MAXPLANETS];
char buttonmap[23] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
int lastm = 0;
time_t delay = 0;                  /* time stamp for decaring war */
time_t rdelay = 0;                 /* time stamp for refitting */
time_t tdelay = 0;                 /* time stamp for T mode extension */
int showPlanetNames = 1;
int autoQuit = 60;
int showStats = 0;
int showHints = 1;
int warnShields = 0;
int warncount = 0;
int warntimer = -1;
int infomapped = 0;
int mustexit = 0;
int messtime = 5;
int keepPeace = 1;

#ifdef GATEWAY
unsigned LONG netaddr = 0;      /* for blessing */

#endif

int msgBeep = 1;                /* ATM - msg beep */

int planetBitmap = 0;
int planetBitmapGalaxy = 0;
int planetHighlighting = 0;
int rotatePlanets = 1;

int logging = 0;
int continueTractor = 1;
int tcounter = 2;
char *title = NULL;
char *shipnos = "0123456789abcdefghijklmnopqrstuvwxyz";

int sock = -1;
int xtrekPort = -1;
int queuePos = -1;
int pickOk = -1;
int lastRank = -1;
int promoted = 0;

#ifdef ROTATERACE
int rotate = 0;
int rotate_deg = 0;

#endif

int loginAccept = -1;
unsigned localflags = 0;
int tournMask = 15;
int nextSocket = 0;             /* socket to use when we get
                                 * ghostbusted... */
char *serverName = NULL;
char *serverNick = NULL;
char defaultsFile[80] = ".xtrekrc";
char *myname = NULL;
int loggedIn = 0;
int reinitPlanets = 0;
int timerDelay = 200000;        /* micro secs between
                                 * updates */
int redrawDelay = 0;            /* 1/10 secs beetween
                                 * redraws */
int reportKills = 1;            /* report kill messages (in
                                 * review window)? */
int phaserWindow = 0;           /* Whether to show phaser window */
#ifdef PHASER_STATS
int phaserStats = 0;        /* How to show phaser stats */
int phaserStatTry = 0;          /* Try/attemps to phaser */
int phaserStatHit = 0;          /* Number of hits */
unsigned long phaserStatDamage = 0;       /* Overall damage inflicted */
#endif

#ifdef RECORDGAME
FILE *recordFile = NULL;        /* recorder: slurp packets
                                 * here */
int playback = 0;
int inplayback = 0;
#endif

FILE *logFile = NULL;           /* log messages to this file */
int scanplayer = 0;             /* who to scan */
int commMode = 0;               /* UDP: 0=TCP only, 1=UDP
                                 * updates */
int commModeReq = 0;            /* UDP: req for comm
                                 * protocol change */
int commStatus = 0;             /* UDP: used when switching
                                 * protocols */
int commSwitchTimeout = 0;      /* UDP: don't wait forever */
int udpTotal = 1;               /* UDP: total #of packets
                                 * received */
int udpDropped = 0;             /* UDP: count of packets
                                 * dropped */
int udpRecentDropped = 0;       /* UDP: #of packets dropped
                                 * recently */
int udpSock = -1;               /* UDP: the socket */
int udpDebug = 0;               /* UDP: debugging info
                                 * on/off */
int udpClientSend = 1;          /* UDP: send our packets
                                 * using UDP? */
int udpClientReceive = 1;          /* UDP: receive with simple
                                 * UDP */
int udpSequenceCheck = 1;         /* UDP: check sequence
                                 * numbers */
int weaponUpdate = 0;           /* Have any weapon packets
                                 * been received recently */

#ifdef GATEWAY
int gw_serv_port, gw_port, gw_local_port;       /* UDP */
char *gw_mach = NULL;           /* UDP */

#endif

/* for router-based firewalls, we need to tunnel through at a specific port */
int baseUdpLocalPort = 0;       /* UDP */

int debug = 0;

int messageon = 0;

#ifdef RSA
char testdata[16];
int useRsa = 1;

#endif

int SBhours = 0;

#ifdef SHORT_PACKETS
int why_dead = 0;
int tryShort = 1;               /* for .xtrekrc option */
int tryShort1 = 0;
int recv_short = 0;
int recv_mesg = 1;
int recv_kmesg = 1;
int recv_threshold = 0;
char recv_threshold_s[8] = { '0', '\0' };
int recv_warn = 1;

#endif

int updatesPerSec = 10;

#ifdef META
/* Metaservers list - comma delimited  */
char *metaServer = "metaserver.us.netrek.org, metaserver2.us.netrek.org";
int metaPort = 3521;
int metaVerbose = 0;
char *metaCache = NULL;
char *metaUDPCache = NULL;
#ifdef METAPING
int metaPing = 1;				/*  ICMP ping the metaserverlist */
#endif
#endif


#ifdef NBT
struct macro_list macro[MAX_MACRO];     /* NBT 2/26/93 */
int MacroMode = 0;
int macrocnt = 0;

#endif

extern double Sin[], Cos[];

W_Icon stipple, clockpic, clockhandpic, genopic, genopic2, icon;

W_Color borderColor, backColor, textColor, myColor, warningColor, shipCol[5],
    rColor, yColor, gColor, unColor, foreColor;

W_Icon base_expview;
W_Icon expview[BMP_SHIPEXPL_FRAMES];
W_Icon base_sbexpview;
W_Icon sbexpview[BMP_SBEXPL_FRAMES];

W_Icon torp_cloud_bitmaps;
W_Icon cloudC[NUM_CTORP_TYPES][BMP_TORPDET_FRAMES];
W_Icon plasma_cloud_bitmaps;
W_Icon plcloudC[NUM_CTORP_TYPES][BMP_TORPDET_FRAMES];
W_Icon mtorp_bitmaps;
W_Icon mtorpC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];
W_Icon torp_bitmaps;
W_Icon torpC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];
W_Icon mplasma_bitmaps;
W_Icon mplasmaC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];
W_Icon plasma_bitmaps;
W_Icon plasmaC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];

W_Icon cloud[BMP_TORPDET_FRAMES];
W_Icon plasmacloud[BMP_TORPDET_FRAMES];
W_Icon etorp, mtorp;
W_Icon eplasmatorp, mplasmatorp;

#ifdef VSHIELD_BITMAPS
W_Icon base_vshield;
W_Icon shield[SHIELD_FRAMES], cloakicon;
int varyShields = 1;

#else
W_Icon shield, cloakicon;

#endif

W_Icon base_hull;
W_Icon hull[BMP_HULL_FRAMES];
int vary_hull = 0;

// Ships
W_Icon ship_bitmaps[5];
W_Icon fed_bitmaps[NUM_TYPES][SHIP_VIEWS], kli_bitmaps[NUM_TYPES][SHIP_VIEWS],
    rom_bitmaps[NUM_TYPES][SHIP_VIEWS], ori_bitmaps[NUM_TYPES][SHIP_VIEWS],
    ind_bitmaps[NUM_TYPES][SHIP_VIEWS];

W_Icon ship_bitmaps1[5];
W_Icon fed_bitmaps1[NUM_TYPES][SHIP_VIEWS], kli_bitmaps1[NUM_TYPES][SHIP_VIEWS],
    rom_bitmaps1[NUM_TYPES][SHIP_VIEWS], ori_bitmaps1[NUM_TYPES][SHIP_VIEWS],
    ind_bitmaps1[NUM_TYPES][SHIP_VIEWS];

W_Icon ship_bitmapsG[5];
W_Icon fed_bitmapsG[NUM_TYPES][SHIP_VIEWS],
    kli_bitmapsG[NUM_TYPES][SHIP_VIEWS], rom_bitmapsG[NUM_TYPES][SHIP_VIEWS],
    ori_bitmapsG[NUM_TYPES][SHIP_VIEWS], ind_bitmapsG[NUM_TYPES][SHIP_VIEWS];

W_Icon ship_bitmapsM[5];
W_Icon fed_bitmapsM[NUM_TYPES][SHIP_VIEWS],
    kli_bitmapsM[NUM_TYPES][SHIP_VIEWS], rom_bitmapsM[NUM_TYPES][SHIP_VIEWS],
    ori_bitmapsM[NUM_TYPES][SHIP_VIEWS], ind_bitmapsM[NUM_TYPES][SHIP_VIEWS];
    
W_Icon ship_bitmapsT[5];
W_Icon fed_bitmapsT[NUM_TYPES][SHIP_VIEWS],
    kli_bitmapsT[NUM_TYPES][SHIP_VIEWS], rom_bitmapsT[NUM_TYPES][SHIP_VIEWS],
    ori_bitmapsT[NUM_TYPES][SHIP_VIEWS], ind_bitmapsT[NUM_TYPES][SHIP_VIEWS];

W_Icon ship_bitmapsHR[5];
W_Icon fed_bitmapsHR[NUM_TYPES],
    kli_bitmapsHR[NUM_TYPES], rom_bitmapsHR[NUM_TYPES],
    ori_bitmapsHR[NUM_TYPES], ind_bitmapsHR[NUM_TYPES];

// Planets
W_Icon planet_unknown_bitmap;
W_Icon planet_unknown[CPLANET_VIEWS];
W_Icon planet_unknown_NR;
W_Icon mplanet_unknown;
W_Icon planet_bitmaps[8], mplanet_bitmaps[8];
W_Icon planet_earth[NUMTEAMS], planet_klingus[NUMTEAMS], planet_orion[NUMTEAMS],
    planet_romulus[NUMTEAMS], planet_agri1[NUMTEAMS], planet_agri2[NUMTEAMS],
    planet_rock1[NUMTEAMS], planet_rock2[NUMTEAMS];
W_Icon mplanet_earth[NUMTEAMS], mplanet_klingus[NUMTEAMS], mplanet_orion[NUMTEAMS],
    mplanet_romulus[NUMTEAMS], mplanet_agri1[NUMTEAMS], mplanet_agri2[NUMTEAMS],
    mplanet_rock1[NUMTEAMS], mplanet_rock2[NUMTEAMS];
W_Icon army_bitmap, marmy_bitmap;
W_Icon wrench_bitmap, mwrench_bitmap;
W_Icon fuel_bitmap, mfuel_bitmap;
W_Icon base_planets;
W_Icon base_mplanets;
W_Icon bplanets[PLANET_VIEWS];
W_Icon bmplanets[MPLANET_VIEWS];

/* jn - SMARTMACRO */

#ifdef NEWMACRO
int MacroNum = 0;

#endif /* NEWMACRO */
char lastMessage[80];
char *classes[] = { "SC", "DD", "CA", "BB", "AS", "SB", "GA", "AT" };
char teamlet[] = { 'I', 'F', 'R', 'X', 'K', 'X', 'X', 'X', 'O', 'X', 'X', 'X',
    'X', 'X', 'X', 'A'
};
char *teamshort[16] = { "IND", "FED", "ROM", "X", "KLI", "X", "X", "X", "ORI",
    "X", "X", "X", "X", "X", "X", "ALL"
};
char pseudo[PSEUDOSIZE];
char defpasswd[PSEUDOSIZE];
char login[PSEUDOSIZE];

struct ship shipvals[NUM_TYPES];

/* 10 Aug 96 - Added curt (short) names -SAC */
struct rank ranks[NUMRANKS] = {
    {0.0, 0.0, 0.0, "Ensign", "Esgn"},
    {2.0, 1.0, 0.0, "Lieutenant", "Lt "},
    {4.0, 2.0, 0.0, "Lt. Cmdr.", "LtCm"},
    {8.0, 3.0, 0.0, "Commander", "Cder",},
    {15.0, 4.0, 0.0, "Captain", "Capt"},
    {20.0, 5.0, 0.0, "Flt. Capt.", "FltC"},
    {25.0, 6.0, 0.0, "Commodore", "Cdor"},
    {30.0, 7.0, 0.0, "Rear Adm.", "RAdm"},
    {40.0, 8.0, 0.0, "Admiral", "Admr"}
};

W_Window messagew, w, mapw, statwin, baseWin = 0, infow, tstatw, war,
    warnw, helpWin, teamWin[4], qwin, messwa, messwt, messwi, messwk,
    playerw, playerw2, planetw, rankw, optionWin = 0, reviewWin;
W_Window udpWin, phaserwin, hintWin;
W_Window waitWin, waitqWin, countWin, motdButtonWin, motdWin;

#ifdef SHORT_PACKETS
W_Window spWin = NULL;

#endif

#ifdef NBT
W_Window macroWin = NULL;

#endif

#ifdef META
W_Window metaWin = NULL;

#endif

int ping = 0;                   /* to ping or not to ping */
LONG packets_sent = 0;          /* # all packets sent to
                                 * server */
LONG packets_received = 0;      /* # all packets received */
W_Window pStats = NULL;

char rankmessage[80];
char deathmessage[80];
char outmessage[MAX_MLENGTH];          /* maximum message length */

char *xdisplay_name = NULL;

int newDistress = 0;
int UseNewMacro = 1;
int UseSmartMacro = 1;
int rejectMacro = 0;
int maskrecip = 0;

unsigned char singleMacro[MAX_MACRO] = "";

int enemyPhasers = 1;

char cloakChars[3] = "??";

int showIND = 0;
int newPlist = 0;
int playerListHack = 0;
int playerListMessaging = 1;

int showMySpeed = 0;

#ifdef JUBILEE_PHASERS
int colorfulPhasers = 0;
#endif

/* tried to automate this as much as possible... the entries are * the
 * character, string identifier, and the default macro for * each distress
 * type. */

/* the index into distmacro array should correspond with the correct
 * dist_type */

struct dmacro_list dist_prefered[NUM_DIST];

/* the index into distmacro array should correspond with the correct
 * dist_type */
/* the character specification is ignored now, kept here anyway for reference */
struct dmacro_list dist_defaults[] = {
    {(unsigned char) ('X'), "no zero", "this should never get looked at"},
/* ^t */
    {(unsigned char) ('\xd4'), "take", " %T%c->%O (%S) Carrying %a to %l%?%n>-1%{ @ %n%}\0"},
/* ^o */
    {(unsigned char) ('\xcf'), "ogg", " %T%c->%O Help Ogg %p at %l\0"},
/* ^b */
    {(unsigned char) ('\xc2'), "bomb", " %T%c->%O %?%n>4%{bomb %l @ %n%!bomb%}\0"},
/* ^c */
    {(unsigned char) ('\xc3'), "space_control", " %T%c->%O Help Control at %L\0"},
/* ^1 */
    {(unsigned char) ('\x91'), "save_planet", " %T%c->%O Emergency at %L!!!!\0"},
/* ^2 */
    {(unsigned char) ('\x92'), "base_ogg", " %T%c->%O Sync with --]> %g <[-- OGG ogg OGG base!!\0"},
/* ^3 */
    {(unsigned char) ('\x93'), "help1", " %T%c->%O Help me! %d%% dam, %s%% shd, %f%% fuel %a armies.\0"},
/* ^4 */
    {(unsigned char) ('\x94'), "help2", " %T%c->%O Help me! %d%% dam, %s%% shd, %f%% fuel %a armies.\0"},
/* ^e */
    {(unsigned char) ('\xc5'), "escorting", " %T%c->%O ESCORTING %g (%d%%D %s%%S %f%%F)\0"},
/* ^p */
    {(unsigned char) ('\xd0'), "ogging", " %T%c->%O Ogging %h\0"},
/* ^m */
    {(unsigned char) ('\xcd'), "bombing", " %T%c->%O Bombing %l @ %n\0"},
/* ^l */
    {(unsigned char) ('\xcc'), "controlling", " %T%c->%O Controlling at %l\0"},
/* ^5 */
    {(unsigned char) ('\x95'), "asw", " %T%c->%O Anti-bombing %p near %b.\0"},
/* ^6 */
    {(unsigned char) ('\x96'), "asbomb", " %T%c->%O DON'T BOMB %l. Let me bomb it (%S)\0"},
/* ^7 */
    {(unsigned char) ('\x97'), "doing1", " %T%c->%O (%i)%?%a>0%{ has %a arm%?%a=1%{y%!ies%}%} at %l.  (%d%% dam, %s%% shd, %f%% fuel)\0"},
/* ^8 */
    {(unsigned char) ('\x98'), "doing2", " %T%c->%O (%i)%?%a>0%{ has %a arm%?%a=1%{y%!ies%}%} at %l.  (%d%% dam, %s%% shd, %f%% fuel)\0"},
/* ^f */
    {(unsigned char) ('\xc6'), "free_beer", " %T%c->%O %p is free beer\0"},
/* ^n */
    {(unsigned char) ('\xce'), "no_gas", " %T%c->%O %p @ %l has no gas\0"},
/* ^h */
    {(unsigned char) ('\xc8'), "crippled", " %T%c->%O %p @ %l crippled\0"},
/* ^9 */
    {(unsigned char) ('\x99'), "pickup", " %T%c->%O %p++ @ %l\0"},
/* ^0 */
    {(unsigned char) ('\x90'), "pop", " %T%c->%O %l%?%n>-1%{ @ %n%}!\0"},
/* F */
    {(unsigned char) ('F'), "carrying", " %T%c->%O %?%S=SB%{Your Starbase is c%!C%}arrying %?%a>0%{%a%!NO%} arm%?%a=1%{y%!ies%}.\0"},
/* ^@ */
    {(unsigned char) ('\xa0'), "other1", " %T%c->%O (%i)%?%a>0%{ has %a arm%?%a=1%{y%!ies%}%} at %l. (%d%%D, %s%%S, %f%%F)\0"},
/* ^# */
    {(unsigned char) ('\x83'), "other2", " %T%c->%O (%i)%?%a>0%{ has %a arm%?%a=1%{y%!ies%}%} at %l. (%d%%D, %s%%S, %f%%F)\0"},
/* E */
    {(unsigned char) ('E'), "help", " %T%c->%O Help(%S)! %s%% shd, %d%% dmg, %f%% fuel,%?%S=SB%{ %w%% wtmp,%!%}%E%{ ETEMP!%}%W%{ WTEMP!%} %a armies!\0"},
    {'\0', '\0', '\0'},
};

struct dmacro_list *distmacro = dist_defaults;

int sizedist = sizeof (dist_defaults);

#ifdef BEEPLITE
char   *distlite[NUM_DIST] =
{
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

int     defLite = 1;
int     useLite = 1;

int     emph_planet_seq_n[MAXPLANETS] =
{0,};
int     emph_player_seq_n[MAXPLAYER] =
{0,};
W_Icon beeplite1;
W_Icon beeplite2;
W_Icon beeplite3;
W_Icon  emph_planet_seq[10];
W_Icon  emph_player_seq[10];
W_Icon  emph_player_seql[10];
int     beep_lite_cycle_time_player = 10;
int     beep_lite_cycle_time_planet = 10;
int     liteflag = 0;
char    F_beeplite_flags = LITE_PLAYERS_MAP |
LITE_PLAYERS_LOCAL |
LITE_SELF |
LITE_PLANETS |
LITE_SOUNDS |
LITE_TTS;

int     tts_len = 0;
int     tts_max_len = 40;
int	last_tts_xpos = 0;
int     last_tts_width = 0;
int     tts_timer = 0;
int     tts_time = 25;
int     tts_ypos = WINSIDE / 2 - 16;		 /* as found in redraw.c *

						  * 
						  * * originally */
char    lastIn[100];

#endif /* BEEPLITE */

#ifdef RCM                      /* Receiver configurable
                                 * Server messages */
struct dmacro_list rcm_msg[] = {
    {'0', "none", "Unknown RCM message"},
    {'1', "kill", "GOD->ALL %i (%S) (%T%c%?%a>0%{+%a armies%!%}) was kill %?%d>0%{%k%!(NO CREDIT)%} for %u (%r%p) %?%w>0%{%W%!%}"},
    {'2', "planet", "GOD->ALL %i (%S) (%T%c%?%a>0%{+%a armies%!%}) killed by %l (%z) %?%w>0%{%W%!%}"},
    {'3', "bomb", "%N->%Z We are being attacked by %i (%T%c) who is %d%% damaged."},
    {'4', "destroy", "%N->%Z %N destroyed by %i (%T%c)"},
    {'5', "take", "%N->%O %N taken by %i (%T%c)"},
    {'6', "ghostbust", "GOD->ALL %i (%S) (%T%c) was kill %k for the GhostBusters"},
    {'\0', '\0', '\0'},
};

#endif /* RCM */

int highlightFriendlyPhasers = 0;

#ifdef MOTION_MOUSE
/* KRP */
int continuousMouse = 1;
int continuousMouseFix = 1;
int motionThresh = 16;
int motion_mouse_enablable = 1;
int motion_mouse_steering = 0;

#endif

#ifdef SHIFTED_MOUSE
/* KRP */
int shiftedMouse = 1;

#endif

int ignoreCaps = 1;             /* Default is to ignore the
                                 * Capslock key SRS */

#ifdef MOUSE_AS_SHIFT
/* KRP */
int mouseAsShift = 0;
int b1_as_shift = 0;
int b2_as_shift = 0;
int b3_as_shift = 0;
int b4_as_shift = 0;
int b5_as_shift = 0;

#endif

#ifdef XTREKRC_HELP
W_Window defWin = NULL;

#endif

#ifdef CONTROL_KEY
int use_control_key = 1;

#endif

#ifdef DOC_WIN
W_Window docwin = NULL, xtrekrcwin = NULL;
int maxdoclines = 0, maxxtrekrclines = 0;

#endif

#ifdef REFRESH_INTERVAL
int refresh_interval = 0;

#endif

#ifdef BRMH
int max_fd = 3;

#endif

#ifdef TOOLS
W_Window toolsWin = NULL;
int shellTools = 1;
struct key_list macroKeys[MAX_KEY];
unsigned char keys[MAX_KEY] = "";

#endif

#ifdef SOUND
int sound_init = 1;
int sound_toggle = 0;
char *sounddir = NULL;
W_Window soundWin = NULL;

#endif

#ifdef HOCKEY_LINES
int showHockeyLinesLocal = 0;
struct s_line local_hockey_lines[NUM_HOCKEY_LINES + 1];
int showHockeyLinesMap = 0;
struct s_line map_hockey_lines[NUM_HOCKEY_LINES + 1];

#endif

#ifdef MULTILINE_MACROS
int multiline_enabled = 0;

#endif

#ifdef FEATURE_PACKETS
int F_feature_packets = 0;

#endif

int F_cloak_maxwarp = 0;
int F_self_8flags = 0;
int F_self_8flags2 = 0;
int F_self_19flags = 1;
int F_ship_cap = 0;
int F_show_all_tractors = 1;

#ifdef RECORDGAME
int F_many_self = 0;
#endif

#ifdef WARP_DEAD
int F_dead_warp = 0;
#endif

#ifdef UDP_PORTSWAP
int portSwap = 0;
#endif

// Load Ship Bitmaps from .BMP files
char *fed_ship_bmp;
char *rom_ship_bmp;
char *kli_ship_bmp;
char *ori_ship_bmp;
char *ind_ship_bmp;
char *fed_ship_bmp_1;
char *rom_ship_bmp_1;
char *kli_ship_bmp_1;
char *ori_ship_bmp_1;
char *ind_ship_bmp_1;
char *fed_ship_bmp_G;
char *rom_ship_bmp_G;
char *kli_ship_bmp_G;
char *ori_ship_bmp_G;
char *ind_ship_bmp_G;
char *fed_ship_bmp_T;
char *rom_ship_bmp_T;
char *kli_ship_bmp_T;
char *ori_ship_bmp_T;
char *ind_ship_bmp_T;
char *fed_ship_bmp_M;
char *rom_ship_bmp_M;
char *kli_ship_bmp_M;
char *ori_ship_bmp_M;
char *ind_ship_bmp_M;
char *fed_ship_bmp_HR;
char *rom_ship_bmp_HR;
char *kli_ship_bmp_HR;
char *ori_ship_bmp_HR;
char *ind_ship_bmp_HR;

// Added to fix thread creation issue
HANDLE MainThread = NULL;
HANDLE InputThread = NULL;

// missing variables
int forceDisplay = 0;
int mungScrollbarColors = 0;
int showMotd = 1;

char *saveFile = NULL;
int saveBig = 1;
int saveWindow = 1;
int saveMacro = 1;
int saveRCD = 1;
#ifdef BEEPLITE
int saveBeeplite = 1;
#endif
#ifdef RCM
int saveRCM = 1;
#endif

int tpDotDist = 10;

struct tractor *tractcurrent = NULL;
struct tractor *tracthead = NULL;
struct tractor *tractrunner;

int agriCAPS = 1;
int agriColor = 2;

int windowMove = 0;
int mainResizeable = 1;
int maxScrollLines = 300;
int mainTitleBar = 0;
int observerMode = 0;
int showHockeyScore = 1;

int observerPorts[] = { 2593, 2595, 4000, 5000, 0 };

char pigcall[256] = "";
char cowid[256] = "";

int serverType = 0;
int beepOnPrivateMessage = 0;

W_Window wam_windows[6];

int showStars = 1;
int warpStreaks = 1;

/* time client connected to server */
time_t  timeStart = 0;

/* dashboard timer data */
int timerType = T_DAY;          /* timer defaults to day time */
time_t  timeBank[T_TOTAL];      /* array of times    */

int omitTeamLetter = 0;
int viewBox = 1;

struct stringlist *defaults = NULL;

SDBUFFER * localSDB = NULL;
SDBUFFER * mapSDB = NULL;

int doubleBuffering = 1; /* Use double buffering to draw map/local */

int disableWinkey = 1;  /* disable WinKey + ContextKey by default */

int exitFlag = 0;       /* no exit by default */

struct cons_buffer * consHead = NULL;    // head of the linked list for console buffer
struct cons_buffer * consTail = NULL;    // tail of the linked list for console buffer

int allowWheelActions = 1;   /* allow wheel actions */

WNDPROC lpfnDefRichEditWndProc;     /* default window proc */

int richText = 0;   /* temporary variable to select rich text message windows */
int newQuit = 0;    /* new quit clock */
int newSound = 1;   /* use new SDL sound */
int newSoundAngles = 1; /* use new SDL sound with angular 3d component */
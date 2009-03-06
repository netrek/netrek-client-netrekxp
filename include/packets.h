
/* Include file for socket I/O xtrek.
 * 
 * Kevin P. Smith 1/29/89 */
#include "copyright2.h"


#define STATUS_TOKEN    "\t@@@" /* ATM */


/* TCP and UDP use identical packet formats; the only difference is that,
 * when in UDP mode, all packets sent from server to client have a sequence
 * number appended to them.
 * 
 * (note: ALL packets, whether sent on the TCP or UDP channel, will have the
 * sequence number.  Thus it's important that client & server agree on when
 * to switch.  This was done to keep critical and non-critical data in sync.) */

#define RECORDPACKET(p) (\
        p==SP_MESSAGE ||\
 	p==SP_PLAYER_INFO ||\
 	p==SP_KILLS ||\
 	p==SP_PLAYER ||\
 	p==SP_YOU ||\
 	p==SP_STATUS ||\
 	p==SP_PLANET ||\
 	p==SP_LOGIN ||\
 	p==SP_FLAGS ||\
 	p==SP_MASK ||\
 	p==SP_PSTATUS ||\
 	p==SP_BADVERSION ||\
 	p==SP_HOSTILE ||\
 	p==SP_STATS ||\
 	p==SP_PL_LOGIN ||\
 	p==SP_RESERVED ||\
 	p==SP_PLANET_LOC)

/* packets sent from xtrek server to remote client */
#define SP_MESSAGE 	1
#define SP_PLAYER_INFO 	2       /* general player info not
                                 * elsewhere */
#define SP_KILLS	3       /* # kills a player has */
#define SP_PLAYER	4       /* x,y for player */
#define SP_TORP_INFO	5       /* torp status */
#define SP_TORP		6       /* torp location */
#define SP_PHASER	7       /* phaser status and
                                 * direction */
#define SP_PLASMA_INFO	8       /* player login information */
#define SP_PLASMA	9       /* like SP_TORP */
#define SP_WARNING	10      /* like SP_MESG */
#define SP_MOTD		11      /* line from .motd screen */
#define SP_YOU		12      /* info on you? */
#define SP_QUEUE	13      /* estimated loc in queue? */
#define SP_STATUS	14      /* galaxy status numbers */
#define SP_PLANET 	15      /* planet armies &
                                 * facilities */
#define SP_PICKOK	16      /* your team & ship was
                                 * accepted */
#define SP_LOGIN	17      /* login response */
#define SP_FLAGS	18      /* give flags for a player */
#define SP_MASK		19      /* tournament mode mask */
#define SP_PSTATUS	20      /* give status for a player */
#define SP_BADVERSION   21      /* invalid version number */
#define SP_HOSTILE	22      /* hostility settings for a
                                 * player */
#define SP_STATS	23      /* a player's statistics */
#define SP_PL_LOGIN	24      /* new player logs in */
#define SP_RESERVED	25      /* for future use */
#define SP_PLANET_LOC	26      /* planet name, x, y */

#define SP_SCAN         27      /* paradise: results of player
                                 * scan */

#define SP_UDP_REPLY    28      /* notify client of UDP
                                 * status */
#define SP_SEQUENCE     29      /* sequence # packet */
#define SP_SC_SEQUENCE  30      /* this trans is
                                 * semi-critical info */

#ifdef RSA
#define SP_RSA_KEY	31      /* handles binary
                                 * verification */
#endif

// Note the conflicting defines for types 32 and 33
#define SP_MOTD_PIC	32	/* paradise - motd bitmap pictures */
#define SP_GENERIC_32	32	/* bronco - 32 byte generic, see struct */
#define SP_STATS2	33	/* paradise - new stats packet */
#define SP_FLAGS_ALL	33	/* bronco - abbreviated flags for all players */
#define SP_STATUS2	34	/* paradise - new status packet */
#define SP_PLANET2	35	/* paradise - new planet packet */
#define SP_NEW_MOTD     36	/* paradise - New MOTD info notification uses */
#define SP_THINGY	37	/* paradise - thingy location */
#define SP_THINGY_INFO	38	/* paradise - thingy status */
#define SP_SHIP_CAP	39	/* Handles server ship mods */

#ifdef SHORT_PACKETS
#define SP_S_REPLY      40      /* reply to send-short
                                 * request */
#define SP_S_MESSAGE    41      /* var. Message Packet */
#define SP_S_WARNING    42      /* Warnings with 4  Bytes */
#define SP_S_YOU        43      /* hostile,armies,whydead,etc
                                 * .. */
#define SP_S_YOU_SS     44      /* your ship status */
#define SP_S_PLAYER     45      /* variable length player
                                 * packet */
#endif

#ifdef PING
#define SP_PING         46      /* ping packet */
#endif

#ifdef SHORT_PACKETS
#define SP_S_TORP       47      /* variable length torp
                                 * packet */
#define SP_S_TORP_INFO  48      /* SP_S_TORP with TorpInfo */
#define SP_S_8_TORP     49      /* optimized SP_S_TORP */
#define SP_S_PLANET     50      /* see SP_PLANET */

#define SP_GPARAM	51	/* paradise game params packet */
/* the following is a family of packets with the same type, but a
   discriminating subtype */
#define SP_PARADISE_EXT1	52
#define SP_PE1_MISSING_BITMAP	0
#define SP_PE1_NUM_MISSILES	1
/* end of packet 52 subtypes */
#define SP_TERRAIN2	53	/* paradise terrain packets */
#define SP_TERRAIN_INFO2 54	/* paradise terrain info */

/* S_P2 */
#define SP_S_SEQUENCE   56      /* SP_SEQUENCE for
                                 * compressed packets */
#define SP_S_PHASER     57      /* see struct */
#define SP_S_KILLS      58      /* # of kills player have */
#define SP_S_STATS      59      /* see SP_STATS */

/* variable length packets */
#define VPLAYER_SIZE    4
#define SHORTVERSION    11      /* S_P2 */
#define OLDSHORTVERSION 10      /* Short packets version 1*/
#endif


/* packets sent from remote client to xtrek server */
#define CP_MESSAGE      1       /* send a message */
#define CP_SPEED	2       /* set speed */
#define CP_DIRECTION	3       /* change direction */
#define CP_PHASER	4       /* phaser in a direction */
#define CP_PLASMA	5       /* plasma (in a direction) */
#define CP_TORP		6       /* fire torp in a direction */
#define CP_QUIT		7       /* self destruct */
#define CP_LOGIN	8       /* log in (name, password) */
#define CP_OUTFIT	9       /* outfit to new ship */
#define CP_WAR		10      /* change war status */
#define CP_PRACTR	11      /* create practice robot? */
#define CP_SHIELD	12      /* raise/lower sheilds */
#define CP_REPAIR	13      /* enter repair mode */
#define CP_ORBIT	14      /* orbit planet/starbase */
#define CP_PLANLOCK	15      /* lock on planet */
#define CP_PLAYLOCK	16      /* lock on player */
#define CP_BOMB		17      /* bomb a planet */
#define CP_BEAM		18      /* beam armies up/down */
#define CP_CLOAK	19      /* cloak on/off */
#define CP_DET_TORPS	20      /* detonate enemy torps */
#define CP_DET_MYTORP	21      /* detonate one of my torps */
#define CP_COPILOT	22      /* toggle copilot mode */
#define CP_REFIT	23      /* refit to different ship
                                 * type */
#define CP_TRACTOR	24      /* tractor on/off */
#define CP_REPRESS	25      /* pressor on/off */
#define CP_COUP		26      /* coup home planet */
#define CP_SOCKET	27      /* new socket for
                                 * reconnection */
#define CP_OPTIONS	28      /* send my options to be
                                 * saved */
#define CP_BYE		29      /* I'm done! */
#define CP_DOCKPERM	30      /* set docking permissions */
#define CP_UPDATES	31      /* set number of usecs per
                                 * update */
#define CP_RESETSTATS	32      /* reset my stats packet */
#define CP_RESERVED	33      /* for future use */

/* NOTE: not implemented. */
#define CP_SCAN         34      /* ATM: request for player
                                 * scan */

#define CP_UDP_REQ      35      /* request UDP on/off */
#define CP_SEQUENCE     36      /* sequence # packet */

#ifdef RSA
#define CP_RSA_KEY      37      /* handles binary
                                 * verification */
#endif

/* Note the conflicting defines for packet type 38*/
#define CP_ASK_MOTD	38	/* paradise - request MOTD */
#define CP_PLANET       38      /* bronco - cross-check planet info */

#define CP_PING_RESPONSE 42     /* client response */

#ifdef SHORT_PACKETS
#define CP_S_REQ                43
#define CP_S_THRS               44
#define CP_S_MESSAGE    45      /* vari. Message Packet */
#define CP_S_RESERVED       46
#define CP_S_DUMMY      47
#endif

#ifdef FEATURE_PACKETS
#define CP_FEATURE	60
#define SP_FEATURE	60
#endif

/* special paradise type tells us when to update the display on playback.
    Not sent or received, only placed in the recorder file */
#define REC_UPDATE 127

#define SOCKVERSION 	4

#define UDPVERSION      10

struct packet_handler
{
    int size;
    void (*handler) ();
};

struct mesg_spacket
{
    char type;                  /* SP_MESSAGE */
    unsigned char m_flags;
    unsigned char m_recpt;
    unsigned char m_from;
    char mesg[80];
};

struct plyr_info_spacket
{
    char type;                  /* SP_PLAYER_INFO */
    char pnum;
    char shiptype;
    char team;
};

struct plyr_login_spacket
{
    char type;                  /* SP_PL_LOGIN */
    char pnum;
    char rank;
    char pad1;
    char name[16];
    char monitor[16];
    char login[16];
};

struct hostile_spacket
{
    char type;                  /* SP_HOSTILE */
    char pnum;
    char war;
    char hostile;
};

struct stats_spacket
{
    char type;                  /* SP_STATS */
    char pnum;
    char pad1;
    char pad2;
    LONG tkills;                /* Tournament kills */
    LONG tlosses;               /* Tournament losses */
    LONG kills;                 /* overall */
    LONG losses;                /* overall */
    LONG tticks;                /* ticks of tournament play
                                 * time */
    LONG tplanets;              /* Tournament planets */
    LONG tarmies;               /* Tournament armies */
    LONG sbkills;               /* Starbase kills */
    LONG sblosses;              /* Starbase losses */
    LONG armies;                /* non-tourn armies */
    LONG planets;               /* non-tourn planets */
    LONG maxkills;              /* max kills as player * 100 */
    LONG sbmaxkills;            /* max kills as sb * 100 */
};

struct flags_spacket
{
    char type;                  /* SP_FLAGS */
    char pnum;                  /* whose flags are they? */
    char tractor;               /* ATM visible tractors */
    char pad2;
    unsigned flags;
};

struct kills_spacket
{
    char type;                  /* SP_KILLS */
    char pnum;
    char pad1;
    char pad2;
    unsigned kills;             /* where 1234=12.34 kills
                                 * and 0=0.00 kills */
};

struct player_spacket
{
    char type;                  /* SP_PLAYER */
    char pnum;
    unsigned char dir;
    char speed;
    LONG x, y;
};

struct torp_info_spacket
{
    char type;                  /* SP_TORP_INFO */
    char war;
    char status;                /* TFREE, TDET, etc... */
    char pad1;                  /* pad needed for cross cpu
                                 * compatibility */
    unsigned short tnum;
    unsigned short pad2;
};

struct torp_spacket
{
    char type;                  /* SP_TORP */
    unsigned char dir;
    unsigned short tnum;
    LONG x, y;
};

struct phaser_spacket
{
    char type;                  /* SP_PHASER */
    char pnum;
    char status;                /* PH_HIT, etc... */
    unsigned char dir;
    LONG x, y;
    LONG target;
};

struct you_spacket
{
    char type;                  /* SP_YOU */
    char pnum;                  /* Guy needs to know this... */
    char hostile;
    char swar;
    char armies;
    char tractor;
    char pad2;
    char pad3;
    unsigned flags;
    LONG damage;
    LONG shield;
    LONG fuel;
    unsigned short etemp;
    unsigned short wtemp;
    unsigned short whydead;
    unsigned short whodead;
};

struct status_spacket
{
    char type;                  /* SP_STATUS */
    char tourn;
    char pad1;
    char pad2;
    unsigned armsbomb;
    unsigned planets;
    unsigned kills;
    unsigned losses;
    unsigned time;
    unsigned LONG timeprod;
};

struct warning_spacket
{
    char type;                  /* SP_WARNING */
    char pad1;
    char pad2;
    char pad3;
    char mesg[80];
};

struct planet_spacket
{
    char type;                  /* SP_PLANET */
    char pnum;
    char owner;
    char info;
    unsigned short flags;
    unsigned short pad2;
    LONG armies;
};

struct torp_cpacket
{
    char type;                  /* CP_TORP */
    unsigned char dir;          /* direction to fire torp */
    char pad1;
    char pad2;
};

struct phaser_cpacket
{
    char type;                  /* CP_PHASER */
    unsigned char dir;
    char pad1;
    char pad2;
};

struct speed_cpacket
{
    char type;                  /* CP_SPEED */
    char speed;
    char pad1;
    char pad2;
};

struct dir_cpacket
{
    char type;                  /* CP_DIRECTION */
    unsigned char dir;
    char pad1;
    char pad2;
};

struct shield_cpacket
{
    char type;                  /* CP_SHIELD */
    char state;                 /* up/down */
    char pad1;
    char pad2;
};

struct repair_cpacket
{
    char type;                  /* CP_REPAIR */
    char state;                 /* on/off */
    char pad1;
    char pad2;
};

struct orbit_cpacket
{
    char type;                  /* CP_ORBIT */
    char state;                 /* on/off */
    char pad1;
    char pad2;
};

struct planet_cpacket
{
    char type;                  /* CP_PLANET */
    char pnum;
    char owner;
    char info;
    unsigned short flags;
    int armies;
};

struct practr_cpacket
{
    char type;                  /* CP_PRACTR */
    char pad1;
    char pad2;
    char pad3;
};

struct bomb_cpacket
{
    char type;                  /* CP_BOMB */
    char state;
    char pad1;
    char pad2;
};

struct beam_cpacket
{
    char type;                  /* CP_BEAM */
    char state;
    char pad1;
    char pad2;
};

struct cloak_cpacket
{
    char type;                  /* CP_CLOAK */
    char state;
    char pad1;
    char pad2;
};

struct det_torps_cpacket
{
    char type;                  /* CP_DET_TORPS */
    char pad1;
    char pad2;
    char pad3;
};

struct copilot_cpacket
{
    char type;                  /* CP_COPLIOT */
    char state;
    char pad1;
    char pad2;
};

struct queue_spacket
{
    char type;                  /* SP_QUEUE */
    char pad1;
    unsigned short pos;
};

struct outfit_cpacket
{
    char type;                  /* CP_OUTFIT */
    char team;
    char ship;
    char pad1;
};

struct pickok_spacket
{
    char type;                  /* SP_PICKOK */
    char state;
    char pad2;
    char pad3;
};

struct login_cpacket
{
    char type;                  /* CP_LOGIN */
    char query;
    char pad2;
    char pad3;
    char name[16];
    char password[16];
    char login[16];
};

struct login_spacket
{
    char type;                  /* SP_LOGIN */
    char accept;                /* 1/0 */
    char pad2;
    char pad3;
    LONG flags;
    char keymap[96];
};

struct tractor_cpacket
{
    char type;                  /* CP_TRACTOR */
    char state;
    char pnum;
    char pad2;
};

struct repress_cpacket
{
    char type;                  /* CP_REPRESS */
    char state;
    char pnum;
    char pad2;
};

struct det_mytorp_cpacket
{
    char type;                  /* CP_DET_MYTORP */
    char pad1;
    unsigned short tnum;
};

struct war_cpacket
{
    char type;                  /* CP_WAR */
    char newmask;
    char pad1;
    char pad2;
};

struct refit_cpacket
{
    char type;                  /* CP_REFIT */
    char ship;
    char pad1;
    char pad2;
};

struct plasma_cpacket
{
    char type;                  /* CP_PLASMA */
    unsigned char dir;
    char pad1;
    char pad2;
};

struct plasma_info_spacket
{
    char type;                  /* SP_PLASMA_INFO */
    char war;
    char status;                /* TFREE, TDET, etc... */
    char pad1;                  /* pad needed for cross cpu
                                 * compatibility */
    unsigned short pnum;
    unsigned short pad2;
};

struct plasma_spacket
{
    char type;                  /* SP_PLASMA */
    char pad1;
    unsigned short pnum;
    LONG x, y;
};

struct playlock_cpacket
{
    char type;                  /* CP_PLAYLOCK */
    char pnum;
    char pad1;
    char pad2;
};

struct planlock_cpacket
{
    char type;                  /* CP_PLANLOCK */
    char pnum;
    char pad1;
    char pad2;
};

struct coup_cpacket
{
    char type;                  /* CP_COUP */
    char pad1;
    char pad2;
    char pad3;
};

struct pstatus_spacket
{
    char type;                  /* SP_PSTATUS */
    char pnum;
    char status;
    char pad1;
};

struct motd_spacket
{
    char type;                  /* SP_MOTD */
    char pad1;
    char pad2;
    char pad3;
    char line[80];
};

struct quit_cpacket
{
    char type;                  /* CP_QUIT */
    char pad1;
    char pad2;
    char pad3;
};

struct mesg_cpacket
{
    char type;                  /* CP_MESSAGE */
    char group;
    char indiv;
    char pad1;
    char mesg[80];
};

struct mask_spacket
{
    char type;                  /* SP_MASK */
    char mask;
    char pad1;
    char pad2;
};

struct socket_cpacket
{
    char type;                  /* CP_SOCKET */
    char version;
    char udp_version;           /* was pad2 */
    char pad3;
    unsigned socket;
};

struct options_cpacket
{
    char type;                  /* CP_OPTIONS */
    char pad1;
    char pad2;
    char pad3;
    unsigned flags;
    char keymap[96];
};

struct bye_cpacket
{
    char type;                  /* CP_BYE */
    char pad1;
    char pad2;
    char pad3;
};

struct badversion_spacket
{
    char type;                  /* SP_BADVERSION */
    char why;
    char pad2;
    char pad3;
};

struct dockperm_cpacket
{
    char type;                  /* CP_DOCKPERM */
    char state;
    char pad2;
    char pad3;
};

struct updates_cpacket
{
    char type;                  /* CP_UPDATES */
    char pad1;
    char pad2;
    char pad3;
    unsigned usecs;
};

struct resetstats_cpacket
{
    char type;                  /* CP_RESETSTATS */
    char verify;                /* 'Y' - just to make sure
                                 * he meant it */
    char pad2;
    char pad3;
};

struct reserved_spacket
{
    char type;                  /* SP_RESERVED */
    char pad1;
    char pad2;
    char pad3;
    char data[16];
};

struct reserved_cpacket
{
    char type;                  /* CP_RESERVED */
    char pad1;
    char pad2;
    char pad3;
    char data[16];
    char resp[16];
};

struct udp_req_cpacket
{                               /* UDP */
    char type;                  /* CP_UDP_REQ */
    char request;
    char connmode;              /* respond with port # or
                                 * just send UDP packet? */
    char pad2;
    int port;                   /* compensate for hosed
                                 * recvfrom() */
};
struct ping_cpacket
{
    char type;                  /* CP_PING_RESPONSE */
    unsigned char number;       /* id */
    char pingme;                /* if client wants server to
                                 * ping */
    char pad1;

    LONG cp_sent;               /* # packets sent to server */
    LONG cp_recv;               /* # packets recv from
                                 * server */
};
struct ping_spacket
{
    char type;                  /* SP_PING */
    unsigned char number;       /* id (ok to wrap) */
    unsigned short lag;         /* delay of last ping in ms */

    unsigned char tloss_sc;     /* total loss server-client
                                 * 0-100% */
    unsigned char tloss_cs;     /* total loss client-server
                                 * 0-100% */

    unsigned char iloss_sc;     /* inc. loss server-client
                                 * 0-100% */
    unsigned char iloss_cs;     /* inc. loss client-server
                                 * 0-100% */
};
struct sequence_cpacket
{                               /* UDP */
    char type;                  /* CP_SEQUENCE */
    char pad1;
    unsigned short sequence;
};


struct sc_sequence_spacket
{                               /* UDP */
    char type;                  /* SP_CP_SEQUENCE */
    char pad1;
    unsigned short sequence;
};

struct udp_reply_spacket
{                               /* UDP */
    char type;                  /* SP_UDP_REPLY */
    char reply;
    char pad1;
    char pad2;
    int port;
};

struct sequence_spacket
{                               /* UDP */
    char type;                  /* SP_SEQUENCE */
    unsigned char flag8;
    unsigned short sequence;
};


struct planet_loc_spacket
{
    char type;                  /* SP_PLANET_LOC */
    char pnum;
    char pad2;
    char pad3;
    LONG x;
    LONG y;
    char name[16];
};

#ifdef FEATURE_PACKETS
struct feature_cpacket
{
    char type;
    char feature_type;
    char arg1, arg2;
    int value;
    char name[80];
};

#endif

#ifdef RSA
struct rsa_key_spacket
{
    char type;                  /* SP_RSA_KEY */
    char pad1;
    char pad2;
    char pad3;
    unsigned char data[KEY_SIZE];
};

struct rsa_key_cpacket
{
    char type;                  /* CP_RSA_KEY */
    char pad1;
    char pad2;
    char pad3;
    unsigned char global[KEY_SIZE];
    unsigned char public[KEY_SIZE];
    unsigned char resp[KEY_SIZE];
};

#endif

struct ship_cap_spacket
{                               /* Server configuration of
                                 * client */
    char type;                  /* screw motd method */
    char operation;             /* 0 = add/change a ship, 1
                                 * = remove a ship */
    unsigned short s_type;      /* SP_SHIP_CAP */
    unsigned short s_torpspeed;
    unsigned short s_phaserrange;
    int s_maxspeed;
    int s_maxfuel;
    int s_maxshield;
    int s_maxdamage;
    int s_maxwpntemp;
    int s_maxegntemp;
    unsigned short s_width;
    unsigned short s_height;
    unsigned short s_maxarmies;
    char s_letter;
    char s_armies;		/* Paradise only */
    char s_name[16];
    char s_desig1;
    char s_desig2;
    unsigned short s_bitmap;
};

#pragma pack(push,1) /* Necessary to remove padding during compile,
                        size of struct needs to be exact, GCC equivalent
                        is __attribute__ (( packed )) */
struct generic_32_spacket
{
    char        type;
    char        pad[31];
};
#define GENERIC_32_LENGTH 32
#define COST_GENERIC_32 (F_sp_generic_32 ? GENERIC_32_LENGTH : 0)
struct generic_32_spacket_a
{
    char        type;
    char        version;        /* alphabetic, 0x60 + version */
    u_short     repair_time;    /* server estimate of repair time in seconds */
    u_short     pl_orbit;       /* what planet player orbiting, -1 if none */
    char        pad1[26];
    /* NOTE: this version didn't use network byte order for the shorts */
};
#define GENERIC_32_VERSION_A 1
struct generic_32_spacket_b
{
    char        type;
    char        version;        /* alphabetic, 0x60 + version */
    u_short     repair_time;    /* server estimate of repair time, seconds  */
    char        pl_orbit;       /* what planet player orbiting, -1 if none  */
    u_short     gameup;                  /* server status flags             */
    u_char      tournament_teams;        /* what teams are involved         */
    u_char      tournament_age;          /* time since last t-mode start    */
    char        tournament_age_units;    /* units for above, see s2du       */
    u_char      tournament_remain;       /* remaining INL game time         */
    char        tournament_remain_units; /* units for above, see s2du       */
    u_char      starbase_remain;         /* starbase reconstruction, mins   */
    u_char      team_remain;             /* team surrender time, seconds    */
    char        pad1[18];
};
#pragma pack(pop) /* Restores packing values saved by push */

#define GENERIC_32_VERSION_B 2
#define GENERIC_32_VERSION GENERIC_32_VERSION_B /* default */

/* SP_GENERIC_32 versioning instructions:

   we start with version 'a', and each time a structure is changed
   increment the version and reduce the pad size, keeping the packet
   the same size ...

   client is entitled to trust fields in struct that were defined at a
   particular version ...

   client is to send CP_FEATURE with SP_GENERIC_32 value 1 for version
   'a', value 2 for version 'b', etc ...

   server is to reply with SP_FEATURE with SP_GENERIC_32 value set to
   the maximum version it supports (not the version requested by the
   client), ...

   server is to send SP_GENERIC_32 packets of the highest version it
   knows about, but no higher than the version the client asks for.
*/

struct flags_all_spacket
{
    char type;			/* SP_FLAGS_ALL */
    char offset;		/* slot number of first flag */
    int flags;			/* two bits per slot */
};
#define FLAGS_ALL_DEAD                   0
#define FLAGS_ALL_CLOAK_ON               1
#define FLAGS_ALL_CLOAK_OFF_SHIELDS_UP   2
#define FLAGS_ALL_CLOAK_OFF_SHIELDS_DOWN 3

#ifdef SHORT_PACKETS
struct shortreq_cpacket
{                               /* CP_S_REQ */
    char type;
    char req;
    char version;
    char pad2;
};

struct threshold_cpacket
{                               /* CP_S_THRS */
    char type;
    char pad1;
    unsigned short thresh;
};

struct shortreply_spacket
{                               /* SP_S_REPLY */
    char type;
    char repl;
    unsigned short winside;
    LONG gwidth;
};

struct youshort_spacket
{                               /* SP_S_YOU */
    char type;

    char pnum;
    char hostile;
    char swar;

    char armies;
    char whydead;
    char whodead;

    char pad1;

    unsigned flags;
};

struct youss_spacket
{                               /* SP_S_YOU_SS */
    char type;
    char pad1;

    unsigned short damage;
    unsigned short shield;
    unsigned short fuel;
    unsigned short etemp;
    unsigned short wtemp;
};

#define VPLANET_SIZE 6

struct planet_s_spacket
{                               /* body of SP_S_PLANET  */
    char pnum;
    char owner;
    char info;
    unsigned char armies;       /* more than 255 Armies ?
                                 * ...  */
    unsigned short flags;
};
struct warning_s_spacket
{                               /* SP_S_WARNING */
    char type;
    unsigned char whichmessage;
    char argument, argument2;   /* for phaser  etc ... */
};

struct player_s_spacket
{
    char type;                  /* SP_S_PLAYER Header */
    char packets;               /* How many player-packets
                                 * are in this packet ( only
                                 * the first 6 bits are
                                 * relevant ) */
    unsigned char dir;
    char speed;
    LONG x, y;                  /* To get the absolute
                                 * Position */
};

/* S_P2 */
struct player_s2_spacket
{
    char type;                  /* SP_S_PLAYER Header */
    char packets;               /* How many player-packets
                                 * are in this packet  (
                                 * only the firs t 6 bits
                                 * are relevant ) */
    unsigned char dir;
    char speed;
    short x, y;                 /* absolute position / 40 */
    unsigned int flags;         /* 16 playerflags */
};

/* The format of the body: struct player_s_body_spacket {       Body of new
 * Player Packet unsigned char pnum;   0-4 = pnum, 5 local or galactic, 6 =
 * 9. x-bit, 7 9. y-bit unsigned char speeddir;       0-3 = speed , 4-7
 * direction of ship unsigned char x;      low 8 bits from X-Pixelcoordinate
 * unsigned char y;      low 8 bits from Y-Pixelcoordinate }; */

struct torp_s_spacket
{
    char type;                  /* SP_S_TORP */
    unsigned char bitset;       /* bit=1 that torp is in
                                 * packet */
    unsigned char whichtorps;   /* Torpnumber of first torp
                                 * / 8 */
    unsigned char data[21];     /* For every torp 2*9 bit
                                 * coordinates */
};

struct mesg_s_spacket
{
    char type;                  /* SP_S_MESSAGE */
    unsigned char m_flags;
    unsigned char m_recpt;
    unsigned char m_from;
    unsigned char length;       /* Length of whole packet */
    char mesg;
    char pad2;
    char pad3;
    char pad[76];
};

struct mesg_s_cpacket
{
    char type;                  /* CP_S__MESSAGE */
    char group;
    char indiv;
    char length;                /* Size of whole packet   */
    char mesg[80];
};

/* S_P2 */
struct kills_s_spacket
{
    char type;                  /* SP_S_KILLS */
    char pnum;                  /* How many kills in packet */
    unsigned short kills;       /* 6 bit player numer   */
    /* 10 bit kills*100     */
    unsigned short mkills[32];
    /* NOTE: this must be identical to MAXPLAYER in Vanilla server */
};

struct phaser_s_spacket
{
    char type;                  /* SP_S_PHASER */
    char status;                /* PH_HIT, etc... */
    unsigned char pnum;         /* both bytes are used for
                                 * more */
    unsigned char target;       /* look into the code   */
    short x;                    /* x coord /40 */
    short y;                    /* y coord /40 */
    unsigned char dir;
    char pad1;
    char pad2;
    char pad3;
};

struct stats_s_spacket
{
    char type;                  /* SP_S_STATS */
    char pnum;
    unsigned short tplanets;    /* Tournament planets */
    unsigned short tkills;      /* Tournament kills */
    unsigned short tlosses;     /* Tournament losses */
    unsigned short kills;       /* overall */
    unsigned short losses;      /* overall */
    unsigned int tticks;        /* ticks of tournament play
                                 * time */
    unsigned int tarmies;       /* Tournament armies */
    unsigned int maxkills;
    unsigned short sbkills;     /* Starbase kills */
    unsigned short sblosses;    /* Starbase losses */
    unsigned short armies;      /* non-tourn armies */
    unsigned short planets;     /* non-tourn planets */
    unsigned int sbmaxkills;    /* max kills as sb * 100 */
};

#endif

/* Shapes of thingys.  It would be best to add to the end of this list and
   try to coordinate your additions with other hackers. */
enum thingy_types {
    SHP_BLANK, SHP_MISSILE, SHP_BOOM, SHP_TORP, SHP_PLASMA, SHP_MINE,
    SHP_PBOOM, SHP_FIGHTER, SHP_WARP_BEACON, SHP_FBOOM, SHP_DBOOM
};

struct thingy_info_spacket {
    char    type;		/* SP_THINGY_INFO */
    char    war;
    unsigned short   shape;		/* a thingy_types */
    unsigned short   tnum;
    unsigned short   owner;
};

struct thingy_spacket {
    char    type;		/* SP_THINGY */
    unsigned char    dir;
    unsigned short   tnum;
    LONG    x, y;
};

/* terrain info for Paradise terrain */
/* 5/16/95 rpg */

struct terrain_info_packet2 {
    unsigned char   type;		/* SP_TERRAIN_INFO2 */
    unsigned char   pad;
    unsigned short  pad2;
    unsigned short  xdim;
    unsigned short  ydim;
};

struct terrain_packet2 {
    unsigned char   type;		/* SP_TERRAIN2 */
    unsigned char   sequence;
    unsigned char   total_pkts;
    unsigned char   length;
    unsigned char   terrain_type[128];	/* Ugh... this needs to be fixed 5/16/95 rpg */
 /* unsigned short  terrain_alt1[128]; */
 /* unsigned short  terrain_alt2[128]; */
};

struct scan_spacket {		/* ATM */
    char    type;		/* SP_SCAN */
    char    pnum;
    char    success;
    char    pad1;
    LONG    p_fuel;
    LONG    p_armies;
    LONG    p_shield;
    LONG    p_damage;
    LONG    p_etemp;
    LONG    p_wtemp;
};

struct motd_pic_spacket {
    char    type;		/* SP_MOTD_PIC */
    char    pad1;
    unsigned short   x, y, page;
    unsigned short   width, height;
    char    bits[1016];
};

 /* This is used to send paradise style stats */
struct stats_spacket2 {
    char    type;		/* SP_STATS2 */
    char    pnum;
    char    pad1;
    char    pad2;

    LONG   genocides;		/* number of genocides participated in */
    LONG   maxkills;		/* max kills ever * 100  */
    LONG   di;			/* destruction inflicted for all time * 100 */
    LONG   kills;		/* Kills in tournament play */
    LONG   losses;		/* Losses in tournament play */
    LONG   armsbomb;		/* Tournament armies bombed */
    LONG   resbomb;		/* resources bombed off */
    LONG   dooshes;		/* armies killed while being carried */
    LONG   planets;		/* Tournament planets conquered */
    LONG   tticks;		/* Tournament ticks */
    /* SB/WB/JS stats are entirely separate */
    LONG   sbkills;		/* Kills as starbase */
    LONG   sblosses;		/* Losses as starbase */
    LONG   sbticks;		/* Time as starbase */
    LONG   sbmaxkills;		/* Max kills as starbase * 100 */
    LONG   wbkills;		/* Kills as warbase */
    LONG   wblosses;		/* Losses as warbase */
    LONG   wbticks;		/* Time as warbase */
    LONG   wbmaxkills;		/* Max kills as warbase * 100 */
    LONG   jsplanets;		/* planets assisted with in JS */
    LONG   jsticks;		/* ticks played as a JS */
    LONG   rank;		/* Ranking of the player */
    LONG   royal;		/* royaly, specialty, rank */
};

 /* status info for paradise stats */
struct status_spacket2 {
    char    type;		/* SP_STATUS2 */
    char    tourn;
    char    pad1;
    char    pad2;
    unsigned int  dooshes;		/* total number of armies dooshed */
    unsigned int  armsbomb;		/* all t-mode armies bombed */
    unsigned int  resbomb;		/* resources bombed */
    unsigned int  planets;		/* all t-mode planets taken */
    unsigned int  kills;		/* all t-mode kills made */
    unsigned int  losses;		/* all t-mode losses */
    unsigned int  sbkills;		/* total kills in SB's */
    unsigned int  sblosses;		/* total losses in Sb's */
    unsigned int  sbtime;		/* total time in SB's */
    unsigned int  wbkills;		/* kills in warbases */
    unsigned int  wblosses;		/* losses in warbases */
    unsigned int  wbtime;		/* total time played in wb's */
    unsigned int  jsplanets;		/* total planets taken by jump ships */
    unsigned int  jstime;		/* total time in a jump ship */
    unsigned int  time;		/* t mode time in this game */
    unsigned int  timeprod;		/* t-mode ship ticks--sort of like */
};


 /* planet info for a paradise planet */
struct planet_spacket2 {
    char    type;		/* SP_PLANET2 */
    char    pnum;		/* planet number */
    char    owner;		/* owner of the planet */
    char    info;		/* who has touched planet */
    LONG   flags;		/* planet's flags */
    LONG   timestamp;		/* timestamp for info on planet */
    LONG   armies;		/* armies on the planet */
};

struct obvious_packet {
    char    type;		/* SP_NEW_MOTD */
    char    pad1;		/* CP_ASK_MOTD */
};

struct paradiseext1_spacket {
    char   type;
    char   subtype;
    short   pad;
};

struct pe1_missing_bitmap_spacket {
    char    type;
    char   subtype;

    short   page;

    short   x, y;
    short   width, height;
};

struct pe1_num_missiles_spacket {
    char    type;		/* SP_PARADISE_EXT1 */
    char   subtype;		/* SP_PE1_NUM_MISSILES */

    short   num;		/* number of missiles */
};

struct scan_cpacket {		/* ATM */
    char    type;		/* CP_SCAN */
    char    pnum;
    char    pad1;
    char    pad2;
};

struct gameparam_spacket {
    char   type;
    char   subtype;		/* this packet is not real */
    /* generic game parameter packet */
    short pad;
};

struct gp_sizes_spacket {
    char    type;
    char    subtype;		/* =0 */

    char   nplayers;
    char   nteams;		/* max of 8 */

    char   nshiptypes;
    char   nranks;		/* number of ranks */
    char   nroyal;		/* number of royalties */
    char   nphasers;

    char   ntorps;
    char   nplasmas;
    char   nthingies;		/* per-player */
    char   gthingies;		/* auxiliary thingies */

    unsigned int  gwidth;		/* galaxy width */
    /* 16 bytes */
    unsigned int  flags;		/* some game parameter flags */
#define	GP_WRAPVERT	(1<<0)
#define	GP_WRAPHORIZ	(1<<1)
#define GP_WRAPALL	(GP_WRAPVERT|GP_WRAPHORIZ)

    /*
       The following bytes are room for growth. The size of this packet is
       unimportant because it only gets sent once.  hopefully we've got
       plenty of room.
    */
    int   ext1;		/* maybe more flags? */
    int   ext2;
    int   ext3;
    /* 32 bytes */

    int   ext4;
    int   ext5;
    int   ext6;
    int   ext7;

    int   ext8;
    int   ext9;
    int   ext10;
    int   ext11;		/* 16 ints, 64 bytes */
};

struct gp_team_spacket {
    char    type;
    char    subtype;		/* =1 */

    char   index;		/* team index */
    char    letter;		/* team letter 'F' */

    char    shortname[3];	/* non-null-terminated 3-letter abbrev 'FED' */
    char   pad;
    /* 8 bytes */
    char    teamname[56];	/* padding to 64 byte packet */
};

struct gp_teamlogo_spacket {
    /*
       This packet contains several adjacent rows of a team's logo bitmap
       Data is in raw XBM format (scanline-padded to 8 bits). Maximum bitmap
       size is 99x99, which takes 1287 (99x13) bytes.
    */
    char    type;
    char    subtype;		/* =2 */

    char    logowidth;		/* <= 99 */
    char    logoheight;		/* <= 99 */

    char    y;			/* y coord of the start of this packets info */
    char    thisheight;		/* the height of this packet's info */
    char    teamindex;		/* which team's logo this is */

    char    data[768 - 7];	/* pad packet to 768 bytes. */
};

struct gp_shipshape_spacket {
    char    type;
    char    subtype;		/* =3 */

    char   shipno;
    char    race;		/* -1 is independent */
    char   nviews;		/* typically 16 */

    char   width, height;
    char   pad1;
};

struct gp_shipbitmap_spacket {
    char    type;
    char    subtype;		/* =4 */

    char   shipno;
    char    race;		/* -1 is independent */
    char   thisview;		/* 0..nviews-1 */

    char   bitmapdata[999];
};

struct gp_rank_spacket {
    char    type;
    char    subtype;		/* =5 */

    char    rankn;		/* rank number */

    char    name[-3 + 64 - 20];	/* name of the rank */

    int   genocides;
    int   milliDI;		/* DI*1000 */
    int   millibattle;	/* battle*1000 */
    int   millistrat;		/* strategy*1000 */
    int   millispec;		/* special ships*1000 */
};

struct gp_royal_spacket {
    char    type;
    char    subtype;		/* =6 */

    char   rankn;		/* rank number */

    char    name[-3 + 64];	/* name of the rank */
};

struct gp_teamplanet_spacket {
    char    type;
    char    subtype;		/* =7 */

    char    teamn;		/* 0..7 */
    char   pad1;

    int   ext1;		/* extensions? */

    /*
       Bitmaps of the team logo and masks.  The bitmap of the planet will be
       constructed with (mask ? logo : planet), applied bitwise. This
       calculation is equivalent to (logo&mask)|(planet&~mask)
    */

    /* bitmap 30x30, X bitmap format (scanline padded to 8 bits) */
    char   tactical[120];
    char   tacticalM[120];

    /* bitmap 16x16, X bitmap format (scanline padded to 8 bits) */
    char   galactic[32];
    char   galacticM[32];
};


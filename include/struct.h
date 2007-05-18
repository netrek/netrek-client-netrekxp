
/* struct.h for the client of an xtrek socket protocol.
 * 
 * Most of the unneeded stuff in the structures has been thrown away. */
#include "copyright.h"

#ifndef _h_struct
#define _h_struct

/* Background Stars Definitions */
struct _star {
    int s_x, s_y;
    int s_color;
};

struct distress
{
    unsigned char sender;
    unsigned char dam, shld, arms, wtmp, etmp, fuelp, sts;
    unsigned char wtmpflag, etempflag, cloakflag, distype, macroflag;
    unsigned char close_pl, close_en, tclose_pl, tclose_en, pre_app, i;
    unsigned char close_j, close_fr, tclose_j, tclose_fr;
    unsigned char cclist[6];    /* allow us some day to cc a
                                 * message up to 5 people */
    /* sending this to the server allows the server to do the cc action */
    /* otherwise it would have to be the client ... less BW this way */
    char preappend[80];         /* text which we pre or
                                 * append */
};

struct dmacro_list
{
    unsigned char c;
    char *name;
    char *macro;
};

struct status
{
    unsigned char tourn;        /* Tournament mode? */
    /* These stats only updated during tournament mode */
    unsigned int armsbomb, planets, kills, losses, time;
    /* Use LONG for this, so it never wraps */
    unsigned LONG timeprod;
};

#ifdef PARADISE
struct thingy {
    int     t_no;
    int     t_shape;		/* State information */
    int     t_owner;
    int     t_x;
    int     t_y;
    unsigned char t_dir;	/* direction */
    int     t_speed;		/* Moving speed */
    int     t_fuse;		/* Life left in current state */
    char    t_war;		/* enemies */
};

struct status2 {		/* paradise status struct */
    int     active;		/* for interfacing with people who */
    unsigned int wait, count;	/* want to get into the game */
    unsigned int number, request, answer;
    unsigned char tourn;	/* Tournament mode? */
    unsigned long dooshes;	/* total number of armies dooshed */
    unsigned long armsbomb;	/* all t-mode armies bombed */
    unsigned long resbomb;	/* resources bombed */
    unsigned long planets;	/* all t-mode planets taken */
    unsigned long kills;	/* all t-mode kills made */
    unsigned long losses;	/* all t-mode losses */
    unsigned long genocides;	/* number of genocides */
    unsigned long sbkills;	/* total kills in SB's */
    unsigned long sblosses;	/* total losses in Sb's */
    unsigned long sbtime;	/* total time in SB's */
    unsigned long wbkills;	/* kills in warbases */
    unsigned long wblosses;	/* losses in warbases */
    unsigned long wbtime;	/* total time played in wb's */
    unsigned long jsplanets;	/* total planets taken by jump ships */
    unsigned long jstime;	/* total time in a jump ship */
    unsigned long time;		/* t-mode time */
    unsigned long timeprod;	/* t-mode ship ticks--sort of like */
    /* manhours in t-mode */
    int     gameup;		/* is game up */
    unsigned long clock;	/* clock for planet info timestamp */
};

struct teaminfo_s {
    char    name[32];		/* this is not meant to limit the length of
				   team names */
    W_Icon  shield_logo;	/* logo that appears in the team choice
				   window */
    char    letter;		/* 1-letter abbreviation */
    char    shortname[4];	/* 3-letter abbreviation */
};

/* MOTD structures */
struct piclist {
    int     page;
    W_Icon  thepic;
    int     x, y;
    int     width, height;
    struct piclist *next;
};
struct page {
    struct list *text;
    struct page *next;
    struct page *prev;
    int     first;
    int     page;
};

#endif

enum dist_type
{
    /* help me do series */
    take = 1, ogg, bomb, space_control,
    save_planet,
    base_ogg,
    help1, help2,

    /* doing series */
    escorting, ogging, bombing, controlling,
    asw,
    asbomb,
    doing1, doing2,

    /* other info series */
    free_beer,                  /* ie. player x is totally
                                 * hosed now */
    no_gas,                     /* ie. player x has no gas */
    crippled,                   /* ie. player x is way hurt
                                 * but may have gas */
    pickup,                     /* player x picked up armies */
    pop,                        /* there was a pop somewhere */
    carrying,                   /* I am carrying */
    other1, other2,

    /* just a generic distress call */
    help
#ifdef RCM
        , rcm
#endif
};

/* The General distress has format:
 * 
 * byte1: 00yzzzzz where zzzzz is dist_type, and y is 1 if this is a more
 * complicated macro and not just a simple distress (a simple distress will
 * ONLY send ship info like shields, armies, status, location, etc.). I guess
 * y=1 can be for !    future expansion.
 * 
 * byte2: 1fff ffff - f = percentage fuel remaining (0-100) byte3: 1ddd dddd - %
 * damage byte4: 1sss ssss - % shields remaining byte5: 1eee eeee - % etemp
 * byte6: 1www wwww - % wtemp byte7: 100a aaaa - armies carried byte8: (lsb
 * of me->p_status) & 0x80 byte9: 1ppp pppp - planet closest to me byte10:
 * 1eee eeee - enemy closest to me byte11: 1ppp pppp - planet closest to
 * target byte12: 1eee eeee - enemy closest to target byte13: 1ttt tttt -
 * tclose_j byte14: 1jjj jjjj - close_j byte15: 1fff ffff - tclose_fr byte16:
 * 1ccc cccc - close_fr byte17+: cc list (each player to cc this message to
 * is 11pp ppp) cc list is terminated by 0x80 (pre-pend) or 0100 0000
 * (append) ) byte18++: the text to pre or append .. depending on termination
 * above. text is null terminated and the last thing in this distress */

#define PFREE 0
#define POUTFIT 1
#define PALIVE 2
#define PEXPLODE 3
#define PDEAD 4
#define POBSERV 5

#define PFSHIELD	0x0001
#define PFREPAIR	0x0002
#define PFBOMB		0x0004
#define PFORBIT		0x0008
#define PFCLOAK		0x0010
#define PFWEP		0x0020
#define PFENG		0x0040
#define PFROBOT		0x0080
#define PFBEAMUP	0x0100
#define PFBEAMDOWN	0x0200
#define PFSELFDEST	0x0400
#define PFGREEN		0x0800
#define PFYELLOW	0x1000
#define PFRED		0x2000
#define PFPLOCK		0x4000  /* Locked on a player */
#define PFPLLOCK	0x8000  /* Locked on a planet */
#define PFCOPILOT	0x10000 /* Allow copilots */
#define PFWAR		0x20000 /* computer reprogramming
                                 * for war */
#define PFPRACTR	0x40000 /* practice type robot (no
                                 * kills) */
#define PFDOCK      0x80000 /* true if docked to a
                                 * starbase */
#define PFREFIT     0x100000        /* true if about to refit */
#define PFREFITTING	0x200000        /* true if currently
                                         * refitting */
#define PFTRACT  	0x400000        /* tractor beam activated */
#define PFPRESS  	0x800000        /* pressor beam activated */
#define PFDOCKOK	0x1000000       /* docking permission */
#define PFSEEN      0x2000000       /* seen by enemy on galactic map? */
#ifdef PARADISE
/* Note overlap with PFWARP/PFOBSERV and PFSNAKE/PFTWARP */
#define PFWARPPREP	  (1<<26)	/* in warp prep [BDyess] */
#define PFWARP		  (1<<27)	/* ship warping */
#define PFAFTER		  (1<<28)	/* after burners on */
#define PFWPSUSPENDED     (1<<29)	/* warp prep suspended [BDyess] */
#define PFSNAKE	          (1<<30)	/* it's a space snake */
#define PFBIRD	          (1<<31)	/* it's a space bird */
#define PFOBSERV          (1<<31)       /* set to something unused */
#define PFTWARP           (1<<31)       /* set to something unused */
#else
#define PFOBSERV	0x8000000       /* for observers */
#define PFTWARP     0x40000000      /* transwarping to base */
#endif

#define KLOGIN      0x00    /* initial state */
#define KQUIT		0x01    /* Player quit */
#define KTORP		0x02    /* killed by torp */
#define KPHASER		0x03    /* killed by phaser */
#define KPLANET		0x04    /* killed by planet */
#define KSHIP		0x05    /* killed by other ship */
#define KDAEMON		0x06    /* killed by dying daemon */
#define KWINNER		0x07    /* killed by a winner */
#define KGHOST		0x08    /* killed because a ghost */
#define KGENOCIDE	0x09    /* killed by genocide */
#define KPROVIDENCE	0x0a    /* killed by a hacker */
#define KPLASMA     0x0b    /* killed by a plasma torpedo */
#define TOURNEND	0x0c    /* tournament game ended */
#define KOVER		0x0d    /* game over  */
#define TOURNSTART	0x0e    /* tournament game starting */
#define KBADBIN		0x0f    /* bad binary */
#ifdef PARADISE
#define KMISSILE      0x10    /* missile, note the overlap with KTORP2! */
#define KASTEROID     0x11    /* asteroid, note the overlap with KSHIP2! */
#else
#define KTORP2      0x10    /* killed by detted torps */
#define KSHIP2      0x11    /* chain-reaction explosions */
#endif
#define KPLASMA2    0x12    /* killed by zapped plasma */

#ifdef PARADISE
#define NUM_TYPES 15
#define NUM_PSHIP_TYPES 7
#define PARADISE_SHIP_OFFSET 7	/* To make jumpship first entry in the paradise ship bitmap array */
#define SCOUT 0
#define DESTROYER 1
#define CRUISER 2
#define BATTLESHIP 3
#define ASSAULT 4
#define STARBASE 5
#define ATT 6
#define JUMPSHIP 7
#define SGALAXY 8	/* Not sure where to put this .. */
#define FLAGSHIP 8
#define WARBASE 9
#define LIGHTCRUISER 10
#define CARRIER 11
#define UTILITY 12
#define PATROL 13
#define PUCK 14
#else
#define NUM_TYPES 8
#define SCOUT 0
#define DESTROYER 1
#define CRUISER 2
#define BATTLESHIP 3
#define ASSAULT 4
#define STARBASE 5
#define SGALAXY	6
#define ATT	7
#endif

struct ship
{
    short s_phaserdamage;
    int s_maxspeed;
    int s_maxfuel;
    int s_maxshield;
    int s_maxdamage;
    int s_maxegntemp;
    int s_maxwpntemp;
    short s_maxarmies;
    short s_width;
    short s_height;
    short s_type;
    int s_torpspeed;
    int s_phaserfuse;
    int s_repair;
};

struct stats
{
    double st_maxkills;         /* max kills ever */
    int st_kills;               /* how many kills */
    int st_losses;              /* times killed */
    int st_armsbomb;            /* armies bombed */
    int st_planets;             /* planets conquered */
    int st_ticks;               /* Ticks I've been in game */
    int st_tkills;              /* Kills in tournament play */
    int st_tlosses;             /* Losses in tournament play */
    int st_tarmsbomb;           /* Tournament armies bombed */
    int st_tplanets;            /* Tournament planets
                                 * conquered */
    int st_tticks;              /* Tournament ticks */
    /* SB stats are entirely separate */
    int st_sbkills;             /* Kills as starbase */
    int st_sblosses;            /* Losses as starbase */
    int st_sbticks;             /* Time as starbase */
    double st_sbmaxkills;       /* Max kills as starbase */
    LONG st_lastlogin;          /* Last time this player was
                                 * played */
    int st_flags;               /* Misc option flags */

#ifdef MOUSE_AS_SHIFT
    unsigned char st_keymap[672];       /* keymap for this player */
#else
    unsigned char st_keymap[96];        /* keymap for this player */
#endif
    int st_rank;                /* Ranking of the player */
};

#ifdef PARADISE
struct stats2 {			/* paradise stats */
    int     st_genocides;	/* number of genocides participated in */
    float   st_tmaxkills;	/* max kills ever */
    float   st_di;		/* total destruction inflicted for all time */
    int     st_tkills;		/* Kills in tournament play */
    int     st_tlosses;		/* Losses in tournament play */
    int     st_tarmsbomb;	/* Tournament armies bombed */
    int     st_tresbomb;	/* resources bombed off */
    int     st_tdooshes;	/* armies killed while being carried */
    int     st_tplanets;	/* Tournament planets conquered */
    int     st_tticks;		/* Tournament ticks */
    /* SB/WB/JS stats are entirely separate */
    int     st_sbkills;		/* Kills as starbase */
    int     st_sblosses;	/* Losses as starbase */
    int     st_sbticks;		/* Time as starbase */
    float   st_sbmaxkills;	/* Max kills as starbase */
    int     st_wbkills;		/* Kills as warbase */
    int     st_wblosses;	/* Losses as warbase */
    int     st_wbticks;		/* Time as warbase */
    float   st_wbmaxkills;	/* Max kills as warbase */
    int     st_jsplanets;	/* planets assisted with in JS */
    int     st_jsticks;		/* ticks played as a JS */
    long    st_lastlogin;	/* Last time this player was played */
    int     st_flags;		/* Misc option flags */
    unsigned char st_keymap[256];	/* keymap for this player */
    int     st_rank;		/* Ranking of the player */
    int     st_royal;		/* royaly, specialty, rank */
};
#endif

#define ST_MAPMODE      1
#define ST_NAMEMODE     2
#define ST_SHOWSHIELDS  4
#define ST_KEEPPEACE    8
#define ST_SHOWLOCAL    16      /* two bits for these two */
#define ST_SHOWGLOBAL   64

struct player
{
    int p_no;
    int p_updates;              /* Number of updates ship
                                 * has survived */
    int p_status;               /* Player status */
    unsigned int p_flags;       /* Player flags */
    char p_name[16];
    char p_login[16];
    char p_monitor[16];         /* Monitor being played on */
    char p_mapchars[2];         /* Cache for map window
                                 * image */
    struct ship p_ship;         /* Personal ship statistics */
    int p_x;
    int p_y;
    unsigned char p_dir;        /* Real direction */
    unsigned char p_desdir;     /* desired direction */
    int p_subdir;               /* fraction direction change */
    int p_speed;                /* Real speed */
    short p_desspeed;           /* Desired speed */
    int p_subspeed;             /* Fractional speed */
    short p_team;               /* Team I'm on */
    int p_damage;               /* Current damage */
    int p_subdamage;            /* Fractional damage repair */
    int p_shield;               /* Current shield power */
    int p_subshield;            /* Fractional shield
                                 * recharge */
    short p_cloakphase;         /* Drawing stage of cloaking
                                 * engage/disengage. */
    short p_ntorp;              /* Number of torps flying */
    short p_nplasmatorp;        /* Number of plasma torps
                                 * active */
    char p_hostile;             /* Who my torps will hurt */
    char p_swar;                /* Who am I at sticky war
                                 * with */
    float p_kills;              /* Enemies killed */
    short p_planet;             /* Planet orbiting or locked
                                 * onto, set only client */
    short pl_orbit;             /* Planet orbiting, sent by server */
    short p_playerl;            /* Player locked onto */

#ifdef ARMY_SLIDER
    int p_armies;               /* XXX: for stats */
#else
    short p_armies;
#endif                          /* ARMY_SLIDER */
    int p_fuel;
    short p_explode;            /* Keeps track of final
                                 * explosion */
    int p_etemp;
    short p_etime;
    int p_wtemp;
    short p_wtime;
    short p_whydead;            /* Tells you why you died */
    short p_whodead;            /* Tells you who killed you */
    struct stats p_stats;       /* player statistics */
#ifdef PARADISE
    short p_ndrone;		/* Number of drones .. why was this missing? */
    short p_totmissiles;	/* number of total missiles [Bill Dyess] */
    struct stats2 p_stats2;     /* Paradise stats */
#endif
    short p_genoplanets;        /* planets taken since last
                                 * genocide */
    short p_genoarmsbomb;       /* armies bombed since last
                                 * genocide */
    short p_planets;            /* planets taken this game */
    short p_armsbomb;           /* armies bombed this game */
    int p_ghostbuster;
    int p_docked;               /* If starbase, # docked to,
                                 * else pno base host */
    int p_port[4];              /* If starbase, pno of ship
                                 * docked to that port, else
                                 * p_port[0] = port # docked
                                 * to on host.   */
    short p_tractor;            /* What player is in tractor
                                 * lock */
    int p_pos;                  /* My position in the player
                                 * file */
    short p_repair_time;        /* estimated time in seconds to a full repair */
};

struct statentry
{
    char name[16], password[16];
    struct stats stats;
};

/* Torpedo states */

#define TFREE 0
#define TMOVE 1
#define TEXPLODE 2
#define TDET 3
#define TOFF 4
#define TSTRAIGHT 5             /* Non-wobbling torp */


struct torp
{
    unsigned char t_status;     /* State information */
    short t_owner;
    char t_war;                 /* enemies */
    int t_x;
    int t_y;
    short t_fuse;               /* Life left in current state */
    unsigned char t_updateFuse; /* Updates before torp will expire */
    unsigned char t_dir;        /* direction */
    int t_clear;                /* Flag to clear exploding torps */
};


/* Plasma Torpedo states */

#define PTFREE 0
#define PTMOVE 1
#define PTEXPLODE 2
#define PTDET 3

struct plasmatorp
{
    unsigned char pt_status;    /* State information */
    char pt_war;                /* enemies */
    short pt_owner;
    short pt_fuse;              /* Life left in current
                                 * state */
    short pt_updateFuse;        /* Time till expiry */
    int pt_x;
    int pt_y;
    int pt_clear;               /* Flag to clear exploding plasmas */
};

#define PHFREE 0x00
#define PHHIT  0x01             /* When it hits a person */
#define PHMISS 0x02
#define PHHIT2 0x04             /* When it hits a photon */

struct phaser
{
    unsigned char ph_status;    /* What it's up to */
    unsigned char ph_dir;       /* direction */
    short ph_target;            /* Who's being hit (for
                                 * drawing) */
    short ph_updateFuse;        /* Time till expiry */
    short ph_fuse;              /* Life left for drawing */
    int ph_x, ph_y;             /* For when it hits a torp */
    int ph_maxfuse;             /* maxfuse normalized for updates per sec */
#ifdef SOUND
    int sound_phaser;		/* If phaser sound should be played or not */
#endif
};


#ifdef RSA
struct rsa_key
{
    unsigned char client_type[KEY_SIZE];
    unsigned char architecture[KEY_SIZE];
    unsigned char global[KEY_SIZE];
    unsigned char public[KEY_SIZE];
};

#endif

/* An important note concerning planets:  The game assumes that the planets
 * are in a 'known' order.  Ten planets per team, the first being the home
 * planet. */

#ifdef PARADISE
/*
   pl_flags is an int of 32 bits:

   bits 16 and 23 currently define the type of the planet.  The
   interpretation of the other bits is dependent upon the planet
   type.

   Here is the interpretation for a planet
   bits 0..3			unknown
   bits 4..6			planetary facilities (REPAIR,FUEL,AGRI)
   bit  7			redraw (archaic, recyclable?)
   bits 8..11			old flags (archaic, recyclable?)
   bits 12..15			paradise planetary facilities
				(REPAIR,FUEL,AGRI,SHIPY)
   bit  16			cosmic object type (also bit 23)
   bits 17,18			planet atmosphere type
   bits 19..21			planetary surface properties
				(DILYTH,METAL,ARABLE)
   bit  22			paradise planet flag (why?)
   bits 23,24			cosmic object type (also bit 16)
   bits 25..31	currently unallocated (7 bits to play with)

   Asteroids are NYI but here is a draft standard:
   bits 12,15			facilities
				(REPAIR,FUEL,SHIPY)
   bit  20			surface properties
				(DILYTH,METAL)
   other bits	currently unallocated

   */

/* facilities, bits 4..6 and 12..15
   valid for planets and asteroids */
#define PLREPAIR   ((1<<12) | (1<<4))	/* planet can repair ships */
#define PLFUEL     ((1<<13) | (1<<5))	/* planet has fuel depot */
#define PLAGRI     ((1<<14) | (1<<6))	/* agricultural thingies built here */
#define PLSHIPYARD ((1<<15))	/* planet has a shipyard on it */
#define PLORESMASK (0x7<<4)	/* mask for original resource flags */
#define PLRESSHIFT       12	/* bit to shift right by for resources */
#define PLRESMASK  (0xF<<PLRESSHIFT)	/* to mask off all but resource bits */

#define PLREDRAW   (1<<7)	/* Player close for redraw */

#define PLHOME 	   (1<< 8)	/* These 4 flags no longer are */
#define PLCOUP     (1<< 9)	/* used in the server */
#define PLCHEAP    (1<<10)
#define PLCORE     (1<<11)

/* cosmic object types, bits 16 and 23, and 24 */
#define PLPLANET	0	/* object is a planet */
#define PLSTAR     (1<<16)	/* object is a star */
#define PLAST	   (1<<23)	/* object is an asteroid NYI */
#define PLNEB	   ((1<<16)|(1<<23))	/* object is a nebula NYI */
#define PLBHOLE    (1<<24)	/* object is a black hole NYI */
#define PLPULSAR   ((1<<16)|(1<<24))	/* object is a pulsar NYI */
#define PLUK1      ((1<<23)|(1<<24))	/* future expansion */
#define PLWHOLE    ((1<<16)|(1<<23)|(1<<24))	/* object is a wormhole */
#define PLTYPEMASK ((1<<16)|(1<<23)|(1<<24))	/* mask to extract object
						   type */
#define PL_TYPE(p) ( (p).pl_flags & PLTYPEMASK )

/* Atmosphere Types, bits 17 and 18.
   Valid for planets.
   */
#define PLATSHIFT	 17	/* number of right bit shifts for atmos bits */
#define PLPOISON   (0<<PLATSHIFT)	/* poison atmosphere, no army growth */
#define PLATYPE3   (1<<PLATSHIFT)	/* slightly toxic, very slow army
					   growth */
#define PLATYPE2   (2<<PLATSHIFT)	/* thin atmosphere, slow army growth */
#define PLATYPE1   (3<<PLATSHIFT)	/* normal human atmosphere, normal
					   growth */
#define PLATMASK   (0x3<<PLATSHIFT)	/* to mask off everything but atmos
					   bits */

/* Surface Properties, bits 19..21
   Valid for planets and asteroids.
   */
#define PLBARREN   0		/* rocky barren surface */
#define PLSURSHIFT 	 19	/* number of bit shift to surface */
#define PLDILYTH   (1<<(PLSURSHIFT+0))	/* dilythium deposits on the planet */
#define PLMETAL    (1<<(PLSURSHIFT+1))	/* metal deposits on the planet */
#define PLARABLE   (1<<(PLSURSHIFT+2))	/* planet has farmland */
#define PLSURMASK  (0x7<<PLSURSHIFT)	/* number of surface combinations */


#define PLPARADISE (1<<22)	/* Paradise server flag set to 1 for P server */
#else
/* the lower bits represent the original owning team */
#define PLREPAIR 0x010
#define PLFUEL 0x020
#define PLAGRI 0x040
#define PLREDRAW 0x080          /* Player close for redraw */
#define PLHOME 0x100            /* home planet for a given
                                 * team */
#define PLCOUP 0x200            /* Coup has occured */
#define PLCHEAP 0x400           /* Planet was taken from
                                 * undefended team */
#define PLCORE 0x800		/* A core world planet */

#endif /* PARADISE */

struct planet
{
    int pl_no;
    int pl_flags;               /* State information */
    int pl_owner;
    int pl_x;
    int pl_y;
    char pl_name[16];
    int pl_namelen;             /* Cuts back on strlen's */
    int pl_armies;
    int pl_info;                /* Teams which have info on
                                 * planets */
    int pl_deadtime;            /* Time before planet will
                                 * support life */
    int pl_couptime;            /* Time before coup may take
                                 * place */
#ifdef PARADISE
    int pl_timestamp;           /* time the info was taken */
#endif
};

#define MVALID 0x01
#define MGOD   0x10
#define MMOO   0x12

#ifdef TOOLS
#define MTOOLS 0x14
#endif

/* order flags by importance (0x100 - 0x400) */
/* restructuring of message flags to squeeze them all into 1 byte - jmn */
/* hopefully quasi-back-compatible: MVALID, MINDIV, MTEAM, MALL, MGOD use up
 * 5 bits. this leaves us 3 bits. since the server only checks for those
 * flags when deciding message related things and since each of the above
 * cases only has 1 flag on at a time we can overlap the meanings of the
 * flags */

#define MINDIV 0x02
/* these go with MINDIV flag */

#ifdef STDBG
#define MDBG   0x20
#endif

#define MCONFIG 0x40            /* config messages from
                                 * server */
#define MDIST 0x60              /* flag distress type
                                 * messages properly */

#ifdef MULTILINE_MACROS
#define MMACRO 0x80
#endif

#define MTEAM  0x04
/* these go with MTEAM flag */
#define MTAKE  0x20
#define MDEST  0x40
#define MBOMB  0x60
#define MCOUP1 0x80
#define MCOUP2 0xA0
#define MDISTR 0xC0             /* flag distress type
                                 * messages */

#define MALL   0x08
/* these go with MALL flag */
#define MGENO  0x20             /* MGENO is not used in INL
                                 * server but beLONGs here */
#define MCONQ  0x20             /* not enought bits to
                                 * distinguish MCONQ/MGENO
                                 * :-( */
#define MKILLA 0x40
#define MKILLP 0x60
#define MKILL  0x80
#define MLEAVE 0xA0
#define MJOIN  0xC0
#define MGHOST 0xE0
/* MMASK not used in INL server */

#define MWHOMSK  0x1f           /* mask with this to find
                                 * who msg to */
#define MWHATMSK 0xe0           /* mask with this to find
                                 * what message about */

/* old flags... #define MVALID 0x01 #define MINDIV 0x02 #define MTEAM  0x04
 * #define MALL   0x08 #define MGOD   0x10
 * 
 * #define MGENO  0x100            order these by importance (0x100 - 0x400)
 * #define MCONQ  0x110 #define MTAKE  0x120 #define MDEST  0x130 #define
 * MKILLA 0x200 #define MBOMB  0x210 #define MKILLP 0x220 #define MKILL 0x230
 * #define MLEAVE 0x300 #define MJOIN  0x310 #define MGHOST 0x320 #define
 * MCOUP1 0x330 #define MCOUP2 0x340    end of old flags  */



struct message
{
    int m_no;
    int m_flags;
    int m_time;
    int m_recpt;
    char m_data[80];
};

/* message control structure */

struct mctl
{
    int mc_current;
};

/* This is a structure used for objects returned by mouse pointing */

#define PLANETTYPE 0x1
#define PLAYERTYPE 0x2

struct obtype
{
    int o_type;
    int o_num;
};

struct rank
{
    float hours, ratings, defense;
    char *name, *cname;
};

struct memory
{
    struct player players[MAXPLAYER];
    struct torp torps[MAXPLAYER * MAXTORP];
    struct plasmatorp plasmatorps[MAXPLAYER * MAXPLASMA];
#ifdef PARADISE
    struct thingy thingies[MAXPLAYER * 20]; // Arbitrary
#endif
    struct status status[1];
    struct planet planets[MAXPLANETS];
    struct phaser phasers[MAXPLAYER];
    struct mctl mctl[1];
    struct message messages[MAXMESSAGE];
    struct ship shipvals[NUM_TYPES];
};

struct plupdate
{
    int plu_update;
    int plu_x, plu_y;
};

struct macro_list
{
    int type;
    int multi;
    unsigned char key;
    char who;
    char *string;
};

/******************************************************************************/
/***                   Distress structure definitions                       ***/
/***                                                                        ***/
/***  The LOW_DISTRESS, MID_DISTRESS, and HIGH_DISTRESS are all used to     ***/
/***  index the correct items within the DISTRESS_BLOCK's item array.  The  ***/
/***  distress block contains a minimum and maximum value for each item, a  ***/
/***  flag to indicate whether the distress on this value is active or not, ***/
/***  and three single character pointers for different severity levels.    ***/
/******************************************************************************/

#define DIST_LOW            ( 0 )
#define DIST_MID            ( 1 )
#define DIST_HIGH           ( 2 )

#define DIST_SHIELDS        ( 0 )
#define DIST_DAMAGE         ( 1 )
#define DIST_WTEMP          ( 2 )
#define DIST_ETEMP          ( 3 )
#define DIST_ARMYS          ( 4 )
#define DIST_FUEL           ( 5 )

typedef struct distress_block
{
    int min, max;
    int on;
    char *item[3];
}

DISTRESS_DESC;

struct distress_list
{                               /* need one for ships and
                                 * one for SBs */
    DISTRESS_DESC problem[6];
};

#ifdef HOCKEY_LINES
struct s_line
{
    int begin_x, begin_y;       /* Start point of the line */
    int end_x, end_y;           /* End point of the line   */
    W_Color color;              /* The color of the line   */
    int orientation;            /* Horizontal or Vertical? */
    int *flag;                  /* Should line be drawn?   */
};

#endif /* HOCKEY_LINES */

struct shipdef
{
    char *name;
    char *rcfile;
    unsigned char *keymap;
    unsigned char *buttonmap;
    unsigned char *ckeymap;
};

#ifdef TOOLS
struct key_list
{
    unsigned char dest;
    char *name;
};
#endif

/* Window Allowed Messages types */
#define WAM_NONE    0x00
#define WAM_INDIV   0x01
#define WAM_TEAM    0x02
#define WAM_KILL    0x04
#define WAM_ALL     0x08
#define WAM_PHASER  0x10

/* SDB flags for windows that use double buffering */
#define SDB_FLAG_NONE	0x00
#define SDB_FLAG_LOCAL	0x01
#define SDB_FLAG_MAP	0x02

typedef struct tagWindow
{
    HWND hwnd;
    short type;
    short border;
    W_Color BorderColor;
    short UsingCreatedCursor;
    HCURSOR cursor;
    short tiled;
    struct Icon *TileIcon;
    short NumItems;
    struct menuItem *items;
    struct stringList *strings;
    short AddedStrings;
    short TextHeight;
    short TextWidth;
    RECT ClipRect;
    W_Callback HandleKeydown;
    W_Callback HandleKeyup;
    W_Callback HandleButton;
    W_Callback HandleExpose;
    struct tagWindow *parent;
    char name[32];
    int orig_x;
    int orig_y;
    int orig_width;
    int orig_height;
    int actual_x;
    int actual_y;
    int actual_width;
    int actual_height;
    int wam;                    /* Window Allowed Messages */
    int sdb_flags;              /* SDB flags */
}
Window;

/* Options taken from rc file */
#define RC_INT  0
#define RC_BOOL 1
#define RC_STR  2

struct save_options
{
    char *name;
    int *int_value;
    int type;
    char *desc[100];
};

struct tractor
{
    int sx, sy, dx, dy, d1x, d1y, d2x, d2y;   /* source (x,y) dest (x1,y1) (x2,y2) */
    struct tractor *next;
};

struct stringlist
{
    char *string;
    char *value;
    struct stringlist *next;
};

typedef struct _sdbuffer
{
    W_Window window;    /* window for current buffer */
    HDC win_dc;         /* window device context */
    HDC mem_dc;         /* memory device context */
    RECT wr;            /* window rectangle */
    HBITMAP mem_bmp;    /* memory to handle bitmap */
    HBITMAP old_bmp;    /* saved bitmap */
}SDBUFFER;

// Linked list for console buffer
struct cons_buffer
{
    char * string;
    struct cons_buffer * next;
};

#endif /* _h_struct */

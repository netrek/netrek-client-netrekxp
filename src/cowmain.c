/******************************************************************************/
/***  File:  cowmain.c                                                      ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <pwd.h>
#include <time.h>
#include <winsock.h>

#include "copyright.h"
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "version.h"
#include "patchlevel.h"
#include "playerlist.h"
#include "parsemeta.h"
#include "map.h"
#include "proto.h"

/******************************************************************************/
/***  Local Globals                                                         ***/
/******************************************************************************/

int takeNearest = 0;
jmp_buf env;
int isFirstEntry;
char defaulttmp[100];

#define RETURNBASE 10
#ifdef PACKET_LOG
extern int log_packets;
#endif


#ifdef GATEWAY
/*-----------------------------------------------------------*/

/* IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
 * 
 * Set MYADDR and MYADDR_MASK to however much of the address is significant. For
 * example, I want to restrict the use of my clients to the 129.212 subnet
 * (all machines within Amdahl), so I set MYADDR=0x81d40000, and MYADDR_MASK
 * to be 0xffff0000.  The host's address will be ANDed with the mask and then
 * compared to MYADDR.
 * 
 * If you only want your client to be run on your host, then you'd use all eight
 * bytes. */
#define MYADDR          0x81d40000
#define MYADDR_MASK     0xffff0000

/* we want these for the client subnet validation */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

extern char *getenv ();
char *home, homedot[256];

#define DEFAULT_GATEWAY         "rebel"

unsigned LONG mkaddr ();


unsigned LONG netaddr;          /* used for blessedness
                                 * checking */
int serv_port;                  /* used for blessedness
                                 * checking */

static char *gateway = DEFAULT_GATEWAY;

#ifdef TREKHOPD
static int trekhopd_port = 6592;
int use_trekhopd = 0;
int port_req = 6592;
char *host_req = "rebel";

#endif


typedef struct
{
    char id[16];
    char inet_addr[24];
    int remote_port;
    int gw_port;
    char full_name[64];
    char comment[40];
}
SERVER_LIST;

#define MAX_SERVER      128
static SERVER_LIST servers[MAX_SERVER]; /* that ought to be enough */

#define SERVER_DIR      "/usr/local/games/"
#define SERVER_FILE     ".trekgwrc"
static int server_count = 0;

#define WSPC    " \t"

/******************************************************************************/
/***  strToNetaddr()                                                        ***/
/******************************************************************************/
unsigned LONG
strToNetaddr (char *str)
{
    SERVER_LIST *slp;
    char *t;
    unsigned LONG answer;
    int i;

    if (!server_count)
    {
        fprintf (stderr, "No server list, cannot resolve id\n");
        return (-1);
    }

    /* find the one we want */
    for (i = 0, slp = servers; i < server_count; i++, slp++)
    {
        if (!strcmp (str, slp->id))
        {
            printf ("%s is %s(%d) (%s)\n", slp->id, slp->full_name,
                    slp->remote_port, slp->comment);
            xtrekPort = slp->gw_port;
            str = slp->inet_addr;
            break;
        }
    }
    if (i == server_count)
    {
        fprintf (stderr, "Specified server not found.\n");
        return (-1);
    }

    /* now "str" is either the original string or slp->inet_addr */
    /* (this will be wrong if -H isn't last on command line) */
    answer = 0;
    t = str;
    for (i = 0; i < 4; i++)
    {
        answer = (answer << 8) | atoi (t);
        while (*t && *t != '.')
            t++;
        if (*t)
            t++;
    }

#ifdef TREKHOPD
    /* do things slightly different */
    if (slp->id == NULL)
    {
        fprintf (stderr, "ERROR: host ID '%s' unknown\n", str);
        return (-1);
    }
    xtrekPort = trekhopd_port;  /* ought to have an arg to
                                 * specify this */
    port_req = slp->remote_port;
    host_req = slp->full_name;
    printf ("Connecting to %s (%d) via trekhopd (%s %d)\n", host_req,
            port_req, serverName, xtrekPort);
#else
    printf ("Connecting to %s through %s port %d\n", str, serverName,
            xtrekPort);
#endif

    return (answer);
}


/* for trekhopd, only gw_local_port is important */
/* (should be possible to eliminate that too, but I want minimal changes) */
typedef struct
{
    int uid;
    int serv_port;
    int port;
    int local_port;
}
UDPMAP;

#define MAX_UDPMAP      32
static UDPMAP udpmap[MAX_UDPMAP];
static int map_count;

/******************************************************************************/
/***  getUdpPort()                                                          ***/
/******************************************************************************/
getUdpPort ()
{
    int i;
    int uid;
    char *gw_m, *gw_p, *gw_lp, *gw_sp, *err, *getenv ();

    /* should always be set prior, but in case not .. */
    if (!gw_mach)
    {
        gw_m = getenv ("GW_MACH");
        if (gw_m)
            gw_mach = gw_m;
        else
            gw_mach = gateway;
    }

    uid = 1;

    for (i = 0; i < map_count; i++)
    {
        if (uid == udpmap[i].uid)
        {
            gw_serv_port = udpmap[i].serv_port;
            gw_port = udpmap[i].port;
            gw_local_port = udpmap[i].local_port;
            return;
        }
    }
    printf ("unable to get ports for your uid\n");      /* debug */

    gw_p = getenv ("GW_PORT");
    gw_sp = getenv ("GW_SPORT");
    gw_lp = getenv ("GW_LPORT");

    if (gw_p)
    {
        gw_port = strtol (gw_p, &err, 10);
        if (err == gw_p)
        {
            fprintf (stderr, "netrek: malformed integer for GW_PORT: %s\n",
                     gw_p);
            /* let something else complain about port 0 */
        }
    }
    else
        gw_port = 5001;
    if (gw_sp)
    {
        gw_serv_port = strtol (gw_sp, &err, 10);
        if (err == gw_sp)
        {
            fprintf (stderr, "netrek: malformed integer for GW_SPORT: %s\n",
                     gw_sp);
            /* let something else complain about port 0 */
        }
    }
    else
        gw_serv_port = 5000;

    if (gw_lp)
    {
        gw_local_port = strtol (gw_lp, &err, 10);
        if (err == gw_lp)
        {
            fprintf (stderr, "netrek: malformed integer for GW_LPORT: %s\n",
                     gw_lp);
            /* let something else complain about port 0 */
        }
    }
    else
        gw_local_port = 5000;

    /* printf("gw_mach: \'%s\'\n", gw_mach); printf("gw_local_port: %d\n",
     * gw_local_port); printf("gw_serv_port: %d\n", gw_serv_port);
     * printf("gw_port: %d\n", gw_port); */
}

/* In the event of problems assiocated with the above include files the
 * following routine can be alternately used to convert a string
 * ("xxx.xxx.xxx.xxx") to an internet address number. */


/******************************************************************************/
/***  mkaddr()                                                              ***/
/******************************************************************************/
/* More network "correct" routine */

unsigned LONG
mkaddr (char *m)
{
    struct in_addr ad;
    struct hostent *hp;

    hp = gethostbyname (m);
    if (!hp)
    {
        ad.s_addr = inet_addr (m);
        if (ad.s_addr == -1)
        {
            fprintf (stderr, "netrek: unknown host \'%s\'\n", m);
            terminate (1);
        }
    }
    else
        MCOPY (hp->h_addr, (char *) &ad, hp->h_length);

    return ad.s_addr;
}

/******************************************************************************/
/***  read_servers()                                                        ***/
/******************************************************************************/
/* This is not very robust. */
void
read_servers ()
{
    FILE *fp;
    SERVER_LIST *slp;
    UDPMAP *ump;
    char buf[128];
    int state;
    char *cp;

    server_count = map_count = 0;

    fp = NULL;
    if (getenv ("HOME") != NULL)
    {
        STRNCPY (homedot, getenv ("HOME"), sizeof (homedot));
        strcat (homedot, "/");
        strcat (homedot, SERVER_FILE);
        fp = fopen (homedot, "r");
    }
    if (fp == NULL)
    {
        /* failed, try common one */
        STRNCPY (buf, SERVER_DIR, sizeof (SERVER_DIR));
        STRNCPY (buf, SERVER_FILE, sizeof (SERVER_FILE));
        fp = fopen (buf, "r");
    }
    if (fp == NULL)
    {
        /* failed, try current directory */
        fp = fopen (SERVER_FILE, "r");
    }
    if (fp == NULL)
    {
        /* failed, give up */
        perror ("warning: Unable to open server list");
        fprintf (stderr, "Tried to open '%s', '%s', and './%s'\n",
                 homedot, buf, SERVER_FILE);
        return;
    }

    state = 0;
    while (1)
    {
        fgets (buf, 128, fp);
        if (ferror (fp) || feof (fp))
        {
            if (ferror (fp))
                perror ("fgets");
            break;
        }

        /* skip blank lines and lines which start with '#' */
        if (*buf == '\0' || *buf == '\n' || *buf == '#')
            continue;
        buf[strlen (buf) - 1] = '\0';   /* strip the trailing '\n' */

        switch (state)
        {
        case 0:                /* "trekhopd" or "gw" */

#ifdef TREKHOPD
            use_trekhopd = 0;
            if (!strcmp (buf, "trekhopd"))
                use_trekhopd = 1;
#endif

            state++;
            break;
        case 1:                /* gateway host */
            gateway = (char *) malloc (strlen (buf) + 1);
            strcpy (gateway, buf);
            state++;
            break;
        case 2:                /* trekhopd port */
            trekhopd_port = atoi (buf);
            state++;
            break;
        case 3:                /* UDP map */
            if (!strcmp (buf, "END"))
            {
                state++;
                break;
            }
            if (map_count >= MAX_UDPMAP)
            {
                fprintf (stderr, "UDP map too large; max is %d entries\n",
                         MAX_UDPMAP);
                break;
            }
            ump = &udpmap[map_count];
            cp = strtok (buf, WSPC);    /* skip ascii uid */
            cp = strtok (NULL, WSPC);
            ump->uid = atoi (cp);
            cp = strtok (NULL, WSPC);
            ump->serv_port = atoi (cp);
            cp = strtok (NULL, WSPC);
            ump->port = atoi (cp);
            cp = strtok (NULL, WSPC);
            ump->local_port = atoi (cp);

#ifdef DEBUG
            printf ("%2d: %-8d %-8d %-8d %-8d\n", map_count,
                    ump->uid, ump->serv_port, ump->port, ump->local_port);
#endif

            map_count++;
            break;

        case 4:                /* host description */
            if (!strcmp (buf, "END"))
            {
                state++;
                break;
            }
            if (server_count >= MAX_SERVER)
            {
                fprintf (stderr, "server list too large; max is %d entries\n",
                         MAX_SERVER);
                break;
            }
            slp = &servers[server_count];
            cp = strtok (buf, WSPC);
            STRNCPY (slp->id, cp, sizeof (slp->id));
            cp = strtok (NULL, WSPC);
            STRNCPY (slp->inet_addr, cp, sizeof (slp->inet_addr));
            cp = strtok (NULL, WSPC);
            slp->remote_port = atoi (cp);
            cp = strtok (NULL, WSPC);
            slp->gw_port = atoi (cp);
            cp = strtok (NULL, WSPC);
            STRNCPY (slp->full_name, cp, sizeof (slp->full_name));
            cp = strtok (NULL, "\"\t");
            STRNCPY (slp->comment, cp, sizeof (slp->comment));

#ifdef DEBUG
            printf ("%2d: %-9s %-15s %-5d %-5d %-25s \"%s\"\n", server_count,
                    slp->id, slp->inet_addr, slp->remote_port, slp->gw_port,
                    slp->full_name, slp->comment);
#endif

            server_count++;
            break;
        case 5:                /* all done! */
            break;
        default:
            fprintf (stderr, "Whoops!\n");
            terminate (2);
        }
    }

    fclose (fp);
}

#endif  /* GATEWAY */              /* end of gateway-specific mass */
/* ------------------------------------------------------------------------- */



/******************************************************************************/
/***  (local) handle_exception()                                            ***/
/******************************************************************************/
static RETSIGTYPE
handle_exception (int _dummy)
{
    printf
        ("floating point exception error detected; attempting to continue\n");
    /* Under Watcom C++, after a signal handler is called the signal reverts
       to SIG_DFL so has to be reset... is this standard? */
    (void) SIGNAL (SIGFPE, handle_exception);
}

/******************************************************************************/
/***  query_cowid()                                                         ***/
/******************************************************************************/
char *
query_cowid (void)
{
    return COWID;
}

/* Variables passing Optional Arguments to cowmain */

char *deffile = NULL;
char *recordFileName = NULL;
char *logFileName = NULL;
char *display_host = NULL;
int passive = 0;
int checking = 0;

#ifdef META
int usemeta = 1;
#endif

/******************************************************************************/
/***  cowmain()                                                             ***/
/******************************************************************************/
int
cowmain (char *server,
         int port,
         char *name)
{
    int intrupt (fd_set * readfds);
    int team, s_type;

    char *cp;
    char buf[80];
    struct passwd *pwent;

    int i;

    i = setjmp (env);           /* Error while initializing */
    if (i >= RETURNBASE)
        return (i - RETURNBASE);        /* Terminate with retcode */

#ifdef GATEWAY
    /* restrict this client to certain machines */
    {
        struct sockaddr_in saddr;
        struct hostent *hp;
        char myname[64];
        LONG myaddr;

        if (gethostname (myname, 64) < 0)
        {
            perror ("gethostname");
            return (1);
        }
        if ((myaddr = inet_addr (myname)) == -1)
        {
            if ((hp = gethostbyname (myname)) == NULL)
            {
                fprintf (stderr, "unable to get addr for local host\n");
                return (1);
            }
            myaddr = *(LONG *) hp->h_addr;
        }

        /* printf("myname = '%s', myaddr = 0x%.8lx\n", myname, myaddr); */
        if ((myaddr & MYADDR_MASK) != MYADDR)
        {
            fprintf (stderr,
                     "Sorry, you may not run this client on this host\n");
            return (1);
        }
    }
#endif

#ifdef GATEWAY
    read_servers ();
    serverName = gateway;
#endif

    pseudo[0] = defpasswd[0] = '\0';

#ifdef RECORDGAME
    if (recordFileName != NULL)
    {
        recordFile = fopen (recordFileName, "wb");
        if (recordFile == NULL)
        {
            perror (recordFileName);
            return (1);

        }
    }
#endif
    if (logFileName != NULL)
    {
        logFile = fopen (logFileName, "a");
        if (logFile == NULL)
        {
            perror (logFileName);
            return (1);
        }
    }
    for (i = 0; i < 80; i++)
    {
        outmessage[i] = '\0';
    }

#ifndef DEBUG
    (void) SIGNAL (SIGFPE, handle_exception);
#endif

    SRANDOM (time (0));

    initDefaults (deffile);

    xtrekPort = port;
    if (server)
    {
        if (xtrekPort < 0)
        {
            sprintf (defaulttmp, "port.%s", server);
            xtrekPort = intDefault (defaulttmp, -1);
            if (checking)
                xtrekPort = xtrekPort - 1;
        }
        sprintf (defaulttmp, "server.%s", server);
        serverName = stringDefault (defaulttmp);
        if (!serverName)
            serverName = server;
    }

    SRANDOM (getpid () * time ((LONG *) 0));

    if (!passive)
    {
        if (!serverName)
            serverName = stringDefault ("server");
        if (!serverName)

#ifdef META
            if (!usemeta)
#endif

                noserver ();
    }

    if (xtrekPort < 0)
    {
        xtrekPort = intDefault ("port", DEFAULT_PORT);
        if (checking)
            xtrekPort = xtrekPort - 1;
    }
    if (checking)
        check ();

    if (!logFileName)
        logFileName = stringDefault ("logfile");
    if (logFileName != NULL)
    {
        logFile = fopen (logFileName, "a");
        if (logFile == NULL)
        {
            perror (logFileName);
            return (1);
        }
    }

#ifdef META
    if (usemeta)
        parsemeta (usemeta);
#endif

/* Check to see if we should load color bitmaps  SRS 1/11/99 */
/* 0 = mono
 * 1 = color with tinted bitmaps in center representing your ship
 * 2 = color with greyscale bitmaps in center representing your ship
 */

    /* This is here for backwards compatibility with old bool default */
    colorClient = booleanDefault ("colorClient", 0);
    /* If it's already 0, it can't hurt to check for int default */
    if (colorClient == 0)
        colorClient = intDefault ("colorClient", 1);

    fed_ship_bmp = "bitmaps/shiplib/fedship.bmp";
    if ((stringDefault ("fedshipbmpfile")) != NULL)
        fed_ship_bmp = stringDefault ("fedshipbmpfile");

    fed_ship_bmp_G = "bitmaps/shiplib/fedshipG.bmp";
    if ((stringDefault ("fedshipGbmpfile")) != NULL)
        fed_ship_bmp_G = stringDefault ("fedshipGbmpfile");

    fed_ship_bmp_T = "bitmaps/shiplib/fedshipT.bmp";
    if ((stringDefault ("fedshipTbmpfile")) != NULL)
        fed_ship_bmp_T = stringDefault ("fedshipTbmpfile");

    fed_ship_bmp_M = "bitmaps/shiplib/fedshipM.bmp";
    if ((stringDefault ("fedshipMbmpfile")) != NULL)
        fed_ship_bmp_M = stringDefault ("fedshipMbmpfile");

    rom_ship_bmp = "bitmaps/shiplib/romship.bmp";
    if ((stringDefault ("romshipbmpfile")) != NULL)
        rom_ship_bmp = stringDefault ("romshipbmpfile");

    rom_ship_bmp_G = "bitmaps/shiplib/romshipG.bmp";
    if ((stringDefault ("romshipGbmpfile")) != NULL)
        rom_ship_bmp_G = stringDefault ("romshipGbmpfile");

    rom_ship_bmp_T = "bitmaps/shiplib/romshipT.bmp";
    if ((stringDefault ("romshipTbmpfile")) != NULL)
        rom_ship_bmp_T = stringDefault ("romshipTbmpfile");

    rom_ship_bmp_M = "bitmaps/shiplib/romshipM.bmp";
    if ((stringDefault ("romshipMbmpfile")) != NULL)
        rom_ship_bmp_M = stringDefault ("romshipMbmpfile");

    kli_ship_bmp = "bitmaps/shiplib/kliship.bmp";
    if ((stringDefault ("klishipbmpfile")) != NULL)
        kli_ship_bmp = stringDefault ("klishipbmpfile");

    kli_ship_bmp_G = "bitmaps/shiplib/klishipG.bmp";
    if ((stringDefault ("klishipGbmpfile")) != NULL)
        kli_ship_bmp_G = stringDefault ("klishipGbmpfile");

    kli_ship_bmp_T = "bitmaps/shiplib/klishipT.bmp";
    if ((stringDefault ("klishipTbmpfile")) != NULL)
        kli_ship_bmp_T = stringDefault ("klishipTbmpfile");

    kli_ship_bmp_M = "bitmaps/shiplib/klishipM.bmp";
    if ((stringDefault ("klishipMbmpfile")) != NULL)
        kli_ship_bmp_M = stringDefault ("klishipMbmpfile");

    ori_ship_bmp = "bitmaps/shiplib/oriship.bmp";
    if ((stringDefault ("orishipbmpfile")) != NULL)
        ori_ship_bmp = stringDefault ("orishipbmpfile");

    ori_ship_bmp_G = "bitmaps/shiplib/orishipG.bmp";
    if ((stringDefault ("orishipGbmpfile")) != NULL)
        ori_ship_bmp_G = stringDefault ("orishipGbmpfile");

    ori_ship_bmp_T = "bitmaps/shiplib/orishipT.bmp";
    if ((stringDefault ("orishipTbmpfile")) != NULL)
        ori_ship_bmp_T = stringDefault ("orishipTbmpfile");

    ori_ship_bmp_M = "bitmaps/shiplib/orishipM.bmp";
    if ((stringDefault ("orishipMbmpfile")) != NULL)
        ori_ship_bmp_M = stringDefault ("orishipMbmpfile");

    ind_ship_bmp = "bitmaps/shiplib/indship.bmp";
    if ((stringDefault ("indshipbmpfile")) != NULL)
        ind_ship_bmp = stringDefault ("indshipbmpfile");

    ind_ship_bmp_G = "bitmaps/shiplib/indshipG.bmp";
    if ((stringDefault ("indshipGbmpfile")) != NULL)
        ind_ship_bmp_G = stringDefault ("indshipGbmpfile");

    ind_ship_bmp_T = "bitmaps/shiplib/indshipT.bmp";
    if ((stringDefault ("indshipTbmpfile")) != NULL)
        ind_ship_bmp_T = stringDefault ("indshipTbmpfile");

    ind_ship_bmp_M = "bitmaps/shiplib/indshipM.bmp";
    if ((stringDefault ("indshipMbmpfile")) != NULL)
        ind_ship_bmp_M = stringDefault ("indshipMbmpfile");

    newwin (display_host, name);

#ifdef META
    if (usemeta)
    {
        metawindow ();
        metainput ();
    }
#endif

    resetdefaults ();

    /* open memory...? */
    openmem ();

#ifdef GATEWAY
    /* pick a nice set of UDP ports */
    getUdpPort ();
#endif

    /* Get login name */
    if (cp = stringDefault ("login"))
        strncpy (login, cp, sizeof (login));
    else if ((pwent = getpwuid ()) != NULL)
        (void) STRNCPY (login, pwent->pw_name, sizeof (login));
    else
        (void) STRNCPY (login, "Bozo2000", sizeof (login));

    login[sizeof (login) - 1] = '\0';

    if (pseudo[0] == '\0')
    {
        if ((cp = stringDefault ("name")) != 0)
            (void) STRNCPY (pseudo, cp, sizeof (pseudo));
        else
            (void) STRNCPY (pseudo, login, sizeof (pseudo));
    }

    pseudo[sizeof (pseudo) - 1] = '\0';

    if (defpasswd[0] == '\0')
        if ((cp = stringDefault ("password")) != 0)
            (void) STRNCPY (defpasswd, cp, sizeof (defpasswd));

    defpasswd[sizeof (defpasswd) - 1] = '\0';



    if (!passive)
    {
        callServer (xtrekPort, serverName);
    }
    else
    {
        connectToServer (xtrekPort);
    }

#ifdef FEATURE_PACKETS
    sendFeature ("FEATURE_PACKETS", 'S', 1, 0, 0);
#endif

    findslot ();

    lastm = mctl->mc_current;

    mapAll ();

    (void) SIGNAL (SIGINT, SIG_IGN);

#ifndef RWATCH
    getname (pseudo, defpasswd);
#else
    MZERO (mystats, sizeof (mystats));
    mystats->st_tticks = 1;
    mystats->st_flags = ST_MAPMODE + ST_NAMEMODE + ST_SHOWSHIELDS +
        ST_KEEPPEACE + ST_SHOWLOCAL * 2 + ST_SHOWGLOBAL * 2;
#endif /* RWATCH */

    loggedIn = 1;
    phaserWindow = booleanDefault ("phaserWindow", phaserWindow);

#ifdef AUTOKEY
    /* autokey.c */
    autoKeyDefaults ();
#endif /* AUTOKEY */

    initkeymap ();
    sendOptionsPacket ();

    /* Set p_hostile to hostile, so if keepPeace is on, the guy starts off
     * hating everyone (like a good fighter should) */
    me->p_hostile = (FED | ROM | KLI | ORI);

    if (!newDashboard)
    {
        sprintf (buf,
                 "Maximum:      %2d  %3d %3d               %3d   %6d   %3d   %3d",
                 0, 0, 0, 0, 0, 0, 0);
        W_WriteText (tstatw, 50, 27, textColor, buf, strlen (buf),
                     W_RegularFont);
    }
    me->p_planets = 0;
    me->p_genoplanets = 0;
    me->p_armsbomb = 0;
    me->p_genoarmsbomb = 0;
    /* Set up a reasonable default */

#ifndef RWATCH
    me->p_whydead = KQUIT;
    me->p_team = ALLTEAM;
    s_type = CRUISER;
#endif /* RWATCH */

    startPing ();               /* support ping stuff */

#ifdef AUTOKEY
    if (autoKey)
    {
        /* XX: changes entire state of display */
        W_AutoRepeatOff ();
    }
#endif

#ifdef SOUND
    Init_Sound ();
#endif

    isFirstEntry = 1;           /* First entry into game */

    i = setjmp (env);           /* Reentry point of game */
    if (i >= RETURNBASE)
        return (i - RETURNBASE);        /* Terminate with retcode */


#ifdef SOUND
    Abort_Sound (ENGINE_SOUND);
#endif

    /* give the player the motd and find out which team he wants */

#ifndef RWATCH
    /* check if this dude is trying to resume a ghostbust slot */
    if (ghoststart)
    {
        char *getteam = "IFR K   O";
        int i;

        ghoststart = 0;

        for (i = 0; i < 9; i++)
            if (getteam[i] == me->p_mapchars[0])
                break;

        me->p_team = i;

        if (me->p_damage > me->p_ship.s_maxdamage)
        {
            me->p_status = POUTFIT;
            entrywindow (&team, &s_type);
        }
        else
            me->p_status = PALIVE;

    }
    else
    {
        /* give the player the motd and find out which team he wants */
        entrywindow (&team, &s_type);
    }


    if (team == -1)
    {
        W_DestroyWindow (w);

#ifdef AUTOKEY
        if (autoKey)
            W_AutoRepeatOn ();
#endif

        sendByeReq ();

#ifdef SOUND
        Exit_Sound ();
#endif

        sleep (1);
        if (logFile != NULL)
            fclose (logFile);
        printf ("OK, bye!\n");

#ifdef PACKET_LOG
        if (log_packets)
            Dump_Packet_Log_Info ();
#endif

        return (0);


    }
#endif /* RWATCH */

    sendVersion ();
    getship (myship, myship->s_type);

#ifdef BRMH
    redrawall = 2;
#else
    redrawall = 1;
#endif

    shipchange (s_type);
    enter ();
    calibrate_stats ();
    W_ClearWindow (w);
    /* for (i = 0; i < NSIG; i++) { (void) SIGNAL(i, SIG_IGN); } */

    me->p_status = PALIVE;      /* Put player in game */
    me->p_ghostbuster = 0;
    PlistNoteUpdate (me->p_no);

    if (showStats)              /* Default showstats are on. */
        W_MapWindow (statwin);

    if (W_IsMapped (pStats))    /* support ping stuff */
        redrawPStats ();

    if (isFirstEntry)
    {
        if (tryUdp && commMode != COMM_UDP)
            sendUdpReq (COMM_UDP);

#ifdef SHORT_PACKETS            /* should we be checking for
                                 * udp on here? */
        if (tryShort)
            sendShortReq (SPK_VON);
        else
        {
            /*
               If tryShort is true, we get an automatic `-' style update
               when the short packets kick in, so this is not necessary.
             */

            sendUdpReq (COMM_UPDATE);
        }
#else
        /* `=' style update to get the kills in the playerlist right */
        sendUdpReq (COMM_UPDATE);
#endif
        /* Send request for updatesPerSec.  New servers now support 10 u/s */
        sendUpdatePacket (1000000 / updatesPerSec);

        isFirstEntry = 0;
    }


#ifdef SOUND
    Play_Sound (ENTER_SHIP_SOUND);
    Play_Sound (ENGINE_SOUND);
#endif

#ifdef HOCKEY_LINES
    init_hockey_lines ();
#endif

    /* Get input until the player quits or dies */
    input ();

    return (0);
}

/******************************************************************************/
/***  terminate()                                                           ***/
/******************************************************************************/
void
terminate (int error)
{
    longjmp (env, RETURNBASE + error);
}

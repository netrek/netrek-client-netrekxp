/******************************************************************************/
/***  File:  parsemeta.c
/***
/***  Function:
/***   Connect to the metaserver to get a list of active netrek servers. Parse
/***   the output and display it in a window to the user. The user can select
/***   the server to play/observe on by left/right clicking the menu option.
/***
/***  Author:
/***   05/1993  Nick Trown             Original version.
/***
/***  Revisions:
/***   05/1993  Andy McFadden          Connect to Metaserver.
/***   
/***   ???      BDyess (Paradise)      Bug Fixes.
/***                                   Add server type field.
/***   
/***   01/1995  Michael Kellen         Don't list Paradise Servers.
/***                                   List empty servers.
/***   
/***   01/1995  James Soutter          Big Parsemeta changes.
/***                                   Included some Paradise Code.
/***                                   Added Known Servers Option.
/***                                   Added option for metaStatusLevel.
/***                                   Bug Fixes.
/***   
/***   08/1995  Jonathan Shekter       Changed to read into buffer in all cases
/***                                   Use findfile() interface for cachefiles.
/***   
/***   06/2003  Erik Hietbrink         Added metaPing option.
/***                                   Added Connecting Now state.
/***   
/******************************************************************************/

#undef DEBUG

#include "config.h"
#include "copyright.h"

#ifdef META
#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <winsock.h>
#include <string.h>
#include <errno.h>
#include <shellapi.h>

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"
#ifdef METAPING
#include <process.h>	// for _getpid
#endif


/* Constants */

#define BUF     6144
#define LINE    80			/* Width of a meta-server line */
#define RTT_AVG_BUFLEN  5   /* number of samples used for average rtt time */

/* Local Types */

struct servers
{
    char address[LINE];
    int port;
    int time;
    int players;
    int status;
    int RSA_client;
    char typeflag;
#ifdef METAPING
	u_long ip_addr;					/* Cache the ip address after DNS lookup */
	DWORD pkt_rtt[RTT_AVG_BUFLEN];	/* store last # ping samples for avg rtt */
									/* -1:init, -2:unknown host, -3:timeout  */
									/* >=0:round trip time in ms             */
#endif
};

struct servers *serverlist = NULL;	/* The record for each server */
int num_servers = 0;            /* The number of servers.       */
char *metaWindowName;           /* The window's name.           */

#ifdef METAPING
u_short metaPing_procId = 0;	/* Process id helps identify own ping replies */
int thread_ready = 0;			/* Awful way of letting the ping thread exit */
HANDLE hThread = NULL;
#endif


/* The status strings:  The order of the strings up until statusNull is
 * important because the meta-client will display all the strings up to a
 * particular point.
 * 
 * The strings after statusNull are internal status types and are formatted
 * separatly from the other strings.
 * 
 * The string corresponding to "statusNull" is assigned to thoes servers which
 * have "statusNobody" or earlier strings in old, cached, meta-server data. */

char *statusStrings[] =
    { "OPEN:", "Wait queue:", "Nobody", "Timed out", "No connection",
    "Active", "CANNOT CONNECT", "DEFAULT SERVER", "CONNECTING NOW"
};

enum statusTypes
{
    statusOpen = 0, statusWait, statusNobody, statusTout, statusNoConnect,
    statusNull, statusCantConnect, statusDefault, statusConnecting
};

int metaStatusLevel = statusTout;


/* Functions */
extern void terminate (int error);


static int
open_port (char *host,
           int port,
           int verbose)
/* The connection to the metaserver is by Andy McFadden. This calls the
 * metaserver and parses the output into something useful */
{
    struct sockaddr_in addr;
    struct hostent *hp;
    int sock;

    /* Connect to the metaserver */
    /* get numeric form */
    if ((addr.sin_addr.s_addr = inet_addr (host)) == -1)
    {
        if ((hp = gethostbyname (host)) == NULL)
        {
            if (verbose)
            {
                LineToConsole ("unknown host '%s'\n", host);
            }
            return (-1);
        }
        else
        {
            addr.sin_addr.s_addr = *(LONG *) hp->h_addr;
        }
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons ((u_short) port);
    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        if (verbose)
            perror ("socket");
        return (-1);
    }
    if (connect (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
        if (verbose)
            perror ("connect");
        closesocket (sock);
        return (-1);
    }
    return (sock);
}


static void
parseInput (char *in,
            FILE * out,
            int statusLevel)
/* Read the information we need from the meta-server. */
{
    char line[LINE + 1], *numstr, *point, **statStr;
    struct servers *slist;
    int rtn, max_servers;
    int count;
#ifdef METAPING
    int i;
#endif

#ifdef DEBUG
    LineToConsole ("In parseInput\n");
#endif

    /* Create some space to hold the entries that will be read.  More space can
     * be added later */

    serverlist = (struct servers *) malloc (sizeof (struct servers) * 10);

    max_servers = 10;
    num_servers = 0;

    /* Add the default server */

    if (serverName)
    {

#ifdef RSA
        serverlist[num_servers].RSA_client = useRsa;
#endif

        strcpy (serverlist[num_servers].address, serverName);
        serverlist[num_servers].port = xtrekPort;
        serverlist[num_servers].status = statusDefault;
        serverlist[num_servers].players = 0;
        serverlist[num_servers].typeflag = ' ';
        num_servers++;
    }


    while (1)
    {
        /* Read a line */
        point = line;
        count = LINE + 1;

        do
        {
            if (!(--count))
            {
                fputs ("Warning: Line from meta server was too long!!!\n",
                       stderr);
                ++point;        /* Pretend we read a '\n' */
                break;
            }

            rtn = *in++;
            if (!rtn)           /* use a zero to mark end of buffer */
                return;

            *(point++) = rtn;
        }
        while (rtn != EOF && rtn != '\n');

        *(--point) = '\0';

        if (out != NULL)        /* Save a copy of the stuff
                                 * we read */
        {
            fputs (line, out);
            putc ('\n', out);
        }


        /* Find somewhere to put the information that is just about to be
         * parsed */

        if (num_servers >= max_servers)
        {
            max_servers += 5;
            serverlist = (struct servers *) realloc (serverlist,
                                                     sizeof (struct servers) *
                                                     max_servers);
        }

        slist = serverlist + num_servers;



        /* Is this a line we want? */

        if (sscanf (line, "-h %s -p %d %d %d",
                    slist->address, &(slist->port), &(slist->time)) != 3)
        {
            continue;
        }

        /* Find the status of the server, eg "Not responding". */

        for (statStr = statusStrings + statusLevel; statStr >= statusStrings;
             --statStr)
        {
            if ((numstr = strstr (line, *statStr)) != NULL)
            {
                (slist->status) = statStr - statusStrings;
                (slist->players) = 0;
                sscanf (numstr, "%*[^0123456789] %d", &(slist->players));
                break;
            }
        }

        if (statStr < statusStrings)    /* No status was allocated */
            continue;


        /* Read the flags */

        slist->RSA_client = (*(point - 5) == 'R');
        slist->typeflag = *(point - 1);

#ifdef METAPING
		/* Initialize the ping rtt fields */
		for (i = 0; i < RTT_AVG_BUFLEN; ++i )
			slist->pkt_rtt[i] = -1;
#endif

        /* Don't list Paradise Servers or *.tamu.edu */
        if (slist->typeflag != 'P' && strstr(slist->address,".tamu.edu") == NULL)
        {

#ifdef DEBUG
            LineToConsole ("HOST:%-30s PORT:%-6d %12s %-5d %d %c\n",
                            serverlist[num_servers].address,
                            serverlist[num_servers].port,
                            statusStrings[serverlist[num_servers].status],
                            serverlist[num_servers].players,
                            serverlist[num_servers].RSA_client,
                            serverlist[num_servers].typeflag);
#endif

            ++num_servers;
        }
    }
}


static int
ReadFromMeta (int statusLevel)
/* Read from the meta-server.  Return TRUE on success and FALSE on failure. */
{
    FILE *out;
    char *cacheName;
    char cacheFileName[PATH_MAX];
    char tmpFileName[PATH_MAX];
    char *sockbuf, *buf;
    int bufleft = BUF - 1;
    int len;
    int sock = 0;
    int i = 0;

    if (stringDefault ("metaServer") != NULL)
        metaServer[0] = stringDefault ("metaServer");

    metaPort = intDefault ("metaPort", metaPort);

    /* Attempt connecting to all servers one by one */
    do
    {
        LineToConsole ("Calling %s on port %d\n", metaServer[i], metaPort);

        sock = open_port (metaServer[i], metaPort, 0);

        if (sock <= 0)
        {
            LineToConsole ("Cannot connect to MetaServer (%s port %d)\n",
                            metaServer[i], metaPort);
        }

        i++;

    } while (sock <= 0 && i <= 1);  /* i shouldn't be bigger than number of servers */

    /* Still no connection -> return */
    if (sock <= 0)
        return 0;

    /* Allocate a buffer and read until full */
    buf = sockbuf = (char *) malloc (BUF);
    while (bufleft > 0 && (len = recv (sock, buf, bufleft, 0)) > 0)
    {
        bufleft -= len;
        buf += len;
#ifdef DEBUG
        LineToConsole ("Read %d bytes from Metaserver\n", len);
#endif
    }
    closesocket (sock);
    *buf = 0;                   /* End of buffer marker */

    if (len < 0)
    {
        perror ("read");
        free (sockbuf);
        return 0;
    }

    cacheName = stringDefault ("metaCache");
    if (cacheName && !findfile (cacheName, cacheFileName))
        strcpy (cacheFileName, cacheName);      /* overwrite existing file if possible */

    if (cacheName)
    {
        len = strlen (cacheFileName);
        strcpy (tmpFileName, cacheFileName);

        /* Create a temporary file with roughly the same name */

        if ((cacheFileName[len - 1] == 'T')
            || (cacheFileName[len - 1] == 't'))
            tmpFileName[len - 1] = 'R';
        else
            tmpFileName[len - 1] = 'T';

        out = fopen (tmpFileName, "w");

        if (out == NULL)
        {
            LineToConsole (
                     "Can not write to the metaCache temporary file `%s'.\n",
                     tmpFileName);
            LineToConsole ("Meta-server read will not be cached.\n");
        }
    }
    else
    {
        out = NULL;
    }

    parseInput (sockbuf, out, statusLevel);

    if (out != NULL)
    {
        fclose (out);

        /* Can't rename file to existing name under NT */
#ifdef _MSC_VER
        _unlink (cacheName);
#else
        unlink (cacheName);
#endif
        if (rename (tmpFileName, cacheName) == -1)
            perror ("Could not write to cache file");
    }

    free (sockbuf);
    metaWindowName = "NetrekXP MetaServer List";

    return 1;
}


static int
ReadFromCache (int statusLevel)
/* Read from the cache.  Return TRUE on success and FALSE on failure. */
{
    FILE *in;
    char *cacheName;
    struct servers *slist;
    char *sockbuf, *buf;
    int bufleft = BUF - 1;
    int len;
    char cacheFileName[PATH_MAX];

    cacheName = stringDefault ("metaCache");

    if (!cacheName)
    {
        LineToConsole ("You must define the .xtrekrc variable `metaCache' in\n");
        LineToConsole ("order to use the `show known servers' option.\n");
        return 0;
    }
    else if (!findfile (cacheName, cacheFileName)
             || !(in = fopen (cacheFileName, "r")))
    {
        LineToConsole (
                 "The metaCache file `%s' is empty or not accessable.\n",
                 cacheName);
        return 0;
    }


    /* Allocate a buffer and read until full. Why do we
       go through this silly stuff? Because sockets are
       not file handles on all verions of Windows */
    buf = sockbuf = (char *) malloc (BUF);

    while (bufleft > 0 && ((len = fread (buf, 1, bufleft, in)) > 0))
    {
        bufleft -= len;
        buf += len;
#ifdef DEBUG
        LineToConsole ("Read %d bytes from Metaserver cache file\n", len);
#endif
    }
    *buf = 0;                   /* End of buffer marker */
    fclose (in);

    if (len < 0)
    {
        perror ("fread");
        free (sockbuf);
        return 0;
    }

    /* Show all servers known to have been reachable */

    if (statusLevel <= statusNobody)
        statusLevel = statusNobody;

    parseInput (sockbuf, NULL, statusLevel);


    /* Don't promise games from old data */
    for (slist = serverlist + num_servers - 1; slist >= serverlist; --slist)
    {
        if (slist->status <= statusNobody)
            slist->status = statusNull;
    }

    free (sockbuf);
    metaWindowName = "Known Servers";

    return 1;
}


int
metaGetServerType (char typeflag)
{
	switch (typeflag)
	{
	case 'P':
		return ST_PARADISE;
	case 'B':
		return ST_BRONCO;
	case 'C':
		return ST_CHAOS;
	case 'I':
		return ST_INL;
	case 'S':
		return ST_STURGEON;
	case 'H':
		return ST_HOCKEY;
	case 'F':
		return ST_DOGFIGHT;
	default:
		return ST_UNKNOWN;
	}
}


void
parsemeta (int metaType)
/* Read and Parse the meta-server information, either from the meta- server
 * (if metaType = 1) or from the cache (if metaType = 2).
 * 
 * NOTE: This function sets the variable "num_servers" which is later used to
 * decide the hight of the meta-server window. */
{
    metaStatusLevel = intDefault ("metaStatusLevel", metaStatusLevel);

    if (metaStatusLevel < 0)
        metaStatusLevel = 0;
    else if (metaStatusLevel >= statusNull)
        metaStatusLevel = statusNull - 1;


    if (metaType == 1)          /* Read from the Meta-server */
    {
        if (ReadFromMeta (metaStatusLevel) || ReadFromCache (metaStatusLevel))
            return;

        terminate (0);
    }

    if (metaType == 2)          /* Read from the Cache first */
    {
        if (ReadFromCache (metaStatusLevel) || ReadFromMeta (metaStatusLevel))
            return;

        terminate (0);
    }
}


static void
metarefresh (int i)
/* Refresh line i in the list */
{
    struct servers *slist;
    W_Color color = textColor;
    char buf[LINE + 1];

#ifdef METAPING
    DWORD lag = 0;
    int idx, replies = 0;
#endif

    slist = serverlist + i;
    
    // Don't list *.tamu.edu
    if (strstr(slist->address,".tamu.edu") != NULL)
        return;
        
#ifdef METAPING
    if (metaPing)
	sprintf (buf, "%-34s %14s ",
		slist->address, statusStrings[slist->status]);
    else
#endif
	sprintf (buf, "%-40s %14s ",
		slist->address, statusStrings[slist->status]);

    if (slist->status == statusConnecting) color = W_Yellow;

    if (slist->status <= statusNull)
    {
        if (slist->status == statusOpen || slist->status == statusWait)
        {
            /* Don't print the number of players if nobody is playing */
            sprintf (buf + strlen (buf), "%-5d  ", serverlist[i].players);
        }
        else
        {
            strcat (buf, "       ");
        }

        switch (serverlist[i].typeflag)
        {
        case 'P':
            strcat (buf, "Paradise");
            break;
        case 'B':
            strcat (buf, "Bronco  ");
            break;
        case 'C':
            strcat (buf, "Chaos   ");
            break;
        case 'I':
            strcat (buf, "INL     ");
            break;
        case 'S':
            strcat (buf, "Sturgeon");
            break;
        case 'H':
            strcat (buf, "Hockey  ");
            break;
        case 'F':
            strcat (buf, "Dogfight");
            break;
        default:
            strcat (buf, "Unknown ");
            break;
        }
    }
    else
    {
	strcat (buf, "               ");
    }

#ifdef METAPING
    if (metaPing)
    {
	/* Print out the lag statistics */
	for (idx = 0; idx < RTT_AVG_BUFLEN; ++idx)
	{
		if (serverlist[i].pkt_rtt[idx] != -3 &&
		    serverlist[i].pkt_rtt[idx] != -2 &&
		    serverlist[i].pkt_rtt[idx] != -1)      // dont count these non-values
		{
			//printf ("i=%d  idx=%d  replies=%d  rtt=%ld  lag=%ld\n", i , idx, replies, serverlist[i].pkt_rtt[idx], lag);
		    lag += serverlist[i].pkt_rtt[idx];
		    replies++;
		}
	}
    
	//printf("i=%d  replies=%ld  idx=%ld   rtt=%ld  %s\n", i, replies,
	//	   ((serverlist[i].cur_idx + RTT_AVG_BUFLEN - 1) % RTT_AVG_BUFLEN),
	//	   serverlist[i].pkt_rtt[(serverlist[i].cur_idx + RTT_AVG_BUFLEN - 1) % RTT_AVG_BUFLEN],
	//	   serverlist[i].address);

	if (replies > 0)
	{
		lag = lag / replies;
		if (lag < 1000) sprintf (buf + strlen (buf), " %3ldms", lag);
		else strcat(buf, " >1sec");
	}
	else if (replies == 0 && serverlist[i].pkt_rtt[0] == -2)
		strcat(buf, " Unknw"); // Unknown host
	else if (replies == 0 && serverlist[i].pkt_rtt[0] == -3)
		strcat(buf, " TmOut"); // TimeOut
	else strcat(buf, "      ");
    }
#endif

    W_WriteText (metaWin, 0, i, color, buf, strlen (buf), 0);
}


void
metawindow (void)
/* Show the meta server menu window */
{
    int i;

#ifdef METAPING
	metaPing = booleanDefault ("metaPing", metaPing);
#endif

    for (i = 0; i < num_servers; i++)
        metarefresh (i);

    /* Give the window the right name */
    W_RenameWindow (metaWin, metaWindowName);

    /* Add quit option */
    W_WriteText (metaWin, 0, num_servers, textColor, "Quit", 4, 0);

	/* Additional Help Options */
	W_WriteText (metaWin, 0, num_servers + 1, W_Yellow, 
		        "    Netrek Homepage | Newbie Manual | Forum | FAQ | Dogfight Manual", 67, 0);

    /* Map window */
    W_MapWindow (metaWin);
}


static void
metadone (void)
/* Unmap the metaWindow */
{
#ifdef METAPING
	if (metaPing)
	{
		/* stop the ping thread */
		if (hThread != NULL)
		{
			thread_ready = 1;
			WaitForSingleObject(hThread, INFINITE); 
			CloseHandle(hThread);
		}
	}
#endif

	/* Unmap window */
    W_UnmapWindow (metaWin);
    free (serverlist);
}


void
metaaction (W_Event * data)
/* Recieve an action in the meta server window.  Check selection to see if
 * was valid.  If it was then we have a winner! */
{
    int sock;
    char buf[80];
    struct servers *slist;
	int x;

#ifdef DEBUG
    LineToConsole ("Got meta window action, y=%d\n", data->y);
#endif
    if ((data->y >= 0) && (data->y < num_servers))
    {
        slist = serverlist + data->y;
        xtrekPort = slist->port;
        if (data->key == W_RBUTTON)     /* Guess at an observer port */
        {
            xtrekPort++;
            LineToConsole ("Attempting to observe on port %d...\n", xtrekPort);
        }
        serverName = strdup (slist->address);
		serverType = metaGetServerType (slist->typeflag);

#ifdef RSA
        useRsa = slist->RSA_client;
#endif
		slist->status = statusConnecting;
        metarefresh (data->y);

        LineToConsole ("Checking %s on port %d\n", serverName, xtrekPort);

        if ((sock = open_port (serverName, xtrekPort, 0)) <= 0)
        {
            slist->status = statusCantConnect;
            metarefresh (data->y);
        }
        else
        {
            closesocket (sock);
            sprintf (buf, "Netrek  @  %s", serverName);
            W_RenameWindow (baseWin, buf);
            metadone ();
        }
    }
    else if (data->y == num_servers) /* Quit selected */
	{
		W_WriteText (metaWin, 0, num_servers, W_Yellow, "Quit", 4, 0);
		metadone();
        terminate (0);
	}
	else if (data->y == num_servers + 1) /* Help Line */
	{
		x = data->x / W_Textwidth;
		if (x >= 0 && x <= 19)			/* Netrek Home Page */
			ShellExecute (NULL, "open", "http://www.netrek.org", NULL, NULL, SW_SHOWNORMAL);
		else if (x >= 21 && x <= 35)	/* Newbie Manual */
			ShellExecute (NULL, "open", "http://www.netrek.org/cow/current/newbie.html", NULL, NULL, SW_SHOWNORMAL);
		else if (x >= 37 && x <= 43)	/* Forums */
			ShellExecute (NULL, "open", "http://groups-beta.google.com/group/rec.games.netrek", NULL, NULL, SW_SHOWNORMAL);
		else if (x >= 45 && x <= 49)	/* FAQ */
			ShellExecute (NULL, "open", "http://www.inl.org/netrek/netrekFAQ.html", NULL, NULL, SW_SHOWNORMAL);
		else if (x >= 51 && x <= 67)	/* Dogfight Manual */
			ShellExecute (NULL, "open", "http://cha.rlie.nl/dfmanual/", NULL, NULL, SW_SHOWNORMAL);
	}
}




void
metainput (void)
/* Wait for actions in the meta-server window.
 * 
 * This is really the meta-server window's own little input() function. It is
 * needed so we don't have to use all the bull in the main input(). Plus to
 * use it I'd have to call mapAll() first and the client would read in the
 * default server and then call it up before I can select a server. */
{
    W_Event data;

#ifdef METAPING
	DWORD IDThread; 

	metaPing_procId = _getpid() & 0xFFFF;

	if (metaPing)
	{
		/* Start the thread doing the pinging */ 
		hThread = CreateThread(NULL,			// no security attributes 
			0,									// use default stack size 
			(LPTHREAD_START_ROUTINE) metaPing_thread,	// thread function 
			NULL,								// no thread function argument 
			0,									// use default creation flags 
			&IDThread);							// returns thread identifier 
 
		// Check the return value for success. 
		if (hThread == NULL)
            LineToConsole ("CreateThread error\n");
	}
#endif

    while (W_IsMapped (metaWin))
    {
		/* wait for any event */
		W_NextEvent (&data);
		
        switch ((int) data.type)
        {
        case W_EV_KEY:
            if (data.Window == metaWin)
                metaaction (&data);
            break;
        case W_EV_BUTTON:
            if (data.Window == metaWin)
                metaaction (&data);
            break;
        case W_EV_EXPOSE:
            break;
        default:
            break;
        }
    }
}


#ifdef METAPING

/* Additional code to do icmp pings from the metawindow */

#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;			// Version and IHL
	u_char	TOS;			// Type Of Service
	short	TotLen;			// Total Length
	short	ID;				// Identification
	short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
}IPHDR, *PIPHDR;


// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence
	char	Data;			// Data
}ICMPHDR, *PICMPHDR;


#define REQ_DATASIZE 32		// Echo Request Data size
#define METAPING_WAITTIME 1000	/* waiting time for icmp replies between loops */

// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;


// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;


// What happened?
void metaPing_ReportError(char *pWhere)
{
	LineToConsole ("\n%s error: %d\n", pWhere, WSAGetLastError());
}


/*
 * Checksum routine for Internet Protocol family headers (C Version)
 * From the (Unix) ping source code from Mike Muuss
 */
u_short metaPing_in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 ) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}


// Fill in echo request header and send to destination
int metaPing_sendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToAddr, u_short nSeq) 
{
	static ECHOREQUEST echoReq;
	int nRet;
	char *pingData = "Netrek PING:visit www.netrek.org!";

	// Fill in echo request
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= metaPing_procId;
	echoReq.icmpHdr.Seq			= nSeq;

	// Save tick count when sent
	echoReq.dwTime				= GetTickCount();

	// Fill in some data to send
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = pingData[nRet];

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum =
		metaPing_in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	// Send the echo request  								  
	nRet = sendto(s,						/* socket */
				 (LPSTR)&echoReq,			/* buffer */
				 sizeof(ECHOREQUEST),
				 0,							/* flags */
				 (LPSOCKADDR)lpstToAddr,	/* destination */
				 sizeof(SOCKADDR_IN));		/* address length */

	if (nRet == SOCKET_ERROR) 
		metaPing_ReportError("sendto()");
	return (nRet);
}


// Receive incoming data and parse out fields
int metaPing_recvEchoReply(SOCKET s,
						   LPSOCKADDR_IN lpsaFrom,
						   DWORD *rtt,
						   u_short *nSeq) 
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);

	// Receive the echo reply	
	nRet = recvfrom(s,					// socket
					(LPSTR)&echoReply,	// buffer
					sizeof(ECHOREPLY),	// size of buffer
					0,					// flags
					(LPSOCKADDR)lpsaFrom,	// From address
					&nAddrLen);			// pointer to address len

	// Check return value
	if (nRet == SOCKET_ERROR) 
	{
		metaPing_ReportError("recvfrom()");
		return SOCKET_ERROR;
	}

	// Ignore ping replies that this process did not sent out
	if (echoReply.echoRequest.icmpHdr.ID != metaPing_procId)
		return -2;

	// Return rtt and sequence number
	*rtt = GetTickCount() - echoReply.echoRequest.dwTime;
    *nSeq = echoReply.echoRequest.icmpHdr.Seq;
	return 0;   		
}


// Use select() to determine when data is waiting to be read
int metaPing_waitForEchoReply(SOCKET s, int waittime)
{
	struct timeval Timeout;
	fd_set readfds;
	int nRet;

	// untill I find a better way of killing this thread, only
	// wait in periods of maximum 250ms. Better for menu responsiveness ;-)
	if (waittime > 250) waittime = 250;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = waittime / 1000;
    Timeout.tv_usec = waittime % 1000;

	nRet=(select(1, &readfds, NULL, NULL, &Timeout));
	return nRet;

}


DWORD WINAPI metaPing_thread(void) 
{
	int		i,nRet;		// workhorses
	u_short		idx = 0;	// index of current sample in ping averaging table
	int		elapsed;	// waiting time for icmp replies between loops
	DWORD	timestamp;	// begin timestamp just before echo reply waiting loop
	u_short	nSeq;		// icmp ping request sequence number
    DWORD	rtt;		// round trip time in milliseconds
	SOCKET	rawSocket;
	struct sockaddr_in	saDest;
	struct sockaddr_in	saSrc;
	struct hostent	*hp;

	// First look up and cache all ip addresses
	for (i = 0; i < num_servers; ++i)
	{
		if ((hp = gethostbyname(serverlist[i].address)) == NULL)
			serverlist[i].ip_addr = inet_addr(serverlist[i].address); // INADDR_NONE if failed
		else
			serverlist[i].ip_addr = *((u_long FAR *) (hp->h_addr));
		}

	// Create a Raw socket
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == SOCKET_ERROR) 
	{
		metaPing_ReportError("socket()");
		return -1;
	}

    while (!thread_ready)
	{
		// Flood ping all netrek servers at once
		for (i = 0; i < num_servers; ++i)
		{
			if (serverlist[i].status != statusConnecting &&
				serverlist[i].ip_addr != INADDR_NONE)
			{
				// Setup destination socket address
				saDest.sin_addr.s_addr = serverlist[i].ip_addr;
				saDest.sin_family = AF_INET;
				saDest.sin_port = 0;
				
				// Send ICMP echo request
				//printf("\nPinging %s [%s]", serverlist[i].address, inet_ntoa(saDest.sin_addr));
				if (metaPing_sendEchoRequest(rawSocket, &saDest, idx) == SOCKET_ERROR)
					serverlist[i].pkt_rtt[idx] = -1;	// Error
				else
					serverlist[i].pkt_rtt[idx] = -3;	// Waiting for ping reply
			}
			else if (serverlist[i].ip_addr == INADDR_NONE)
				serverlist[i].pkt_rtt[idx] = -2;		// Unknown Host
		}

		// Listen for about one second between for possible replies
		timestamp = GetTickCount();

		while (!thread_ready &&
			(elapsed = GetTickCount() - timestamp) < METAPING_WAITTIME)
		{
			//printf("\nelapsed=%ld  waittime=%ld", elapsed, (METAPING_WAITTIME - elapsed));

			// Use select() to wait for data to be received
			if ((nRet = metaPing_waitForEchoReply(rawSocket, METAPING_WAITTIME - elapsed))
				== SOCKET_ERROR)
			{
				metaPing_ReportError("select()");
				break;
			}
			else if (nRet)
			{
				// Receive reply
				if (!metaPing_recvEchoReply(rawSocket, &saSrc, &rtt, &nSeq))
				{
					//printf("\nReply from: %s: rtt=%ldms seq=%d", inet_ntoa(saSrc.sin_addr), rtt, nSeq);
					for (i = 0; i < num_servers; ++i)
						if (saSrc.sin_addr.s_addr == serverlist[i].ip_addr) 
						{
							serverlist[i].pkt_rtt[nSeq % RTT_AVG_BUFLEN] = rtt;
							break;
						}
				}
			}
		}

		// Refresh the displayed ping stats
		for (i = 0; i < num_servers; ++i) metarefresh(i);

		// Proceed to the next cycle of ping samples
		idx = (idx + 1) % RTT_AVG_BUFLEN;
	}

	if (closesocket(rawSocket) == SOCKET_ERROR)
		metaPing_ReportError("closesocket()");
	return 0;
}

#endif /* METAPING */

#endif /* META */

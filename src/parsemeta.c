/* meta.c
 * 
 * - Nick Trown         May 1993        Original Version. - Andy McFadden
 * May 1993 ?   Connect to Metaserver. - BDyess (Paradise)      ???
 * ug Fixes.  Add server type field. - Michael Kellen   Jan 1995        Don't
 * list Paradise Servers. List empty servers. - James Soutter   Jan 1995
 * Big Parsemeta changes.  Included some Paradise Code.  Added Known Servers
 * Option.  Added option for metaStatusLevel.  Bug Fixes.
 * - Jonathan Shekter Aug 1995 -- changed to read into buffer in all cases,
 * use findfile() interface for cachefiles.
 */

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
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* Constants */

#define BUF     6144
#define LINE    80              /* Width of a meta-server
                                 * line */
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
};

struct servers *serverlist = NULL;      /* The record for each
                                         * server.      */
int num_servers = 0;            /* The number of servers.       */
char *metaWindowName;           /* The window's name.           */



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
    "Active", "CANNOT CONNECT", "DEFAULT SERVER"
};

enum statusTypes
{
    statusOpen = 0, statusWait, statusNobody, statusTout, statusNoConnect,
    statusNull, statusCantConnect, statusDefault
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
                fprintf (stderr, "unknown host '%s'\n", host);
            return (-1);
        }
        else
        {
            addr.sin_addr.s_addr = *(LONG *) hp->h_addr;
        }
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
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

#ifdef DEBUG
    printf ("In parseInput\n");
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


        /* Don't list Paradise Servers  */

        if (slist->typeflag != 'P')
        {

#ifdef DEBUG
            printf ("HOST:%-30s PORT:%-6d %12s %-5d %d %c\n",
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
    int sock;

    if ((stringDefault ("metaServer")) != NULL)
        metaServer = stringDefault ("metaServer");

    metaPort = intDefault ("metaPort", metaPort);

    if ((sock = open_port (metaServer, metaPort, 1)) <= 0)
    {
        fprintf (stderr, "Cannot connect to MetaServer (%s , %d)\n",
                 metaServer, metaPort);
        return 0;
    }

    /* Allocate a buffer and read until full */
    buf = sockbuf = (char *) malloc (BUF);
    while (bufleft > 0 && (len = recv (sock, buf, bufleft, 0)) > 0)
    {
        bufleft -= len;
        buf += len;
#ifdef DEBUG
        printf ("Read %d bytes from Metaserver\n", len);
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
            fprintf (stderr,
                     "Can not write to the metaCache temporary file `%s'.\n",
                     tmpFileName);
            fprintf (stderr, "Meta-server read will not be cached.\n");
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
    metaWindowName = "MetaServer List";

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
        fprintf (stderr,
                 "You must define the .xtrekrc variable `metaCache' in\n");
        fprintf (stderr, "order to use the `show known servers' option.\n");
        return 0;
    }
    else if (!findfile (cacheName, cacheFileName)
             || !(in = fopen (cacheFileName, "r")))
    {
        fprintf (stderr,
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
        printf ("Read %d bytes from Metaserver cache file\n", len);
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
metarefresh (int i,
             W_Color color)
/* Refresh line i in the list */
{
    char buf[LINE + 1];
    struct servers *slist;

    slist = serverlist + i;

    sprintf (buf, "%-40s %14s ",
             slist->address, statusStrings[slist->status]);

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
            strcat (buf, "Bronco");
            break;
        case 'C':
            strcat (buf, "Chaos");
            break;
        case 'I':
            strcat (buf, "INL");
            break;
        case 'S':
            strcat (buf, "Sturgeon");
            break;
        case 'H':
            strcat (buf, "Hockey");
            break;
        case 'F':
            strcat (buf, "Dogfight");
            break;
        default:
            strcat (buf, "Unknown");
            break;
        }
    }

    W_WriteText (metaWin, 0, i, color, buf, strlen (buf), 0);
}


void
metawindow (void)
/* Show the meta server menu window */
{
    int i;

    for (i = 0; i < num_servers; i++)
        metarefresh (i, textColor);

    /* Give the window the right name */
    W_RenameWindow (metaWin, metaWindowName);

    /* Add quit option */
    W_WriteText (metaWin, 0, num_servers, textColor, "Quit", 4, 0);

    /* Map window */
    W_MapWindow (metaWin);
}


static void
metadone (void)
/* Unmap the metaWindow */
{
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

#ifdef DEBUG
    printf ("Got meta window action, y=%d\n", data->y);
#endif
    if ((data->y >= 0) && (data->y < num_servers))
    {
        slist = serverlist + data->y;
        xtrekPort = slist->port;
        if (data->key == W_RBUTTON)     /* Guess at an observer port */
        {
            xtrekPort++;
            printf ("Attempting to observe on port %d...\n", xtrekPort);
        }
        serverName = strdup (slist->address);

#ifdef RSA
        useRsa = slist->RSA_client;
#endif

        metarefresh (data->y, W_Yellow);
        W_Flush ();

        if ((sock = open_port (serverName, xtrekPort, 0)) <= 0)
        {
            slist->status = statusCantConnect;
            metarefresh (data->y, textColor);
        }
        else
        {
            closesocket (sock);
            sprintf (buf, "Netrek  @  %s", serverName);
            W_RenameWindow (baseWin, buf);
            metadone ();
        }
    }
    else if (data->y == num_servers)
        terminate (0);
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

    while (W_IsMapped (metaWin))
    {
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

#endif

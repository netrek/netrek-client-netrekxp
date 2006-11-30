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
/***   11/2006  Bill Balcerski	       Merged COW UDP metaserver, server solicitation,
/**                                    and multicast server packet code
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
#define LINE    80		/* Width of a meta-server line			*/
#define MAXMETABYTES 2048	/* maximum metaserver UDP packet size		*/
static int msock = -1;		/* the socket to talk to the metaservers	*/
static int sent = 0;		/* number of solicitations sent			*/
static int seen = 0;		/* number of replies seen			*/
static int type;		/* type of connection requested			*/
#define RTT_AVG_BUFLEN  5	/* number of samples used for average rtt time	*/

    /* from meta.h of metaserver code */
#define SS_WORKING 0
#define SS_QUEUE 1
#define SS_OPEN 2
#define SS_EMPTY 3
#define SS_NOCONN 4
#define SS_INIT 5
    /* not a real metaserver number, but overcomes a limitation of dropping text */
    /* description of sp->why_dead */
#define SS_TOUT 6

/* Local Types */

struct servers
{
    char    address[LINE];	/* host name or ip address of server	*/
    int     port;
    int     age;		/* age in seconds as received		*/
    time_t  when;		/* date time this record received	*/
    int     refresh;		/* data needs redisplaying		*/
    int     lifetime;		/* remaining cache life of entry        */
    int     players;
    int     status;
    char    typeflag;
    char    comment[LINE];
#ifdef METAPING
    u_long ip_addr;			/* Cache the ip address after DNS lookup */
    DWORD pkt_rtt[RTT_AVG_BUFLEN];	/* store last # ping samples for avg rtt */
					/* -1:init, -2:unknown host, -3:timeout  */
					/* >=0:round trip time in ms             */
#endif
};

struct servers *serverlist = NULL;	/* The record for each server.  */
static int num_servers = 0;		/* The number of servers.       */
int metaHeight = 0;			/* The number of list lines.	*/
char *metaWindowName;			/* The window's name.           */
int statusLevel;

#ifdef METAPING
u_short metaPing_procId = 0;		/* Process id helps identify own ping replies */
int thread_ready = 0;			/* Awful way of letting the ping thread exit */
HANDLE hThread = NULL;
#endif


/* The status strings:  The order of the strings up until statusNull is
 * important because the meta-client will display all the strings up to a
 * particular point.
 * 
 * The strings after statusNull are internal status types and are formatted
 * separately from the other strings.
 * 
 * The string corresponding to "statusNull" is assigned to those servers which
 * have "statusNobody" or earlier strings in old, cached, meta-server data. */

char *statusStrings[] =
{   "OPEN:", "Wait queue:", "Nobody", "Timed out", "No connection",
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

int
metablock (char *host)
/* Block connections to known servers not willing to handle default logins */
{
//    if ( ((!strcmp(login, "new-be-nice") || !strcmp(login, "netrek-player")) && (strstr(host,".antinewbie.server") != NULL))
//        return 1;
    return 0;
}

static void
parseInput (char *in,
            FILE * out)
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

            *(point++) = (char) rtn;
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
            size_t size;
            
            max_servers += 5;
            size = sizeof(struct servers) * max_servers;
            serverlist = (struct servers *) realloc (serverlist, size);
        }

        slist = serverlist + num_servers;



        /* Is this a line we want? */

        if (sscanf (line, "-h %s -p %d %d %*d",
                    slist->address, &(slist->port), &(slist->age)) != 3)
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
        slist->typeflag = *(point - 1);

        strcpy(slist->comment, "");
    
#ifdef METAPING
	/* Initialize the ping rtt fields */
	for (i = 0; i < RTT_AVG_BUFLEN; ++i )
		slist->pkt_rtt[i] = (unsigned long) -1;
#endif

        /* Don't list servers we cannot use */
        if (slist->typeflag != 'P' && !metablock(slist->address))
        {
#ifdef DEBUG
            LineToConsole ("HOST:%-30s PORT:%-6d %12s %-5d %d %c\n",
                            serverlist[num_servers].address,
                            serverlist[num_servers].port,
                            statusStrings[serverlist[num_servers].status],
                            serverlist[num_servers].players,
                            serverlist[num_servers].typeflag);
#endif

            ++num_servers;
        }
    }
}

static int ReadMetasSend()
{
  char *metaservers;		/* our copy of the metaserver host names */
  char *token;			/* current metaserver host name          */
  struct sockaddr_in address;	/* the address of the metaservers	 */
 
  /* host names of metaservers, default in data.c, comma delimited */
  if ((stringDefault("metaServer")) != NULL)
    metaServer = stringDefault("metaServer");

  /* port number of metaservers, unlikely to change, not a list */
  metaPort = intDefault("metaPort", metaPort);

  /* whether to report everything that happens */
  metaVerbose = booleanDefault("metaVerbose", metaVerbose);

  /* create the socket */
  if (msock < 0) {
    msock = socket(AF_INET, SOCK_DGRAM, 0);
    if (msock < 0) { perror("ReadMetasSend: socket"); return 0; }
    
    /* bind the socket to any address */
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family      = AF_INET;
    address.sin_port        = 0;
    if (bind(msock,(struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("ReadMetasSend: bind");
      closesocket(msock);
      return 0;
    }
  }

  /* send request to a multicast metaserver on local area network */
  address.sin_family = AF_INET;
  address.sin_port = htons(metaPort);
  address.sin_addr.s_addr = inet_addr("224.0.0.1");
  if (metaVerbose)
    LineToConsole ("Requesting player list from nearby servers on %s\n",
                    inet_ntoa(address.sin_addr));
  if (sendto(msock, "?", 1, 0, (struct sockaddr *)&address,
	     sizeof(address)) < 0) {
    perror("ReadMetasSend: sendto");
  } else {
    sent++;
  }

  /* try each of the comma delimited metaserver host names */
  metaservers = strdup(metaServer);
  token = strtok(metaservers,",");

  while (token != NULL) {
    /* compose the address structure */
    address.sin_family = AF_INET;
    address.sin_port = htons(metaPort);

    /* skip any blanks */
    while (*token == ' ') token++;

    /* attempt numeric translation first */
    if ((address.sin_addr.s_addr = inet_addr(token)) == -1) {
      struct hostent *hp;
        
      /* then translation by name */
      if ((hp = gethostbyname(token)) == NULL) {
        /* if it didn't work, return failure and warning */
        LineToConsole ("Cannot resolve host %s, check for DNS problems?\n", token);
      } else {
        int i;

        /* resolution worked, send a query to every metaserver listed */
        for(i=0;;i++) {

	  /* check for end of list of addresses */
	  if (hp->h_addr_list[i] == NULL) break;
	  address.sin_addr.s_addr = *(long *) hp->h_addr_list[i];
	  if (metaVerbose)
            LineToConsole ("Requesting player list from metaserver %s at %s\n",
                            token, inet_ntoa(address.sin_addr));
	  if (sendto(msock, "?", 1, 0, (struct sockaddr *)&address,
		sizeof(address)) < 0) {
	    perror("ReadMetasSend: sendto");
	  } else {
	    sent++;
	  }
        }
      }
    } else {
      /* call to inet_addr() worked, host name is in IP address form */
      if (metaVerbose)
        LineToConsole ("Requesting player list from metaserver %s\n",
                        inet_ntoa(address.sin_addr));
      if (sendto(msock, "?", 1, 0, (struct sockaddr *)&address,
	sizeof(address)) < 0) {
        perror("ReadMetasSend: sendto");
      } else {
        sent++;
      }
    }
    /* look for next host name in list */
    token = strtok(NULL,",");
  } /* while (token != NULL) */

  metaWindowName = "Netrek XP 2006 MetaServer List";
  return sent;
}

/* allocate or extend the server list */
static void grow(int servers)
{
  int size;
  if (serverlist == NULL) {
    size = sizeof(struct servers) * servers;
    serverlist = (struct servers *) malloc(size);
  } else {
    size = sizeof(struct servers) * ( servers + num_servers );
    serverlist = (struct servers *) realloc(serverlist, size);
  }
}

static struct servers *server_find(char *address, int port)
{
  int j;

  for(j=0;j<num_servers;j++) {
    struct servers *sp = serverlist + j;
    if ((!strcmp(sp->address, address)) && (sp->port == port)) {
      return sp;
    }
  }
  return NULL;
}

static void version_r(struct sockaddr_in *address) {
  char *p;
  int servers, i;
  time_t now = time(NULL);

  /* number of servers */
  p = strtok(NULL,"\n");
  if (p == NULL) return;
  servers = atoi(p);

  /* sanity check on number of servers */
  if (servers > 2048) return;
  if (servers < 0) return;

  if (metaVerbose) 
    LineToConsole ("Metaserver at %s responded with %d server%s\n",
		   inet_ntoa(address->sin_addr),
		   servers,
		   servers == 1 ? "" : "s" );

  if (servers == 0) return;

  /* for each server listed by this metaserver packet */
  for(i=0;i<servers;i++) {
    struct servers *sp = NULL;
    char *host, type;
    int port, status, age, players, queue, throwaway;
    int tempstatus, tempplayers;
      
    throwaway = 0;

    host = strtok(NULL,",");		/* hostname */
    if (host == NULL) continue;

    p = strtok(NULL,",");		/* port */
    if (p == NULL) continue;
    port = atoi(p);

    p = strtok(NULL,",");		/* status */
    if (p == NULL) continue;
    status = atoi(p);

    p = strtok(NULL,",");		/* age (of data in seconds) */
    if (p == NULL) continue;
    age = atoi(p);

    p = strtok(NULL,",");		/* players */
    if (p == NULL) continue;
    players = atoi(p);

    p = strtok(NULL,",");		/* queue size */
    if (p == NULL) continue;
    queue = atoi(p);

    p = strtok(NULL,"\n");		/* server type */
    if (p == NULL) continue;
    type = p[0];

    /* Metaserver and client define status level differently, must convert
       status so that client option metaStatusLevel works properly.  And
       temporarily record player field as well.  */
    switch (status) {
    case SS_QUEUE:
      tempstatus = statusWait;
      tempplayers = queue;
      break;
    case SS_OPEN:
      tempstatus = statusOpen;
      tempplayers = players;
      break;
    case SS_EMPTY:
      tempstatus = statusNobody;
      tempplayers = 0;
      break;
    case SS_TOUT:
      tempstatus = statusTout;
      tempplayers = 0;
      break;
    case SS_NOCONN:			/* no connection */
    case SS_WORKING:		/* metaserver should not return this */
    case SS_INIT:			/* nor this */
    default:
      tempstatus = statusNoConnect;
      tempplayers = 0;
      break;
    }

    /* ignore servers based on converted status */
    if (tempstatus > statusLevel)
      throwaway++;

    /* ignore paradise servers */
    if (type == 'P') throwaway++;

    /* if it's to be thrown away, do not add this server, skip to next */
    if (throwaway) continue;

    /* find in current server list? */
    sp = server_find(host, port);

    /* if it was found, check age */
    if (sp != NULL) {
      if ((now-age) < (sp->when-sp->age)) {
	sp->age = (int)now - (int)(sp->when-sp->age);
	sp->when = now;
	sp->refresh = 1;
	sp->lifetime = 20;
	continue;
      } else {
	sp->age = age;
	sp->when = now;
	sp->lifetime = 20;
      }
    } else {
      /* not found, store it at the end of the list */
      grow(1);
      sp = serverlist + num_servers;
      num_servers++;
      strncpy(sp->address,host,LINE);
      sp->port = port;
      sp->age = age;
      sp->when = now;
      sp->lifetime = 4;
    }
    sp->refresh = 1;

    /* Use converted status and player values */
    sp->status = tempstatus;
    sp->players = tempplayers;

    sp->typeflag = type;
    strcpy(sp->comment, "");
#ifdef METAPING
    /* Initialize the ping rtt fields */
    for (i = 0; i < RTT_AVG_BUFLEN; ++i )
      sp->pkt_rtt[i] = (unsigned long) -1;
#endif
  }
}

static void version_s(struct sockaddr_in *address)
{
  char *p;
  time_t now = time(NULL);
  int throwaway = 0;
  char type;
  char *comment, *host;
  int port, ports, players, queue, i;
  struct servers *sp;

  /* use return address on packet as host address for this server,
  since it isn't practical for the server to know it's own address; as
  is the case with multihomed machines */
  host = inet_ntoa(address->sin_addr);

  if (metaVerbose) 
    LineToConsole ("Server at %s responded\n", host);

  p = strtok(NULL,",");	/* server type */
  if (p == NULL) return;
  type = p[0];
  
  /* ignore paradise servers */
  if (type == 'P') return;
  
  p = strtok(NULL,",");		/* comment */
  if (p == NULL) return;
  comment = strdup(p);

  p = strtok(NULL,",");		/* number of ports */
  if (p == NULL) return;
  ports = atoi(p);

  // TODO: accept more than one port reply
  
  p = strtok(NULL,",");		/* port */
  if (p == NULL) return;
  port = atoi(p);
  
  p = strtok(NULL,",");		/* players */
  if (p == NULL) return;
  players = atoi(p);

  p = strtok(NULL,",");		/* queue size */
  if (p == NULL) return;
  queue = atoi(p);

  /* find in current server list? */
  sp = server_find(host, port);

  /* if it was not found, add it to the end of the list */
  if (sp == NULL) {
    grow(1);
    sp = serverlist + num_servers;
    num_servers++;
  }

  /* add or update the entry */
  strncpy(sp->address, host, LINE);
  sp->port = port;
  sp->age = 0;
  sp->when = now;
  sp->refresh = 1;
  sp->lifetime = 20;
  sp->players = players;
  sp->status = statusOpen;
  sp->typeflag = type;
  strncpy(sp->comment, comment, LINE);
  free(comment);
#ifdef METAPING
  /* Initialize the ping rtt fields */
  for (i = 0; i < RTT_AVG_BUFLEN; ++i )
    sp->pkt_rtt[i] = (unsigned long) -1;
#endif
}

static int ReadMetasRecv(int x)
{
  struct sockaddr_in address;	/* the address of the metaservers	 */
  int length;			/* length of the address		 */
  int bytes;			/* number of bytes received from meta'   */
  fd_set readfds;		/* the file descriptor set for select()	 */
  struct timeval firsttimeout;	/* timeout for first select() call	 */
  struct timeval timeout;	/* timeout for select() call		 */
  char packet[MAXMETABYTES];	/* buffer for packet returned by meta'	 */
  int isawsomething = 0;        /* have I seen a response at all?        */ 

  /* now await and process replies */
  while(1) {
    char *p;

    FD_ZERO(&readfds);
    if (msock >= 0) FD_SET(msock, &readfds);
    firsttimeout.tv_sec=4;
    firsttimeout.tv_usec=0;
    timeout.tv_sec=0;
    timeout.tv_usec=250000;

//  if (x != -1) FD_SET(x, &readfds);
    if ((select(FD_SETSIZE, &readfds, NULL, NULL,
               (x == -1) ? &firsttimeout : &timeout)) < 0) {
//	       (x != -1) ? NULL : &timeout)) < 0) {
      perror("ReadMetasRecv: select");
      return 0;
    }

    /* if x activity, return immediately */
//  if (x != -1 && FD_ISSET(x, &readfds)) return 0;
    if (msock < 0) return 0;

    /* if the wait timed out, then we give up */
    if (!FD_ISSET(msock, &readfds)) {
      if(isawsomething)
        return 1;          /* I do have new metaserver data */
      else
        return 0;          /* I don't have metaserver data at all */
    }

    /* so we have data back from a metaserver or server */
    length = sizeof(address);
    bytes = recvfrom(msock, packet, MAXMETABYTES, 0, (struct sockaddr *)&address,
	&length );
    if (bytes < 0) {
      /* Don't return on the initial call, allow time for other metaservers */
      if (x != -1) {
        perror("ReadMetasRecv: recvfrom");
        return 0;
      }
    }
    else
      isawsomething++;

    /* terminate the packet received */
    packet[bytes] = 0;
#ifdef DEBUG
    LineToConsole("%s", packet);
#endif /* DEBUG */

    /* process the packet, updating our server list */

    /* version identifier */
    p = strtok(packet,",");
    if (p == NULL) continue;

    switch (p[0]) {
    case 'r': version_r(&address); seen++; break;
    case 's': version_s(&address); seen++; break;
    }

    /* finished processing the packet */

    /* if this is the first call, return on first reply, for sizing list */
    if (x == -1 && isawsomething) return 1;

    /* if we have seen the same number of replies to what we sent, end */
    if (sent == seen) return 1;
  }
}

static void SaveMetasCache()
{
  FILE *cache;
  char cacheFileName[PATH_MAX];
  char tmpFileName[PATH_MAX];
  char *cacheName;
  int len;

  cacheName = stringDefault("metaUDPCache");
  /* overwrite existing file if possible */
  if (cacheName && !findfile(cacheName, cacheFileName))
   strcpy(cacheFileName, cacheName);

  if (cacheName)
  {
      len = strlen(cacheFileName);
      strcpy(tmpFileName, cacheFileName);

      /* create a temporary file with roughly the same name */
      if ((cacheFileName[len - 1] == 'T') || (cacheFileName[len - 1] == 't'))
        tmpFileName[len-1] = 'R';
      else
        tmpFileName[len-1] = 'T';
      
      cache = fopen(tmpFileName, "w");

      if (cache == NULL)
      {
      	  LineToConsole ("Cannot create a metaUDPCache temporary file `%s'\n",
                          tmpFileName);
          LineToConsole("Meta-server read will not be cached.\n");
      }
  }
  else
  {
      cache = NULL;
  }


  if (cache != NULL)
  {

      fwrite(&statusLevel, sizeof(statusLevel), 1, cache);
      fwrite(&num_servers, sizeof(num_servers), 1, cache);
      fwrite(serverlist, sizeof(struct servers), num_servers, cache);

      fclose(cache);

      /* Can't rename file to existing name under NT */
#ifdef _MSC_VER
      _unlink(cacheName);
#else
      unlink(cacheName);
#endif
  
      if (rename(tmpFileName, cacheName) == -1)
	perror("Could not rename new cache file");
  }

}

static void LoadMetasCache()
{
  FILE *cache;
  char *cacheName;
  char cacheFileName[PATH_MAX];
  int  i;

  cacheName = stringDefault("metaUDPCache");

  if(!cacheName)
  {
      num_servers = 0;
      return;
  }

  findfile(cacheName, cacheFileName);

  cache = fopen(cacheFileName, "r");
  if (cache == NULL) 
  { 
      num_servers = 0; 
      return; 
  }
 
  /* ignore the cache if user changed statusLevel */
  fread(&i, sizeof(i), 1, cache);
  if (i != statusLevel)
  {
      num_servers = 0;
      fclose(cache);
      return; 
  }
 

  /* read the server list into memory from the file */
  fread(&num_servers, sizeof(num_servers), 1, cache);
  serverlist = (struct servers *) malloc(sizeof(struct servers)*num_servers);
  fread(serverlist, sizeof(struct servers), num_servers, cache);
  fclose(cache);
  
  /* hunt and kill old server lines from cache */
  for(i=0;i<num_servers;i++)
  {
      int j;

      /* mark each server as needing to be refreshed */
      serverlist[i].refresh = 1;

      /* skip the deletion below if the entry was received recently */
      if (serverlist[i].lifetime-- > 0) continue;

      /* delete this entry by moving the ones above down */
      for(j=i;j<num_servers-1;j++)
      {
	  memcpy(&serverlist[j],&serverlist[j+1],sizeof(struct servers));
      }
      
      /* adjust the current entry pointer, limit, and resize the memory */
      i--;
      num_servers--;
      serverlist = 
	(struct servers *) realloc(serverlist, 
				   sizeof(struct servers) * ( num_servers ));
  }
}

static int
ReadFromMeta ()
/* Read from the meta-server.  Return TRUE on success and FALSE on failure. */
{
    FILE *out;
    char *metaservers;		/* our copy of the metaserver host names */
    char *token;		/* current metaserver host name          */
    char cacheFileName[PATH_MAX];
    char tmpFileName[PATH_MAX];
    char *sockbuf, *buf;
    int bufleft = BUF - 1;
    int len = 0;
    int sock = 0;

    if (stringDefault ("metaServer") != NULL)
        metaServer = stringDefault ("metaServer");

    metaPort = intDefault ("metaPort", metaPort);
    
    /* try each of the comma delimited metaserver host names */
    metaservers = strdup(metaServer);
    token = strtok(metaservers,",");

    do
    {
    	/* skip any blanks */
        while (*token == ' ') token++;

        LineToConsole ("Calling %s on port %d\n", token, metaPort);

        sock = open_port (token, metaPort, 0);

        if (sock <= 0)
        {
            LineToConsole ("Cannot connect to MetaServer (%s port %d)\n",
                            token, metaPort);
        }

        /* look for next host name in list */
        token = strtok(NULL,",");

    } while (sock <= 0 && token != NULL);

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

    metaCache = stringDefault ("metaCache");
    if (metaCache && !findfile (metaCache, cacheFileName))
        strcpy (cacheFileName, metaCache);      /* overwrite existing file if possible */

    if (metaCache)
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
            LineToConsole ("Cannot write to the metaCache temporary file `%s'.\n",
                            tmpFileName);
            LineToConsole ("Meta-server read will not be cached.\n");
        }
    }
    else
    {
        out = NULL;
    }

    parseInput (sockbuf, out);

    if (out != NULL)
    {
        fclose (out);

        /* Can't rename file to existing name under NT */
#ifdef _MSC_VER
        _unlink (metaCache);
#else
        unlink (metaCache);
#endif
        if (rename (tmpFileName, metaCache) == -1)
            perror ("Could not write to cache file");
    }

    free (sockbuf);
    metaWindowName = "Netrek XP 2006 MetaServer List";

    return 1;
}


static int
ReadFromCache ()
/* Read from the cache.  Return TRUE on success and FALSE on failure. */
{
    FILE *in;
    struct servers *slist;
    char *sockbuf, *buf;
    int bufleft = BUF - 1;
    int len;
    char cacheFileName[PATH_MAX];

    metaCache = stringDefault ("metaCache");

    if (!metaCache)
    {
        LineToConsole ("You must define the netrekrc variable `metaCache' in\n");
        LineToConsole ("order to use the `show known servers' option.\n");
        return 0;
    }
    else if (!findfile (metaCache, cacheFileName)
             || !(in = fopen (cacheFileName, "r")))
    {
        LineToConsole (
                 "The metaCache file `%s' is empty or not accessable.\n",
                 metaCache);
        return 0;
    }


    /* Allocate a buffer and read until full. Why do we
       go through this silly stuff? Because sockets are
       not file handles on all versions of Windows */
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

    parseInput (sockbuf, NULL);


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
/* Read and Parse the metaserver information, either from the metaservers
 * by UDP (1), from a single metaserver by TCP (3), or from the cache (2).
 * 
 * NOTE: This function sets the variable "num_servers" which is
 * used in newwin() to set the height of the meta-server window.
 */
{
    metaStatusLevel = intDefault ("metaStatusLevel", metaStatusLevel);

    if (metaStatusLevel < 0)
        metaStatusLevel = 0;
    else if (metaStatusLevel >= statusNull)
        metaStatusLevel = statusNull - 1;

    statusLevel = metaStatusLevel;

    type = metaType;
    switch (type)
    {
        case 1:
            ReadMetasSend();
	    LoadMetasCache();
	    if (num_servers == 0) ReadMetasRecv(-1);
	    if (num_servers != 0) {
	    	/* Allocate 4 spots for header/refresh/quit/link, and 3 slots for additional servers */
	        metaHeight = num_servers + 7;
	    } else {
	        LineToConsole("Warning: no response from metaservers, are you firewalled?\n"
		              "         (no reply to probe on UDP port %d)\n", metaPort);
	        /* Allocate 4 spots for header/refresh/quit/link, and 8 server slots */
	        metaHeight = num_servers + 12;
	    }
            return;
	    break;
        case 2:
	    if (ReadFromCache() || ReadFromMeta()) 
	    {
                /* Allocate 3 spots for header/quit/link */
	        metaHeight = num_servers + 3;
	        return;
	    }
	    terminate(0);
	    break;
        case 3:
	    if (ReadFromMeta() || ReadFromCache()) 
            {
                /* Allocate 3 spots for header/quit/link */
                metaHeight = num_servers + 3;
                return;
            }
	    terminate(0);
	    break;
    }
}


static void
metarefresh (int i,
             W_Color color)
/* Refresh line i in the list */
{
    struct servers *sp;
    char comment[LINE];
    char buf[LINE + 1];

#ifdef METAPING
    DWORD lag = 0;
    int idx, replies = 0;
#endif

    /* can't say a thing if line is beyond server list */
    if (i >= num_servers) {
        /* but we can at least blank the line shown */
        if (i < metaHeight-3)
            W_WriteText(metaWin, 0, i+1, color, "", 0, 0);
        return;
    }

    sp = serverlist + i;
    
    if (metablock(sp->address))
        return;

#ifdef METAPING
    if (metaPing)
    {
      if (strlen(sp->comment))
      {
        strncpy(comment, sp->comment, 34);
        comment[34] = '\0';
        sprintf(buf, "%-34s ", comment);
      }
      else
	sprintf (buf, "%-34s ", sp->address);
    }
    else
    {
#endif
      if (strlen(sp->comment))
      {
        strncpy(comment, sp->comment, 40);
        comment[40] = '\0';
        sprintf(buf, "%-40s ", comment);
      }
      else
	sprintf (buf, "%-40s ", sp->address);
#ifdef METAPING
    }
#endif

    sprintf(buf + strlen (buf), "%14s ", statusStrings[sp->status]);

    if (sp->status == statusConnecting) color = W_Yellow;

    if (sp->status <= statusNull)
    {
        if (sp->status == statusOpen || sp->status == statusWait)
        {
            /* Don't print the number of players if nobody is playing */
            sprintf (buf + strlen (buf), "%-5d  ", sp->players);
        }
        else
        {
            strcat (buf, "       ");
        }

        switch (sp->typeflag)
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
        if (type == 1)
	{
	    int age = sp->age;
	    char *units;

	    if (age > 86400)
	    {
	        age = age / 86400;
	        units = "d";
	    }
	    else if (age > 3600)
	    {
	        age = age / 3600;
	        units = "h";
	    }
	    else if (age > 90)
	    {
	        age = age / 60;
	        units = "m";
	    }
	    else
	    {
                units = "s";
	    }
	    sprintf(buf + strlen(buf), " %4d%s", age, units);
	}
	else
	{
	    strcat (buf, "      ");
	}
    }
    else {
        /* Pad string with spaces for formatting purposes */
        strcat (buf, "                     ");
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

    W_WriteText (metaWin, 0, i+1, color, buf, strlen (buf), 0);
    sp->refresh = 0;
}


void
metawindow (void)
/* Show the meta server menu window */
{
    int i;
    char *header;

#ifdef METAPING
    metaPing = booleanDefault ("metaPing", metaPing);

    if (metaPing) {
        if (type == 1) {
            header = "Server ----------------------------------- Status ------ Type ----- Age - Ping";
        }
        else {
            header = "Server ----------------------------------- Status ------ Type ----------- Ping";
        }
    }
    else {
#endif
        if (type == 1) {
            header = "Server ----------------------------------------- Status ------ Type ------ Age";
        }
        else {
            header = "Server ----------------------------------------- Status ------ Type ----------";
        }
#ifdef METAPING
    }
#endif

    W_WriteText(metaWin, 0, 0, W_Yellow, header, strlen(header), 0);
    
    for (i = 0; i < metaHeight; i++)
        metarefresh (i, textColor);

    /* Give the window the right name */
    W_RenameWindow (metaWin, metaWindowName);

    /* Add refresh option */
    if (type == 1)
        W_WriteText(metaWin, 0, metaHeight-3, W_Yellow, "Refresh", 7, 0);

    /* Add quit option */
    W_WriteText (metaWin, 0, metaHeight-2, W_Yellow, "Quit", 4, 0);

    /* Additional Help Options */
    W_WriteText (metaWin, 0, metaHeight-1, W_Yellow, 
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
    if (type == 1)
        SaveMetasCache();
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
    if ((data->y > 0) && (data->y <= num_servers))
    {
        slist = serverlist + data->y - 1;
        xtrekPort = slist->port;
        if (data->key == W_RBUTTON)     /* Guess at an observer port */
        {
            xtrekPort++;
            LineToConsole ("Attempting to observe on port %d...\n", xtrekPort);
            metarefresh(data->y - 1, W_Cyan);
        }
        else
            metarefresh(data->y - 1, W_Yellow);

        serverName = strdup (slist->address);
	serverType = metaGetServerType (slist->typeflag);

	slist->status = statusConnecting;

        LineToConsole ("Checking %s on port %d\n", serverName, xtrekPort);

        if ((sock = open_port (serverName, xtrekPort, 0)) <= 0)
        {
            slist->status = statusCantConnect;
            metarefresh (data->y - 1, W_Red);
        }
        else
        {
            metarefresh(data->y - 1, W_Green);
            closesocket (sock);
            sprintf (buf, "Netrek  @  %s", serverName);
            W_RenameWindow (baseWin, buf);
            metadone ();
        }
    }
    else if (data->y == (metaHeight-3) && type == 1) /* Refresh selected */
    {
        W_WriteText(metaWin, 0, metaHeight-3, W_Red, "Asking for refresh from metaservers and nearby servers", 54, 0);
        ReadMetasSend();
    }
    else if (data->y == (metaHeight-2)) /* Quit selected */
    {
	W_WriteText (metaWin, 0, metaHeight-2, W_Yellow, "Quit", 4, 0);
	metadone();
        terminate (0);
    }
    else if (data->y == (metaHeight-1)) /* Links Line */
    {
	x = data->x / W_Textwidth;
	if (x >= 0 && x <= 19)		/* Netrek Home Page */
		ShellExecute (NULL, "open", "http://www.netrek.org", NULL, NULL, SW_SHOWNORMAL);
	else if (x >= 21 && x <= 35)	/* Newbie Manual */
		ShellExecute (NULL, "open", "http://genocide.netrek.org/beginner/newbie.php", NULL, NULL, SW_SHOWNORMAL);
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

	metaPing_procId = (unsigned short) (_getpid() & 0xFFFF);

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
    	if (type == 1)
	{
	    do
            {
	        if (ReadMetasRecv(W_Socket())) metawindow();
	    } while (!W_EventsPending());
	}
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
	answer = (unsigned short) (~sum);				/* truncate to 16 bits */
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

	FD_ZERO(&readfds);
	if (s >= 0) FD_SET(s, &readfds);

	// until I find a better way of killing this thread, only
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
		return (unsigned long) -1;
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
					serverlist[i].pkt_rtt[idx] = (unsigned long) -1;	// Error
				else
					serverlist[i].pkt_rtt[idx] = (unsigned long) -3;	// Waiting for ping reply
			}
			else if (serverlist[i].ip_addr == INADDR_NONE)
				serverlist[i].pkt_rtt[idx] = (unsigned long) -2;		// Unknown Host
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
		for (i = 0; i < num_servers; ++i) metarefresh(i, textColor);

		// Proceed to the next cycle of ping samples
		idx = (unsigned short) ((idx + 1) % RTT_AVG_BUFLEN);
	}

	if (closesocket(rawSocket) == SOCKET_ERROR)
		metaPing_ReportError("closesocket()");
	return 0;
}

#endif /* METAPING */

#endif /* META */

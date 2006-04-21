/******************************************************************************/
/***  File:  check.c                                                        ***/
/***  Function:                                                             ***/
/***  Revisions:                                                            ***/
/***     9/20/93  VEG changed last while loop of file to improve clarity.   ***/
/***     Also re-indented and spaced to make source easier to read.         ***/
/***     Lastly, changed size of declared character buffer from 4096 + 1    ***/
/***     to just 4096, since read only reads up to the number of characters ***/
/***     requested.  Decided to make this a local define, BUF_SIZE.         ***/
/***                                                                        ***/
/******************************************************************************/

/******************************************************************************/
/***                             Include Files                              ***/
/******************************************************************************/
#include "config.h"
#include "copyright.h"
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>
#include <time.h>
#include <errno.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

extern void terminate (int error);

/******************************************************************************/
/***     Size lof character data buffer                                     ***/
/******************************************************************************/
#define  BUF_SIZE    ( 4096 )

/******************************************************************************/
/***                          Static Character Buffer                       ***/
/******************************************************************************/
static char buf[BUF_SIZE];

/******************************************************************************/
/***  check()  will obtain the connection to the server and will forever    ***/
/***  read server data into a 4096 byte buffer area, unless an error is     ***/
/***  detected.                                                             ***/
/******************************************************************************/
void
check (void)
{
    struct sockaddr_in addr;
    struct hostent *hp;
    int cc, sock;

    /* get numeric form */
    addr.sin_addr.s_addr = inet_addr (serverName);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        if ((hp = gethostbyname (serverName)) == NULL)
        {
            LineToConsole ("unknown host '%s'\n", serverName);
            terminate (0);
        }
        else
        {
            addr.sin_addr.s_addr = *(LONG *) hp->h_addr;
        }
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons ((u_short) xtrekPort);
    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror ("socket");
        terminate (0);
    }
    LineToConsole ("checking %s on port %d\n", serverName, xtrekPort);
    if (connect (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
        perror ("connect");
        close (sock);
        terminate (0);
    }

    while ((cc = recv (sock, buf, BUF_SIZE, 0)) > 0)
    {
        fwrite (buf, cc, 1, stdout);
    }
    if (cc < 0)
    {
        perror ("read");
    }
    close (sock);
    terminate (0);
}

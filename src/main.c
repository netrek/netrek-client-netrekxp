/******************************************************************************/
/***  File:  main.c
/***
/***  Function:
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "version.h"
#include "copyright.h"
#include "cowapi.h"
#include "Wlib.h"
#include "struct.h"
#include "proto.h"
#include "data.h"
#include "parsemeta.h"

char *servertmp = NULL;


/******************************************************************************/
/***  printUsage()
/******************************************************************************/
static void
printUsage (char *prog)
{
    LineToConsole ("%s %s\n", version, mvers);
    LineToConsole ("Usage: %s [options] [display-name]\n", prog);

    LineToConsole ("Options:\n");
    LineToConsole (" [-h servername]     specify a server\n");
    LineToConsole (" [-p port number]    specify a port to connect to\n");
    LineToConsole (" [-r defaultsfile]   specify defaults file\n");
    LineToConsole (" [-s socketnum]      specify listen socket port for manual start\n");
    LineToConsole (" [-U udp_port]       specify client UDP port (useful for some firewalls)\n");
#ifdef RECORDGAME
    LineToConsole (" [-F filename]       view recorded game from 'filename'\n");
    LineToConsole (" [-f filename]       record game into 'filename'\n");
#endif
    LineToConsole (" [-l filename]       record messages into 'filename'\n");

    LineToConsole (" [-A]   character password\n");
    LineToConsole (" [-C]   character name\n");

#ifdef GATEWAY
    LineToConsole (" [-H]   specify host (via gateway)\n");
#endif

#ifdef RSA
    LineToConsole (" [-o]   use old style binary verification\n");
    LineToConsole (" [-R]   use RSA binary verification\n");
#endif

#ifdef PACKET_LOG
    LineToConsole (" [-P]   log server packets, repeat for increased information\n");
#endif

    LineToConsole (" [-c]   run players check on the server\n");

#ifdef META
    LineToConsole (" [-m]   check metaserver for active servers\n");
    LineToConsole (" [-k]   display known servers\n");
#endif

    LineToConsole (" [-D]   output debug info\n");
    LineToConsole (" [-n]   show console window\n");
    LineToConsole (" [-u]   show usage\n");
    LineToConsole (" [-v]   display client version info\n");

}

/******************************************************************************/
/***  main() or main2()
/******************************************************************************/
void
main2 (int argc,
       char *argv[])
{
    int usage = 0;
    int err = 0;
    int inplayback = 0;
	int	hideConsole = 1;
    char *name, *ptr;
    int i;

#ifdef GATEWAY
    int hset = 0;

#endif
    int xtrekPort = -1;

    name = argv[0];

#ifdef GATEWAY
    netaddr = 0;
#endif


    pseudo[0] = defpasswd[0] = '\0';

    for (i = 1; i < argc; i++)
    {
        ptr = argv[i];

        if (*ptr == '-')
            ptr++;
        else
            break;

        while (*ptr)
        {
            switch (*ptr)
            {

            case 'C':          /* character name */
                if (i < argc)
                {
                    (void) STRNCPY (pseudo, argv[i], sizeof (pseudo));
                    i++;
                }
                else
                    usage++;
                break;

            case 'A':          /* authorization password */
                if (i < argc)
                {
                    (void) STRNCPY (defpasswd, argv[i], sizeof (defpasswd));
                    i++;
                }
                else
                    usage++;
                break;

            case 'u':           /* program usage */
                usage++;
                hideConsole = 0;
                break;

            case 'c':           /* run ck_players */
                checking = 1;
                hideConsole = 0;
                break;

            case 's':           /* listen socket number */
                if (i < argc)
                {
                    xtrekPort = atoi (argv[i + 1]);
                    passive = 1;
                    i++;
                }
                else
                    usage++;
                break;
#ifdef RECORDGAME
            case 'F':           /* playback recorded cambot */
                inplayback = 1;
                /* No break */

            case 'f':           /* record to cambot file */
                if (i < argc)
                {
                    recordFileName = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;
#endif
            case 'l':           /* log to file */
                if (i < argc)
                {
                    logFileName = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;

            case 'p':           /* port to connect to */
                if (i < argc)
                {
                    xtrekPort = atoi (argv[i + 1]);
                    i++;
                }
                else
                    usage++;
                break;

            case 'd':
                if (i < argc)
                {
                    display_host = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;

#ifdef META
            case 'm':           /* show metaserver window */
                if (usemeta && usemeta != 1)
                {
                    fputs ("The options -k and -m are mutually exclusive\n",
                           stderr);
                    err++;
                }
                usemeta = 1;
                break;

            case 'k':           /* show metacache window */
                if (usemeta && usemeta != 2)
                {
                    fputs ("The options -k and -m are mutually exclusive\n",
                           stderr);
                    err++;
                }
                usemeta = 2;
                break;
#endif

#ifdef RSA
            case 'o':               /* try old binary verification */
                useRsa = -1;        /* will be reset leter, set
                                     * negative here * to flag
                                     * that it should override
                                     * xtrekrc */
                LineToConsole ("Using standard binary verification\n");
                break;

            case 'R':               /* try RSA verification */
                useRsa = -2;        /* will be reset leter, set
                                     * negative here * to flag
                                     * that it should override
                                     * xtrekrc */
                LineToConsole ("Using RSA verification\n");
                break;
#else
            case 'R':
                LineToConsole ("This client does not support RSA verification\n");

            case 'o':
                LineToConsole ("Using standard binary verification\n");
                break;
#endif

            case 'h':           /* server to connect to */
                if (i < argc)
                {
                    servertmp = argv[i + 1];
                    if (metablock(servertmp))
                    {
                        LineToConsole ("Default logins not welcome there, please edit your netrekrc file and add a 'login: yourhandle' line\n");
                        exit (0);
                    }
                    usemeta = 0;
                    i++;
                }
                else
                    usage++;

#ifdef GATEWAY
                gw_mach = servertmp;
#endif

                break;

#ifdef GATEWAY
            case 'H':           /* gateway to connect through */
                hset++;
                if (i < argc)
                {
                    netaddr = strToNetaddr (argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;
#endif
            case 'U':           /* local UDP port */
                if (i < argc)
                {
                    if ((baseUdpLocalPort = atoi (argv[i + 1])) == 0)
                    {
                        LineToConsole ("Error: -U requires a port number\n");
                        exit (1);
                    }
                    i++;
                }
                else
                    usage++;
                break;

#ifdef PACKET_LOG
            case 'P':           /* log packets */
                log_packets++;
                break;
#endif
            case 'G':           /* try restarting previous session */
                if (i < argc)
                {
                    ghoststart++;
                    ghost_pno = atoi (argv[i + 1]);
                    LineToConsole ("Emergency restart being attempted...\n");
                    i++;
                }
                else
                    usage++;
                break;
            
            case 't':           /* window title */
                if (i < argc)
                {
                    title = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;
            
            case 'r':           /* defaults file */
                if (i < argc)
                {
                    deffile = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;
            
            case 'D':           /* add debug info */
                debug++;
                hideConsole = 0;
                break;
            
            case 'n':           /* don't hide console window */
                hideConsole = 0;
                break;

            case 'v':           /* output version info */
                hideConsole = 0;
                LineToConsole ("%s %s\n", version, mvers);
                LineToConsole ("%s\n", CBUGS);
#ifdef RSA
                LineToConsole ("RSA key installed: %s --- Created by: %s\n", key_name, client_creator);
                LineToConsole ("Client type: %s\n", client_type);
                LineToConsole ("Client arch: %s\n", client_arch);
                LineToConsole ("Key permutation date: %s\n", client_key_date);
                LineToConsole ("Comments: %s\n", client_comments);
#endif
                exit (0);
                break;
            
            default:
                LineToConsole ("%s: unknown option '%c'\n", name, *ptr);
                err++; 
                break;
            }   /* end switch */

            ptr++;
        }       /* end while */
    }           /* end for */

    if (!usemeta && !servertmp)  /* no meta type was selected, pick metaserver */
        usemeta = 1;
        
    if (hideConsole)
        FreeConsole ();

    if (usage || err)
    {
        printUsage (name);
        exit (err);
    }

#ifdef GATEWAY
    if (!hset) use_trekhopd = 0;        /* allow use via normal connections */
    if (netaddr == 0)
    {
        LineToConsole ("netrek: no remote address set (-H).  
                          Restricted server will not work.\n");
    }
#endif

#ifdef RECORDGAME
    if (inplayback)
        err = pbmain (name);
    else
#endif
        err = cowmain (servertmp, xtrekPort, name);
                                        
    exit (err);
}

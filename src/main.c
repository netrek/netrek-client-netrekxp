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
#include "copyright.h"
#include "cowapi.h"
#include "Wlib.h"
#include "struct.h"
#include "proto.h"

char *servertmp = NULL;


/******************************************************************************/
/***  printUsage()
/******************************************************************************/
static void
printUsage (char *prog)
{
    printf ("%s\n", COWID);
    printf ("Usage: %s [options] [display-name]\n", prog);
    printf ("Options:\n");
    printf (" [-h servername]     Specify a server\n");
    printf (" [-p port number]     Specify a port to connect to\n");
    printf (" [-r defaultsfile]   Specify defaults file\n");
    printf
        (" [-s socketnum]      Specify listen socket port for manual start\n");
    printf (" [-u]   show usage\n");
    printf (" [-A]   character password\n");
    printf (" [-C]   character name\n");

#ifdef GATEWAY
    printf (" [-H]   specify host (via gateway)\n");
#endif

    printf
        (" [-U udp_port]       Specify client UDP port (useful for some firewalls)\n");

#ifdef RSA
    printf (" [-o]   use old-style binary verification)\n");
    printf (" [-R]   use RSA binary verification\n");
#endif

#ifdef PACKET_LOG
    printf (" [-P]   Log server packets, repeat for increased information\n");
#endif

    printf (" [-c]   to just do ck_players on the server\n");
    printf (" [-f filename]   Record game into 'filename'\n");
    printf (" [-l filename]   Record messages into 'filename'\n");

#ifdef META
    printf (" [-m]   check metaserver for active servers\n");
    printf (" [-k]   display known servers\n");
#endif

    printf (" [-v]   display client version info\n");

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
    char *name, *ptr;
    int i;

#ifdef GATEWAY
    int hset = 0;

#endif
    int first = 1;
    int xtrekPort = -1;

    name = argv[0];
//  if ((ptr = RINDEX(name, "/")) != NULL)
//    name = ptr + 1;

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

            case 'u':
                usage++;
                break;

            case 'c':
                checking = 1;
                break;

            case 's':
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
            case 'F':
                inplayback = 1;
                /* No break */

            case 'f':
                if (i < argc)
                {
                    recordFileName = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;
#endif
            case 'l':
                if (i < argc)
                {
                    logFileName = argv[i + 1];
                    i++;
                }
                else
                    usage++;
                break;

            case 'p':
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
            case 'm':
                if (usemeta && usemeta != 1)
                {
                    fputs ("The options -k and -m are mutuall exclusive\n",
                           stderr);
                    err++;
                }
                usemeta = 1;
                break;

            case 'k':
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
            case 'o':
                useRsa = -1;        /* will be reset leter, set
                                         * negative here * to flag
                                         * that it should override
                                         * xtrekrc */
                printf ("Using standard binary verification\n");
                break;

            case 'R':
                useRsa = -2;        /* will be reset leter, set
                                         * negative here * to flag
                                         * that it should override
                                         * xtrekrc */
                printf ("Using RSA verification\n");
                break;
#else
            case 'R':
                printf ("This client does not support RSA verification\n");

            case 'o':
                printf ("Using standard binary verification\n");
                break;
#endif

            case 'h':
                if (i < argc)
                {
                    servertmp = argv[i + 1];
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
            case 'H':
                hset++;
                if (i < argc)
                {
                netaddr = strToNetaddr (argv[i + 1]; i++;}
                                        else
                                        usage++; break;
#endif
            case 'U':
                                        if (i < argc)
                                        {
                                        if ((baseUdpLocalPort =
                                             atoi (argv[i + 1])) == 0)
                                        {
                                        fprintf (stderr,
                                                 "Error: -U requires a port number\n");
                                        exit (1);}
                                        i++;}
                                        else
                                        usage++; break;
#ifdef PACKET_LOG
            case 'P':
                                        log_packets++; break;
#endif
            case 'G':
                                        if (i < argc)
                                        {
                                        ghoststart++;
                                        ghost_pno = atoi (argv[i + 1]);
                                        printf
                                        ("Emergency restart being attempted...\n");
                                        i++;}
                                        else
            usage++; break; case 't':
                                        if (i < argc)
                                        {
                                        title = argv[i + 1]; i++;}
                                        else
            usage++; break; case 'r':
                                        if (i < argc)
                                        {
                                        deffile = argv[i + 1]; i++;}
                                        else
            usage++; break; case 'D':
            debug++; break; case 'v':
                                        printf ("%s\n", COWID);
                                        printf ("%s\n", CBUGS);
#ifdef RSA
                                        printf
                                        ("RSA key installed: %s --- Created by: %s\n",
                                         key_name, client_creator);
                                        printf ("     Client type: %s\n",
                                                client_type);
                                        printf ("     Client arch: %s\n",
                                                client_arch);
                                        printf
                                        ("     Key permutation date: %s\n",
                                         client_key_date);
                                        printf ("     Comments: %s\n",
                                                client_comments);
#endif
            exit (0); break; default:
                                        fprintf (stderr, "%s: unknown option '%c'\n", name, *ptr); err++; break;}       /* end switch */

                                        ptr++;} /* end while */
                                        }       /* end for */

                                        if (usage || err)
                                        {
                                        printUsage (name); exit (err);}

#ifdef GATEWAY
                                        if (!hset) use_trekhopd = 0;    /* allow use via normal
                                                                         * connections */
                                        if (netaddr == 0)
                                        {
                                        fprintf (stderr,
                                                 "netrek: no remote address set (-H).  Restricted server will not work.\n");}
#endif

#ifdef RECORDGAME
                                        if (inplayback) err = pbmain (name);
                                        else
#endif
                                        err =
                                        cowmain (servertmp, xtrekPort, name);
                                        exit (err);}

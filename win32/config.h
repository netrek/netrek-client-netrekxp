/*
        Netrek Configuration file       -              by Kurt Siegl
*/

#ifndef __CONFIG_H
#define __CONFIG_H

/* 
################################################################################
        Type of code (select in configure.in)    
################################################################################
*/

#define COW
#undef SERVER

/*
################################################################################
		Compilation specific defines
################################################################################
*/

#define CBUGS "Send bugs/suggestions to: billbalcerski@hotmail.com"

/* 
################################################################################
        All The fun defines      
################################################################################
*/

/*
   Defines
      Some possible values:
   NOTE: Only NON-system specific defines belong here
*/

/* 
   ------------------------------------------------------------
                Common defines
   ------------------------------------------------------------
*/

/*      RCD             - Support Reciever variable distresses */
#define RCD                     
                                                     
/*      PING            - Ping statistics */
#define PING

/*      SHORT_PACKETS   - Reduce Network load */
#define SHORT_PACKETS


/*      FEATURE_PACKETS - feature packets */
#define FEATURE_PACKETS

/* 
   ------------------------------------------------------------
                COW only defines
   ------------------------------------------------------------
*/
#ifdef COW

/*      THREADED                - Multi-threaded input handling                 */
#define THREADED

/*      NBT                     - Nick trown's macro code                       */
#define NBT

/*      CORRUPTED_PACKETS       - prevents some seg faults but verbose output   */
#define CORRUPTED_PACKETS

/*      ARMY_SLIDER             - some sort of funky status window thing        */
#define ARMY_SLIDER

/*      META                    - show active servers via metaserver - NBT      */
#define META

/*      METAPING                - improvements to metaserver window             */
#define METAPING

/*      PACKET_LOG              - log packets to stdout (for bandwith measurement) */
#define PACKET_LOG

/*      NEWMACRO                - newmacro language                             */
#define NEWMACRO 

/*      SMARTMACRO              - macro extension (needs NEWMACRO)              */
#define SMARTMACRO

/*      MULTILINE_MACROS        - enables multiline macros                      */
#define MULTILINE_MACROS

/*      XTREKRC_HELP            - Hadley's xtrekrc window (stolden by KP)       */
#undef XTREKRC_HELP

/*      TOOLS                   - Various tools, like shell escape, ...         */
#undef TOOLS

/*      SOUND                   - support of external sound players             */
#define SOUND

/*      HOCKEY_LINES            - allow showing of hockey lines                 */
#define HOCKEY_LINES

/*      BEEPLITE                - player and planets highlighting		*/
#define BEEPLITE

/*      VSHIELD_BITMAPS         - damage dependent shields (hull)               */
#define VSHIELD_BITMAPS

/*      JUBILEE_PHASERS         - Cycle phaser color when it's a hit            */
#define JUBILEE_PHASERS

/*      WARP_DEAD               - Use death detection                           */
#undef  WARP_DEAD

/*      PARADISE                - Support paradise servers                      */
#undef PARADISE

/*      RCM                     - Receiver configurable Server messages         */
#define RCM

#define PLIST1 
#define PLIST 
#define CONTROL_KEY
#define BRMH 
#define DOC_WIN
#define RABBIT_EARS 
#define ROTATERACE

/* client options */
#define NEW_DASHBOARD_2
#define MOUSE_AS_SHIFT
#define SHIFTED_MOUSE
#define MOTION_MOUSE
#define DIST_KEY_NAME
#define XTRA_MESSAGE_UI
#define PHASER_STATS


/* Swap ports around to try to go through firewalls */
#define UDP_PORTSWAP
#define USE_PORTSWAP

/* Enabled playback of recorded games */
#define RECORDGAME

/* We want reverse-playback!!! */
#define REVERSE_PLAYBACK

#endif          /* COW */


/* 
   ------------------------------------------------------------
                SERVER only defines
   ------------------------------------------------------------
*/
#ifdef SERVER

/*     AUTOMOTD                 - Updates your MOTD every 12 hours */
#undef AUTOMOTD

/*      CHAIN_REACTION          - Don't give credit for chain reactions */
#define CHAIN_REACTION

/*      CHECKMESG               - Check for god messages and/or log all messages */
#define CHECKMESG

/*      ERIKPLAGUE              - Define for Erik's Plagueing */
#define ERIKPLAGUE

/*      FEATURES                - Server/client communication */
#define FEATURES

/*      FULL_HOSTNAMES          - Show entire player location (domain) */
#define FULL_HOSTNAMES

/*      INL_RESOURCES           - Use INL resources */
#define INL_RESOURCES

/*      INL_POP                 - Use INL style planet popping scheme */
#define INL_POP

/*      NO_PLANET_PLAGUE        - Define for plagueing (Don't use with ERIKPLAGUE) */
#undef  NO_PLANET_PLAGUE

/*      NO_UNWARRING_BOMBING    - No 3rd space bombing */
#define NO_UNWARRING_BOMBING

/*      SHOW_RSA                - Display the client type at login */
#define SHOW_RSA

/*      RESETGALAXY             - Reset the galaxy after each conquer */
#define RESETGALAXY

/*      RESTRICT_BOMB           - No bombing out of t-mode */
#define RESTRICT_BOMB

/*      SELF_RESET              - Galaxy will reset if the daemon dies */
#define SELF_RESET

/*      SHORT_THRESHOLD         - For Short Packets */
#define SHORT_THRESHOLD

/*      SURRENDER_SHORT         - Cut surrender time in half */
#undef  SURRENDER_SHORT

/*      TWO_RACE                - Conquer only a 1/4 of the galaxy for reset */
#define TWO_RACE

/*      SENDFLAGS               - Print flags set in MOTD */
#undef  SENDFLAGS

/*      CHECK_ENV               - Check environment variable NTSERV_PATH for location of system files */
#undef  CHECK_ENV

/*      GPPRINT                 - Print which path is being used to find the system files */
#undef  GPPRINT

/*       ONCHECK                - support for trekon player check */
#define  ONCHECK

/*      NBR                     - Leave in for server. It tells programs like xsg that it is this code. */
#define NBR

/*      DS                      - Daemon Sychronization - Tedd Hadley */
#define DS


/*      NEED_EXIT               - For systems that need exit defined */
#define NEED_EXIT

/*      NEWMASK                 - New tournament selection code. Less rigid than older code. */
#define NEWMASK

/*      SB_TRANSWARP            - starbase transwarp */
#define SB_TRANSWARP

/*     FLAT_BONUS               - 3x bonus for killing last army on a planet */
#undef FLAT_BONUS

/*      BASEPRACTICE            - trainingserver support */
#define BASEPRACTICE

/*     NEW_CREDIT               - give 1 planet for destroying, two planets for taking */
#undef NEW_CREDIT

/*      FASTER_SB_MAXUPDATES    - allow 10 ups/sec for SB's */
#define FASTER_SB_MAXUPDATES

/*      SBFUEL_FIX              - fix starbase re-fueling */
#define SBFUEL_FIX

/*     NEW_ETEMP                - Wreck's etemp fix */
#undef NEW_ETEMP

/*      DOGFIGHT                - .sysdef interface for dogfight robot */
#define DOGFIGHT

/*      OBSERVERS               - allow players to watch */
#define OBSERVERS

/*      VOTING                  - allow voting mode */
#define VOTING

/*     ALLOW_EJECT              - define to allow players to eject others */
#ifdef VOTING
#undef ALLOW_EJECT
#endif

/*      NO_BRUTALITY            - disallow fighting between waiting players in the dogfighting mode */
#ifdef DOGFIGHT
#define NO_BRUTALITY
#endif

/*     AS_CLOAK                 - gives cloaked AS's more randomness in their apparent positions */
#undef AS_CLOAK

/*     GENO_COUNT               - keep track of a player's winning genocides */
#undef GENO_COUNT

#ifdef VOTING
/*      AUTO_INL                - starts up INL robot by majority vote   */
#define AUTO_INL

/*      AUTO_PRACTICE           - starts up PRACTICE robot by majority vote   */
#ifdef BASEPRACTICE
#define AUTO_PRACTICE
#endif

/*      AUTO_HOCKEY             - starts up HOCKEY robot by majority vote   */
#define AUTO_HOCKEY

/*      AUTO_DOGFIGHT           - starts up DOGFIGHT robot by majority vote   */
#ifdef DOGFIGHT
#define AUTO_DOGFIGHT
#endif

/*      TRIPLE_PLANET_MAYHEM    - enable voting for the three planet cool server idea by felix@coop.com */
#define TRIPLE_PLANET_MAYHEM
#endif
                                
/*     MESSAGES_ALL_TIME        - allow messaging during freezes like twarp, refit, and war decl.  */
#undef MESSAGES_ALL_TIME


#endif          /* SERVER */


/*
##############################################################################
       All system dependencies should be defined here 
##############################################################################
*/

#define NEED_EXIT

/* Automatic generated system dependend defines                         */

#define STDC_HEADERS 1          /* 13/01/94 omit hosed index [007] */

#define RETSIGTYPE void
#undef pid_t 
#undef uid_t 
#undef gid_t
#undef size_t 
#undef vfork 
#define inline


#define PATH_MAX 255

#define  nint(x) ((int)(x))

extern double rint(double);

#define R_OK 4


/* System dependend macros                                              */

#define SIGNAL(x,y)   signal(x,y)

#define MCMP(x,y,z)     memcmp(x,y,z)
#define MCOPY(b1,b2,l)  memcpy(b2,b1,l)
#define MZERO(b1,l)     memset(b1,0,l)
#define INDEX(s,c)      strchr(s,c)
#define RINDEX(s,c)     strrchr(s,c)

#ifdef SPEEDUP          /* strncpy is terrible slow */
#include <string.h>
/* #define STRNCPY(x,y,z)       memcpy(x,y,z) */
#define STRNCPY(x,y,z)  memccpy(x,y,'\0',z) 
#else
#define STRNCPY(x,y,z)  strncpy(x,y,z)
#endif

/* #define WAIT3(x,y,z)  waitpid(-1, x, y ) */

// Win32 has a rand() and srand() function.... use that.
#define RANDOM()	rand()
#define SRANDOM(x)	srand(x)


#define LONG long
#define U_LONG u_long

#define VOID void
typedef char CHAR;
typedef short SHORT;

/*  System dependend Include files */


/* Some GMP/RSA based stuff SRS 2/9/98 */
#ifdef RSA

#define MPTYPEDEF typedef MINT *MPTYPE;
#define assignItom(x,i) x= itom(i)
/* I had to rename pow in the gmp libraries to mpow */
#define mp_pow(x, y, z, a) mpow(x, y, z, a)
#define INC_MP <mp.h>

#endif
/* End of GMP/RSA based stuff SRS 2/9/98 */

/* Unchecked machine dependencies */

#ifdef WIN32
typedef unsigned short int ushort;
#define strncasecmp strncmp
#ifdef THREADED
typedef unsigned long int SEMAPHORE;
#define CREATE_SEMAPHORE(initstate) CreateEvent(NULL, 1, initstate, NULL)
#define SET_SEMAPHORE(sem) SetEvent((HANDLE)sem)
#define RESET_SEMAPHORE(sem) ResetEvent((HANDLE)sem)
#endif
#endif

#ifdef _MSC_VER
/* winnt.h stuff... */
#define VOID void
typedef char CHAR;
typedef short SHORT;
/* winnt.h */
#ifndef strdup
#define strdup  _strdup
#endif

#ifndef strcmpi
#define strcmpi _strcmpi
#endif

#ifndef memccpy
#define memccpy _memccpy
#endif

#ifndef stricmp
#define stricmp _stricmp
#endif

#ifndef getpid
#define getpid _getpid
#endif

/* For math.h, really */
#define hypot _hypot
/* for io.h */
#define read   _read
#define close  _close
#define access _access
#define unlink _unlink
#endif

#endif  /* __CONFIG_H */


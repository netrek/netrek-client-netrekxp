/*
        Netrek Configuration file       -              by Kurt Siegl
*/

#ifndef __CONFIG_H
#define __CONFIG_H

/*
################################################################################
		Compilation specific defines
################################################################################
*/

#define CBUGS "Send bugs/suggestions to: billbalcerski@gmail.com"

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
                Client defines
   ------------------------------------------------------------
*/

/*      THREADED                - Multi-threaded input handling                 */
#define THREADED

/*      CORRUPTED_PACKETS       - prevents some seg faults but verbose output   */
#define CORRUPTED_PACKETS

/*      META                    - show active servers via metaserver - NBT      */
#define META

/*      METAPING                - improvements to metaserver window             */
#define METAPING

/*      PACKET_LOG              - log packets to stdout (for bandwith measurement) */
#define PACKET_LOG

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

/*      WARP_DEAD               - Use death detection                           */
#undef  WARP_DEAD

/*      PARADISE                - Support paradise servers                      */
#define PARADISE

/*      RCM                     - Receiver configurable Server messages         */
#define RCM

#define PLIST1 
#define PLIST 
#define DOC_WIN
#define ROTATERACE

/* client options */
#define MOUSE_AS_SHIFT
#define SHIFTED_MOUSE
#define MOTION_MOUSE
#define XTRA_MESSAGE_UI
#define PHASER_STATS

/* Enabled playback of recorded games */
#define RECORDGAME

/* We want reverse-playback!!! */
#define REVERSE_PLAYBACK

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


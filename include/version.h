#include "config.h"

/* store the version info here */

#ifdef STABLE
static char mvers[] = "2.02";
static char version[] = "COW 2.02";

#define LIBMAJOR 2
#define LIBMINOR 2

#else
static char mvers[] = "3.00";
static char version[] = "Netrek 2000";

#define LIBMAJOR 3
#define LIBMINOR 0

#if 1
#define ALPHACODER "Caesar"
#define ALPHAREF "0"
#endif
#endif

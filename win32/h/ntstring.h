/* Include file to map Microsoft Visual C++ 4.0 string.h stuff from
 * _functionname to functionname */

#include <string.h>

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

#include "config.h"
// Ships
// -----

#define SHIP_VIEWS  32

/* #define rosette(x)   ((((x) + 8) / 16) & 15)
 *  - This function works well for 16 ship views
 *
 * #define rosette(x)   ((((x) + 256/VIEWS/2) / (256/VIEWS) + VIEWS) % VIEWS)
 *  - This function is more generic
 * An example using 16 views:
 * (((x + 8) / 16 + 16)  %  16)  */
#define rosette(x)   ((((x) + 256/SHIP_VIEWS/2) / (256/SHIP_VIEWS) + SHIP_VIEWS) % SHIP_VIEWS)


#define BMP_SHIP_WIDTH  20
#define BMP_SHIP_HEIGHT 20

#define BMP_FED_SHIP 1
#define BMP_ORI_SHIP 4
#define BMP_KLI_SHIP 3
#define BMP_ROM_SHIP 5
#define BMP_IND_SHIP 2


// Weapons
// -------

#ifdef COLORIZE
#define BMP_FED_TORP   1
#define BMP_FED_TORP_DET   9
#define BMP_FED_PLASMA   14
#define BMP_FED_PLASMA_DET   22
#define BMP_ORI_TORP   27
#define BMP_ORI_TORP_DET   35
#define BMP_ORI_PLASMA   40
#define BMP_ORI_PLASMA_DET   48
#define BMP_KLI_TORP   53
#define BMP_KLI_TORP_DET   61
#define BMP_KLI_PLASMA   66
#define BMP_KLI_PLASMA_DET   74
#define BMP_ROM_TORP   79
#define BMP_ROM_TORP_DET   87
#define BMP_ROM_PLASMA   92
#define BMP_ROM_PLASMA_DET   100
#define BMP_IND_TORP   105
#define BMP_IND_TORP_DET   113
#define BMP_IND_PLASMA   118
#define BMP_IND_PLASMA_DET   126

#define BMP_TORPDET_FRAMES 5
#define BMP_TORP_FRAMES 8

#define BMP_TORP_WIDTH             5
#define BMP_TORP_HEIGHT            5
#define BMP_PLASMATORP_WIDTH       7
#define BMP_PLASMATORP_HEIGHT      7

#define BMP_TORPDET_WIDTH 9
#define BMP_TORPDET_HEIGHT 9
#define BMP_PLASMATORPDET_WIDTH 15
#define BMP_PLASMATORPDET_HEIGHT 15
#else /* COLORIZE */
#define BMP_EPLASMA 6
#define BMP_ETORP 7
#define BMP_MPLASMA 8
#define BMP_MTORP 9
#define BMP_CLOUD 1
#define BMP_PLCLOUD 10

#define BMP_TORPDET_FRAMES 5
#define BMP_TORP_FRAMES 8

#define BMP_TORP_WIDTH             3
#define BMP_TORP_HEIGHT            3
#define BMP_EPLASMA_WIDTH       7
#define BMP_EPLASMA_HEIGHT      7
#define BMP_MPLASMA_WIDTH       5
#define BMP_MPLASMA_HEIGHT      5

#define BMP_TORPDET_WIDTH 9
#define BMP_TORPDET_HEIGHT 9
#define BMP_PLASMATORPDET_WIDTH 15
#define BMP_PLASMATORPDET_HEIGHT 15

#define BMP_TORPDET_WIDTH 9
#define BMP_TORPDET_HEIGHT 9
#define BMP_PLASMATORPDET_WIDTH 15
#define BMP_PLASMATORPDET_HEIGHT 15
#endif /* COLORIZE */

// Miscellaneous
// ----------

#define BMP_SHIP_EXPLOSION 8
#define BMP_SB_EXPLOSION 1
#define BMP_CLOCK 13
#define BMP_CLOAK 14
#define BMP_ICON 15
#define BMP_SHIELD 16
#define BMP_VSHIELD 17
#define BMP_STIPPLE 22

#define BMP_SHIPEXPL_FRAMES 5
#define BMP_SBEXPL_FRAMES 7

#define BMP_SHIPEXPL_WIDTH 66
#define BMP_SHIPEXPL_HEIGHT 66
#define BMP_SBEXPL_WIDTH 180
#define BMP_SBEXPL_HEIGHT 180

#define BMP_CLOCK_WIDTH 48
#define BMP_CLOCK_HEIGHT 48
#define BMP_CLOAK_WIDTH 20
#define BMP_CLOAK_HEIGHT 20
#define BMP_ICON_WIDTH 112
#define BMP_ICON_HEIGHT 80
#define BMP_SHIELD_WIDTH 20
#define BMP_SHIELD_HEIGHT 20
#define BMP_STIPPLE_WIDTH 16
#define BMP_STIPPLE_HEIGHT 16

// Planets
// -------

#define BMP_PLANET000 1
#define BMP_PLANET001 2
#define BMP_PLANET010 3
#define BMP_PLANET011 4
#define BMP_PLANET100 5
#define BMP_PLANET101 6
#define BMP_PLANET110 7
#define BMP_PLANET111 8
#define BMP_PLANETUNKN 9
#define BMP_MPLANET000 10
#define BMP_MPLANET001 11
#define BMP_MPLANET010 12
#define BMP_MPLANET011 13
#define BMP_MPLANET100 14
#define BMP_MPLANET101 15
#define BMP_MPLANET110 16
#define BMP_MPLANET111 17
#define BMP_MPLANETUNKN 18

#define BMP_PLANET_WIDTH 30
#define BMP_PLANET_HEIGHT 30
#define BMP_MPLANET_WIDTH 16
#define BMP_MPLANET_HEIGHT 16

#define PLANET_VIEWS 9
#define MPLANET_VIEWS 9

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
 * (((x + 8) / 16 + 16)  %  16)
 * Or 32 views:
 * (((x + 4) / 8 + 32)   %  32)  */
#define rosette(x)   ((((x) + 256/SHIP_VIEWS/2) / (256/SHIP_VIEWS) + SHIP_VIEWS) % SHIP_VIEWS)


#define BMP_SHIP_WIDTH  20
#define BMP_SHIP_HEIGHT 20
#define BMP_SHIP_WIDTH_HR  80
#define BMP_SHIP_HEIGHT_HR 80

#define BMP_FED_SHIP 1
#define BMP_ORI_SHIP 4
#define BMP_KLI_SHIP 3
#define BMP_ROM_SHIP 5
#define BMP_IND_SHIP 2
#define BMP_PSHIP 6

// Weapons
// -------

#define NUM_CTORP_TYPES   6

#define BMP_TORPDET_FRAMES 5
#define BMP_TORP_FRAMES 8
#define BMP_HULL_FRAMES 8

#define BMP_EPLASMA 6
#define BMP_ETORP 7
#define BMP_MPLASMA 8
#define BMP_MTORP 9
#define BMP_CLOUD 1
#define BMP_PLCLOUD 10

#define BMP_PLASMAC 6
#define BMP_TORPC 7
#define BMP_MPLASMAC 8
#define BMP_MTORPC 9
#define BMP_CLOUDC 1
#define BMP_PLCLOUDC 10

#define BMP_TORP_WIDTH             3
#define BMP_TORP_HEIGHT            3
#define BMP_CTORP_WIDTH             5
#define BMP_CTORP_HEIGHT            5
#define BMP_TORPDET_WIDTH 8
#define BMP_TORPDET_HEIGHT 8
#define BMP_CTORPDET_WIDTH 9
#define BMP_CTORPDET_HEIGHT 9

#define BMP_EPLASMA_WIDTH       7
#define BMP_EPLASMA_HEIGHT      7
#define BMP_MPLASMA_WIDTH       5
#define BMP_MPLASMA_HEIGHT      5
#define BMP_PLASMATORPDET_WIDTH 13
#define BMP_PLASMATORPDET_HEIGHT 13
#define BMP_CPLASMATORP_WIDTH       7
#define BMP_CPLASMATORP_HEIGHT      7
#define BMP_CPLASMATORPDET_WIDTH 15
#define BMP_CPLASMATORPDET_HEIGHT 15


// Miscellaneous
// ----------

#define BMP_SHIP_EXPLOSION 8
#define BMP_SB_EXPLOSION 1
#define BMP_CLOCK 13
#define BMP_CLOCKHAND 14
#define BMP_CLOAK 15
#define BMP_SHIELD 16
#define BMP_VSHIELD 17
#define BMP_HULL 18
#define BMP_STIPPLE 22
#define BMP_GENO 23
#define BMP_GENO2 24
#define BMP_FEDTEAM 25
#define BMP_ROMTEAM 26
#define BMP_KLITEAM 27
#define BMP_ORITEAM 28

#define BMP_SHIPEXPL_FRAMES 5
#define BMP_SBEXPL_FRAMES 7

#define BMP_SHIPEXPL_WIDTH 66
#define BMP_SHIPEXPL_HEIGHT 66
#define BMP_SBEXPL_WIDTH 180
#define BMP_SBEXPL_HEIGHT 180

#define BMP_CLOCK_WIDTH 48
#define BMP_CLOCK_HEIGHT 48
#define BMP_CHAND_WIDTH 100
#define BMP_CHAND_HEIGHT 100
#define BMP_CCLOCK_WIDTH 100
#define BMP_CCLOCK_HEIGHT 100
#define BMP_CLOAK_WIDTH 20
#define BMP_CLOAK_HEIGHT 20
#define BMP_TEAM_SELECT_WIDTH 100
#define BMP_TEAM_SELECT_HEIGHT 100
#define BMP_GENO_WIDTH 500
#define BMP_GENO_HEIGHT 500
#define BMP_SHIELD_WIDTH 20
#define BMP_SHIELD_HEIGHT 20
#define BMP_STIPPLE_WIDTH 16
#define BMP_STIPPLE_HEIGHT 16

#ifdef BEEPLITE
#define BMP_BEEPLITE1 1
#define BMP_BEEPLITE2 2
#define BMP_BEEPLITE3 3
#endif
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

#define PARADISE_PLANET_VIEWS 13
#define PARADISE_MPLANET_VIEWS 13
#define PLANET_VIEWS 9
#define MPLANET_VIEWS 9

// Color planets

// Homeworlds
#define BMP_EARTH 1
#define BMP_ROMULUS 2
#define BMP_ORION 3
#define BMP_KLINGUS 4

// Agris
#define BMP_AGRI1 5
#define BMP_AGRI2 6

// Regular planets
#define BMP_ROCK1 7
#define BMP_ROCK2 8

// Untouched planet
#define BMP_PLANET_UNKNOWN 9
#define BMP_PLANET_UNKNOWN_NR 10

// Map version of unknown planet
#define BMP_PLANET_MUNKNOWN 11

// Resources
#define BMP_ARMYBMP 10
#define BMP_ARMY_WIDTH 10
#define BMP_ARMY_HEIGHT 30
#define BMP_WRENCHBMP 11
#define BMP_WRENCH_WIDTH 120
#define BMP_WRENCH_HEIGHT 40
#define BMP_FUELBMP 12
#define BMP_FUEL_WIDTH 40
#define BMP_FUEL_HEIGHT 120
#define BMP_CPLANET_WIDTH 120
#define BMP_CPLANET_HEIGHT 120

#define CPLANET_VIEWS 16

#define BMP_DRONE 9
#define BMP_DRONEC 10
#define BMP_DRONE_WIDTH 6
#define BMP_DRONE_HEIGHT 6
#define BMP_DRONECLOUD 12
#define BMP_DRONECLOUDC 13
#define BMP_DRONEDET_FRAMES 5
#define BMP_DRONEDET_WIDTH 13
#define BMP_DRONEDET_HEIGHT 13
#define BMP_FIGHTER 9
#define BMP_FIGHTERC 10
#define BMP_FIGHTER_WIDTH 5
#define BMP_FIGHTER_HEIGHT 5
#define BMP_FIGHTERCLOUD 12
#define BMP_FIGHTERCLOUDC 13
#define BMP_FIGHTERDET_FRAMES 5
#define BMP_FIGHTERDET_WIDTH 13
#define BMP_FIGHTERDET_HEIGHT 13
#define BMP_STAR 12
#define BMP_MSTAR 13
#define BMP_GEARBMP 14
#define BMP_WORMHOLE 15
#define BMP_MWORMHOLE 16
#define BMP_GEAR_WIDTH 10
#define BMP_GEAR_HEIGHT 10
#define BMP_STAR_WIDTH 30
#define BMP_STAR_HEIGHT 30
#define BMP_MSTAR_WIDTH 16
#define BMP_MSTAR_HEIGHT 16
#define STAR_VIEWS 5
#define BMP_WORMHOLE_WIDTH 80
#define BMP_WORMHOLE_HEIGHT 80
#define BMP_MWORMHOLE_WIDTH 16
#define BMP_MWORMHOLE_HEIGHT 16
#define WORMHOLE_VIEWS 8

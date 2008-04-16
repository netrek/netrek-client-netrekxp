/******************************************************************************/
/***  File:  getship.c                                                      ***/
/***                                                                        ***/
/***  Function: getship.c for client of socket protocol.                    ***/
/***   This file has been mangled so it only sets the ship characteristics  ***/
/***   needed.                                                              ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include <stdio.h>
#include <sys/types.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* fill in ship characteristics */

void
getshipdefaults ()
{
    shipvals[SCOUT].s_phaserdamage = 75;        /* scout: was 75 */
    shipvals[SCOUT].s_torpspeed = 16;   /* scout: was 16 */
    shipvals[SCOUT].s_maxspeed = 12;    /* scout:  */
    shipvals[SCOUT].s_maxfuel = 5000;   /* scout:  */
    shipvals[SCOUT].s_maxarmies = 2;    /* scout:  */
    shipvals[SCOUT].s_maxshield = 75;   /* scout: was 75 */
    shipvals[SCOUT].s_maxdamage = 75;   /* scout:  */
    shipvals[SCOUT].s_maxwpntemp = 1000;        /* scout:  */
    shipvals[SCOUT].s_maxegntemp = 1000;        /* scout:  */
    shipvals[SCOUT].s_type = SCOUT;     /* scout:  */
    shipvals[SCOUT].s_width = 20;       /* scout:  */
    shipvals[SCOUT].s_height = 20;      /* scout:  */
    shipvals[SCOUT].s_phaserfuse = 10;  /* scout:  */
    shipvals[SCOUT].s_repair = 80;      /* scout:  */
    shipvals[SCOUT].s_letter = 's';
    shipvals[SCOUT].s_desig[0] = 'S';
    shipvals[SCOUT].s_desig[1] = 'C';
    shipvals[SCOUT].s_bitmap = SCOUT;

    shipvals[DESTROYER].s_phaserdamage = 85;    /* destroyer: */
    shipvals[DESTROYER].s_torpspeed = 14;       /* destroyer: */
    shipvals[DESTROYER].s_maxspeed = 10;        /* destroyer: */
    shipvals[DESTROYER].s_maxfuel = 7000;       /* destroyer: */
    shipvals[DESTROYER].s_maxarmies = 5;        /* destroyer: */
    shipvals[DESTROYER].s_maxshield = 85;       /* destroyer: */
    shipvals[DESTROYER].s_maxdamage = 85;       /* destroyer: */
    shipvals[DESTROYER].s_maxwpntemp = 1000;    /* destroyer: */
    shipvals[DESTROYER].s_maxegntemp = 1000;    /* destroyer: */
    shipvals[DESTROYER].s_width = 20;   /* destroyer: */
    shipvals[DESTROYER].s_height = 20;  /* destroyer: */
    shipvals[DESTROYER].s_type = DESTROYER;     /* destroyer: */
    shipvals[DESTROYER].s_phaserfuse = 10;      /* destroyer: */
    shipvals[DESTROYER].s_repair = 100;         /* destroyer: */
    shipvals[DESTROYER].s_letter = 'd';
    shipvals[DESTROYER].s_desig[0] = 'D';
    shipvals[DESTROYER].s_desig[1] = 'D';
    shipvals[DESTROYER].s_bitmap = DESTROYER;

    shipvals[BATTLESHIP].s_phaserdamage = 105;  /* battleship: */
    shipvals[BATTLESHIP].s_torpspeed = 12;      /* battleship: */
    shipvals[BATTLESHIP].s_maxspeed = 8;        /* battleship: */
    shipvals[BATTLESHIP].s_maxfuel = 14000;     /* battleship: */
    shipvals[BATTLESHIP].s_maxarmies = 6;       /* battleship: */
    shipvals[BATTLESHIP].s_maxshield = 130;     /* battleship: */
    shipvals[BATTLESHIP].s_maxdamage = 130;     /* battleship: */
    shipvals[BATTLESHIP].s_maxwpntemp = 1000;   /* battleship: */
    shipvals[BATTLESHIP].s_maxegntemp = 1000;   /* battleship: */
    shipvals[BATTLESHIP].s_width = 20;  /* battleship: */
    shipvals[BATTLESHIP].s_height = 20; /* battleship: */
    shipvals[BATTLESHIP].s_type = BATTLESHIP;   /* battleship: */
    shipvals[BATTLESHIP].s_phaserfuse = 10;     /* battleship: */
    shipvals[BATTLESHIP].s_repair = 125;        /* battleship: */
    shipvals[BATTLESHIP].s_letter = 'b';
    shipvals[BATTLESHIP].s_desig[0] = 'B';
    shipvals[BATTLESHIP].s_desig[1] = 'B';
    shipvals[BATTLESHIP].s_bitmap = BATTLESHIP;
    
    shipvals[ASSAULT].s_phaserdamage = 80;      /* assault */
    shipvals[ASSAULT].s_torpspeed = 16; /* assault */
    shipvals[ASSAULT].s_maxspeed = 8;   /* assault */
    shipvals[ASSAULT].s_maxfuel = 6000; /* assault */
    shipvals[ASSAULT].s_maxarmies = 20; /* assault */
    shipvals[ASSAULT].s_maxshield = 80; /* assault */
    shipvals[ASSAULT].s_maxdamage = 200;        /* assault */
    shipvals[ASSAULT].s_maxwpntemp = 1000;      /* assault */
    shipvals[ASSAULT].s_maxegntemp = 1200;      /* assault */
    shipvals[ASSAULT].s_width = 20;     /* assault */
    shipvals[ASSAULT].s_height = 20;    /* assault */
    shipvals[ASSAULT].s_type = ASSAULT; /* assault */
    shipvals[ASSAULT].s_phaserfuse = 10; /* assault */
    shipvals[ASSAULT].s_repair = 120;    /* assault */
    shipvals[ASSAULT].s_letter = 'a';
    shipvals[ASSAULT].s_desig[0] = 'A';
    shipvals[ASSAULT].s_desig[1] = 'S';
    shipvals[ASSAULT].s_bitmap = ASSAULT;

    shipvals[STARBASE].s_phaserdamage = 120;    /* starbase */
    shipvals[STARBASE].s_torpspeed = 14;        /* starbase */
    shipvals[STARBASE].s_maxfuel = 60000;       /* starbase */
    shipvals[STARBASE].s_maxarmies = 25;        /* starbase */
    shipvals[STARBASE].s_maxshield = 500;       /* starbase */
    shipvals[STARBASE].s_maxdamage = 600;       /* starbase */
    shipvals[STARBASE].s_maxspeed = 2;  /* starbase */
    shipvals[STARBASE].s_maxwpntemp = 1300;     /* starbase */
    shipvals[STARBASE].s_maxegntemp = 1000;     /* starbase */
    shipvals[STARBASE].s_width = 20;    /* starbase */
    shipvals[STARBASE].s_height = 20;   /* starbase */
    shipvals[STARBASE].s_type = STARBASE;       /* starbase */
    shipvals[STARBASE].s_phaserfuse = 4;        /* starbase */
    shipvals[STARBASE].s_repair = 140;  /* starbase */
    shipvals[STARBASE].s_letter = 'o';
    shipvals[STARBASE].s_desig[0] = 'S';
    shipvals[STARBASE].s_desig[1] = 'B';
    shipvals[STARBASE].s_bitmap = STARBASE;
    
    shipvals[ATT].s_phaserdamage = 10000;       /* att: */
    shipvals[ATT].s_torpspeed = 30;     /* att: */
    shipvals[ATT].s_maxspeed = 60;      /* att: */
    shipvals[ATT].s_maxfuel = 60000;    /* att: */
    shipvals[ATT].s_maxarmies = 1000;   /* att: */
    shipvals[ATT].s_maxshield = 30000;  /* att: */
    shipvals[ATT].s_maxdamage = 30000;  /* att: */
    shipvals[ATT].s_maxwpntemp = 10000; /* att: */
    shipvals[ATT].s_maxegntemp = 10000; /* att: */
    shipvals[ATT].s_width = 20; /* att: */
    shipvals[ATT].s_height = 20;        /* att: */
    shipvals[ATT].s_type = ATT; /* att: */
    shipvals[ATT].s_phaserfuse = 2; /* att: */
    shipvals[ATT].s_repair = 30000; /* att: */
    shipvals[ATT].s_letter = 'X';
    shipvals[ATT].s_desig[0] = 'A';
    shipvals[ATT].s_desig[1] = 'T';
    shipvals[ATT].s_bitmap = ATT;

    shipvals[SGALAXY].s_phaserdamage = 100;     /* galaxy: */
    shipvals[SGALAXY].s_torpspeed = 13; /* galaxy: */
    shipvals[SGALAXY].s_maxspeed = 9;   /* galaxy: */
    shipvals[SGALAXY].s_maxfuel = 12000;        /* galaxy: */
    shipvals[SGALAXY].s_maxarmies = 5;  /* galaxy: */
    shipvals[SGALAXY].s_maxshield = 140;        /* galaxy: */
    shipvals[SGALAXY].s_maxdamage = 120;        /* galaxy: */
    shipvals[SGALAXY].s_maxwpntemp = 1000;      /* galaxy: */
    shipvals[SGALAXY].s_maxegntemp = 1000;      /* galaxy: */
    shipvals[SGALAXY].s_width = 20;     /* galaxy: */
    shipvals[SGALAXY].s_height = 20;    /* galaxy: */
    shipvals[SGALAXY].s_type = SGALAXY; /* galaxy: */
    shipvals[SGALAXY].s_phaserfuse = 10; /* galaxy: */
    shipvals[SGALAXY].s_repair = 112;    /* galaxy: */
    shipvals[SGALAXY].s_letter = 'g';
    shipvals[SGALAXY].s_desig[0] = 'G';
    shipvals[SGALAXY].s_desig[1] = 'A';
    shipvals[SGALAXY].s_bitmap = SGALAXY;

    shipvals[CRUISER].s_phaserdamage = 100;     /* cruiser: */
    shipvals[CRUISER].s_torpspeed = 12; /* cruiser: */
    shipvals[CRUISER].s_maxspeed = 9;   /* cruiser: */
    shipvals[CRUISER].s_maxfuel = 10000;        /* cruiser: */
    shipvals[CRUISER].s_maxarmies = 10; /* cruiser: */
    shipvals[CRUISER].s_maxshield = 100;        /* cruiser: */
    shipvals[CRUISER].s_maxdamage = 100;        /* cruiser: */
    shipvals[CRUISER].s_maxwpntemp = 1000;      /* cruiser: */
    shipvals[CRUISER].s_maxegntemp = 1000;      /* cruiser: */
    shipvals[CRUISER].s_width = 20;     /* cruiser: */
    shipvals[CRUISER].s_height = 20;    /* cruiser: */
    shipvals[CRUISER].s_type = CRUISER; /* cruiser: */
    shipvals[CRUISER].s_phaserfuse = 10; /* cruiser: */
    shipvals[CRUISER].s_repair = 110;   /* cruiser: */
    shipvals[CRUISER].s_letter = 'c';
    shipvals[CRUISER].s_desig[0] = 'C';
    shipvals[CRUISER].s_desig[1] = 'A';
    shipvals[CRUISER].s_bitmap = CRUISER;

#ifdef PARADISE
    shipvals[JUMPSHIP].s_phaserdamage = 25;
    shipvals[JUMPSHIP].s_torpspeed = 18;
    shipvals[JUMPSHIP].s_maxspeed = 20;
    shipvals[JUMPSHIP].s_maxfuel = 50000;
    shipvals[JUMPSHIP].s_maxarmies = 0;
    shipvals[JUMPSHIP].s_maxshield = 5;
    shipvals[JUMPSHIP].s_maxdamage = 60;
    shipvals[JUMPSHIP].s_maxwpntemp = 1300;
    shipvals[JUMPSHIP].s_maxegntemp = 5000;
    shipvals[JUMPSHIP].s_type = JUMPSHIP;
    shipvals[JUMPSHIP].s_width = 20;
    shipvals[JUMPSHIP].s_height = 20;
    shipvals[JUMPSHIP].s_phaserfuse = 4;
    shipvals[JUMPSHIP].s_repair = 200;
    shipvals[JUMPSHIP].s_letter = 'j';
    shipvals[JUMPSHIP].s_desig[0] = 'J';
    shipvals[JUMPSHIP].s_desig[1] = 'S';
    shipvals[JUMPSHIP].s_bitmap = JUMPSHIP;

    shipvals[FLAGSHIP].s_phaserdamage = 102;
    shipvals[FLAGSHIP].s_torpspeed = 12;
    shipvals[FLAGSHIP].s_maxspeed = 9;
    shipvals[FLAGSHIP].s_maxfuel = 12500;
    shipvals[FLAGSHIP].s_maxarmies = 6;
    shipvals[FLAGSHIP].s_maxshield = 115;
    shipvals[FLAGSHIP].s_maxdamage = 115;
    shipvals[FLAGSHIP].s_maxwpntemp = 1000;
    shipvals[FLAGSHIP].s_maxegntemp = 1500;
    shipvals[FLAGSHIP].s_type = FLAGSHIP;
    shipvals[FLAGSHIP].s_width = 20;
    shipvals[FLAGSHIP].s_height = 20;
    shipvals[FLAGSHIP].s_phaserfuse = 10;
    shipvals[FLAGSHIP].s_repair = 118;
    shipvals[FLAGSHIP].s_letter = 'f';
    shipvals[FLAGSHIP].s_desig[0] = 'F';
    shipvals[FLAGSHIP].s_desig[1] = 'L';
    shipvals[FLAGSHIP].s_bitmap = FLAGSHIP;

    shipvals[WARBASE].s_phaserdamage = 125;
    shipvals[WARBASE].s_torpspeed = 15;
    shipvals[WARBASE].s_maxspeed = 3;
    shipvals[WARBASE].s_maxfuel = 50000;
    shipvals[WARBASE].s_maxarmies = 0;
    shipvals[WARBASE].s_maxshield = 250;
    shipvals[WARBASE].s_maxdamage = 500;
    shipvals[WARBASE].s_maxwpntemp = 1500;
    shipvals[WARBASE].s_maxegntemp = 1000;
    shipvals[WARBASE].s_type = WARBASE;
    shipvals[WARBASE].s_width = 20;
    shipvals[WARBASE].s_height = 20;
    shipvals[WARBASE].s_phaserfuse = 5;
    shipvals[WARBASE].s_repair = 170;
    shipvals[WARBASE].s_letter = 'w';
    shipvals[WARBASE].s_desig[0] = 'W';
    shipvals[WARBASE].s_desig[1] = 'B';
    shipvals[WARBASE].s_bitmap = WARBASE;

    shipvals[LIGHTCRUISER].s_phaserdamage = 90;
    shipvals[LIGHTCRUISER].s_torpspeed = 13;
    shipvals[LIGHTCRUISER].s_maxspeed = 10;
    shipvals[LIGHTCRUISER].s_maxfuel = 8500;
    shipvals[LIGHTCRUISER].s_maxarmies = 3;
    shipvals[LIGHTCRUISER].s_maxshield = 95;
    shipvals[LIGHTCRUISER].s_maxdamage = 90;
    shipvals[LIGHTCRUISER].s_maxwpntemp = 1000;
    shipvals[LIGHTCRUISER].s_maxegntemp = 1500;
    shipvals[LIGHTCRUISER].s_type = LIGHTCRUISER;
    shipvals[LIGHTCRUISER].s_width = 20;
    shipvals[LIGHTCRUISER].s_height = 20;
    shipvals[LIGHTCRUISER].s_phaserfuse = 10;
    shipvals[LIGHTCRUISER].s_repair = 80;
    shipvals[LIGHTCRUISER].s_letter = 'l';
    shipvals[LIGHTCRUISER].s_desig[0] = 'C';
    shipvals[LIGHTCRUISER].s_desig[1] = 'L';
    shipvals[LIGHTCRUISER].s_bitmap = LIGHTCRUISER;

    shipvals[CARRIER].s_phaserdamage = 95;
    shipvals[CARRIER].s_torpspeed = 13;
    shipvals[CARRIER].s_maxspeed = 9;
    shipvals[CARRIER].s_maxfuel = 15000;
    shipvals[CARRIER].s_maxarmies = 3;
    shipvals[CARRIER].s_maxshield = 120;
    shipvals[CARRIER].s_maxdamage = 150;
    shipvals[CARRIER].s_maxwpntemp = 1000;
    shipvals[CARRIER].s_maxegntemp = 1500;
    shipvals[CARRIER].s_type = CARRIER;
    shipvals[CARRIER].s_width = 20;
    shipvals[CARRIER].s_height = 20;
    shipvals[CARRIER].s_phaserfuse = 6;
    shipvals[CARRIER].s_repair = 105;
    shipvals[CARRIER].s_letter = 'v';
    shipvals[CARRIER].s_desig[0] = 'C';
    shipvals[CARRIER].s_desig[1] = 'V';
    shipvals[CARRIER].s_bitmap = CARRIER;

    shipvals[UTILITY].s_phaserdamage = 80;
    shipvals[UTILITY].s_torpspeed = 15;
    shipvals[UTILITY].s_maxspeed = 7;
    shipvals[UTILITY].s_maxfuel = 16000;
    shipvals[UTILITY].s_maxarmies = 12;
    shipvals[UTILITY].s_maxshield = 120;
    shipvals[UTILITY].s_maxdamage = 220;
    shipvals[UTILITY].s_maxwpntemp = 1000;
    shipvals[UTILITY].s_maxegntemp = 1800;
    shipvals[UTILITY].s_type = UTILITY;
    shipvals[UTILITY].s_width = 20;
    shipvals[UTILITY].s_height = 20;
    shipvals[UTILITY].s_phaserfuse = 8;
    shipvals[UTILITY].s_repair = 120;
    shipvals[UTILITY].s_letter = 'u';
    shipvals[UTILITY].s_desig[0] = 'U';
    shipvals[UTILITY].s_desig[1] = 'T';
    shipvals[UTILITY].s_bitmap = UTILITY;

    shipvals[PATROL].s_phaserdamage = 50;
    shipvals[PATROL].s_torpspeed = 15;
    shipvals[PATROL].s_maxspeed = 13;
    shipvals[PATROL].s_maxfuel = 4000;
    shipvals[PATROL].s_maxarmies = 1;
    shipvals[PATROL].s_maxshield = 50;
    shipvals[PATROL].s_maxdamage = 40;
    shipvals[PATROL].s_maxwpntemp = 1000;
    shipvals[PATROL].s_maxegntemp = 1500;
    shipvals[PATROL].s_type = PATROL;
    shipvals[PATROL].s_width = 20;
    shipvals[PATROL].s_height = 20;
    shipvals[PATROL].s_phaserfuse = 8;
    shipvals[PATROL].s_repair = 50;
    shipvals[PATROL].s_letter = 'p';
    shipvals[PATROL].s_desig[0] = 'P';
    shipvals[PATROL].s_desig[1] = 'T';
    shipvals[PATROL].s_bitmap = PATROL;

    shipvals[PUCK].s_phaserdamage = 75;
    shipvals[PUCK].s_torpspeed = 16;
    shipvals[PUCK].s_maxspeed = 12;
    shipvals[PUCK].s_maxfuel = 5000;
    shipvals[PUCK].s_maxarmies = 2;
    shipvals[PUCK].s_maxshield = 75;
    shipvals[PUCK].s_maxdamage = 75;
    shipvals[PUCK].s_maxwpntemp = 1000;
    shipvals[PUCK].s_maxegntemp = 1000;
    shipvals[PUCK].s_type = PUCK;
    shipvals[PUCK].s_width = 20;
    shipvals[PUCK].s_height = 20;
    shipvals[PUCK].s_phaserfuse = 10;
    shipvals[PUCK].s_repair = 80;
    shipvals[PUCK].s_letter = 'k';
    shipvals[PUCK].s_desig[0] = 'P';
    shipvals[PUCK].s_desig[1] = 'U';
    shipvals[PUCK].s_bitmap = PUCK;
#endif
}

/******************************************************************************/
/***  getship()                                                             ***/
/******************************************************************************/
void
getship (struct ship *shipp,
         int s_type)
{
    memcpy ((char *) shipp, (char *) &(shipvals[s_type]),
            sizeof (struct ship));
}

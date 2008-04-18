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

void
initialize_shiptypes ()
{
    int     i;
    struct shiplist *temp;

    /* start at -1, the default shiptype */
    for (i = -1; i < nshiptypes; i++) {
	temp = (struct shiplist *) malloc(sizeof(struct shiplist));
	temp->ship = (struct ship *) malloc(sizeof(struct ship));
	getshipdefaults(temp->ship, i);
	temp->next = shiptypes;
	if (temp->next)
	    temp->next->prev = temp;
	temp->prev = NULL;
	shiptypes = temp;
    }
}

/* now returns a pointer to where the ship data is located.  This way
   if the data is later changed by the server everybody gets updated.
   Plus as a bonus it's more efficient :)   [Bill Dyess] */
struct ship *
getship(int s_type)
{
    struct shiplist *temp, *new;

    temp = shiptypes;
    while (temp) {
	if (temp->ship->s_type == s_type) {
	    return temp->ship;
	}
	temp = temp->next;
    }
    /*
       ok, that shiptype is unheard of.  Assume a new shiptype, and get the
       values for CA.  Also add the ship to the list so if it gets updated by
       the server later everyone stays happy.  [Bill Dyess]
    */
    printf("Error:  getship of unknown ship type %d, using CA defaults\n", s_type);
    temp = shiptypes;
    while (temp) {
	if (temp->ship->s_type == DEFAULT) {
	    printf("Adding ship type %d\n", s_type);
	    /* now add the new ship to the list */
	    new = (struct shiplist *) malloc(sizeof(struct shiplist));
	    new->ship = (struct ship *) malloc(sizeof(struct ship));
	    new->next = shiptypes;
	    new->prev = NULL;
	    if (shiptypes)
		shiptypes->prev = new;
	    shiptypes = new;
	    memmove(new->ship, temp->ship, sizeof(struct ship));
	    new->ship->s_type = s_type;
	    return new->ship;
	}
	temp = temp->next;
    }
    return temp->ship;

}

/* fill in ship characteristics */
void
getshipdefaults (struct ship *shipp, int s_type)
{
  switch (s_type) {
  case SCOUT:
  case PUCK:
    shipp->s_phaserdamage = 75;        /* scout: was 75 */
    shipp->s_torpspeed = 16;   /* scout: was 16 */
    shipp->s_maxspeed = 12;    /* scout:  */
    shipp->s_maxfuel = 5000;   /* scout:  */
    shipp->s_maxarmies = 2;    /* scout:  */
    shipp->s_maxshield = 75;   /* scout: was 75 */
    shipp->s_maxdamage = 75;   /* scout:  */
    shipp->s_maxwpntemp = 1000;        /* scout:  */
    shipp->s_maxegntemp = 1000;        /* scout:  */
    shipp->s_type = SCOUT;     /* scout:  */
    shipp->s_width = 20;       /* scout:  */
    shipp->s_height = 20;      /* scout:  */
    shipp->s_phaserfuse = 10;  /* scout:  */
    shipp->s_repair = 80;      /* scout:  */
    if(s_type == PUCK) {
      shipp->s_type = PUCK;
      shipp->s_letter = 'k';
      shipp->s_desig[0] = 'P';
      shipp->s_desig[1] = 'U';
      shipp->s_bitmap = PUCK;
    } else {
      shipp->s_type = SCOUT;
      shipp->s_letter = 's';
      shipp->s_desig[0] = 'S';
      shipp->s_desig[1] = 'C';
      shipp->s_bitmap = SCOUT;
    }
    break;
  case DESTROYER:
    shipp->s_phaserdamage = 85;    /* destroyer: */
    shipp->s_torpspeed = 14;       /* destroyer: */
    shipp->s_maxspeed = 10;        /* destroyer: */
    shipp->s_maxfuel = 7000;       /* destroyer: */
    shipp->s_maxarmies = 5;        /* destroyer: */
    shipp->s_maxshield = 85;       /* destroyer: */
    shipp->s_maxdamage = 85;       /* destroyer: */
    shipp->s_maxwpntemp = 1000;    /* destroyer: */
    shipp->s_maxegntemp = 1000;    /* destroyer: */
    shipp->s_width = 20;   /* destroyer: */
    shipp->s_height = 20;  /* destroyer: */
    shipp->s_type = DESTROYER;     /* destroyer: */
    shipp->s_phaserfuse = 10;      /* destroyer: */
    shipp->s_repair = 100;         /* destroyer: */
    shipp->s_letter = 'd';
    shipp->s_desig[0] = 'D';
    shipp->s_desig[1] = 'D';
    shipp->s_bitmap = DESTROYER;
    break;
  default:
  case DEFAULT:
  case CRUISER:
    shipp->s_phaserdamage = 100;     /* cruiser: */
    shipp->s_torpspeed = 12; /* cruiser: */
    shipp->s_maxspeed = 9;   /* cruiser: */
    shipp->s_maxfuel = 10000;        /* cruiser: */
    shipp->s_maxarmies = 10; /* cruiser: */
    shipp->s_maxshield = 100;        /* cruiser: */
    shipp->s_maxdamage = 100;        /* cruiser: */
    shipp->s_maxwpntemp = 1000;      /* cruiser: */
    shipp->s_maxegntemp = 1000;      /* cruiser: */
    shipp->s_width = 20;     /* cruiser: */
    shipp->s_height = 20;    /* cruiser: */
    shipp->s_type = s_type; /* cruiser OR default */
    shipp->s_phaserfuse = 10; /* cruiser: */
    shipp->s_repair = 110;   /* cruiser: */
    shipp->s_letter = 'c';
    shipp->s_desig[0] = 'C';
    shipp->s_desig[1] = 'A';
    shipp->s_bitmap = CRUISER;
    break;
  case BATTLESHIP:
    shipp->s_phaserdamage = 105;  /* battleship: */
    shipp->s_torpspeed = 12;      /* battleship: */
    shipp->s_maxspeed = 8;        /* battleship: */
    shipp->s_maxfuel = 14000;     /* battleship: */
    shipp->s_maxarmies = 6;       /* battleship: */
    shipp->s_maxshield = 130;     /* battleship: */
    shipp->s_maxdamage = 130;     /* battleship: */
    shipp->s_maxwpntemp = 1000;   /* battleship: */
    shipp->s_maxegntemp = 1000;   /* battleship: */
    shipp->s_width = 20;  /* battleship: */
    shipp->s_height = 20; /* battleship: */
    shipp->s_type = BATTLESHIP;   /* battleship: */
    shipp->s_phaserfuse = 10;     /* battleship: */
    shipp->s_repair = 125;        /* battleship: */
    shipp->s_letter = 'b';
    shipp->s_desig[0] = 'B';
    shipp->s_desig[1] = 'B';
    shipp->s_bitmap = BATTLESHIP;
    break;
  case ASSAULT:
    shipp->s_phaserdamage = 80;      /* assault */
    shipp->s_torpspeed = 16; /* assault */
    shipp->s_maxspeed = 8;   /* assault */
    shipp->s_maxfuel = 6000; /* assault */
    shipp->s_maxarmies = 20; /* assault */
    shipp->s_maxshield = 80; /* assault */
    shipp->s_maxdamage = 200;        /* assault */
    shipp->s_maxwpntemp = 1000;      /* assault */
    shipp->s_maxegntemp = 1200;      /* assault */
    shipp->s_width = 20;     /* assault */
    shipp->s_height = 20;    /* assault */
    shipp->s_type = ASSAULT; /* assault */
    shipp->s_phaserfuse = 10; /* assault */
    shipp->s_repair = 120;    /* assault */
    shipp->s_letter = 'a';
    shipp->s_desig[0] = 'A';
    shipp->s_desig[1] = 'S';
    shipp->s_bitmap = ASSAULT;
    break;
  case STARBASE:
    shipp->s_phaserdamage = 120;    /* starbase */
    shipp->s_torpspeed = 14;        /* starbase */
    shipp->s_maxfuel = 60000;       /* starbase */
    shipp->s_maxarmies = 25;        /* starbase */
    shipp->s_maxshield = 500;       /* starbase */
    shipp->s_maxdamage = 600;       /* starbase */
    shipp->s_maxspeed = 2;  /* starbase */
    shipp->s_maxwpntemp = 1300;     /* starbase */
    shipp->s_maxegntemp = 1000;     /* starbase */
    shipp->s_width = 20;    /* starbase */
    shipp->s_height = 20;   /* starbase */
    shipp->s_type = STARBASE;       /* starbase */
    shipp->s_phaserfuse = 4;        /* starbase */
    shipp->s_repair = 140;  /* starbase */
    shipp->s_letter = 'o';
    shipp->s_desig[0] = 'S';
    shipp->s_desig[1] = 'B';
    shipp->s_bitmap = STARBASE;
    break;
  case ATT:                      /* or GALAXY */
    if (paradise) {
      shipp->s_phaserdamage = 10000;       /* att: */
      shipp->s_torpspeed = 30;     /* att: */
      shipp->s_maxspeed = 60;      /* att: */
      shipp->s_maxfuel = 60000;    /* att: */
      shipp->s_maxarmies = 1000;   /* att: */
      shipp->s_maxshield = 30000;  /* att: */
      shipp->s_maxdamage = 30000;  /* att: */
      shipp->s_maxwpntemp = 10000; /* att: */
      shipp->s_maxegntemp = 10000; /* att: */
      shipp->s_width = 20; /* att: */
      shipp->s_height = 20;        /* att: */
      shipp->s_type = ATT; /* att: */
      shipp->s_phaserfuse = 2; /* att: */
      shipp->s_repair = 30000; /* att: */
      shipp->s_letter = 'X';
      shipp->s_desig[0] = 'A';
      shipp->s_desig[1] = 'T';
      shipp->s_bitmap = ATT;
    }
    else {
      shipp->s_phaserdamage = 100;     /* galaxy: */
      shipp->s_torpspeed = 13; /* galaxy: */
      shipp->s_maxspeed = 9;   /* galaxy: */
      shipp->s_maxfuel = 12000;        /* galaxy: */
      shipp->s_maxarmies = 5;  /* galaxy: */
      shipp->s_maxshield = 140;        /* galaxy: */
      shipp->s_maxdamage = 120;        /* galaxy: */
      shipp->s_maxwpntemp = 1000;      /* galaxy: */
      shipp->s_maxegntemp = 1000;      /* galaxy: */
      shipp->s_width = 20;     /* galaxy: */
      shipp->s_height = 20;    /* galaxy: */
      shipp->s_type = SGALAXY; /* galaxy: */
      shipp->s_phaserfuse = 10; /* galaxy: */
      shipp->s_repair = 112;    /* galaxy: */
      shipp->s_letter = 'g';
      shipp->s_desig[0] = 'G';
      shipp->s_desig[1] = 'A';
      shipp->s_bitmap = SGALAXY;
    }
    break;
  case JUMPSHIP:                     /* or ATT */
    if (paradise) {
      shipp->s_phaserdamage = 25;
      shipp->s_torpspeed = 18;
      shipp->s_maxspeed = 20;
      shipp->s_maxfuel = 50000;
      shipp->s_maxarmies = 0;
      shipp->s_maxshield = 5;
      shipp->s_maxdamage = 60;
      shipp->s_maxwpntemp = 1300;
      shipp->s_maxegntemp = 5000;
      shipp->s_type = JUMPSHIP;
      shipp->s_width = 20;
      shipp->s_height = 20;
      shipp->s_phaserfuse = 4;
      shipp->s_repair = 200;
      shipp->s_letter = 'j';
      shipp->s_desig[0] = 'J';
      shipp->s_desig[1] = 'S';
      shipp->s_bitmap = JUMPSHIP;
    }
    else {
      shipp->s_phaserdamage = 10000;       /* att: */
      shipp->s_torpspeed = 30;     /* att: */
      shipp->s_maxspeed = 60;      /* att: */
      shipp->s_maxfuel = 60000;    /* att: */
      shipp->s_maxarmies = 1000;   /* att: */
      shipp->s_maxshield = 30000;  /* att: */
      shipp->s_maxdamage = 30000;  /* att: */
      shipp->s_maxwpntemp = 10000; /* att: */
      shipp->s_maxegntemp = 10000; /* att: */
      shipp->s_width = 20; /* att: */
      shipp->s_height = 20;        /* att: */
      shipp->s_type = ATT; /* att: */
      shipp->s_phaserfuse = 2; /* att: */
      shipp->s_repair = 30000; /* att: */
      shipp->s_letter = 'X';
      shipp->s_desig[0] = 'A';
      shipp->s_desig[1] = 'T';
      shipp->s_bitmap = ATT;
    }
    break;
  case FLAGSHIP:
    shipp->s_phaserdamage = 102;
    shipp->s_torpspeed = 12;
    shipp->s_maxspeed = 9;
    shipp->s_maxfuel = 12500;
    shipp->s_maxarmies = 6;
    shipp->s_maxshield = 115;
    shipp->s_maxdamage = 115;
    shipp->s_maxwpntemp = 1000;
    shipp->s_maxegntemp = 1500;
    shipp->s_type = FLAGSHIP;
    shipp->s_width = 20;
    shipp->s_height = 20;
    shipp->s_phaserfuse = 10;
    shipp->s_repair = 118;
    shipp->s_letter = 'f';
    shipp->s_desig[0] = 'F';
    shipp->s_desig[1] = 'L';
    shipp->s_bitmap = FLAGSHIP;
    break;
  case WARBASE:
    shipp->s_phaserdamage = 125;
    shipp->s_torpspeed = 15;
    shipp->s_maxspeed = 3;
    shipp->s_maxfuel = 50000;
    shipp->s_maxarmies = 0;
    shipp->s_maxshield = 250;
    shipp->s_maxdamage = 500;
    shipp->s_maxwpntemp = 1500;
    shipp->s_maxegntemp = 1000;
    shipp->s_type = WARBASE;
    shipp->s_width = 20;
    shipp->s_height = 20;
    shipp->s_phaserfuse = 5;
    shipp->s_repair = 170;
    shipp->s_letter = 'w';
    shipp->s_desig[0] = 'W';
    shipp->s_desig[1] = 'B';
    shipp->s_bitmap = WARBASE;
    break;
  case LIGHTCRUISER:
    shipp->s_phaserdamage = 90;
    shipp->s_torpspeed = 13;
    shipp->s_maxspeed = 10;
    shipp->s_maxfuel = 8500;
    shipp->s_maxarmies = 3;
    shipp->s_maxshield = 95;
    shipp->s_maxdamage = 90;
    shipp->s_maxwpntemp = 1000;
    shipp->s_maxegntemp = 1500;
    shipp->s_type = LIGHTCRUISER;
    shipp->s_width = 20;
    shipp->s_height = 20;
    shipp->s_phaserfuse = 10;
    shipp->s_repair = 80;
    shipp->s_letter = 'l';
    shipp->s_desig[0] = 'C';
    shipp->s_desig[1] = 'L';
    shipp->s_bitmap = LIGHTCRUISER;
    break;
  case CARRIER:
    shipp->s_phaserdamage = 95;
    shipp->s_torpspeed = 13;
    shipp->s_maxspeed = 9;
    shipp->s_maxfuel = 15000;
    shipp->s_maxarmies = 3;
    shipp->s_maxshield = 120;
    shipp->s_maxdamage = 150;
    shipp->s_maxwpntemp = 1000;
    shipp->s_maxegntemp = 1500;
    shipp->s_type = CARRIER;
    shipp->s_width = 20;
    shipp->s_height = 20;
    shipp->s_phaserfuse = 6;
    shipp->s_repair = 105;
    shipp->s_letter = 'v';
    shipp->s_desig[0] = 'C';
    shipp->s_desig[1] = 'V';
    shipp->s_bitmap = CARRIER;
    break;
  case UTILITY:
    shipp->s_phaserdamage = 80;
    shipp->s_torpspeed = 15;
    shipp->s_maxspeed = 7;
    shipp->s_maxfuel = 16000;
    shipp->s_maxarmies = 12;
    shipp->s_maxshield = 120;
    shipp->s_maxdamage = 220;
    shipp->s_maxwpntemp = 1000;
    shipp->s_maxegntemp = 1800;
    shipp->s_type = UTILITY;
    shipp->s_width = 20;
    shipp->s_height = 20;
    shipp->s_phaserfuse = 8;
    shipp->s_repair = 120;
    shipp->s_letter = 'u';
    shipp->s_desig[0] = 'U';
    shipp->s_desig[1] = 'T';
    shipp->s_bitmap = UTILITY;
    break;
  case PATROL:
    shipp->s_phaserdamage = 50;
    shipp->s_torpspeed = 15;
    shipp->s_maxspeed = 13;
    shipp->s_maxfuel = 4000;
    shipp->s_maxarmies = 1;
    shipp->s_maxshield = 50;
    shipp->s_maxdamage = 40;
    shipp->s_maxwpntemp = 1000;
    shipp->s_maxegntemp = 1500;
    shipp->s_type = PATROL;
    shipp->s_width = 20;
    shipp->s_height = 20;
    shipp->s_phaserfuse = 8;
    shipp->s_repair = 50;
    shipp->s_letter = 'p';
    shipp->s_desig[0] = 'P';
    shipp->s_desig[1] = 'T';
    shipp->s_bitmap = PATROL;
    break;
  }
}

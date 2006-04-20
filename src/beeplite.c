/* beeplite.c
 *
 * $Log: beeplite.c,v $
 * Revision 1.3  2006/04/19 15:18:27  modemhero
 * Tidied up the save_options function, fixing some spacing errors, and puttting the
 * keymap/buttonmap on the top of the list due to its importance.  Also added the
 * sounddir option.  The eventual goal is to make save_options generate a file that contains
 * everything from your netrekrc file, so the options menus act as an in-game netrekrc editor.
 * Once everything is in place, save_options could write over your current netrekrc (the default
 * will be to write to a separate file, but there will be a menu option to save over your netrekrc).
 * Currently it writes to a separate file (netrek.sav) because it still doesn't save everything that is needed.
 * Default windows placements need to be saved, uselite options need to be saved, review window mapping
 * needs to be saved, option server/servernick/servertype needs to be saved, and lastly, the save
 * keymap routine is broken.
 *
 * Fixed mainResizeable loading from rc/saving.  Also made default setting = on.
 *
 * Revision 1.2  2006/04/19 13:02:33  modemhero
 * Rewrote TTS centering/refresh code so that text is properly centered and clears properly when the message expires.
 * The old code used an average text width that was not so good, resulting in poor centering for long messages.  The new code measures the length of the message directly.
 * Also, the W_ClearArea function was not catching the 3 left-most pixels for whatever reason, so the call to this function slightly extends the area to clear.
 * Updated COW manual to reflect change of tts_pos variable to tts_ypos.
 * This patch could be applied to other windows clients as well.
 *
 * Revision 1.1  2006/04/18 13:47:24  modemhero
 * First attempt at reimport of beeplite into NetrekXP.  Unresolved issue: improper text clearing on TTS beeplite messages.  Still to add: saving of beeplite settings in save_options.
 *
 * Revision 1.1.1.1  1998/11/01 17:24:08  siegl
 * COW 3.0 initial revision
 *
 */

#include "config.h"
#ifdef BEEPLITE
#include "copyright.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

#ifndef SERVER
#include "Wlib.h"
#endif

#include "defs.h"
#include "struct.h"
#include "data.h"
#include "map.h"
#include "proto.h"

rcdlite(struct distress *dist)
/* the info */
{
    char    message[100];
    int     len = 0;


    if ((dist == NULL) ||
    /* (dist->sender == me->p_no) || */
        (distlite[dist->distype] == NULL))

      return;

    len = makedistress(dist, message, distlite[dist->distype]);

    if (len <= 0)
      return;

    makelite(dist, message);
}


litedefaults(void)
{
    if (distlite[take] == NULL)
      distlite[take] = "/c/l";
    if (distlite[base_ogg] == NULL)
      distlite[base_ogg] = "/g/m";
    if (distlite[pickup] == NULL)
      distlite[pickup] = "/p";
    if (distlite[generic] == NULL)
      distlite[generic] = "%?%S=SB%{/c%}";
}

liteplanet(struct planet *l)
{
    emph_planet_seq_n[l->pl_no] = beep_lite_cycle_time_planet;
    l->pl_flags |= PLREDRAW;			 /* Leave redraw on until * * 
						  * done highlighting */
    l->pl_flags |= PLCLEAR;			 /* Leave redraw on until * * 
						  * done highlighting */
}

liteplayer(struct player *j)
{
    if (!j || (j->p_flags & PFCLOAK))
      return;

    redrawPlayer[j->p_no] = 1;

    emph_player_seq_n[j->p_no] = beep_lite_cycle_time_player;
}


/* small permutation on makedistress.  Searches for the highliting *
 * arguments, ignores everything else. */

makelite(struct distress * dist, char *pm)
/* the info */
/* macro to parse, used for distress and macro */
{
    struct player *sender;
    struct player *j;
    struct planet *l;
    char   *strcap(char *s);
    char    c;


    sender = &players[dist->sender];

    if (!(*pm))
    {
      return (0);
    }

    /* first step is to substitute variables */
    while (*pm)
    {
        if (*pm == '/')
	{
	    pm++;

	    if (!pm)
	      continue;

	    switch (c = *(pm++))
	    {

	    case 'P':				 /* push player id into buf */
	    case 'G':				 /* push friendly player id * 
						  * 
						  * * into buf */
	    case 'H':				 /* push enemy target player
						  * * * id into buf */

	    case 'p':				 /* push player id into buf */
	    case 'g':				 /* push friendly player id * 
						  * 
						  * * into buf */
	    case 'h':				 /* push enemy target player
						  * * * id into buf */

	        switch (c)
		{
		case 'p':
		    j = &players[dist->tclose_j];
		    break;
		case 'g':
		    j = &players[dist->tclose_fr];
		    break;
		case 'h':
		    j = &players[dist->tclose_en];
		    break;
		case 'P':
		    j = &players[dist->close_j];
		    break;
		case 'G':
		    j = &players[dist->close_fr];
		    break;
		default:
		    j = &players[dist->close_en];
		    break;
		}
	        liteflag = (F_beeplite_flags & (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL));
	        liteplayer(j);
	        break;

	    case 'B':				 /* highlites planet nearest
						  * * * sender */
	    case 'b':
	        l = &planets[dist->close_pl];
	        if (F_beeplite_flags & LITE_PLANETS)
		  liteplanet(l);
	        break;
	    case 'L':				 /* highlites planet nearest
						  * * * pointer */
	    case 'l':
	        l = &planets[dist->tclose_pl];
	        if (F_beeplite_flags & LITE_PLANETS)
		  liteplanet(l);
	        break;
	    case 'U':				 /* highlites enemy nearest * 
						  * 
						  * * pointer */
	    case 'u':
	        j = &players[dist->tclose_en];
	        liteflag = (F_beeplite_flags & (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL));
	        liteplayer(j);
	        break;
	    case 'c':				 /* highlites sender */
	    case 'I':
	    case 'i':
	        liteflag = (F_beeplite_flags & (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL));
	        liteplayer(sender);
	        break;
	    case 'M':				 /* highlites me */
	    case 'm':
	        if (F_beeplite_flags & LITE_SELF)
		{
		    liteflag = (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL);
		    liteplayer(me);
		}
	        break;
	    case '0':

	        if (F_beeplite_flags & LITE_SOUNDS)
		{

#ifdef SOUND
		    if (sound_toggle)
		      Play_Sound(MESSAGE_SOUND);
		    else
#endif

		    W_Beep();
		}
	        break;

#ifdef SOUND
	    case '1':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE1_SOUND);
	        break;
	    case '2':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE2_SOUND);
	        break;
	    case '3':
  	        if (F_beeplite_flags & LITE_SOUNDS)
	  	  Play_Sound(MESSAGE3_SOUND);
	        break;
	    case '4':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE4_SOUND);
	        break;
	    case '5':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE5_SOUND);
	        break;
	    case '6':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE6_SOUND);
	        break;
	    case '7':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE7_SOUND);
	        break;
	    case '8':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE8_SOUND);
	        break;
	    case '9':
	        if (F_beeplite_flags & LITE_SOUNDS)
		  Play_Sound(MESSAGE9_SOUND);
	        break;
#endif

	      /* Text between:  /|    |   will be displayed with TTS */
	    case '|':
	        if (F_beeplite_flags & LITE_TTS)
		{
		    for (tts_len = 0; (*pm != '|' && tts_len < tts_max_len); tts_len++)
		      lastIn[tts_len] = *pm++;
		    lastIn[tts_len] = '\0';
		    tts_timer = tts_time;
		}
	        break;

	    default:
	        /* try to continue * bad macro character is skipped entirely, *
	         * * * the message will be parsed without whatever argument has
	         * * * occurred. - jn */
	        warning("Bad Macro character in distress!");
	        fprintf(stderr, "Unrecognizable special character in distress pass 1: %c\n", *(pm - 1));
	        break;
	    }
	}
        else
	{
	    pm++;
	}

    }

  return (1);
}
#endif /* BEEPLITE */
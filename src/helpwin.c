/******************************************************************************/
/***  File:  helpwin.c                                                      ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***  helpwin.c copyright 1991 ERic mehlhaff Free to use, hack, etc. Just keep
/***  these credits here. Use of this code may be dangerous to your health
/***  and/or system. Its use is at your own risk. I assume no responsibility for
/***  damages, real, potential, or imagined, resulting  from the use of it.
/***  
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/


#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* this is a set of routines that makes up a multi column help window, * and
 * shows just what the keymaps current keymap representation of the * keys
 * are. *
 * 
 * fillhelp() handles the filling in if the strings for the help window *
 * update_Help_to_Keymap() checks the keymap and sets it up in hte * help
 * window. *
 * 
 * 
 * 
 * Format for each entry is follows: * first character is the hard-coded
 * character representation for the key * in the keymap.  Useful for when you
 * re-key things. This  could confuse * people who do'nt know the keymap
 * format. *
 * 
 * the next few spaces are either spaces or  keys that also do that * function.
 * Note that if you have more than 3 keys set to do the same * thing, they
 * will not be displayed. * So, you could, I suppose map everything to 'Q'
 * and it would not * show, but that's a pretty bizarre situation. *
 * 
 * 
 * 
 * Bugs & Comments: * You have to be sure that helpWin is defined to be big
 * enough to handle * all the messages.  That's pretty much a trial&error
 * by-hand thing * at this point *
 * 
 */



/* fills in the help window to note all keys also mapped to the *  listed
 * functions */
void update_Help_to_Keymap (char *helpmessage);

#define HELPMESSAGES	(sizeof(help_message)/ sizeof(char *))
/* this is the number of help messages there are */

char *help_message[] = {
    "0     Set speed",
    "1     Set speed",
    "2     Set speed",
    "3     Set speed",
    "4     Set speed",
    "5     Set speed",
    "6     Set speed",
    "7     Set speed",
    "8     Set speed",
    "9     Set speed",
    ")     speed = 10",
    "!     speed = 11",
    "@     speed = 12",
    "%     speed = maximum",
    "#     speed = 1/2 maximum",
    ".     Afterburners",
    "J     Engage Warp",
    "^j    Toggle warp prep",
    "<     slow speed 1",
    ">     speed up 1",
    "k     Set course",
    "p     Fire phaser",
    "t     Fire photon torpedo",
    "f     Fire plasma torpedo",
    "d     detonate enemy torps",
    "D     detonate own torps",
    "L     List players",
    "P     List planets",
    "S     Status graph toggle",
    "]     Put up shields",
    "[     Put down shields",
    "u     Shield toggle",
    "s     Shield toggle",
    "i     Info on player/planet",
    "I     Extended info on player",
    "b     Bomb planet",
    "z     Beam up armies",
    "x     Beam down armies",
    "{     Cloak",
    "}     Uncloak",
    "T     Toggle tractor beam",
    "y     Toggle pressor beam",
    "_     Turn on tractor beam",
    "^     Turn on pressor beam",
    "$     Turn off tractor/pressor beam",
    "R     Enter repair mode",
    "o     Orbit planet or dock to outpost",
    "e     Docking permission toggle",
    "r     Refit (change ship type)",
    "Q     Quit",
    "q     Fast Quit",
    "?     Message window toggle",
    "c     Cloaking device toggle",
    "l     Lock on to player/planet",
    ";     Lock on to planet/starbase",
    "h     Help window toggle",
    "H     Local hockey lines toggle",
    "G     Map hockey lines toggle",
    "w     War declarations window",
    "N     Planet names toggle",
    "*     Send practice bot/Warp to base",
    "E     Send Distress signal",
    "F     Send armies carried report",
    "U     Show rankings window",
    "m     Message Window Zoom",
    "/     Toggle sorted player list",
    "'     Toggle alternate player list",
    ":     Toggle message logging",
    "+     Show UDP options window",
    "=     Update all",
    ",     Ping stats window",

#ifdef SHORT_PACKETS
    "`     Toggle PacketWindow",
    "-     Update small",
    "|     Update medium",
#endif                          /* SHORT_PACKETS */

    "      (space) Unmap special windows",
    "\\     Reset user timer",

#ifdef NBT
    "X     Enter Macro Mode",
    "X?    Show current Macros",
#endif

#ifdef TOOLS
    "M     Toggle shell tools window",
#endif

#ifdef SOUND
    "~     Sound control window",
    "v     Decrease sound volume",
    "V     Increase sound volume",
#endif

    "&     Reread netrekrc.txt",
    "\"     Save current options",
    0
};

#define MAXHELP 40
/* maximum length in characters of key explanation */

#define PBHELPMESSAGES    (sizeof(pbhelp_message)/ sizeof(char *))

char *pbhelp_message[] = {
    "0     Stop playback",
    "1     Set playback speed (slow)",
    "2     Set playback speed",
    "3     Set playback speed (normal)",
    "4     Set playback speed",
    "5     Set playback speed",
    "6     Set playback speed",
    "7     Set playback speed",
    "8     Set playback speed",
    "9     Set playback speed",
    "!     Set playback speed (fastest)",
    "@     Set playback speed (as !)",
    "%     Set playback speed (as !)",
    "#     Set playback speed (as !)",
    "<     Speed up playback",
    ">     Speed down playback",
    "R     Start from the beginning",
    "(     Reverse playback",
    ")     Normal playback",
    "?     Message window toggle",
    "l     Lock on player",
    "i     Info on player",
    "I     Extended info on player",
    "h     This window",
    "H     Local hockey lines toggle",
    "G     Map hockey lines toggle",
    "S     Status graph toggle",
    0
};

struct help_message_location {
	int column;
	int row;
} hm_loc[HELPMESSAGES];



/******************************************************************************/
/***  fillhelp()                                                            ***/
/******************************************************************************/
void
fillhelp (void)
{
    register int i = 0, row, column;
    char helpmessage[MAXHELP];


    /* 4 column help window. This may be changed depending on font size */
    for (column = 0; column < 4; column++)
    {
        for (row = 1; row < HELPMESSAGES / 4 + 2; row++)
        {
            if (help_message[i] == 0)
                break;
            else
            {
                STRNCPY (helpmessage, help_message[i], sizeof (helpmessage));
                update_Help_to_Keymap (helpmessage);
                W_WriteText (helpWin, MAXHELP * column, row - 1, textColor,
                             helpmessage, strlen (helpmessage),
                             W_RegularFont);
				hm_loc[i].column = column;
				hm_loc[i].row = row - 1;
                i++;
            }
        }
        if (help_message[i] == 0)
            break;
    }
}


/******************************************************************************/
/***  helpaction()                                                          ***/
/******************************************************************************/
void
helpaction (W_Event * data)
{
	int i, message_number = -1;
	int row, column = 0;

	/* Close window? */
	if (data->key == W_MBUTTON
#ifdef SHIFTED_MOUSE
            || (data->key == W_RBUTTON2)
#endif
	)
	{
		W_UnmapWindow (helpWin);
		return;
	}

	/* Let's find row and column from mouse coordinates */
	row = (data->y - 4)/ W_Textheight;
	for (i = 0; i < 4; i++)
	{
		if (((MAXHELP * (i + 1)) - ((data->x - 3) / W_Textwidth)) > 0)
		{
			column = i;
			break;
		}
	}

	/* Now we want to know what message sits there */
	for (i = 0; i < HELPMESSAGES; i++)
	{
		if (hm_loc[i].column == column &&
			hm_loc[i].row == row)
		{
			message_number = i;
			break;
		}
	}

	if (message_number != -1)
	{
		/* And now we will remap the key in that message */
		myship->s_keymap[(data->key) - 32] = help_message[message_number][0];

		/* Finally we want to update window */
		fillhelp ();
	}
}

/******************************************************************************/
/***  pbfillhelp()                                                          ***/
/******************************************************************************/
void
pbfillhelp (void)
{
    register int i = 0, row, column;
    char helpmessage[MAXHELP];


    /* 2 column help window. THis may be changed depending on font size */
    for (column = 0; column < 2; column++)
    {
        for (row = 1; row < PBHELPMESSAGES / 2 + 1; row++)
        {
            if (pbhelp_message[i] == 0)
                break;
            else
            {
                STRNCPY (helpmessage, pbhelp_message[i], sizeof (helpmessage));
                W_WriteText (helpWin, MAXHELP * column, row - 1, textColor,
                             helpmessage, strlen (helpmessage),
                             W_RegularFont);
                i++;
            }
        }
        if (pbhelp_message[i] == 0)
            break;
    }
}


/******************************************************************************/
/* update_Help_to_Keymap()
/*
/* this takes the help messages and puts in the keymap, so the player can see
/* just what does  what!
/* 
/* ordinary format:       "U     Show rankings window", translatedd here to "[
/* sE  Computer options window", */
/******************************************************************************/
void
update_Help_to_Keymap (char *helpmessage)
{
    int i, num_mapped = 0;
	int default_set = 0;
    char key;


    key = helpmessage[0];


    if (key < 32 || key > 126)
        return;

    if (strlen (helpmessage) < 6)
        return;

    for (i = 0; i < 96; i++)
    {
        if (myship->s_keymap[i] != key)
            continue;
        if (i + 32 == key)
	{
	    default_set = 1;
            continue;
	} /* it's already there! don't add it! */

        /* we've found a key mapped to key! */
        /* the key is i+32 */
        num_mapped++;
        if (num_mapped > 3)
            continue;           /* we've found too many! */

        /* put the key in the string */
        if (i == 0)
            helpmessage[1 + num_mapped] = 2;    /* Let's draw space-like character */
        else
            helpmessage[1 + num_mapped] = (char) (i + 32);
    }


    /* clear spaces if any area available */
    switch (num_mapped)
    {
    case 0:
		if (!default_set)
		{
			helpmessage[2] = 'O';
			helpmessage[3] = 'f';
			helpmessage[4] = 'f';
			break;
		}
		else
			helpmessage[2] = ' ';
    case 1:
        helpmessage[3] = ' ';
    case 2:
        helpmessage[4] = ' ';
    case 3:
    default:
        helpmessage[5] = ' ';
    }

    return;
}

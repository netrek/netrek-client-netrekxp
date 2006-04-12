
/* playerlist.c
 * 
 * Fairly substantial re-write to do variable player lists: Sept 93 DRG Major
 * rewrite for demand driven updates -> huge speedup: Oct. 94 [007] Major
 * rewrite to fix some bugs and speed things up     : Jan. 95 [Zork]
 * 
 * Sort observers separatly: opserver if (pl->p_flags & PFOBSERV) is true.  : Jul 01 [SRS]
 *
 */

#include "config.h"
#include "copyright.h"

#include <stdio.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "string_util.h"
#include "playerlist.h"
#include "proto.h"

#define MaxPlistField 18        /* The width of the longest
                                 * possible plist field */

#define IsEmpty(x) (!x)
#define IsNotEmpty(x) (x)
#define IsZero(x) (!x)
#define IsNotZero(x) (x)
#define TRUE 1
#define FALSE 0

/* Current list of flags       */
/* ' ' - White Space           */
/* 'b' - Armies Bombed         */
/* 'd' - Damage Inflicted (DI) */
/* 'k' - Max Kills             */
/* 'l' - Login Name            */
/* 'n' - Ship Number           */
/* 'p' - Planets Taken         */
/* 'r' - Ratio                 */
/* 's' - Speed                 */
/* 'v' - Deaths per hour       */
/* 'w' - War staus             */
/* 'B' - Bombing               */
/* 'C' - Curt (short) rank     */
/* 'D' - Defense               */
/* 'H' - Hours Played          */
/* 'K' - Kills                 */
/* 'L' - Losses                */
/* 'M' - Display, Host Machine */
/* 'N' - Name                  */
/* 'O' - Offense               */
/* 'P' - Planets               */
/* 'R' - Rank                  */
/* 'S' - Total Rating (stats)  */
/* 'T' - Ship Type             */
/* 'V' - Kills per hour        */
/* 'W' - Wins                  */

/*
   Global Variables

   partitionPlist    : Separate the goodies from baddies in the sorted list?
   plistCustomLayout : The value of `playerlist' in the defaults file.
   plistReorder      : True only if the order of the playerlist is out of date.
   playerListStyle   : The current style number for the player list.
   plistUpdated      : True only if the player list is out of date.
   sortMyTeamFirst   : Should my team go first or second in the sorted list?
   sortPlayers       : Should the player list be sorted?
   updatePlayer[plr] : The playerlist entry for "plr" is old.

   plistHasHostile   : True if "Hostile" is a field in the current list.
   plistHasSpeed     : True if "Speed" is true in the current playerlist.
*/

int partitionPlist = FALSE;
char *plistCustomLayout;
int plistReorder = FALSE;
int playerListStyle = 0;
int playerListObserver = 0;
int plistUpdated = FALSE;
int sortMyTeamFirst = FALSE;
int sortPlayers = FALSE;
char updatePlayer[MAXPLAYER + 1];

#ifdef PLIST2
int plistHasHostile = FALSE;
int plistHasSpeed = FALSE;
#endif /* PLIST2 */


/*
   Local Variables

   plistLayout       : The fields in the current playerlist.
   plistPos[plr]     : The player list row assigned to each player.
   plistWidth        : The width of the playerlist.
   my_classes        : The letters to go with each ship type.
*/

char *plistLayout = "";
static int plistPos[MAXPLAYER];
static int plistWidth = 0;
static char *my_classes[NUM_TYPES] =
    { "SC", "DD", "CA", "BB", "AS", "SB", "GA", "??" };

/* Local Functions */

static int PlistHeader (char *layout,
                        int doWrite);
static void PlistLine (struct player *j,
                       int pos);
static void WriteSortedPlist (void);
static void WriteUnsortedPlist (void);

int pl_row[MAXPLAYER];  /* players location in current plist */

void
InitPlayerList ()
/* Set the global variables associtated with the player list.  This
 * should be called when the defaults file is loaded or reloaded.
 */
{
    /* Find the default style number for the player list. */

    plistCustomLayout = stringDefault ("playerlist");



    /* Select a style number base on the information in the defaults
     * (.xtrekrc) file. */

/* Default this to Kill Watch style - SRS 11/29/02 */
    playerListStyle = intDefault ("playerListStyle", 3);

    if (IsZero (playerListStyle) && IsEmpty (plistCustomLayout))
    {
        fputs
            ("Warning: `playerListStyle' set to 0 but `playerlist' is not set.\n",
             stderr);
        fputs ("\tPlease correct your defaults (.xtrekrc) file.\n", stderr);
        playerListStyle = -1;
    }

#ifdef ALWAYS_USE_PLIST_CUSTOM_LAYOUT
    if (IsNotEmpty (plistCustomLayout))
        playerListStyle = 0;
    else
#endif

    if (playerListStyle > PLISTLASTSTYLE)
    {
        playerListStyle = PLISTLASTSTYLE;
    }
    else if (playerListStyle == -1)
    {
        /* Backward compatibility */

        if (IsNotEmpty (plistCustomLayout))
        {
            playerListStyle = 0;
        }
        else if (booleanDefault ("newPlist", FALSE))
        {
            playerListStyle = 2;
        }
        else
            /* use the old playerlist */
        {
            playerListStyle = 1;
        }
    }


    /* Read the partitionPlist flag to tell us whether to break up the player
     * list with blank lines. */

    partitionPlist = booleanDefault ("partitionPlist", partitionPlist);


    /* Sort the playerlist? */

    sortPlayers = booleanDefault ("sortPlayers", TRUE);


    /* My team goes first (or second)? */

    sortMyTeamFirst = booleanDefault ("sortMyTeamFirst", sortMyTeamFirst);

    /* Do we show observers in playerlist?  0 = all, 1 = only players, 2 = only observers */
    playerListObserver = intDefault ("playerListObserver", 0);

    /* plistUpdate[MAXPLAYER] must always be TRUE because thats how we no when
     * to stop looking for a changed player. */

    updatePlayer[MAXPLAYER] = TRUE;
    RedrawPlayerList ();
}


int
PlistMaxWidth ()
/* Calculate the maximum width of all the defined player lists so that the
 * width of the player list window can be defined. */
{
    plistCustomLayout = stringDefault ("playerlist");

    if (IsNotEmpty (plistCustomLayout))
        plistWidth = PlistHeader (plistCustomLayout, FALSE);
    else
        plistWidth = 0;

    if (plistWidth < 83)
        plistWidth = 83;

    return plistWidth;
}


void
RedrawPlayerList ()
/* Completly redraw the player list, rather than incrementally updating the
 * list as with UpdatePlayerList().
 * 
 * This function should be called if the playerListStyle changes or if the
 * window has just been revealed.
 * 
 * This function is called automatically from InitPlayerList. */
{
    int i;

    if (IsEmpty (me) || !W_IsMapped (playerw))
        return;


    /* Translate this style number into a player list layout string. */

    switch (playerListStyle)
    {
    case 0:                    /* Custom */
        if (IsEmpty (plistCustomLayout))
            plistLayout = "";
        else
            plistLayout = plistCustomLayout;
        break;

    case 1:                    /* Old Style */
        plistLayout = "nTRNKWLr O D d ";
        break;

    case 2:                    /* COW Style */
        plistLayout = "nTR N  K lrSd";
        break;

    case 3:                    /* Kill Watch Style */
        plistLayout = "nTK  RNlr Sd";
        break;

    case 4:                    /* BRMH Style */
        plistLayout = "nTR N  K l M";
        break;

    default:
        fprintf (stderr, "Unknown playerListStyle in ChangedPlistStyle().\n");
        break;
    }

    // If this is the Observer only player listing,
    // use a custom playerlist useful to that purpose
    if (playerListObserver == 2)
        plistLayout = "n N l M";

    /* Redraw the player list. */

    W_ClearWindow (playerw);
    (void) PlistHeader (plistLayout, TRUE);

    plistReorder = TRUE;
    plistUpdated = TRUE;

    for (i = 0; i < MAXPLAYER; i++)
        updatePlayer[i] = TRUE;

    UpdatePlistFn ();
}


void
UpdatePlistFn ()
/* Update the player list.
 * 
 * This function should usually be called through the UpdatePlayerList() macro
 * (see playerlist.h).
 * 
 * This function works incrimentally.  If a dramatic change has taken place
 * (i.e. if playerListStyle changes) then RedrawPlayerList() should be called
 * instead. */
{
    int count;
    char *update;

    plistUpdated = FALSE;

    if (!W_IsMapped (playerw))
        return;

    if (!plistReorder)
    {
        /* Redraw the lines that have changed. */

        update = updatePlayer - 1;

        for (;;)
        {
            /* Find the changed player as quickly as possible. */

            do
                ++update;
            while (!(*update));


            /* Is this a valid player?  Remember updatePlayer[MAXPLAYER] is
             * always TRUE to make the above loop more efficient.  */

            count = update - updatePlayer;
            if (count == MAXPLAYER)
                break;

            *update = FALSE;


            /* We should not get updates for free players any more, but just
             * incase a packet arrives late... */

            if (playerListHack)
            {
                if (players[count].p_status != PFREE)
                {
                    if (playerListObserver == 0)
                        PlistLine (players + count, plistPos[count]);
                    else if (playerListObserver == 1 &&
                             players[count].p_mapchars[1] <= 'f')
                        PlistLine (players + count, plistPos[count]);
                    else if (playerListObserver == 2 &&
                            !(players[count].p_mapchars[1] <= 'f'))
                        PlistLine (players + count, plistPos[count]);
                }
            }
            else
            {
                if ((players[count].p_status != PFREE) &&
                    ((playerListObserver == 0) ||
                    ((playerListObserver == 1) &&
                    !(players[count].p_flags & PFOBSERV)) ||
                    ((playerListObserver == 2) &&
                    (players[count].p_flags & PFOBSERV))))
                    PlistLine (players + count, plistPos[count]);
            }
        }
    }
    else
    {
        /* Reorder the player list.  Note that this may not require a full
         * rewrite. */

        plistReorder = FALSE;

        if (sortPlayers)
            WriteSortedPlist ();
        else
            WriteUnsortedPlist ();
    }
}


static void
WriteSortedPlist ()
/* Update the order of the players in the list and write out any changes. */
{
    int row, i, last;
    struct player *current;
    int teamPos[NUMTEAM + 1];
    int *pos;

    static int plistLastRow = -1;
    static int blankLine = -1;
    static int blankLine2 = -1;
    static int myTeam = -1;


    /* If I have changed team, redraw everything (to get the colors right). */

    if (remap[me->p_team] != myTeam)
    {
        myTeam = remap[me->p_team];

        for (pos = plistPos + MAXPLAYER - 1; pos >= plistPos; --pos)
            *pos = -1;
    }


    /* If partitionPlist is false, reset the blank line markers */

    if (!partitionPlist)
    {
        blankLine = -1;
        blankLine2 = -1;
    }


    /* Count the number of players in each team. */

    for (i = NUMTEAM; i >= 0; --i)
        teamPos[i] = 0;

    for (current = players + MAXPLAYER - 1; current >= players; --current)
    {
        if (playerListHack)
        {
            if (current->p_status != PFREE)
            {
                if (playerListObserver == 0)
                    ++teamPos[remap[current->p_team]];
                else if (playerListObserver == 1 &&
                         current->p_mapchars[1] <= 'f')
                    ++teamPos[remap[current->p_team]];
                else if (playerListObserver == 2 &&
                        !(current->p_mapchars[1] <= 'f'))
                    ++teamPos[remap[current->p_team]];
            }
        }
        else
        {
            if ((current->p_status != PFREE) &&
                ((playerListObserver == 0) ||
                ((playerListObserver == 1) && !(current->p_flags & PFOBSERV))
                || ((playerListObserver == 2)
                && (current->p_flags & PFOBSERV))))
                ++teamPos[remap[current->p_team]];
        }
    }


    /* Find the row after the last player in each team. */

    last = 1;                   /* The title is line zero */

    if (sortMyTeamFirst)        /* My team comes at the top */
    {
        last += teamPos[myTeam];
        teamPos[myTeam] = last;

        if (partitionPlist)
        {
            if (blankLine != last)
            {
                blankLine = last;
                W_ClearArea (playerw, 0, last, plistWidth, 1);
            }

            ++last;
        }
    }

    for (i = 1; i <= NUMTEAM; ++i)
    {
        if (i != myTeam)
        {
            last += teamPos[i];
            teamPos[i] = last;
        }
    }

    if (!sortMyTeamFirst)       /* My team comes below the others */
    {
        if (partitionPlist)
        {
            if (blankLine != last)
            {
                blankLine = last;
                W_ClearArea (playerw, 0, last, plistWidth, 1);
            }

            ++last;
        }

        last += teamPos[myTeam];
        teamPos[myTeam] = last;
    }

    if (myTeam != NOBODY)
    {
        /* Separate the goodies from the arriving players. */

        if (partitionPlist)
        {
            if (blankLine2 != last)
            {
                blankLine2 = last;
                W_ClearArea (playerw, 0, last, plistWidth, 1);
            }

            ++last;
        }

        last += teamPos[NOBODY];
        teamPos[NOBODY] = last;
    }


    /* Clear some lines if people have left. */

    for (row = last; row < plistLastRow; ++row)
        W_ClearArea (playerw, 0, row, plistWidth, 1);

    plistLastRow = last;



    /* Write out each player that has either changed position or has
     * new stats. */

    for (i = MAXPLAYER - 1, current = players + MAXPLAYER - 1;
         i >= 0; --i, --current)
    {
        if (current->p_status == PFREE)
        {
            updatePlayer[i] = FALSE;
            continue;
        }
        if (playerListHack)
        {
            if (playerListObserver == 0)
            {
                row = --(teamPos[remap[current->p_team]]);
                if ((!updatePlayer[i]) && plistPos[i] == row)
                    continue;

                plistPos[i] = row;
                updatePlayer[i] = FALSE;

                PlistLine (current, row);
            }
            else if (playerListObserver == 1 &&
                     current->p_mapchars[1] <= 'f')
            {
                row = --(teamPos[remap[current->p_team]]);
                if ((!updatePlayer[i]) && plistPos[i] == row)
                    continue;

                plistPos[i] = row;
                updatePlayer[i] = FALSE;

                PlistLine (current, row);
            }
            else if (playerListObserver == 2 &&
                    !(current->p_mapchars[1] <= 'f'))
            {
                row = --(teamPos[remap[current->p_team]]);
                if ((!updatePlayer[i]) && plistPos[i] == row)
                    continue;

                plistPos[i] = row;
                updatePlayer[i] = FALSE;

                PlistLine (current, row);
            }
        }            
        else
        {
            if ((playerListObserver == 0) ||
                ((playerListObserver == 1) && !(current->p_flags & PFOBSERV)) ||
                ((playerListObserver == 2) && (current->p_flags & PFOBSERV)))
            {
                row = --(teamPos[remap[current->p_team]]);
                if ((!updatePlayer[i]) && plistPos[i] == row)
                    continue;

                plistPos[i] = row;
                updatePlayer[i] = FALSE;

                PlistLine (current, row);
            }
        }
    }

}


static void
WriteUnsortedPlist (void)
/*
 *  Update the order of the players in the list and write out any
 *  changes.
 */
{
    int count;
    int pos;
    char *update;
    static int myTeam = -1;


    /*
     *  If I have changed team, redraw everything (to get the colors
     *  right).
     */

    if (remap[me->p_team] != myTeam)
    {
        myTeam = remap[me->p_team];

        for (update = updatePlayer + MAXPLAYER; update >= updatePlayer;
             --update)
        {
            *update = TRUE;
        }
    }


    update = updatePlayer - 1;

    for (;;)
    {
        /* Find the changed player as quickly as possible. */

        do
            ++update;
        while (!(*update));


        /* Is this a valid player?  Remember updatePlayer[MAXPLAYER]
         * is always TRUE to make the above loop more efficient.       */

        count = update - updatePlayer;
        if (count == MAXPLAYER)
            break;


        /* Update the player. */
        *update = FALSE;

        pos = count + 1;
        plistPos[count] = pos;

        if (players[count].p_status != PFREE)
            PlistLine (players + count, pos);
        else
            W_ClearArea (playerw, 0, pos, plistWidth, 1);
    }
}



static int
PlistHeader (char *layout,
             int doWrite)
/* Analyse the heading (field names) for a player list, and set the
 * plistHasSpeed and plistHasHostile flags.
 * 
 * If doWrite is TRUE, write the heading to the list.
 * 
 * RETURN the width of the player list. */
{
    char header[BUFSIZ];
    int num = 0;

#ifdef PLIST2
    plistHasSpeed = FALSE;
    plistHasHostile = FALSE;
#endif /* PLIST2 */

    for (; IsNotZero (*layout); ++layout)
    {
        if (num + MaxPlistField >= BUFSIZ)
        {
            /* Assume that we have tested the standard layouts so that only
             * custom layouts can be too long.
             * 
             * If a standard layout is found to be too long then some compiler's
             * code will dump core here because of an attempt to write over a
             * constant string. */

            fprintf (stderr, "Playerlist truncated to fit buffer.\n");
            layout = '\0';
            break;
        }

        switch (*layout)
        {
        case 'n':              /* Ship Number */
            STRNCPY (&header[num], " No", 3);
            num += 3;
            break;
        case 'T':              /* Ship Type */
            STRNCPY (&header[num], " Ty", 3);
            num += 3;
            break;
        case 'C':              /* Curt (short) Rank */
            STRNCPY (&header[num], " Rank", 5);
            num += 5;
            break;
        case 'R':              /* Rank */
            STRNCPY (&header[num], " Rank      ", 11);
            num += 11;
            break;
        case 'N':              /* Name */
            STRNCPY (&header[num], " Name            ", 17);
            num += 17;
            break;
        case 'K':              /* Kills */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Kills", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Kills", 6);
                num += 6;
            }
            break;
        case 'l':              /* Login Name */
            STRNCPY (&header[num], " Login           ", 17);
            num += 17;
            break;
        case 'O':              /* Offense */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Offse", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Offse", 6);
                num += 6;
            }
            break;
        case 'W':              /* Wins */
            STRNCPY (&header[num], "  Wins", 6);
            num += 6;
            break;
        case 'D':              /* Defense */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Defse", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }   
            else
            {
                STRNCPY (&header[num], " Defse", 6);
                num += 6;
            }
            break;
        case 'L':              /* Losses */
            STRNCPY (&header[num], "  Loss", 6);
            num += 6;
            break;
        case 'S':              /* Total Rating (stats) */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Stats", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Stats", 6);
                num += 6;
            }
            break;
        case 'r':              /* Ratio */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Ratio", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Ratio", 6);
                num += 6;
            }
            break;
        case 'd':              /* Damage Inflicted (DI) */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], "   DI   ", 6 + (*(layout + 1) - '0'));
                num += 6 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], "      DI", 8);
                num += 8;
            }
            break;
        case ' ':              /* White Space */
            header[num] = ' ';
            num += 1;
            break;

#ifdef PLIST1
        case 'B':              /* Bombing */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Bmbng", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Bmbng", 6);
                num += 6;
            }
            break;
        case 'b':              /* Armies Bombed */
            STRNCPY (&header[num], " Bmbed", 6);
            num += 6;
            break;
        case 'P':              /* Planets */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Plnts", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Plnts", 6);
                num += 6;
            }
            break;
        case 'p':              /* Planets Taken */
            STRNCPY (&header[num], " Plnts", 6);
            num += 6;
            break;
        case 'M':              /* Display, Host Machine */
            STRNCPY (&header[num], " Host Machine    ", 17);
            num += 17;
            break;
        case 'H':              /* Hours Played */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Hours ", 5 + (*(layout + 1) - '0'));
                num += 5 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Hours ", 7);
                num += 7;
            }
            break;
        case 'k':              /* Max Kills */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '2')
            {
                STRNCPY (&header[num], " Max K", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], " Max K", 6);
                num += 6;
            }
            break;
        case 'V':              /* Kills per hour */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '1')
            {
                STRNCPY (&header[num], "   KPH", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], "   KPH", 6);
                num += 6;
            }
            break;
        case 'v':              /* Deaths per hour */
            if (*(layout + 1) >= '0' && *(layout + 1) <= '1')
            {
                STRNCPY (&header[num], "   DPH", 4 + (*(layout + 1) - '0'));
                num += 4 + (*(layout + 1) - '0');
                layout++;
            }
            else
            {
                STRNCPY (&header[num], "   DPH", 6);
                num += 6;
            }
            break;
#endif

#ifdef PLIST2
        case 'w':              /* War staus */
            STRNCPY (&header[num], " War Stat", 9);
            num += 9;
            plistHasHostile = TRUE;
            break;
        case 's':              /* Speed */
            STRNCPY (&header[num], " Sp", 3);
            num += 3;
            plistHasSpeed = TRUE;
            break;
#endif

        default:
            fprintf (stderr,
                     "%c is not an option for the playerlist\n", *layout);
            break;
        }
    }

    header[num] = '\0';

    if (doWrite)
        W_WriteText (playerw, 0, 0, textColor, header, num, W_RegularFont);

    return num;
}


static void
PlistLine (struct player *j,
           int pos)
/* Write the player list entry for player `j' on line `pos'. */
{
    char buf[BUFSIZ];
    char *ptr;
    char *buffPoint;
    int kills, losses, my_ticks;
    float pRating, oRating, dRating, bRating, Ratings;
    float KillsPerHour, LossesPerHour;  /* Added 12/27/93 ATH */
    double ratio, max_kills;

    /* Fill pl_row to get right player placement in the list */
    pl_row[j->p_no] = pos;

    if (j->p_ship.s_type == STARBASE)
    {
        kills = j->p_stats.st_sbkills;
        losses = j->p_stats.st_sblosses;
        max_kills = j->p_stats.st_sbmaxkills;

        if (SBhours)
            my_ticks = j->p_stats.st_sbticks;
        else
            my_ticks = j->p_stats.st_tticks;

        KillsPerHour = (float) ((float) (my_ticks == 0) ? 0.0 :
            (float) kills *36000.0 / (float) my_ticks);

        LossesPerHour = (float) ((float) (my_ticks == 0) ? 0.0 :
            (float) losses *36000.0 / (float) my_ticks);
    }
    else
    {
        kills = j->p_stats.st_kills + j->p_stats.st_tkills;
        losses = j->p_stats.st_losses + j->p_stats.st_tlosses;
        max_kills = j->p_stats.st_maxkills;
        my_ticks = j->p_stats.st_tticks;
        KillsPerHour = (float) ((float) (my_ticks == 0) ? 0.0 :
            (float) j->p_stats.st_tkills * 36000.0 / (float) my_ticks);
        LossesPerHour = (float) ((float) (my_ticks == 0) ? 0.0 :
            (float) j->p_stats.st_tlosses * 36000.0 / (float) my_ticks);
    }

    if (losses == 0)
        ratio = (double) kills;
    else
        ratio = (double) kills / (double) losses;

    if (!j->p_stats.st_tticks)
    {
        oRating = pRating = bRating = Ratings = 0.0;
        dRating = defenseRating (j);
    }
    else
    {
        oRating = offenseRating (j);
        pRating = planetRating (j);
        bRating = bombingRating (j);
        Ratings = oRating + pRating + bRating;
        dRating = defenseRating (j);
        if ((j->p_ship.s_type == STARBASE) && (SBhours))
        {                       /* If SB, show KPH for
                                 * offense etc. */
            oRating = KillsPerHour;
            dRating = LossesPerHour;
        }
    }


    buffPoint = buf;

    for (ptr = plistLayout; IsNotZero (*ptr); ++ptr)
    {
        *(buffPoint++) = ' ';

        switch (*ptr)
        {
        case 'n':              /* Ship Number */
            if (j->p_status != PALIVE)
            {
                *(buffPoint++) = ' ';
                *(buffPoint++) = shipnos[j->p_no];
            }
            else
            {
                *(buffPoint++) = teamlet[j->p_team];
                *(buffPoint++) = shipnos[j->p_no];
            }

            break;

        case 'T':              /* Ship Type */
            if (j->p_status != PALIVE)
            {
                *(buffPoint++) = ' ';
                *(buffPoint++) = ' ';
            }
            else
            {
                *(buffPoint++) = my_classes[j->p_ship.s_type][0];
                *(buffPoint++) = my_classes[j->p_ship.s_type][1];
            }

            break;

        case 'C':              /* Curt (short) Rank */
            format (buffPoint, ranks[j->p_stats.st_rank].cname, 4, 0);
            buffPoint += 4;
            break;

        case 'R':              /* Rank */
            format (buffPoint, ranks[j->p_stats.st_rank].name, 10, 0);
            buffPoint += 10;
            break;

        case 'N':              /* Name */
            format (buffPoint, j->p_name, 16, 0);
            buffPoint += 16;
            break;

        case 'K':              /* Kills */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                if (j->p_kills > 100.0)
                    /* Cheat a bit */
                    ftoa (j->p_kills, buffPoint - 1, 0, 3, (*(ptr + 1) - '0'));
                else
                    ftoa (j->p_kills, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                if (j->p_kills > 100.0)
                    /* Cheat a bit */
                    ftoa (j->p_kills, buffPoint - 1, 0, 3, 2);
                else
                    ftoa (j->p_kills, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'l':              /* Login Name */
            format (buffPoint, j->p_login, 16, 0);
            buffPoint += 16;
            break;

        case 'O':              /* Offense */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa (oRating, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (oRating, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'W':              /* Wins */
            itoapad (kills, buffPoint, 0, 5);
            buffPoint += 5;
            break;

        case 'D':              /* Defense */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa (dRating, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (dRating, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'L':              /* Losses */
            itoapad (losses, buffPoint, 0, 5);
            buffPoint += 5;
            break;

        case 'S':              /* Total Rating (stats) */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa (Ratings, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (Ratings, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'r':              /* Ratio */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa ((float) ratio, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa ((float) ratio, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'd':              /* Damage Inflicted (DI) */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa ((float) (Ratings * (j->p_stats.st_tticks / 36000.0)),
                        buffPoint, 0, 4, (*(ptr + 1) - '0'));
                buffPoint += 5 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa ((float) (Ratings * (j->p_stats.st_tticks / 36000.0)),
                        buffPoint, 0, 4, 2);
                buffPoint += 7;
            }
            break;

        case ' ':              /* White Space */
            break;

#ifdef PLIST1
        case 'B':              /* Bombing */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa (bRating, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (bRating, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'b':              /* Armies Bombed */
            itoapad (j->p_stats.st_tarmsbomb + j->p_stats.st_armsbomb,
                     buffPoint, 0, 5);
            buffPoint += 5;
            break;

        case 'P':              /* Planets */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa (pRating, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (pRating, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'p':              /* Planets Taken */
            itoapad (j->p_stats.st_tplanets + j->p_stats.st_planets,
                     buffPoint, 0, 5);
            buffPoint += 5;
            break;

        case 'M':              /* Display, Host Machine */
            format (buffPoint, j->p_monitor, 16, 0);
            buffPoint += 16;
            break;

        case 'H':              /* Hours Played */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa ((float) (my_ticks / 36000.0), buffPoint, 0, 3, (*(ptr + 1) - '0'));
                buffPoint += 4 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa ((float) (my_ticks / 36000.0), buffPoint, 0, 3, 2);
                buffPoint += 6;
            }
            break;

        case 'k':              /* Max Kills  */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '2')
            {
                ftoa ((float) max_kills, buffPoint, 0, 2, (*(ptr + 1) - '0'));
                buffPoint += 3 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa ((float) max_kills, buffPoint, 0, 2, 2);
                buffPoint += 5;
            }
            break;

        case 'V':              /* Kills Per Hour  */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '1')
            {
                ftoa (KillsPerHour, buffPoint, 0, 3, (*(ptr + 1) - '0'));
                buffPoint += 4 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (KillsPerHour, buffPoint, 0, 3, 1);
                buffPoint += 5;
            }
            break;

        case 'v':              /* Deaths Per Hour  */
            if (*(ptr + 1) >= '0' && *(ptr + 1) <= '1')
            {
                ftoa (LossesPerHour, buffPoint, 0, 3, (*(ptr + 1) - '0'));
                buffPoint += 4 + (*(ptr + 1) - '0');
                ptr++;
            }
            else
            {
                ftoa (LossesPerHour, buffPoint, 0, 3, 1);
                buffPoint += 5;
            }
            break;
#endif

#ifdef PLIST2
        case 'w':              /* War staus */
            if (j->p_swar & me->p_team)
                STRNCPY (buffPoint, "WAR     ", 8);
            else if (j->p_hostile & me->p_team)
                STRNCPY (buffPoint, "HOSTILE ", 8);
            else
                STRNCPY (buffPoint, "PEACEFUL", 8);

            buffPoint += 8;
            break;

        case 's':              /* Speed */
            itoapad (j->p_speed, buffPoint, 0, 2);
            buffPoint += 2;
            break;
#endif

        default:
            break;
        }
    }

    *buffPoint = '\0';
    // Don't draw the line if pos has been set to -1
    if (pos > -1)
        W_WriteText (playerw, 0, pos, playerColor (j),
                     buf, buffPoint - buf, shipFont (j));
}

int
GetPlayerFromPlist (int x, int y)
{
    int i;
    int player_no = MAXPLAYER; /* just to be sure the player does not exist */
    int flag = 0;

    if (y > MAXPLAYER - 1) y = MAXPLAYER - 1;
    else if (y < 0) y = 0;

    /* Let's find what player sits in poition y in the list */
    for (i=0; i < MAXPLAYER; i++)
        if (pl_row[i] == y)
        {
            if (playerListHack)
            {
                if (playerListObserver == 0)
                {
                    player_no = i;
                    break;
                }
                else if (playerListObserver == 1 &&
                        (players[i].p_mapchars[1] <= 'f'))
                {
                    player_no = i;
                    break;
                }
                else if (playerListObserver == 2 &&
                       !(players[i].p_mapchars[1] <= 'f'))
                {
                    player_no = i;
                    break;
                }
            }
            else
            {
                if (playerListObserver == 0)
                {
                    player_no = i;
                    break;
                }
                else if (playerListObserver == 1 &&
                       !(players[i].p_flags & PFOBSERV))
                {
                    player_no = i;
                    break;
                }
                else if (playerListObserver == 2 &&
                        (players[i].p_flags & PFOBSERV))
                {
                    player_no = i;
                    break;
                }
            }
        }

    if (player_no != MAXPLAYER)
    {
        if (playerListHack)
        {
            if (playerListObserver == 0)
                flag = 1;
            else if (playerListObserver == 1 &&
                    (players[player_no].p_mapchars[1] <= 'f'))
                flag = 1;
            else if (playerListObserver == 2 &&
                   !(players[player_no].p_mapchars[1] <= 'f'))
                flag = 1;
        }
        else
        {
            if (playerListObserver == 0)
                flag = 1;
            else if (playerListObserver == 1 &&
                   !(players[player_no].p_flags & PFOBSERV))
                flag = 1;
            else if (playerListObserver == 2 &&
                    (players[player_no].p_flags & PFOBSERV))
                flag = 1;
        }
    }

    if (flag)
    {
        if (playerListHack)
        {
            if (players[player_no].p_mapchars[1] <= 'f')
            {
                if (!((players[player_no].p_flags & PFCLOAK) &&
                      (players[player_no].p_team != me->p_team)) &&
                    ((players[player_no].p_x != -10000) && (players[player_no].p_y != -10000)) &&
					((players[player_no].p_x != -100000) && (players[player_no].p_y != -100000)))
                 return player_no;
            }
            else
                return player_no;
        }
        else
        {
            if (!(players[player_no].p_flags & PFOBSERV))
            {
                if (!((players[player_no].p_flags & PFCLOAK) && 
                      (players[player_no].p_team != me->p_team)) &&
                    ((players[player_no].p_x != -10000) && (players[player_no].p_y != -10000)) &&
					((players[player_no].p_x != -100000) && (players[player_no].p_y != -100000)))
                 return player_no;
            }
            else
                return player_no;
        }
    }

	// We didn't find player or its state is wrong
    return (-1);
}
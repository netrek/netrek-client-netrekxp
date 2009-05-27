/******************************************************************************/
/***  File: ranklist.c
/***
/***  Function:
/***
/***  Revisions:
/***    Kevin P. Smith 12/5/88
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/


#include "config.h"
#include "copyright2.h"

#include <stdio.h>

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* Calculate DI to next rank, following server logic */
float toNextRank(int rank)
{
    int hourratio;
    float rankDI, myDI, oRating, pRating, bRating, Ratings;
    
    /* TODO: add support for INL mode */
    if (!strcmp(me->p_name, "guest") || !strcmp(me->p_name, "Guest"))
        hourratio = 5;
    else
        hourratio = 1;

    oRating = offenseRating (me);
    pRating = planetRating (me);
    bRating = bombingRating (me);
    Ratings = oRating + pRating + bRating;
    myDI = (float) (Ratings * (me->p_stats.st_tticks / 36000.0));
    rankDI = ranks[rank].ratings * ranks[rank].hours / hourratio;

    if (Ratings > ranks[rank].ratings)
    {
        if (myDI > rankDI)
            return (0.0);
        else
            return (rankDI - myDI);
    }
    else if (Ratings > (ranks[rank-1].ratings))
    {
        if (myDI > 2*rankDI)
            return (0.0);
        else
            return (2*rankDI - myDI);
    }
    else if (me->p_stats.st_rank > 0 && Ratings > (ranks[rank-2].ratings))
    {
        if (myDI > 4*rankDI)
            return (0.0);
        else
            return (4*rankDI - myDI);
    }
    else if (me->p_stats.st_rank >= 4 && Ratings > (ranks[rank-3].ratings))
    {
        if (myDI > 8*rankDI)
            return (0.0);
        else
            return (8*rankDI - myDI);
    }
    else
        return (-1);
}

void
print_ranks_paradise(void)
{
    register int i;
    char    buf[80];

    (void) strcpy(buf, "  Rank       genocides  DI    battle strategy  special ships");
    W_WriteText(rankw, 1, 1, textColor, buf, strlen(buf), W_BoldFont);
    for (i = 0; i < nranks2; i++) {
	sprintf(buf, "%-11.11s %5d %8.2f %8.2f %8.2f   %7.2f",
		ranks2[i].name,
		ranks2[i].genocides,
		ranks2[i].di,
		ranks2[i].battle,
		ranks2[i].strategy,
		ranks2[i].specship);
	if (mystats->st_rank == i) {
	    W_WriteText(rankw, 1, i + 2, W_Cyan, buf, strlen(buf), W_BoldFont);
	} else {
	    W_WriteText(rankw, 1, i + 2, textColor, buf, strlen(buf), W_RegularFont);
	}
    }
    strcpy(buf, "To achieve a rank, you need a high enough number of");
    W_WriteText(rankw, 1, i + 3, textColor, buf, strlen(buf), W_RegularFont);
    strcpy(buf, "genocides, a high enough DI, a high enough battle");
    W_WriteText(rankw, 1, i + 4, textColor, buf, strlen(buf), W_RegularFont);
    strcpy(buf, "rating, a high enough strategy rating, and a high");
    W_WriteText(rankw, 1, i + 5, textColor, buf, strlen(buf), W_RegularFont);
    strcpy(buf, "enough special ship rating");
    W_WriteText(rankw, 1, i + 6, textColor, buf, strlen(buf), W_RegularFont);
}

void
ranklist (void)
{
    if (paradise)
        print_ranks_paradise();
    else
    {
    register int i;
    char buf[100];

    /* W_ClearWindow(rankw); */

    (void) strcpy (buf, "  Rank       Hours  Offense  Ratings      DI");
    W_WriteText (rankw, 1, 1, textColor, buf, strlen (buf), W_BoldFont);
    for (i = 0; i < nranks; i++)
    {
        sprintf (buf, "%-11.11s %5.0f %8.2f %8.2f   %7.2f",
                 ranks[i].name,
                 ranks[i].hours,
                 ranks[i].offense,
                 ranks[i].ratings, ranks[i].ratings * ranks[i].hours);
        if (mystats->st_rank == i)
        {
            if (i < nranks-1)
            {
                char buf2[35];
                float DI;
                if ((DI = toNextRank(i+1)) != -1)
                {
                    sprintf(buf2, " (%.2f DI until next rank)", DI);
                    strcat(buf, buf2);
                }
                else
                    strcat(buf, " Need higher ratings for next rank");
            }
            W_WriteText (rankw, 1, i + 2, W_Cyan, buf, strlen (buf),
                         W_BoldFont);
        }
        else
        {
            W_WriteText (rankw, 1, i + 2, textColor, buf, strlen (buf),
                         W_RegularFont);
        }
    }
    strcpy (buf, "To achieve a rank, you need the corresponding DI");
    W_WriteText (rankw, 1, i + 3, textColor, buf, strlen (buf),
                 W_RegularFont);
    strcpy (buf, "in less than the hours allowed (DI = ratings x hours).");
    W_WriteText (rankw, 1, i + 4, textColor, buf, strlen (buf),
                 W_RegularFont);
    strcpy (buf, "OR, get offense+bombing+planets above corresponding Ratings");
    W_WriteText (rankw, 1, i + 5, textColor, buf, strlen (buf),
                 W_RegularFont);
    strcpy (buf, "Promotions also occur at 2xDI with Ratings - 1");
    W_WriteText (rankw, 1, i + 6, textColor, buf, strlen (buf),
                 W_RegularFont);
    strcpy (buf, "4xDI with Ratings - 2, and 8xDI with Ratings - 3");
    W_WriteText (rankw, 1, i + 7, textColor, buf, strlen (buf),
                 W_RegularFont);
    }
}

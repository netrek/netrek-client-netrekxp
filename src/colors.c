/* colors.c
 * 
 * Kevin P. Smith  6/11/89 */
#include "config.h"
#include "copyright2.h"
#include <stdio.h>
#include <string.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

void
getColorDefs (void)
{
    borderColor = W_Grey;
    backColor = W_Black;
    foreColor = W_White;
    textColor = W_White;

#ifdef RACE_COLORS
    shipCol[0] = W_Ind;
    shipCol[1] = W_Fed;
    shipCol[2] = W_Rom;
    shipCol[3] = W_Kli;
    shipCol[4] = W_Ori;
#else
    shipCol[0] = W_Grey;
    shipCol[1] = W_Yellow;
    shipCol[2] = W_Red;
    shipCol[3] = W_Green;
    shipCol[4] = W_Cyan;
#endif

    warningColor = W_Red;
    unColor = W_Grey;
    rColor = W_Red;
    yColor = W_Yellow;
    gColor = W_Green;
    myColor = W_White;
}

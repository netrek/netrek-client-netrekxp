/* rotate.c
 * 
 */
#include "config.h"
#include "copyright2.h"

#include <stdio.h>
#include <sys/types.h>
#include <math.h>
#include <errno.h>
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "map.h"
#include "proto.h"

#ifdef ROTATERACE
void
rotate_dir (unsigned char *d,
            int r)
{
    (*d) += r;
}

/* general rotation */

void
rotate_coord (int *x,
              int *y,
              int d,
              int cx,
              int cy)
/* values used and returned */
/* degree (pi == 128) */
/* around center point */
{
    register int ox, oy;

    ox = *x;
    oy = *y;

    switch (d)
    {

    case 0:
        return;

    case 64:
    case -192:
        *x = cy - oy + cx;
        *y = ox - cx + cy;
        break;

    case 128:
    case -128:
        *x = cx - ox + cx;
        *y = cy - oy + cy;
        break;

    case 192:
    case -64:
        *x = oy - cy + cx;
        *y = cx - ox + cy;
        break;

    default:
        {
            /* do it the hard way */
            double dir;
            double r, dx, dy;
            double rd = (double) d * 3.1415927 / 128.;

            if (*x != cx || *y != cy)
            {
                dx = (double) (*x - cx);
                dy = (double) (cy - *y);
                dir = atan2 (dx, dy) - 3.1415927 / 2.;
                r = hypot (dx, dy);
                dir += rd;
                *x = (int) (r * cos (dir) + cx);
                *y = (int) (r * sin (dir) + cy);
            }
        }
    }
}

void
rotateGalaxy (void)
{
    register i;
    register struct planet *l;
    register struct player *j;

    redrawall = 1;
    reinitPlanets = 1;

    for (i = 0, l = planets; i < nplanets; i++, l++)
    {
        if (rotate)
        {
            rotate_deg = -old_rotate_deg + rotate * 64;
            rotate_coord (&l->pl_x, &l->pl_y, rotate_deg, GWIDTH / 2,
                          GWIDTH / 2);
            rotate_deg = rotate * 64;
        }
        else
        {
            rotate_deg = -old_rotate_deg;
            rotate_coord (&l->pl_x, &l->pl_y, rotate_deg, GWIDTH / 2,
                          GWIDTH / 2);
            rotate_deg = 0;
        }
    }

    /* we could wait for the server to do this but looks better if we
     * do it now. */
    for (i = 0, j = players; i < MAXPLAYER; i++, j++)
    {
        if (j->p_status != PALIVE)
            continue;
        if (rotate)
        {
            rotate_deg = -old_rotate_deg + rotate * 64;
            rotate_coord (&j->p_x, &j->p_y, rotate_deg,
                          GWIDTH / 2, GWIDTH / 2);
            rotate_dir (&j->p_dir, rotate_deg);

            rotate_deg = rotate * 64;
        }
        else
        {
            rotate_deg = -old_rotate_deg;
            rotate_coord (&j->p_x, &j->p_y, rotate_deg,
                          GWIDTH / 2, GWIDTH / 2);
            rotate_dir (&j->p_dir, rotate_deg);
            rotate_deg = 0;
        }
    }
    /* phasers/torps/etc .. wait for server */

    old_rotate = rotate;
    old_rotate_deg = rotate_deg;
}
#endif

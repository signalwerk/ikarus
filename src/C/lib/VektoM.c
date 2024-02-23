/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          VektoM.c

   Functions:       1) VektoMCalc()
                    2) VektoM()

---------------------------------------------------------------------------------

   Description:  calculation of the slope of the secant and the distance to the
                 next IK-point of a contour and storage in the knotlist

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow       17.07.1987
   Lau          01.09.1988           direct reference of sqrt..replaced by math.h
   H.Schwarz    04.01.1990           static Vekto4Calc()
   Rachow       28.02.1991           include IKBasic.h
   Rachow       07.05.1991           type Float, HelpKnots removed

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "VektoM.h"
#include <math.h>

static void VektoMCalc(pknot, x, y)
    KnotPtr pknot; /* pointer to the list of IK-points */
Float x, y;        /* coordinates of the next IK-point */

/*-----------------------------------------------------------------------------*/
{

    Float dx, dy, ds;

    dx = x - pknot->x;
    dy = y - pknot->y;

    ds = sqrt(dx * dx + dy * dy);

    pknot->ds = ds;

    if (ds == 0.0)
    {
        pknot->c0 = 1.0;
        pknot->s0 = 0.0;
    }
    else
    {
        pknot->c0 = dx / ds;
        pknot->s0 = dy / ds;
    }
}

/*-----------------------------------------------------------------------------*/

void VektoM(pknot, ianf, iend, istart, iktot)

    KnotPtr pknot; /* pointer to the list of IK-points */
int ianf;          /* index of startpoint */
int iend;          /* index of endpoint */
int istart;        /* index of last startpoint */
int iktot;         /* # digitizations - 1 */

/*-----------------------------------------------------------------------------*/

{

    KnotPtr pend, pi, pi1;

    pend = pknot + iend;

    if ((pknot + ianf)->knotType == pntTangt) /* Anfang tangential */
        ianf--;

    for (pi = pknot + ianf; pi < pend; pi++)
    {
        pi1 = pi + 1;
        VektoMCalc(pi, pi1->x, pi1->y);
    }

    if (pend->knotType == pntTangt) /* Ende tangential */
    {
        if (iend == iktot && (pend + 1)->knotType < pntAngle)
            pi1 = pknot + istart + 1;
        else
            pi1 = pend + 1;

        VektoMCalc(pend, pi1->x, pi1->y);
    }
}

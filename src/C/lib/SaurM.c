/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          SaurM.c

   Functions:       SaurM()

---------------------------------------------------------------------------------

   Description: Calculates two arcs between two curvepoints according to
                the SAUR-algorithm. In the IK-points the arcs get the tangents which
                were calculated with splinM, in the meeting-point between the
                IK-points the get the slope of the cord.

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Knoth            20.08.1987       where it all began
   Knoth            29.10.87         dividing the struct Knot
   Rachow           28.02.1991       parameter change
   Rachow           07.05.1991       type Float, HelpKnots removed

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "SaurM.h"
#include "DraikM.h"

#ifndef INCLUDE_OUTPUT_H
#include "Output.h"
#endif

/*-----------------------------------------------------------------------------*/

void SaurM(m, n, reduceFactor, knotList)
    KnotPtr knotList; /* pointer to list of IK-points */
int m;                /* index of startpoint */
int n;                /* index of endpoint */
Float reduceFactor;   /* display factor */

/*----------------------------------------------------------------------------*/
{

    int nn1, i, j, in;

    Float dr, ca, ce, r1, r2, sh, xm1, ym1,
        xm2, ym2, xh1, yh1, xa, ya, sa, se;

    Boolean kurve;

    nn1 = penDown;

    /* loop handling all IK-points */
    for (i = m + 1; i <= n; i++)
    {
        kurve = true;
        j = i - 1; /* index of previous point */

        /* calculate parameters of angle */
        in = DraikM((knotList + j)->c0, (knotList + j)->s0, (knotList + j)->c,
                    (knotList + j)->s, (knotList + i)->c, (knotList + i)->s, &sa, &se, &ca, &ce);

        if (in == 0)
            kurve = false; /* if it is a small angle, draw a straight line */
        else
        {
            /* else calculate arcs */
            dr = (1. - ce) / (1. - ca);
            r2 = (knotList + j)->ds / (dr * sa + se); /* radius of 2nd circle */
            sh = r2 * (1. - ce);                      /* cordheight */

            if (sh < reduceFactor)
                kurve = false; /* if cordheight is small, straight line */
            else
            {
                r1 = dr * r2;

                if ((r1 < 1e-5) && (r1 > -1e-5))
                    kurve = false; /* small radius, straight line */
                else
                {
                    if (in < 0)
                    {
                        r1 = -r1;
                        r2 = -r2;
                    }

                    /* calculate center points and auxiliary points */
                    xm1 = (knotList + j)->x - (r1 * (knotList + j)->s);
                    ym1 = (knotList + j)->y + (r1 * (knotList + j)->c);

                    xm2 = (knotList + i)->x - (r2 * (knotList + i)->s);
                    ym2 = (knotList + i)->y + (r2 * (knotList + i)->c);

                    xh1 = xm1 + (r1 * (knotList + j)->s0);
                    yh1 = ym1 - (r1 * (knotList + j)->c0);

                    xa = (knotList + j)->x;
                    ya = (knotList + j)->y;

                    /* draw first arc */
                    KreisM(xa, ya, xh1, yh1, xm1, ym1, r1, sh, reduceFactor);

                    xa = xh1;
                    ya = yh1;

                    /* draw second arc */
                    KreisM(xa, ya, (knotList + i)->x, (knotList + i)->y, xm2, ym2, r2, sh, reduceFactor);
                }
            }
        }
        if (kurve == false)
            GeradeM((knotList + i)->x, (knotList + i)->y, nn1, reduceFactor);
    }
}
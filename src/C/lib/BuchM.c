/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          BuchM.c

   Functions:       BuchM()

---------------------------------------------------------------------------------

   Description:  Main function for the calculation of lines and arcs for the
                 display of an IK-character. Parts of a contour may be specified.
                 A display factor can be input.

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow           22.07.1987
   Knoth            27.11.1987       test if error
   Rachow           28.02.1991       include IKBasic.h
   Rachow           07.05.1991       type Float, HelpKnots removed
   Rachow           22.01.1992       save c,s of knot n, if angle point

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "BuchM.h"
#include "SplinM.h"
#include "SaurM.h"

#ifndef INCLUDE_OUTPUT_H
#include "Output.h"
#endif

/*-----------------------------------------------------------------------------*/

void BuchM(ianf, iend, istart, iktot, reduceFactor, pknot)

    int ianf;       /* Index of startpoint */
int iend;           /* Index of endpoint */
int istart;         /* Index of last startpoint */
int iktot;          /* # digitizations - 1 */
Float reduceFactor; /* display factor */
KnotPtr pknot;      /* Pointer to the list of IK-points */

/*----------------------------------------------------------------------------*/
{

    KnotPtr pi;
    int i, m, n, pen;

    /* Mark startpoint which is treated like the endpoint of a straight line */

    pen = penUp;
    GeradeM((pknot + ianf)->x, (pknot + ianf)->y, pen, reduceFactor);

    i = ianf + 1;
    pi = pknot + i;

    while (i <= iend) /* loop handling all points */
    {
        if (pi->knotType != pntCurve)     /* Non-curvepoints are */
        {                                 /* treated as endpoints of straight */
            if (pi->knotType == pntStart) /* lines */
            {
                pen = penUp;
                istart = i;
            }
            else
            {
                pen = penDown;
            }
            GeradeM(pi->x, pi->y, pen, reduceFactor);
        }
        else
        {
            pen = penDown;
            m = i - 1;

            while (i < iend) /* calculate length of curve */
            {
                i++;
                pi++;

                if (pi->knotType != pntCurve)
                    break;
            }

            if (pi->knotType < pntAngle || pi->knotType > pntTangt)
            {
                i--;
                pi--;
            }
            n = i;
            if (n - m <= 1) /* curve is represented by straight line */
            {
                GeradeM(pi->x, pi->y, pen, reduceFactor);
            }
            else
            {
                SplinM(pknot, m, n, istart, iktot); /* calculate tangents in curvepoints*/
                SaurM(m, n, reduceFactor, pknot);   /* calculate arcs */
            }
        }
        i++;
        pi++;
    }
}
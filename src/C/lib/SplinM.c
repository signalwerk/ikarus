/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          SplinM.c

   Functions:       SplinM()

---------------------------------------------------------------------------------

   Description:  Calculation of the tangents in the curvepoints of an IK-
                 contour. sine and cosine of the slope are stored in the knotlist.
                 The calculation is according to an algorithm using cubic splines
                 described in the book of H. Späth

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow           20.07.1987
   Lau              01.09.88         direct reference of sqrt... replaced by math.h
   Rachow           28.02.1991       include IKBasic.h
   Rachow           07.05.1991       type Float, HelpKnots removed

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "SplinM.h"
#include "KottaM.h"
#include "VektoM.h"
#include <math.h>

/*-----------------------------------------------------------------------------*/

void SplinM(pknot, ianf, iend, istart, iktot)

    KnotPtr pknot; /* Pointer auf die Liste der IK-Punkte */
int ianf;          /* Index des Anfangspunkts */
int iend;          /* Index des Endpunkts */
int istart;        /* Index des letzten Startpunktes */
int iktot;         /* Gesamtanzahl Digitalisierungen - 1 */

/*----------------------------------------------------------------------------*/
{

    KnotPtr panf, pend, pn1, pk, pk1;
    int k, ka, n1;
    Float ds, z, dxk, dyk, h1, c1, s1, h2, c2, s2;

    /* for all points of the curve calculate slope and distance to the next
       IK-point and store them in the knotlist */
    VektoM(pknot, ianf, iend, istart, iktot);

    n1 = iend - 1;
    pn1 = pknot + n1;

    panf = pknot + ianf;
    pend = pknot + iend;

    switch (panf->knotType) /* set the starting values */
    {                       /* depending on the type of the first */
    case pntTangt:          /* point */
        ka = ianf + 1;
        panf->g = 0.0;
        panf->c = (panf - 1)->c0;
        panf->s = (panf - 1)->s0;

        break;

    case pntCurve:
        ka = ianf + 1;
        panf->g = 0.0;

        break;

    default:
        ka = ianf;
        panf->g = 0.5;
        panf->c = 1.5 * panf->c0; /* set curvature at the end = 0 */
        panf->s = 1.5 * panf->s0;

        break;
    }

    /* Solving the linear system of equation by using Gauss-Elimination
       for tridiagonal systems */

    for (k = ianf, pk = panf; k < iend; k++, pk++)
    {
        ds = pk->ds;

        if (ds > 0.1)
            h2 = 1.0 / ds;
        else
            h2 = 1.0;
        c2 = 3.0 * pk->c0 * h2;
        s2 = 3.0 * pk->s0 * h2;

        if (k != ianf)
        {
            z = 1.0 / (2.0 * (h1 + h2) - h1 * pk1->g);

            pk->g = z * h2;
            pk->c = z * (c1 + c2 - h1 * pk1->c);
            pk->s = z * (s1 + s2 - h1 * pk1->s);

            if (k == n1)
            {
                if (pend->knotType == pntTangt)
                {
                    pend->c = pend->c0;
                    pend->s = pend->s0;

                    pk->c -= pk->g * pend->c;
                    pk->s -= pk->g * pend->s;

                    break;
                }
                else
                {
                    if (pend->knotType == pntCurve && iend != iktot &&
                        (pend + 1)->knotType >= pntAngle)
                    {
                        pk->c -= pk->g * pend->c;
                        pk->s -= pk->g * pend->s;

                        break;
                    }
                }

                z = 1.0 / (2.0 - pn1->g);

                pend->c = z * (3.0 * pn1->c0 - pn1->c);
                pend->s = z * (3.0 * pn1->s0 - pn1->s);
            }
        }
        c1 = c2;
        s1 = s2;
        h1 = h2;
        pk1 = pk;
    }

    while (1)
    {
        c1 = pk->c;
        s1 = pk->s;

        h2 = sqrt(c1 * c1 + s1 * s1);

        if (h2 != 0.0)
            h2 = 1.0 / h2;
        else
            h2 = 1.0;

        pk->c = c1 * h2;
        pk->s = s1 * h2;

        k--;
        pk--;

        if (k < ka)
            break;

        pk->c -= pk->g * c1;
        pk->s -= pk->g * s1;
    }

    switch (pend->knotType) /* special treatment of the endpoint */
    {
    case pntCurve:
        if (pend->x != panf->x || pend->y != panf->y)
            break;

        dxk = panf->c + pend->c;
        dyk = panf->s + pend->s;

        h2 = sqrt(dxk * dxk + dyk * dyk);

        if (h2 == 0.0)
            break;

        h2 = 1.0 / h2;

        panf->c = dxk * h2;
        panf->s = dyk * h2;

        pend->c = panf->c;
        pend->s = panf->s;

        break;

    case pntAngle:
        (pend - 2)->g = pend->c;
        pn1->g = pend->s;

        if (iend != iktot && (pend + 1)->knotType >= pntAngle)
            break;

        if (pend->x != panf->x || pend->y != panf->y)
            break;

        pend->c = (pknot + istart)->c;
        pend->s = (pknot + istart)->s;
    }
    KottaM(pknot, ianf, iend); /* modify tangents if necessary */
}
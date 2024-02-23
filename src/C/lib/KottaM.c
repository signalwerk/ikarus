/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          KottaM.c

   Functions:       KottaM()

---------------------------------------------------------------------------------

   Description:  Correction of tangents

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow           21.07.1987
   Rachow           28.02.1991       include IKBasic.h
   Rachow           08.05.1991       type Float, HelpKnots removed

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "AusglM.h"
#include "KottaM.h"
#include "DraikM.h"

/*-----------------------------------------------------------------------------*/

void KottaM(pknot, m, n)

    KnotPtr pknot; /* pointer to list of IK-points*/
int m;             /* index of startpoint */
int n;             /* index of endpoint */

/*----------------------------------------------------------------------------*/
{

    KnotPtr pm, pm1, pn, pi;
    int in, im;
    Float c1, s1, c2, s2, c3, s3, cs, ss, css, sss, sa1, sa2, sa11, sa22, ca1, ca2, ca11, ca22;

    pm = pknot + m;
    pm1 = pm + 1;
    pn = pknot + n;

    c1 = pm->c;
    s1 = pm->s;

    c2 = pm1->c;
    s2 = pm1->s;

    cs = pm->c0;
    ss = pm->s0;

    in = DraikM(cs, ss, c1, s1, c2, s2, &sa1, &sa2, &ca1, &ca2);

    for (pi = pm1; pi < pn; pi++)
    {

        if ((pi + 1)->knotType != pntAngle)
        {
            c3 = (pi + 1)->c;
            s3 = (pi + 1)->s;
        }
        else
        {
            c3 = (pi - 1)->g;
            s3 = pi->g;
        }

        css = pi->c0;
        sss = pi->s0;

        im = DraikM(css, sss, c2, s2, c3, s3, &sa11, &sa22, &ca11, &ca22);

        if (in != 0)
        {
            if (im != 0)
            {
                if (in * im >= 0)
                {
                    int temp = AusglM(in, sa1, sa2, sa11, sa22, &c2, &s2);
                    if (temp != 0)
                    {
                        im = DraikM(css, sss, c2, s2, c3, s3, &sa11, &sa22, &ca11, &ca22);

                        pi->c = c2;
                        pi->s = s2;
                    }
                }
            }
            else
            {
                c2 = css;
                s2 = sss;

                c3 = css;
                s3 = sss;

                pi->c = c2;
                pi->s = s2;

                (pi + 1)->c = c3;
                (pi + 1)->s = s3;
            }
        }

        in = im;

        sa1 = sa11;
        sa2 = sa22;

        ca1 = ca11;
        ca2 = ca22;

        cs = css;
        ss = sss;

        c1 = c2;
        s1 = s2;

        c2 = c3;
        s2 = s3;
    }

    pi->c = c2;
    pi->s = s2;
    if (pi->knotType != pntCurve)
    {
        (pi - 2)->g = c2;
        (pi - 1)->g = s2;
    }
}
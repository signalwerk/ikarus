/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          DraikM.c

   Functions:       DraikM()

---------------------------------------------------------------------------------

   Description:  Calculates the angles in a rightangled triangle which is
                 spanned by the cord between two IK-points and the tangents in
                 these points.

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow           21.07.1987
   Rachow           27.02.1991       include IKBasic.h
   Rachow           08.05.1991       type Float, HelpKnots removed

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "DraikM.h"

/*-----------------------------------------------------------------------------*/

int DraikM(cs, ss, ci1, si1, ci, si, psa1, psa2, pca1, pca2)

Float cs, ss;       /* sin/cos of slope of cord */
Float ci1, si1;     /* sin/cos of slope of tangent in 1st IK-point */
Float ci, si;       /* sin/cos of slope of tangent in 2nd IK-point */
Float *psa1, *pca1; /* pointer to sin/cos of 1st angle */
Float *psa2, *pca2; /* pointer to sin/cos of 2nd angle */

/*----------------------------------------------------------------------------*/
{

    Float sa1, sa2;
    int in;

    sa1 = cs * si1 - ci1 * ss;
    sa2 = cs * si - ci * ss;

    *psa1 = sa1;
    *psa2 = sa2;

    if (sa1 > 0)
    {
        if (sa2 > 0)
            return 0;

        sa2 = -sa2;
        *psa2 = sa2;

        in = -1;
    }
    else
    {
        if (sa2 < 0)
            return 0;

        sa1 = -sa1;
        *psa1 = sa1;

        in = 1;
    }

    if (sa1 < 0.0015 || sa2 < 0.0015)
        return 0;

    *pca1 = cs * ci1 + ss * si1;
    *pca2 = cs * ci + ss * si;

    if (*pca1 < 0.1 || *pca2 < 0.1)
        return 0;

    return in;
}

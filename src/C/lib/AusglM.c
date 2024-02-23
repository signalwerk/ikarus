/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          AusglM.c

   Functions:       AusglM()

---------------------------------------------------------------------------------

   Description:  Compensation of triangles which are too skewed. One
                 cathetus of each triangle forms a straight line. This
                 common direction is corrected to obtain more symmetrical
                 triangles.

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow           21.07.1987
   Lau              01.09.88         direct referencing of sqrt replaced by math.h
   Schwarz          23.02.89         if(cdel<0.99999) removed
   Rachow           27.02.1991       include IKBasic.h
   Rachow           07.05.1991       type Float, HelpKnots removed

------------------------------------------------------------------------------*/

#include "IKBasic.h"
#include "AusglM.h"
#include <math.h>

/*-----------------------------------------------------------------------------*/

int AusglM(idreh, sa1, sa2, sa11, sa22, pci, psi)

int idreh;        /* sense of rotation of the two triangles */
Float sa1, sa2;   /* sin of angles of 1st triangle */
Float sa11, sa22; /* sin of angles of 2nd triangle */
Float *pci, *psi; /* input: common direction of cathetus*/
                  /* output: new common direction */

/* return value: */
/* =0: no correction */
/* =1: correction executed */

/*----------------------------------------------------------------------------*/
{

    Float dr, del, del1, del2, deldr, c, cdel, cdels;

    dr = idreh * 0.7;

    del1 = (sa1 - sa2) * dr;
    del2 = (sa11 - sa22) * dr;

    del = (del1 + del2) * 0.7;
    deldr = del * dr;

    if (deldr >= 0) /* del positive: rotation right */
    {               /* del negative: rotation left */
        if (deldr < 0.01)
            return 0;

        if (deldr > del2 * dr)
            del = del2; /* sa11 gets smaller */
    }
    else
    {
        if (deldr > -0.01)
            return 0;

        if (deldr < del1 * dr)
            del = del1; /* sa2 gets smaller */
    }

    cdels = 1.0 - del * del;
    cdel = sqrt(cdels); /* rotate (ci, si) */

    c = (*pci) * cdel - (*psi) * del;

    *psi = (*pci) * del + (*psi) * cdel;
    *pci = c;

    return 1;
}

/*-------------------------------------------------------------------------------

   (C) Copyright 1987-92 by URW Unternehmensberatung, Tel.: (040) 606 05 - 0
                                      Harksheider Strasse 102, D-2000 Hamburg 65

--------------------------------------------------------------------------------

   Project:         IKARUS M

   Module:          IKBasic.h

---------------------------------------------------------------------------------

   Description:  BasiclncludeFile for the functions of IKARUS M

---------------------------------------------------------------------------------

   Author:          Date:            Changes:
   Rachow           27.02.1991
   Rachow           06.05.1991       new Knot struct with Float
------------------------------------------------------------------------------*/

#ifndef IKBASIC_H 
#define IKBASIC_H 

#include <stdbool.h>  
typedef short Short;  
typedef float Float;  
typedef bool Boolean; 

#define penUp 0
#define penDown 1

typedef struct        /* this structure describes one IK-point; an array */
{                     /* containing all points of a character is called KnotList */
    Float x;          /* X-coordinate */
    Float y;          /* Y-coordinate */
    Short knotType;   /* type of point */
    Float c;          /* cosine of tangent */
    Float s;          /* sine of tangent */
    Float g;          /* auxiliary variable */
    Float c0;         /* cosine of cord */
    Float s0;         /* sine of cord */
    Float ds;         /* distance of two points */
    Boolean selected; /* auxiliary variable */
}

Knot,
    *KnotPtr, **KnotHandle;

enum
{
    pntStart = 12, /* startpoint */
    pntAngle,      /* cornerpoint */
    pntCurve,      /* curvepoint */
    pntTangt       /* tangentpoint */
};

#endif // IKBASIC_H 

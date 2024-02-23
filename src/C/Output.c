
#include "IKBasic.h"

#include "Output.h"

unsigned int pointOutputIndex = 0;

// Function definition
void GeradeM(Float x, Float y, int pen, Float reduceFactor)
{

    if (pointOutputIndex++ > 0)
    {
        printf(",\n");
    }
    printf("  {\n");
    printf("    \"type\": \"GeradeM\",\n");
    printf("    \"x\": %f,\n", x);
    printf("    \"y\": %f,\n", y);
    printf("    \"pen\": %d,\n", pen);
    printf("    \"reduceFactor\": %f\n", reduceFactor);
    printf("  }");
}

// Function definition
void KreisM(Float xa, Float ya, Float x, Float y, Float xm2, Float ym2, Float r, Float sh, Float reduceFactor)
{
    if (pointOutputIndex++ > 0)
    {
        printf(",\n");
    }
    printf("  {\n");
    printf("    \"type\": \"KreisM\",\n");
    printf("    \"xa\": %f,\n", xa);
    printf("    \"ya\": %f,\n", ya);
    printf("    \"x\": %f,\n", x);
    printf("    \"y\": %f,\n", y);
    printf("    \"xm2\": %f,\n", xm2);
    printf("    \"ym2\": %f,\n", ym2);
    printf("    \"r\": %f,\n", r);
    printf("    \"sh\": %f,\n", sh);
    printf("    \"reduceFactor\": %f\n", reduceFactor);
    printf("  }");
}
//
// Created by Florian Epple on 28.11.23.
//

#ifndef MCAPC_C
#define MCAPC_C

#include <stdio.h>
#include <stdlib.h>

#include "functionals.c"

double mcapc_calcra(float h, float m, float s)
{
    if (h < 0 || m < 0 || s < 0)
    {
        return -1;
    }

    return ((15 * h) + ((double)m / 4) + ((double)s / 240));
}

double mcapc_calcd(float d, float m, float s, int dir)
{
    if ((1 < dir || dir < 0) || d < 0 || m < 0 || s < 0) // dir has to be 1 or 0
    {
        return -1;
    }

    int sign = (dir == 1) ? -1 : 1;

    return (sign * (d + ((double)m / 60) + (double)s / 3600));
}

double mcapc_adjdeg(double deg)
{
    int degSign = (deg > 0) - (deg < 0);
    double degAbs = deg * degSign;

    if (degAbs > 180)
    {
        int factor = (int)(degAbs / 180);
        degAbs -= 180 * factor;
    }

    deg = degAbs * degSign;

    return deg;
}

char *mcapc_fromstr(char str[], char format[])
{
    char buffer[7][100];
    for (int i = 0; i < 7; ++i)
    {
        buffer[i][0] = '\0';
    }

    fnc_strsplitby(str, ';', buffer, 7, 100);

    float rh = atof(buffer[0]);
    float rm = atof(buffer[1]);
    float rs = atof(buffer[2]);

    float resra = mcapc_calcra(rh, rm, rs);

    float dd = atof(buffer[3]);
    float dm = atof(buffer[4]);
    float ds = atof(buffer[5]);
    float dir = atof(buffer[6]);

    float resd = mcapc_calcd(dd, dm, ds, dir);

    char *output = (char *)malloc(100 * sizeof(char));

    if (output != NULL)
    {
        snprintf(output, 100, format, resra, resd);
    }

    return output;
}

double mcapc_calcdiff(double d1, double d2) { return mcapc_adjdeg(d2 - d1); }

#endif // MCAPC_C
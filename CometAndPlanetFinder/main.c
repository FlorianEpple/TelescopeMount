/**
 * @file main.c
 * @author Florian Epple
 * @brief Calulator for right ascension and delta
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>

/**
 * @brief Calc the right ascension of hours, minutes and seconds
 *
 * @return double (right ascension)
 */
double CalcRightAscension(float h, float m, float s)
{
    if (h < 0 || m < 0 || s < 0)
    {
        return -1;
    }

    return ((15 * h) + ((double)m / 4) + ((double)s / 240));
}

/**
 * @brief Calc the delta of degrees, minutes and seconds
 *
 * @param dir 0 for positive, 1 for negative
 * @return double (delta)
 */
double CalcDelta(float d, float m, float s, int dir)
{
    if ((1 < dir || dir < 0) || d < 0 || m < 0 || s < 0) // dir has to be 1 or 0
    {
        return -1;
    }

    int sign = (dir == 1) ? -1 : 1;

    return (sign * (d + ((double)m / 60) + (double)s / 3600));
}

/**
 * @brief Adjusts the degrees (to 180°)
 *
 * @param deg Pointer to the degrees
 */
void adjustDegrees(double *deg)
{
    int degSign = (*deg > 0) - (*deg < 0);
    double degAbs = *deg * degSign;

    if (degAbs > 180)
    {
        int factor = (int)(degAbs / 180);
        degAbs -= 180 * factor;
    }

    *deg = degAbs * degSign;
}

/**
 * @brief Prompt a Integer input
 *
 * @param number Pointer to number
 * @param quest The prompt string
 */
void promptInt(int *number, char quest[])
{
    {
        printf("%s", quest);
        if (scanf("%d", number) != 1)
        {
            number = NULL;
        }
    }
}

/**
 * @brief Prompt a Float input
 *
 * @param number Pointer to number
 * @param quest The prompt string
 */
void promptFloat(float *number, char quest[])
{
    printf("%s", quest);
    if (scanf("%f", number) != 1)
    {
        *number = 0.0;
        while (getchar() != '\n')
            ;
    }
}

int main(int argc, char const *argv[])
{
    float rh = 0;
    float rm = 0;
    float rs = 0;

    promptFloat(&rh, "(right ascension) hours   [float]-> "); // get the user input
    promptFloat(&rm, "(right ascension) minutes [float]-> ");
    promptFloat(&rs, "(right ascension) seconds [float]-> ");

    double RA = CalcRightAscension(rh, rm, rs); // calc right ascension

    adjustDegrees(&RA); // adjust the degrees (to 180°)

    printf("right ascension of %.0fh %.0fm %.1fs is %f\n\n", rh, rm, rs, RA); // print result

    ////////////////////////////////

    float dd = 0;
    float dm = 0;
    float ds = 0;
    float dir = 0;

    promptFloat(&dd, "(delta) degrees [float]-> "); // get the user input
    promptFloat(&dm, "(delta) minutes [float]-> ");
    promptFloat(&ds, "(delta) seconds [float]-> ");
    promptFloat(&dir, "(delta) direction [0=pos, 1=neg]-> ");

    double Delta = CalcDelta(dd, dm, ds, dir); // calc delta

    adjustDegrees(&Delta); // adjust the degrees (to 180°)

    printf("right ascension of %.0f° %.0f' %.1f\" is %f\n\n", dd, dm, ds, Delta); // print result

    ////////////////////////////////

    int dessicion = -1;

    char outputFormat[] = "RA%.6f;Delta%.6f";
    char output[100];

    sprintf(output, outputFormat, RA, Delta); // format the output

    printf("Should the string \"%s\" be sent out to the Arduino?\n", output); // ask if the string should be sent out to the Arduino
    promptInt(&dessicion, "(1=yes, else=no) -> ");

    if (dessicion == 1) // send the string to the Arduino if dessicion is 1
    {
        printf("\n  -> Send output \"%s\"\n\n", output);

        /*
         *
         * Print the output serial...
         *
         */
    }

    return 0;
}

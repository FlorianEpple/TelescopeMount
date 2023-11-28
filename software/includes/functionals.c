//
// Created by Florian Epple on 27.11.23.
//

#ifndef FUNCTIONALS_C
#define FUNCTIONALS_C

#include <stdio.h>

void fnc_strsplitby(char str[], char divider, char (*data)[], int maxRows, int maxCols)
{
    int i = 0; // Index for the input string
    int k = 0; // Index for rows in the data array
    int c = 0; // Index for columns in the data array

    while (str[i] != '\0' && k < maxRows && c < maxCols)
    {
        if (str[i] != divider)
        {
            (*data)[k * maxCols + c] = str[i];
            c++;
        }
        else
        {
            (*data)[k * maxCols + c] = '\0'; // Null-terminate the substring
            k++;
            c = 0;
        }

        i++;
    }

    // Null-terminate the last substring
    if (c > 0)
    {
        (*data)[k * maxCols + c] = '\0';
    }
}

void fnc_strtolower(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

#endif // FUNCTIONALS_C
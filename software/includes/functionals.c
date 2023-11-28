//
// Created by Florian Epple on 27.11.23.
//

#ifndef FUNCTIONALS_C
#define FUNCTIONALS_C

#include <stdio.h>
#include <stdlib.h>

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

int fnc_iswhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int fnc_strgetfpos(char const *str)
{
    int i = 0;
    while (fnc_iswhitespace(str[i]))
    {
        i++;
    }
    return i;
}

int fnc_strlen(char const *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

int fnc_strgetlpos(char const *str)
{
    int i = fnc_strlen(str) - 1;
    while (fnc_iswhitespace(str[i]))
    {
        i--;
    }
    return i;
}

int fnc_trimlen(char const *str)
{
    return (fnc_strgetlpos(str) - fnc_strgetfpos(str));
}

char *fnc_strtrim(char const *str)
{
    char *trim = NULL;
    int i, len, start, end;

    if (str != NULL)
    {
        i = 0;
        len = fnc_trimlen(str) + 1;
        trim = (char *)malloc(len);
        start = fnc_strgetfpos(str);

        while (i < len)
        {
            trim[i] = str[start];
            i += 1;
            start += 1;
        }

        trim[i] = '\0';
    }
    return (trim);
}

#endif // FUNCTIONALS_C
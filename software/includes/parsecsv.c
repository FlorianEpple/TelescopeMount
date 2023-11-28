//
// Created by Florian Epple on 27.11.23.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "functionals.c"

int csv_countfields(char line[])
{
    int k = 1;
    for (int i = 0; i < strlen(line); i++)
        if (line[i] == ';')
            k++;
    return k;
}

void csv_inifields(char (*fields)[100], int fieldn)
{
    for (int i = 0; i < fieldn; i++)
    {
        fields[i][0] = '\0';
    }
}

void csv_parsefields(char *buffer, char (*fields)[], int fieldn, int charlen)
{
    fnc_strsplitby(buffer, ';', fields, fieldn, charlen);
}

int csv_scanfor(FILE *source, int from_field, int to_field, char str[])
{
    fnc_strtolower(str);

    if (source == NULL)
    {
        printf("Cant access csv source: csv_scanfor");
        return -1;
    }

    printf("Scanning csv for %s...\n", str);

    char linebuffer[1024];
    int c = 0;

    // max founds //
    const int _MAX_FOUNDS_ = 200;

    int founds_index[_MAX_FOUNDS_];
    char founds_str[_MAX_FOUNDS_][100];
    int k = 0;

    for (int i = 0; i < _MAX_FOUNDS_; i++)
    {
        founds_index[i] = -1;
        founds_str[i][0] = '\0';
    }
    ///////////////

    while (fgets(linebuffer, sizeof linebuffer, source) != NULL)
    {
        if (linebuffer[strlen(linebuffer) - 1] == '\n')
            linebuffer[strlen(linebuffer) - 1] = '\0'; // remove the \n at the end of the line

        int fieldn = csv_countfields(linebuffer); // count the fields in csv line
        char fields[fieldn][100];
        csv_inifields(fields, fieldn);

        csv_parsefields(linebuffer, fields, fieldn, 100);

        for (int i = from_field; i < from_field + to_field; i++)
        {
            char *fieldcpy = strdup(fields[i]);
            fnc_strtolower(fieldcpy);

            if (strstr(fieldcpy, str) != NULL)
            {
                if (k < _MAX_FOUNDS_)
                {
                    founds_index[k] = c;
                    strcpy(founds_str[k], fields[i]);
                }

                k++;
            }
        }

        c++;
    }

    if (1 < k)
    {
        for (int i = 0; i < _MAX_FOUNDS_; i++)
        {
            if (founds_index[i] == -1)
            {
                break;
            }

            char indexingstr[10];
            sprintf(indexingstr, "(%d)", i + 1);

            printf(" %5s %-15s found in line %6d\n", indexingstr, founds_str[i], founds_index[i] + 1);
        }

        if (0 < k - _MAX_FOUNDS_)
        {
            printf(" + %d overflows\n", k - _MAX_FOUNDS_);
        }
    }
    else
    {
        printf(" %s found in line %d\n", founds_str[0], founds_index[0] + 1);
    }

    return 0;
}
//
// Created by Florian Epple on 27.11.23.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "functionals.c"
#include "env.c"

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

int csv_scanfor(char filename[], int from_field, int to_field, char str[])
{
    fnc_strtolower(str);

    FILE *source;
    source = fopen(filename, "r");

    if (source == NULL)
    {
        printf("Cant access csv source: csv_scanfor");

        fclose(source);
        return -1;
    }

    printf("Scanning csv for %s...\n", str);

    char linebuffer[1024];
    int c = 0;

    int founds_index[env_get("MAX_FOUNDS")];
    char founds_str[env_get("MAX_FOUNDS")][100];
    int k = 0;

    for (int i = 0; i < env_get("MAX_FOUNDS"); i++)
    {
        founds_index[i] = -1;
        founds_str[i][0] = '\0';
    }

    while (fgets(linebuffer, sizeof linebuffer, source) != NULL)
    {
        if (linebuffer[strlen(linebuffer) - 1] == '\n')
            linebuffer[strlen(linebuffer) - 1] = '\0'; // remove the \n at the end of the line

        int fieldn = csv_countfields(linebuffer); // count the fields in csv line
        char fields[fieldn][100];
        csv_inifields(fields, fieldn);

        csv_parsefields(linebuffer, fields, fieldn, 100);

        for (int i = from_field; i < from_field + to_field && i < fieldn; i++)
        {
            char *fieldcpy = strdup(fields[i]);
            fnc_strtolower(fieldcpy);

            if (strstr(fieldcpy, str) != NULL)
            {
                if (k < env_get("MAX_FOUNDS"))
                {
                    founds_index[k] = c;
                    strcpy(founds_str[k], fields[i]);
                }

                k++;
                break;
            }

            free(fieldcpy); // Free dynamically allocated memory.
        }

        c++;
    }

    if (1 < k)
    {
        for (int i = 0; i < env_get("MAX_FOUNDS"); i++)
        {
            if (founds_index[i] == -1)
            {
                break;
            }

            char indexingstr[10];
            sprintf(indexingstr, "(%d)", i + 1);

            printf(" %5s  %-15s found in line %6d\n", indexingstr, founds_str[i], founds_index[i] + 1);
        }

        if (0 < k - env_get("MAX_FOUNDS"))
        {
            printf(" + %d overflows (change this in .env at \"MAX_FOUNDS\")\n", k - env_get("MAX_FOUNDS"));
        }

        printf("\n");

        while (1)
        {
            int founds_ = 0;
            char foundschar_[100];
            foundschar_[0] = '\0';
            char decision_ = '\0';
            char _ = '\0';

            printf(" Enter a number between 1 and %d: (\'*\' for escape) ", k);
            scanf("%s", foundschar_);
            scanf("%c", &_);

            if (strcmp(fnc_strtrim(foundschar_), "*") == 0)
            {
                return -1;
            }

            founds_ = atoi(foundschar_);

            if (0 < founds_ && founds_ < k + 1)
            {
                printf(" Select object \"%s\"? (y/n) ", founds_str[founds_ - 1]);
                scanf("%c", &decision_);

                if (decision_ == 'y')
                {
                    fclose(source);
                    return founds_index[founds_ - 1] + 1;
                }
            }
        }
    }
    else
    {
        if (founds_index[0] == -1)
        {
            printf("Not found: \"%s\"\n", str);

            fclose(source);
            return -1;
        }
        else
        {
            char decision_ = 'n';

            printf(" %s found in line %d\n", founds_str[0], founds_index[0] + 1);
            printf("\n");
            printf(" Select object \"%s\"? (y/n) ", founds_str[0]);
            scanf("%c", &decision_);

            fclose(source);

            if (decision_ != 'y')
                return -1;

            return founds_index[0];
        }
    }

    fclose(source);
    return 0;
}

char *csv_getat(char filename[], int line, int field)
{
    FILE *source;
    source = fopen(filename, "r");

    if (source == NULL)
    {
        printf("Can't access csv source: csv_getat\n");
        fclose(source);
        return NULL;
    }

    char linebuffer[1024];
    int i = 0;

    while (fgets(linebuffer, sizeof(linebuffer), source) != NULL)
    {
        if (line == i)
        {
            if (linebuffer[strlen(linebuffer) - 1] == '\n')
                linebuffer[strlen(linebuffer) - 1] = '\0';

            int fieldn = csv_countfields(linebuffer);

            if (fieldn >= field)
            {
                char(*fields)[100] = malloc(fieldn * 100);
                if (fields == NULL)
                {
                    fclose(source);
                    return NULL;
                }

                csv_inifields(fields, fieldn);
                csv_parsefields(linebuffer, fields, fieldn, 100);

                char *result = strdup(fields[field - 1]);

                free(fields);

                fclose(source);
                return result;
            }
        }

        i++;
    }

    fclose(source);
    return NULL;
}
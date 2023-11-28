//
//  telescopesteering.c
//  TeselscopeSteering
//
//  Created by Florian Epple on 27.11.23.
//

#include <stdio.h>

#include "includes/parsecsv.c"
#include "includes/env.c"
#include "includes/mcapc.c"

int main(int argc, char *argv[])
{
    ////////////////////////////////////////////////////////////////
    if (env_nexits())
    {
        printf("Environment variables not initialized: Not found .env\n");
        return 1;
    }
    ////////////////////////////////////////////////////////////////

    char csvfilename[] = "/Users/florianepple/Documents/IT/Papa/TelescopeMount/software/assets/coordinates.csv";

    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////

    while (1)
    {
        char objectnameA[100];
        objectnameA[0] = '\0';
        int objindexA = -1;
        char *manA = malloc(100);

        while (objindexA == -1)
        {
            char _;
            printf("Enter Object A: ");
            scanf("%s", objectnameA);
            scanf("%c", &_);

            if (strstr(objectnameA, ";"))
            {
                printf("\n");
                printf(" ------------------------------------------\n");
                printf(" ## Manual identification is switched ON ##\n");
                printf(" ------------------------------------------\n");

                char decision_ = 'y';
                char _;
                while (1)
                {
                    if (decision_ != 'y')
                    {
                        printf("\n");
                        printf(" Coordinates for A: (\'*\' for escape) ");
                        scanf("%s", objectnameA);
                        scanf("%c", &_);

                        if (strcmp(objectnameA, "*") == 0)
                        {
                            objindexA = -1;
                            break;
                        }
                    }

                    manA = mcapc_fromstr(objectnameA, "%.6f;%.6f");
                    printf(" Use: %s ? (y/n) ", manA);
                    scanf("%c", &decision_);
                    scanf("%c", &_);

                    if (decision_ == 'y')
                    {
                        objindexA = -2;
                        break;
                    }
                }
            }
            else
            {
                objindexA = csv_scanfor(csvfilename, env_get("OBJ_COO_DESIGN_START"), env_get("OBJ_COO_DESIGN_START") + env_get("OBJ_COO_DESIGN_LENGTH"), objectnameA);

                if (-1 < objindexA)
                {
                }
            }

            printf("\n");
        }

        char objectnameB[100];
        objectnameB[0] = '\0';
        int objindexB = -1;
        char *manB = malloc(100);

        while (objindexB == -1)
        {
            char _;
            printf("Enter Object B: ");
            scanf("%s", objectnameB);
            scanf("%c", &_);

            if (strstr(objectnameB, ";"))
            {
                printf("\n");
                printf(" ------------------------------------------\n");
                printf(" ## Manual identification is switched ON ##\n");
                printf(" ------------------------------------------\n");

                char decision_ = 'y';
                char _;
                while (1)
                {
                    if (decision_ != 'y')
                    {
                        printf("\n");
                        printf(" Coordinates for B: (\'*\' for escape) ");
                        scanf("%s", objectnameB);
                        scanf("%c", &_);

                        if (strcmp(objectnameB, "*") == 0)
                        {
                            objindexB = -1;
                            break;
                        }
                    }

                    manB = mcapc_fromstr(objectnameB, "%.6f;%.6f");
                    printf(" Use: %s ? (y/n) ", manB);
                    scanf("%c", &decision_);
                    scanf("%c", &_);

                    if (decision_ == 'y')
                    {
                        objindexB = -2;
                        break;
                    }
                }
            }
            else
            {
                objindexB = csv_scanfor(csvfilename, env_get("OBJ_COO_DESIGN_START"), env_get("OBJ_COO_DESIGN_START") + env_get("OBJ_COO_DESIGN_LENGTH"), objectnameB);

                if (-1 < objindexB)
                {
                }
            }

            printf("\n");
        }

        if (0 < objindexA)
        {
            printf("For A index:  %d\n", objindexA);
        }
        else if (objindexA == -2)
        {
            printf("For A manual: %s\n", manA);
        }

        if (0 < objindexB)
        {
            printf("For B index:  %d\n", objindexB);
        }
        else if (objindexB == -2)
        {
            printf("For B manual: %s\n", manB);
        }

        exit(0);
    }

    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////

    char obj[] = "m1";
    int objindex = -1;

    objindex = csv_scanfor(csvfilename, env_get("OBJ_COO_DESIGN_START"), env_get("OBJ_COO_DESIGN_START") + env_get("OBJ_COO_DESIGN_LENGTH"), obj);

    if (objindex == -1)
    {
        return 1;
    }

    char *ra = csv_getat(csvfilename, objindex, env_get("OBJ_COO_RA") + 1);
    if (ra != NULL)
    {
        printf("RA of \"%s\" is %s\n", obj, ra);
        free(ra);
    }
    else
    {
        printf("Error retrieving RA for \"%s\"\n", obj);
    }

    return 0;
}
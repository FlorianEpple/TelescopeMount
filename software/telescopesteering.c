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

    char csvfilename[1024];
    csvfilename[0] = '\0';

    strcpy(csvfilename, env_getstr("CVS_FILEPATH"));

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
            }

            printf("\n");
        }

        // inizialization of object data
        double objdataA[2], objdataB[2];
        for (int i = 0; i < 2; i++)
        {
            objdataA[i] = objdataB[i] = 0.0;
        }

        // allocation of object data
        if (0 < objindexA)
        {
            char objectrealnA[100];
            strcpy(objectrealnA, csv_getat(csvfilename, objindexA - 1, env_get("OBJ_COO_DESIGN_START") + 1));

            objdataA[0] = (double)atof(csv_getat(csvfilename, objindexA - 1, env_get("OBJ_COO_RA") + 1));
            objdataA[1] = (double)atof(csv_getat(csvfilename, objindexA - 1, env_get("OBJ_COO_DELTA") + 1));

            printf("For A existing:  %-9s | RA: %11.6f | Delta: %11.6f\n", objectrealnA, objdataA[0], objdataA[1]);
        }
        else if (objindexA == -2)
        {
            char idenmanA[2][100];
            fnc_strsplitby(manA, ';', idenmanA, 2, 100);

            objdataA[0] = (double)atof(idenmanA[0]);
            objdataA[1] = (double)atof(idenmanA[1]);

            printf("For A manually: %-9s | RA: %11.6f | Delta: %11.6f\n", "(unknown)", objdataA[0], objdataA[1]);
        }

        if (0 < objindexB)
        {
            char objectrealnB[100];
            strcpy(objectrealnB, csv_getat(csvfilename, objindexB - 1, env_get("OBJ_COO_DESIGN_START") + 1));

            objdataB[0] = (double)atof(csv_getat(csvfilename, objindexB - 1, env_get("OBJ_COO_RA") + 1));
            objdataB[1] = (double)atof(csv_getat(csvfilename, objindexB - 1, env_get("OBJ_COO_DELTA") + 1));

            printf("For B existing:  %-9s | RA: %11.6f | Delta: %11.6f\n", objectrealnB, objdataB[0], objdataB[1]);
        }
        else if (objindexB == -2)
        {
            char idenmanB[2][100];
            fnc_strsplitby(manB, ';', idenmanB, 2, 100);

            objdataB[0] = (double)atof(idenmanB[0]);
            objdataB[1] = (double)atof(idenmanB[1]);

            printf("For B manually: %-9s | RA: %11.6f | Delta: %11.6f\n", "(unknown)", objdataB[0], objdataB[1]);
        }
        printf("\n");

        char findecision_ = 'y';
        while (1)
        {

            printf("Do you want to use these values? (y/n) ");
            scanf(" %c", &findecision_);

            if (findecision_ == 'y')
            {
                findecision_ = '0';
                break;
            }

            printf("Do you really want to throw these values away? (y/n) ");
            scanf(" %c", &findecision_);

            if (findecision_ == 'y')
            {
                findecision_ = '1';
                break;
            }
        }

        printf("\n");

        if (findecision_ == '0')
            break;
    }

    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////

    return 0;
}
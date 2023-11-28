//
//  telescopesteering.c
//  TeselscopeSteering
//
//  Created by Florian Epple on 27.11.23.
//

#include <stdio.h>

#include "includes/parsecsv.c"
#include "includes/env.c"

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

        while (objindexA < 0)
        {
            char _;
            printf("Enter Object A: ");
            scanf("%s", objectnameA);
            scanf("%c", &_);

            objindexA = csv_scanfor(csvfilename, env_get("OBJ_COO_DESIGN_START"), env_get("OBJ_COO_DESIGN_START") + env_get("OBJ_COO_DESIGN_LENGTH"), objectnameA);

            printf("\n");
        }

        char objectnameB[100];
        objectnameB[0] = '\0';
        int objindexB = -1;

        while (objindexB < 0)
        {
            char _;
            printf("Enter Object B: ");
            scanf("%s", objectnameB);
            scanf("%c", &_);

            objindexB = csv_scanfor(csvfilename, env_get("OBJ_COO_DESIGN_START"), env_get("OBJ_COO_DESIGN_START") + env_get("OBJ_COO_DESIGN_LENGTH"), objectnameB);

            printf("\n");
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
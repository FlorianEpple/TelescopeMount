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
    FILE *csvfile;
    char filename[] = "/Users/florianepple/Documents/IT/Papa/TelescopeMount/software/assets/coordinates.csv";

    csvfile = fopen(filename, "r");

    if (csvfile == NULL)
    {
        printf("Cant access csv source: main");
    }

    char obj[] = "tau";

    csv_scanfor(csvfile, env_get("OBJ_COO_DESIGN_START"), env_get("OBJ_COO_DESIGN_START") + env_get("OBJ_COO_DESIGN_LENGTH"), obj);

    return 0;
}
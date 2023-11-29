//
// Created by Florian Epple on 28.11.23.
//

#ifndef ENV_C
#define ENV_C

#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functionals.c"

void env_getfname(char *fname)
{
    char cwd[1024];

#ifdef _WIN32
    if (GetCurrentDirectory(MAX_PATH, cwd) == 0)
    {
        perror("Error getting current working directory");
        return;
    }
#elif defined(__APPLE__)
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Error getting current working directory");
        return;
    }
#endif

    char filename[1024];
    sprintf(filename, "%s/%s", cwd, ".env");

    strcpy(fname, filename);
}

int env_nexists()
{
    FILE *envf;
    char filename[1024];

    env_getfname(filename);

    envf = fopen(filename, "r");

    if (envf == NULL)
    {
        fclose(envf);
        return 1;
    }

    fclose(envf);

    return 0;
}

char *env_getstr(char *key)
{
    FILE *envf;
    char filename[1024];

    env_getfname(filename);

    envf = fopen(filename, "r");

    if (envf == NULL)
    {
        printf("Error opening file at %s", filename);
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), envf) != NULL)
    {
        if (strstr(buffer, "=") == NULL || strstr(buffer, "#") != NULL)
            continue;

        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        char keypair[2][500];

        fnc_strsplitby(buffer, '=', keypair, 2, 500);

        if (strcmp(fnc_strtrim(keypair[0]), key) == 0)
        {
            char *val = fnc_strtrim(keypair[1]);

            if (val[0] == '\"')
                val[0] = ' ';

            if (val[strlen(val) - 1] == '\"')
                val[strlen(val) - 1] = ' ';

            val = fnc_strtrim(val);

            fclose(envf);
            return val;
        }
    }

    fclose(envf);

    return NULL;
}

int env_get(char *key)
{
    if (env_getstr(key) != NULL)
    {
        return atoi(env_getstr(key));
    }
    return 0;
}

#endif // ENV_C
//
// Created by Florian Epple on 28.11.23.
//
#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functionals.c"

char *env_getstr(char *key)
{
    FILE *envf;
    char cwd[1024];
    char filename[1024];

#ifdef _WIN32
    if (GetCurrentDirectory(MAX_PATH, cwd) == 0)
    {
        printf("Error getting current working directory: in env_getstr()");
        return "";
    }
#elif defined(__APPLE__)
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Error getting current working directory: in env_getstr()");
        return "";
    }
#endif

    sprintf(filename, "%s/%s", cwd, ".env");

    envf = fopen(filename, "r");

    if (envf == NULL)
    {
        printf("Error opening file at %s", filename);
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), envf) != NULL)
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        char keypair[2][100];

        fnc_strsplitby(buffer, '=', keypair, 2, 100);

        printf("%s equals %s\n", keypair[0], keypair[1]);
    }

    return "0";
}

int env_get(char *key)
{
    return atoi(env_getstr(key));
}
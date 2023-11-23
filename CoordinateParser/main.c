#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>

#define MAX_FIELDS 15
#define MAX_LINES 22436

#define OBJ_COO_RH 0
#define OBJ_COO_RM 1
#define OBJ_COO_RS 2
#define OBJ_COO_RA 3
#define OBJ_COO_V 4
#define OBJ_COO_DG 5
#define OBJ_COO_DM 6
#define OBJ_COO_DS 7
#define OBJ_COO_DELTA 8
#define OBJ_COO_DESIGN_START 9
#define OBJ_COO_DESIGN_LENGTH 6

void parse_csv_line(char *line, char *fields[], int max_fields)
{
    char *token;
    int i = 0;

    // strtok is used to split the line into tokens
    token = strtok(line, ";");
    while (token != NULL && i < max_fields)
    {
        fields[i++] = strdup(token); // Allocate memory for each field
        token = strtok(NULL, ";");
    }
    // Ensure the fields array is properly terminated with NULL
    fields[i] = NULL;
}

void free_csv_data(char *csv_data[MAX_LINES][MAX_FIELDS])
{
    for (int i = 0; i < MAX_LINES; i++)
    {
        for (int j = 0; j < MAX_FIELDS; j++)
        {
            free(csv_data[i][j]); // Free the memory for each field
        }
    }
}

void parse_coordinates(FILE *file, char *data[MAX_LINES][MAX_FIELDS])
{
    // Initialize csv_data array
    for (int i = 0; i < MAX_LINES; i++)
    {
        for (int j = 0; j < MAX_FIELDS; j++)
        {
            data[i][j] = NULL;
        }
    }

    char line[MAX_LINES];
    char *fields[MAX_FIELDS];

    int line_count = 0;

    // Read and process each line of the file
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_LINES)
    {
        // Remove newline character at the end of the line
        line[strcspn(line, "\n")] = 0;

        // Parse the CSV line
        parse_csv_line(line, fields, MAX_FIELDS);

        // Copy fields to the 2D array
        for (int i = 0; i < MAX_FIELDS && fields[i] != NULL; i++)
        {
            data[line_count][i] = fields[i];
        }

        line_count++;
    }
}

void replaceChar(char *str, char target, char replacement)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == target)
        {
            str[i] = replacement;
        }
    }
}

void adjustDegrees(double *diff)
{
    int diffSign = (*diff > 0) - (*diff < 0);
    double diffAbs = *diff * diffSign;

    if (diffAbs > 180)
    {
        int factor = (int)(diffAbs / 180);
        diffAbs -= 180 * factor;
    }

    *diff = diffAbs * diffSign;
}

void promptStr(char *buffer, char quest[])
{
    printf("%s", quest);
    fgets(buffer, sizeof(buffer), stdin);

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
}

void promptInt(int *number, char quest[])
{
    {
        printf("%s", quest);
        if (scanf("%d", number) != 1)
        {
            number = NULL;
        }
    }
}

void stringToLower(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int findIn2D(char *csv_data[MAX_LINES][MAX_FIELDS], char object[])
{
    stringToLower(object);

    int foundedAt[MAX_LINES];
    int foundedIndex = 0;

    for (int i = 0; i < MAX_LINES; i++)
    {
        for (int j = OBJ_COO_DESIGN_START; j < OBJ_COO_DESIGN_START + OBJ_COO_DESIGN_LENGTH; j++) // in range of the object designations
        {
            if (csv_data[i][j] == NULL)
            {
                continue;
            }

            char *dataAtIndex = csv_data[i][j];
            stringToLower(dataAtIndex);

            if (strcmp(dataAtIndex, object) == 0)
            {
                return i;
            }

            if (strstr(dataAtIndex, object) != NULL) // if obj was found
            {
                foundedAt[foundedIndex++] = i + 1;
                break;
            }
        }
    }

    if (foundedAt[0] == 0)
    {
        return -1;
    }

    if (foundedIndex == 1)
    {
        printf(" ! Using \"%s\" !\n", csv_data[foundedAt[0] - 1][OBJ_COO_DESIGN_START]);
        return foundedAt[0] - 1;
    }

    printf("\n-> Found multiple objects:\n\n");
    for (size_t i = 0; i < foundedIndex; i++)
    {
        printf("  (%zu)\t on index %d found:", i + 1, foundedAt[i] - 1);

        // Print context information inline
        for (size_t j = OBJ_COO_DESIGN_START; j < OBJ_COO_DESIGN_START + OBJ_COO_DESIGN_LENGTH; j++)
        {
            if (csv_data[foundedAt[i] - 1][j] == NULL)
            {
                continue;
            }

            printf(" | %s", csv_data[foundedAt[i] - 1][j]);
        }

        printf("\n");
    }

    int selectedIndex;

    while (1)
    {
        promptInt(&selectedIndex, "\nEnter the index of your object |> ");
        if (selectedIndex > 0 && foundedIndex + 1 > selectedIndex)
        {
            break;
        }
        printf("Please enter a number from %d to %d\n", 1, foundedIndex);
    }

    return foundedAt[selectedIndex - 1] - 1;
}

int parseUserInput(int argc, char *argv[], char *csv_data[MAX_LINES][MAX_FIELDS], char *objA_name, char *objB_name, int *objA_index, int *objB_index)
{
    if (argc < 2)
    {
        while (1)
        {
            promptStr(objA_name, "Enter the first Object  |> ");
            if (strlen(objA_name) > 1)
            {
                break;
            }
            else
            {
                printf("%s is not a valid Object (need at least two characters)\n", objA_name);
            }
        }

        *objA_index = findIn2D(csv_data, objA_name);

        while (1)
        {
            promptStr(objB_name, "Enter the second Object |> ");
            if (strlen(objB_name) > 1)
            {
                break;
            }
            else
            {
                printf("%s is not a valid Object (need at least two characters)\n", objB_name);
            }
        }

        *objB_index = findIn2D(csv_data, objB_name);
    }
    else
    {
        if (strlen(argv[1]) < sizeof(objA_name))
        {
            strcpy(objA_name, argv[1]);
            *objA_index = findIn2D(csv_data, objA_name);
        }
        else
        {
            fprintf(stderr, "Object A name is too long\n");
            return 1;
        }

        if (strlen(argv[2]) < sizeof(objB_name))
        {
            strcpy(objB_name, argv[2]);
            *objB_index = findIn2D(csv_data, objB_name);
        }
        else
        {
            fprintf(stderr, "Object B name is too long\n");
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    ////////////////////////////////
    FILE *file;
    char *csv_data[MAX_LINES][MAX_FIELDS];

    char outputFormat[] = "RA%.6f;Delta%.6f";
    char output[100];

    // const char *serialPort = "/dev/ttyUSB0"; // Adjust this based on your system

    ////////////////////////////////
    // Get the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    ////////////////////////////////
    // Open the CSV file
    char filePath[2048];
    strcpy(filePath, cwd);
    strcat(filePath, "/coordinates.csv");

    file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    parse_coordinates(file, csv_data);

    // Close the file
    fclose(file);

    ////////////////////////////////
    char objA_name[100];
    char objB_name[100];

    int objA_index = -1;
    int objB_index = -1;

    if (parseUserInput(argc, argv, csv_data, objA_name, objB_name, &objA_index, &objB_index) > 0)
    {
        return 1;
    }

    //================================================================================================================================
    //----------------------------------------------------------------
    ////////////////////////////////    Check if the objects exist:
    //----------------------------------------------------------------
    if (objA_index < 0) // if object A was not found
    {
        fprintf(stderr, "Not found: %s\n", objA_name);
        return 1;
    }
    if (objB_index < 0) // if object B was not found
    {
        fprintf(stderr, "Not found: %s\n", objB_name);
        return 1;
    }
    //----------------------------------------------------------------
    ////////////////////////////////    Convert the names into the first designation:
    //----------------------------------------------------------------
    strcpy(objA_name, csv_data[objA_index][OBJ_COO_DESIGN_START]);
    strcpy(objB_name, csv_data[objB_index][OBJ_COO_DESIGN_START]);
    //----------------------------------------------------------------
    ////////////////////////////////    Calc the placeholder of the objects:
    //----------------------------------------------------------------
    char objA_plch[100] = "";
    for (int i = 0; i < strlen(objA_name); i++)
    {
        strcat(objA_plch, " ");
    }
    char objB_plch[100] = "";
    for (int i = 0; i < strlen(objB_name); i++)
    {
        strcat(objB_plch, " ");
    }
    //----------------------------------------------------------------
    ////////////////////////////////    Get the RA col from both objs:
    //----------------------------------------------------------------
    char *objA_RA = csv_data[objA_index][OBJ_COO_RA]; // get the val of the RA col
    char *objB_RA = csv_data[objB_index][OBJ_COO_RA];

    replaceChar(objA_RA, ',', '.'); // adapt the val to a double
    replaceChar(objB_RA, ',', '.');

    const double objA_RAd = strtod(objA_RA, NULL); // parse the val to a double
    const double objB_RAd = strtod(objB_RA, NULL);

    printf("\n RA of \"%s\": %s-> ", objA_name, objB_plch);
    printf("%sh %smin %ss", csv_data[objA_index][OBJ_COO_RH], csv_data[objA_index][OBJ_COO_RM], csv_data[objA_index][OBJ_COO_RS]);

    printf("\n RA of \"%s\": %s-> ", objB_name, objA_plch);
    printf("%sh %smin %ss", csv_data[objB_index][OBJ_COO_RH], csv_data[objB_index][OBJ_COO_RM], csv_data[objB_index][OBJ_COO_RS]);

    printf("\n");
    //----------------------------------------------------------------
    ////////////////////////////////    Get the Delta col from both objs:
    //----------------------------------------------------------------
    char *objA_Delta = csv_data[objA_index][OBJ_COO_DELTA]; // get the val of the Delta col
    char *objB_Delta = csv_data[objB_index][OBJ_COO_DELTA];

    replaceChar(objA_Delta, ',', '.'); // adapt the val to a double
    replaceChar(objB_Delta, ',', '.');

    const double objA_Deltad = strtod(objA_Delta, NULL); // parse the val to a double
    const double objB_Deltad = strtod(objB_Delta, NULL);

    printf("\n Delta of \"%s\": %s-> ", objA_name, objB_plch);
    printf("%s° %s' %s\"", csv_data[objA_index][OBJ_COO_DG], csv_data[objA_index][OBJ_COO_DM], csv_data[objA_index][OBJ_COO_DS]);

    printf("\n Delta of \"%s\": %s-> ", objB_name, objA_plch);
    printf("%s° %s' %s\"", csv_data[objB_index][OBJ_COO_DG], csv_data[objB_index][OBJ_COO_DM], csv_data[objB_index][OBJ_COO_DS]);

    printf("\n");
    //----------------------------------------------------------------
    ////////////////////////////////    Calc the differences
    //----------------------------------------------------------------
    double RA_diff = objA_RAd - objB_RAd;          // get diff of RA
    double Delta_diff = objA_Deltad - objB_Deltad; // get diff of Delta

    adjustDegrees(&RA_diff); // adjust the degrees
    adjustDegrees(&Delta_diff);

    sprintf(output, outputFormat, RA_diff, Delta_diff); // format the output
    //----------------------------------------------------------------
    ////////////////////////////////    Send the output:
    //----------------------------------------------------------------
    printf("\n  -> Send output \"%s\"\n\n", output);

    /*
     *
     * Print the output serial...
     *
     */

    //----------------------------------------------------------------
    ////////////////////////////////
    //================================================================================================================================

    // Free allocated memory
    free_csv_data(csv_data);

    return 0;
}

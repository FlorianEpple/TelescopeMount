//
//  telescopesteering.c
//  TeselscopeSteering
//
//  Created by Florian Epple on 27.11.23.
//

#ifdef _WIN32
#include <conio.h>
#endif

#include <stdio.h>
#include <time.h>

#include "includes/parsecsv.c"
#include "includes/env.c"
#include "includes/mcapc.c"
#include "includes/serial.c"

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
        // inizialization of object data
        double objdataA[2], objdataB[2];
        for (int i = 0; i < 2; i++)
        {
            objdataA[i] = objdataB[i] = 0.0;
        }

        ////////////////////////////////////////////////////////////////
        time_t currentTime;
        time(&currentTime);

        char *dateTimeString = ctime(&currentTime);

        if (dateTimeString[strlen(dateTimeString) - 1] == '\n')
            dateTimeString[strlen(dateTimeString) - 1] = '\0';

        printf("\e[1;1H\e[2J"); // clear console

        printf("=================== %s ===================\n", dateTimeString);
        printf("\n");
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

            // allocation of object data
            if (0 < objindexA)
            {
                char objectrealnA[100];
                strcpy(objectrealnA, csv_getat(csvfilename, objindexA - 1, env_get("OBJ_COO_DESIGN_START") + 1));

                objdataA[0] = (double)atof(csv_getat(csvfilename, objindexA - 1, env_get("OBJ_COO_RA") + 1));
                objdataA[1] = (double)atof(csv_getat(csvfilename, objindexA - 1, env_get("OBJ_COO_DELTA") + 1));

                printf("For A existing:  %-9s | Alpha: %11.6f | Delta: %11.6f\n", objectrealnA, objdataA[0], objdataA[1]);
            }
            else if (objindexA == -2)
            {
                char idenmanA[2][100];
                fnc_strsplitby(manA, ';', idenmanA, 2, 100);

                objdataA[0] = (double)atof(idenmanA[0]);
                objdataA[1] = (double)atof(idenmanA[1]);

                printf("For A manually: %-9s | Alpha: %11.6f | Delta: %11.6f\n", "(unknown)", objdataA[0], objdataA[1]);
            }

            if (0 < objindexB)
            {
                char objectrealnB[100];
                strcpy(objectrealnB, csv_getat(csvfilename, objindexB - 1, env_get("OBJ_COO_DESIGN_START") + 1));

                objdataB[0] = (double)atof(csv_getat(csvfilename, objindexB - 1, env_get("OBJ_COO_RA") + 1));
                objdataB[1] = (double)atof(csv_getat(csvfilename, objindexB - 1, env_get("OBJ_COO_DELTA") + 1));

                printf("For B existing:  %-9s | Alpha: %11.6f | Delta: %11.6f\n", objectrealnB, objdataB[0], objdataB[1]);
            }
            else if (objindexB == -2)
            {
                char idenmanB[2][100];
                fnc_strsplitby(manB, ';', idenmanB, 2, 100);

                objdataB[0] = (double)atof(idenmanB[0]);
                objdataB[1] = (double)atof(idenmanB[1]);

                printf("For B manually: %-9s | Alpha: %11.6f | Delta: %11.6f\n", "(unknown)", objdataB[0], objdataB[1]);
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

        /*
         * Dtata processing
         */

        printf("================================================================\n");
        printf("\n");

        printf("Calculate distance...\n");

        double radiff = mcapc_calcdiff(objdataA[0], objdataB[0]);
        double deldiff = mcapc_calcdiff(objdataA[1], objdataB[1]);

        printf("\n");
        printf("Alpha distance: %11.6f deg\n", radiff);
        printf("Delta distance: %11.6f deg\n", deldiff);
        printf("\n");

        char findecision_ = 'y';
        while (1)
        {

            printf("Do you want to send this data to the Arduino? (y/n) ");
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

            printf("\n");
        }

        printf("\n");

        if (findecision_ == '0')
        {
            char serialport[100], outputstr[200], outputformat[100];
            int baudrate = env_get("SERIAL_BAUD");

            strcpy(serialport, env_getstr("SERIAL_PORT"));
            strcpy(outputformat, env_getstr("OUTPUT_FORMAT"));

            while (1)
            {
                char decision_[100];

                printf("Using port %s ? (y/<port>) ", serialport);
                scanf(" %s", decision_);

                if (strcmp(decision_, "y") == 0)
                    break;

                printf("Switching port %s to %s...\n", serialport, decision_);
                printf("\n");
                strcpy(serialport, decision_);
            }

            sprintf(outputstr, outputformat, radiff, deldiff);

            while (1)
            {
                printf("\n");
                printf(" Using serial port %s\n", serialport);
                printf(" Using baud rate   %d\n", baudrate);
                printf(" Sending string   \"%s\" ...\n", outputstr);

                /*
                 * Send ouput to the arduino
                 */
                if (srl_sendstr(serialport, outputstr, baudrate) == 0)
                {
                    printf("\n");
                    printf("The string was sent successfully.\n");
                    break;
                }
                else
                {
                    printf("\n");
                    printf("Sending canceled.\n");

                    char decision_;
                    printf("Retry sending? (y/n) ");
                    scanf(" %c", &decision_);

                    if (decision_ != 'y')
                        break;

                    printf("\n");
                    printf(" ---");
                    printf("\n");
                }
            }
        }
        else
        {
            printf("Sending canceled.\n");
        }

        // press any key to continue...:
#if defined(__APPLE__)
        fnc_setnonblock(1);
#endif

        printf("\n");
        printf("\n");
        printf("Press any key to continue..\n");

#ifdef _WIN32
        if (_kbhit())
        {
            char ch = _getch();
        }
#elif defined(__APPLE__)
        char ch;

        while (1)
        {
            ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
            if (bytesRead > 0)
                break;
        }

        fnc_setnonblock(0);
#endif

        printf("\n");
    }

    return 0;
}
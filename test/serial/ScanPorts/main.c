#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <string.h>
#include <dirent.h>
#endif

void listSerialPorts();

#ifdef _WIN32
void listSerialPorts()
{
    char portName[10];
    for (int i = 1; i <= 256; ++i)
    {
        sprintf(portName, "COM%d", i);
        HANDLE hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if (hSerial != INVALID_HANDLE_VALUE)
        {
            printf("Arduino is connected to port: %s\n", portName);
            CloseHandle(hSerial);
        }
    }
}
#elif defined(__APPLE__)
void listSerialPorts()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/dev");
    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            printf("Scanning at /dev/%s\n", entry->d_name);
            if (strncmp(entry->d_name, "ttyUSB", 6) == 0 || strncmp(entry->d_name, "ttyACM", 6) == 0)
            {
                printf("Arduino is connected to port: /dev/%s\n", entry->d_name);
                return;
            }
        }

        closedir(dir);
    }
}
#endif

int main()
{
    listSerialPorts();
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <dirent.h>

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

int main()
{
    listSerialPorts();
    return 0;
}
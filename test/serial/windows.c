#include <stdio.h>
#include <windows.h>

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

int main()
{
    listSerialPorts();
    return 0;
}
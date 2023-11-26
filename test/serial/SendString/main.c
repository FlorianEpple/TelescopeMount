#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>

int main()
{
    // Get user input for the COM port
    char comPort[10];
    printf("Enter COM port (e.g., COM3): ");
    scanf("%s", comPort);

    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open the serial port
    hSerial = CreateFile(comPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error opening serial port\n");
        return 1;
    }

    // Set the serial parameters
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;   // Set your baud rate
    dcbSerialParams.ByteSize = 8;          // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity = NOPARITY;     // No parity

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        fprintf(stderr, "Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    char data[100];
    printf("Message (max 100): ");
    scanf("%s", data);

    strcat(data, "\n");

    DWORD bytesWritten;
    if (!WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL))
    {
        fprintf(stderr, "Error writing to serial port\n");
        CloseHandle(hSerial);
        return 1;
    }

    printf("Sent: %s", data);

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
#endif
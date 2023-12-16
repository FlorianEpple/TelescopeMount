//
// Created by Florian Epple on 29.11.23.
//

#ifndef SERIAL_C
#define SERIAL_C

#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

#include <stdio.h>
#include <string.h>

int srl_sendstr(const char port[], const char str[], unsigned int baud);

#ifdef _WIN32
int srl_sendstr(const char port[], const char str[], unsigned int baud)
{
    char comPort[100];
    char strcopy[1024];

    sprintf(strcopy, "%s\n", str);

    sprintf(comPort, "\\\\.\\%s", port);

    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open the serial port
    hSerial = CreateFile(comPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        perror("\n");
        fprintf(stderr, "Error opening serial port\n");
        return 1;
    }

    // Set the serial parameters
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        perror("\n");
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = baud;       // Set your baud rate
    dcbSerialParams.ByteSize = 8;          // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity = NOPARITY;     // No parity

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        perror("\n");
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
        perror("\n");
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    DWORD bytesWritten;
    if (!WriteFile(hSerial, strcopy, strlen(strcopy), &bytesWritten, NULL))
    {
        perror("\n");
        fprintf(stderr, "Error writing to serial port\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
#elif defined(__APPLE__)
int srl_sendstr(const char port[], const char str[], unsigned int baud)
{
    int fd = open(port, O_RDWR);
    char strcopy[1024];

    sprintf(strcopy, "%s\n", str);

    if (fd == -1)
    {
        perror("\n");
        perror("Error opening serial port");
        return 1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    options.c_cflag |= (CLOCAL | CREAD); // Enable receiver and set local mode
    options.c_cflag &= ~PARENB;          // No parity
    options.c_cflag &= ~CSTOPB;          // 1 stop bit
    options.c_cflag &= ~CSIZE;           // Mask the character size bits
    options.c_cflag |= CS8;              // 8 data bits

    tcsetattr(fd, TCSANOW, &options);

    write(fd, strcopy, strlen(strcopy));

    close(fd);

    return 0;
}

#endif

#endif // SERIAL_C
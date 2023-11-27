#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int main()
{
    const char *port = "/dev/cu.usbserial-14320"; // Replace with the actual serial port of your Arduino
    int fd = open(port, O_RDWR);

    if (fd == -1)
    {
        perror("Error opening serial port");
        return 1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600); // Set baud rate to 9600
    cfsetospeed(&options, B9600);

    options.c_cflag |= (CLOCAL | CREAD); // Enable receiver and set local mode
    options.c_cflag &= ~PARENB;          // No parity
    options.c_cflag &= ~CSTOPB;          // 1 stop bit
    options.c_cflag &= ~CSIZE;           // Mask the character size bits
    options.c_cflag |= CS8;              // 8 data bits

    tcsetattr(fd, TCSANOW, &options);

    // Write a string to the serial port
    const char *message = "Hallo\n";
    write(fd, message, strlen(message));

    close(fd);
    return 0;
}

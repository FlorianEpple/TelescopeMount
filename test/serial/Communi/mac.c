#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define SERIAL_PORT "/dev/cu.usbserial-1420" // Change this to your Arduino's serial port

void set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0)
    {
        perror("Error from tcgetattr");
        exit(-1);
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);

    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;

    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("Error from tcsetattr");
        exit(-1);
    }
}

void set_blocking(int fd, int should_block)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0)
    {
        perror("Error from tcgetattr");
        exit(-1);
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
    {
        perror("Error setting term attributes");
    }
}

int main()
{
    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);

    if (serial_fd < 0)
    {
        perror("Error opening serial port");
        return -1;
    }

    set_interface_attribs(serial_fd, B9600, 0); // Set baud rate to 9600 bps, no parity
    set_blocking(serial_fd, 0);                 // Set non-blocking

    char buffer[256];

    // Reading from Arduino
    printf("Reading from Arduino:\n");
    ssize_t n = read(serial_fd, buffer, sizeof(buffer));

    if (n < 0)
    {
        perror("Error reading from serial port");
    }
    else
    {
        printf("Read %zd bytes: %.*s\n", n, (int)n, buffer);
    }

    // Writing to Arduino
    char *message = "Hello from C program!\n";
    printf("Writing to Arduino:\n");
    ssize_t bytes_written = write(serial_fd, message, strlen(message));

    if (bytes_written < 0)
    {
        perror("Error writing to serial port");
    }
    else
    {
        printf("Wrote %zd bytes\n", bytes_written);
    }

    close(serial_fd);

    return 0;
}

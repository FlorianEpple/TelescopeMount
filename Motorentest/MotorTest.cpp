#include "Motor.cpp"
#include <functional>
#include <iostream>

int motorPinA = 0;
int motorPinB = 1;

int main(int argc, char const *argv[])
{
    Motor alpha(motorPinA, motorPinB);

    int arg1 = std::atoi(argv[1]);
    int arg2 = std::atoi(argv[2]);
    int arg3 = std::atoi(argv[3]);

    alpha.acceleration(arg1, arg2, arg3);

    return 0;
}

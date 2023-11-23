#include "AccelStepper.h"
#include <iostream>

AccelStepper::AccelStepper(int pinA, int pinB)
{
    // constructor
    this->_speed = 0;
}

void AccelStepper::setSpeed(int n)
{
    this->_speed = n;
}

void AccelStepper::run()
{
    std::cout << this->_speed << std::endl;
}
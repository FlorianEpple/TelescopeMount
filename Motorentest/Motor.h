// Motor.h
#ifndef MOTOR_H
#define MOTOR_H

#include "AccelStepper.cpp"

class Motor : public AccelStepper
{
public:
    Motor(int pinA, int pinB) : AccelStepper(pinA, pinB) {}

    void acceleration(int n, int t, int beginSpeed = 0);
};

#endif // MOTOR_H
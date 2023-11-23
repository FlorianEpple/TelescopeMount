// AccelStepper.h
#ifndef ACCELSTEPPER_H
#define ACCELSTEPPER_H

class AccelStepper
{
private:
    int _speed;

public:
    AccelStepper(int pinA, int pinB);

    void setSpeed(int n);
    void run();
};

#endif // ACCELSTEPPER_H
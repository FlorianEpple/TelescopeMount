#include "Motor.h"

#include <iostream>
#include <thread>
#include <chrono>

void Motor::acceleration(int n, int t, int beginSpeed)
{
    int diff = n - beginSpeed;
    int dt = t / std::abs(diff);
    int k = 0;

    for (int i = beginSpeed; i <= std::abs(diff) + beginSpeed; ++i)
    {
        this->setSpeed(diff < 0 ? beginSpeed - k : beginSpeed + k);
        this->run();

        ++k;

        std::chrono::milliseconds duration(dt);
        std::this_thread::sleep_for(duration);
    }
}
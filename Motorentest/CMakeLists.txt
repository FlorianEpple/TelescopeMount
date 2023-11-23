cmake_minimum_required(VERSION 3.26)
project(Motorentest)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Motorentest
        AccelStepper.cpp
        AccelStepper.h
        Motor.cpp
        Motor.h
        MotorTest.cpp)

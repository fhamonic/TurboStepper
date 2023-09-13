# TurboStepper

An Arduino library for controlling stepper motors with trapezoidal velocity profiles using Interrupt Service Routines (ISR).
It is designed around C++ templates to be:
* Versatile: You can implement your own timers, profiles and steppers interfaces.
* Efficient: Constants known at compile time are optimized.
* Easy-to-use: Hardware details are encapsulated within classes while still triggering errors when incompatible parameters are detected.

You can use 2 steppers per timer so up to 6 steppers on an Arduino Nano but don't overdo the microstepping and rotational speed. The maximum safe steps/s with an Arduino Nan is about 10k, the bottleneck being the end of the accelerating phase.  

[![Generic badge](https://img.shields.io/badge/license-Boost%20Software%20License-blue)](https://www.boost.org/users/license.html)

## Dependencies

Uses the library [digitalWriteFast](https://www.arduino.cc/reference/en/libraries/digitalwritefast/).

## Code example

```cpp
#include "ATMEGA328P_Timer1.hpp"
#include "StepperMotor.hpp"
#include "TrapezoidalProfile.hpp"

// step pin = 2
// dir pin = 3
// steps/turn = 800
// max speed = 12 (turns/sec)
// max acceleration = 4 (turns/sec/sec)
using Stepper1 = Stepper<2, 3, 800, 12, 4>;
// use Timer1 with 2MHz clock
using Timer = ATMEGA328P::Timer1<C2MHz>;
using Profile = TrapezoidalProfile<Stepper1, Timer::CounterA>;

ISR(TIMER1_COMPA_vect) { Profile::DoStep(); }

void setup() {
    Stepper1::Setup();
    Timer::Setup();
    Profile::Setup();
}

void loop() {
    Profile::MoveTo(40000);
    while(true);
}
```
    

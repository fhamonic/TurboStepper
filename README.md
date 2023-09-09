# TurboStepper

An Arduino library for controlling stepper motors with trapezoidal velocity profiles using Interrupt Service Routines (ISR).
It is designed around C++ templates to be:
* Versatile: You can implement your own timers, profiles and steppers interfaces.
* Efficient: Constants known at compile time are optimized.
* Easy-to-use: Hardware details are encapsulated within classes while still triggering error when incompatible parameters are detected.

You can use 2 steppers per timer so up to 6 steppers on an Arduino Nano but don't overdo the microstepping and rotational speed, in this case, to prevent CPU bottleneck. The maximum steps/s is about 10k on Arduino Nano.

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
// max speed = 1 (turns/sec)
// max acceleration = 2 (turns/sec/sec)
Stepper<2, 3, 800, 1, 2> stepper;
// use Time1 with 250kHz clock
ATMEGA328P_Timer1<C250kHz> timer1;
TrapezoidalProfile<decltype(stepper), decltype(timer1)::CounterA> profile(
    stepper);

ISR(TIMER1_COMPA_vect) { profile.do_step(); }

void setup() {
    stepper.setup();
    timer1.setup();
    profile.setup();
}

void loop() {
    profile.moveTo(4000);
    while(true);
}
```
    

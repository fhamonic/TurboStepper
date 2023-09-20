# TurboStepper

An Arduino library for controlling stepper motors and RC servo motors with Interrupt Service Routines (ISR).
Unlike the AccelStepper library, using ISRs does not require periodic calls to the ```stepper.run()``` function in the main loop.
Since ISRs are quick interruptions of the main loop execution, this also guaranty that steps will be done in time, even if the program is executing a slow operation like an ```analogRead()``` or a ```Serial.read()```.

The library is designed around templates to be:
* Versatile: You can easily add implement your own timers, profiles and steppers interfaces.
* Efficient: Constants known at compile time are optimized to make the ISRs as quick as possible.
* Easy-to-use: Hardware details are encapsulated within classes while still triggering errors when incompatible parameters are detected.

The provided implementations allows using 2 steppers per timer so up to 6 steppers on an Arduino Nano with a maximum safe steps/s of about 8k, the bottleneck being the end of the accelerating phase.  

[![Generic badge](https://img.shields.io/badge/license-Boost%20Software%20License-blue)](https://www.boost.org/users/license.html)

## Dependencies

Uses the library [digitalWriteFast](https://www.arduino.cc/reference/en/libraries/digitalwritefast/).

## Code examples

```cpp
#include <TurboStepper.hpp>

// step pin = 2
// dir pin = 3
// steps/turn = 800
// max speed = 2 (turns/sec)
// max acceleration = 4 (turns/sec/sec)
using Stepper1 = Stepper<2, 3, 800, 2, 4>;
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
    Profile::MoveForward(4000);
    while(Profile::IsMoving());
    Profile::MoveBackward(4000);
    while(Profile::IsMoving());
}
```


```cpp
#include <TurboStepper.hpp>

// use Timer2 with 15_625Hz clock
using Timer2 = ATMEGA328P::Timer2<C15_625Hz>;
// PWM pin = 4
using Servo1 = Servo<4, Timer2::CounterA>;

ISR(TIMER2_COMPA_vect) { Servo1::DoPulse(); }

void setup() {
    Timer2::Setup();
    Servo1::Setup();
}

void loop() {
    Servo1::MoveTo(0);
    delay(2000);
    Servo1::MoveTo(180.0);
    delay(2000);
}
```
    

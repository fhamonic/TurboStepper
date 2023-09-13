#include "ATMEGA328P/Timer1.hpp"
#include "StepperMotor.hpp"
#include "TrapezoidalProfile.hpp"

// step pin = 2
// dir pin = 3
// steps/turn = 800
// max speed = 1 (turns/sec)
// max acceleration = 2 (turns/sec/sec)
using Stepper1 = Stepper<2, 3, 800, 12, 8>;
// use Time1 with 2MHz clock
using Timer = ATMEGA328P::Timer1<C2MHz>;
using Profile = TrapezoidalProfile<Stepper1, Timer::CounterA>;

ISR(TIMER1_COMPA_vect) { Profile::DoStep(); }

void setup() {
    Stepper1::Setup();
    Timer::Setup();
    Profile::Setup();
}

void loop() {
    Profile::MoveForward(60000);
    while(true)
        ;
}

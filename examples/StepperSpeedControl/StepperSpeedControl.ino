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
    Profile::SetSpeed(1);
    delay(2000);
    Profile::SetSpeed(2);
    delay(2000);
    Profile::SetSpeed(1);
    delay(2000);
    Profile::SetSpeed(0);
    while(Profile::IsMoving()) ;
}
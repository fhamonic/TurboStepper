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
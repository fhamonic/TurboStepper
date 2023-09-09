#include "ATMEGA328P_Timer1.hpp"
#include "StepperMotor.hpp"
#include "TrapezoidalProfile.hpp"

// step_pin = 2
// dir pin = 3
// steps/turn = 800
// max turns/sec = 1 (speed)
// max turns/sec/sec = 2 (acceleration)
Stepper<2, 3, 800, 1, 2> stepper;
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

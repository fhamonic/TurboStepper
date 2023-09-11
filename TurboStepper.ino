#include "ATMEGA328P/Timer1.hpp"
#include "StepperMotor.hpp"
#include "TrapezoidalProfile.hpp"

// step pin = 2
// dir pin = 3
// steps/turn = 800
// max speed = 1 (turns/sec)
// max acceleration = 2 (turns/sec/sec)
Stepper<2, 3, 800, 14, 4> stepper;
// use Time1 with 250kHz clock
ATMEGA328P::Timer1<C2MHz> timer;
TrapezoidalProfile<decltype(stepper), decltype(timer)::CounterA> profile(
    stepper);
ISR(TIMER1_COMPA_vect) { profile.do_step(); }

void setup() {
    Serial.begin(9600);
    stepper.setup();
    timer.setup();
    profile.setup();

    // Serial.println("min/max:");
    // Serial.println(decltype(profile)::MIN_TICKS_PER_STEP);
    // Serial.println(decltype(profile)::MAX_TICKS_PER_STEP);
    // Serial.println("start");
}

long int pos = 1600;
void loop() {
    for(int i = 0; i < 4; ++i) {
        profile.moveTo(pos);
        while(!profile.stopped())
            ;
        delay(1000);
        pos *= 4;
    }
    while(true)
        ;
}

#include "ATMEGA328P/Timer2.hpp"
#include "Servo.hpp"

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
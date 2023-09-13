// #include "StepperMotor.hpp"
// #include "TrapezoidalProfile.hpp"

// // step pin = 2
// // dir pin = 3
// // steps/turn = 800
// // max speed = 1 (turns/sec)
// // max acceleration = 2 (turns/sec/sec)
// Stepper<2, 3, 800, 8, 4> stepper;
// struct Counter {
//     static long unsigned int delay_us;
//     static bool active;
//     static constexpr unsigned long int MAX_VALUE = 65535UL;
//     static constexpr unsigned long int TICKS_PER_SEC = 2000000UL;
//     static void enable() { active = true; }
//     static void disable() { active = false; }
//     static void set(unsigned long int ticks) { delay_us = ticks; }
//     template <typename T>
//     static void increment(T ticks) {
//         delay_us = ticks;
//     }
// };
// unsigned long int Counter::delay_us = 0;
// bool Counter::active = false;

// volatile TrapezoidalProfile<decltype(stepper), Counter> profile(stepper);

// void setup() {
//     stepper.setup();
//     profile.setup();

//     Serial.begin(115200);
//     Serial.println("min/max:");
//     Serial.println(decltype(profile)::MIN_TICKS_PER_STEP);
//     Serial.println(decltype(profile)::MAX_TICKS_PER_STEP);
//     Serial.println("start");
// }

// void loop() {
//     profile.moveTo(1600);
//     Serial.print('[');
//     while(Counter::active) {
//         unsigned long int t = micros();
//         Serial.print(Counter::delay_us);
//         Serial.print(',');
//         profile.DoStep();
//     }
//     Serial.println(']');

//     while(true)
//         ;
// }

// // screen /dev/ttyUSB0 9600

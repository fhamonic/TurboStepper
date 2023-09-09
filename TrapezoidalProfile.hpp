#ifndef TRAPEZOIDAL_PROFILE_HPP
#define TRAPEZOIDAL_PROFILE_HPP

// constexpr float constexpr_sqrt(float x, float curr, float prev) {
//     return curr == prev ? curr
//                         : constexpr_sqrt(x, 0.5 * (curr + x / curr), curr);
// }
// constexpr float constexpr_sqrt(float x) { return constexpr_sqrt(x, x, 0); }

template <typename Stepper, typename Counter>
struct TrapezoidalProfile {
    static constexpr unsigned long int MAX_TICKS_PER_STEP =
        Counter::TICKS_PER_SEC /
        sqrt(Stepper::STEPS_PER_TURN * Stepper::MAX_TURNS_PER_SEC_PER_SEC);
    static constexpr unsigned long int MIN_TICKS_PER_STEP =
        Counter::TICKS_PER_SEC /
        (Stepper::STEPS_PER_TURN * Stepper::MAX_TURNS_PER_SEC);

    static_assert(MAX_TICKS_PER_STEP <= Counter::MAX_VALUE,
                  "Stepper microstepping too low, Stepper acceleration too low "
                  "or Timer resolution too high");
    // Safety of 100us per step
    static_assert(MIN_TICKS_PER_STEP > 0.000100 * Counter::TICKS_PER_SEC,
                  "Stepper microstepping too high, Stepper max speed too high "
                  "or Timer resolution too low");

    Stepper & stepper;

    long int targetStepPos;
    long int middleStepPos;
    long int rampDownStepPos;
    long int n = 0;
    float d = 0;

    void (TrapezoidalProfile::*phase_ptr)(void);

    TrapezoidalProfile(Stepper & s) : stepper(s) {}

    void setup() { targetStepPos = stepper.stepPos; }

    void accel_step() {
        stepper.stepHIGH();
        n += 4;
        d -= (2.0 * d) / (n + 1);
        Counter::increment(d);
        stepper.stepLOW();
        if(d <= MIN_TICKS_PER_STEP) {  // reached max speed
            d = MIN_TICKS_PER_STEP;
            phase_ptr = &run_step;
            rampDownStepPos = middleStepPos - stepper.stepPos + middleStepPos;
        }
        if(stepper.stepPos == middleStepPos) {  // reached middle point
            phase_ptr = &decel_step;
        }
    }

    void run_step() {
        stepper.stepHIGH();
        Counter::increment(d);
        stepper.stepLOW();
        if(stepper.stepPos == rampDownStepPos) {  // reached end of cruise
            phase_ptr = &decel_step;
        }
    }

    void decel_step() {
        stepper.stepHIGH();
        n -= 4;
        d *= static_cast<float>(n + 1) / (n - 1);
        Counter::increment(d);
        stepper.stepLOW();
        if(stepper.stepPos == targetStepPos) {
            Counter::disable();
        }
    }

    void do_step() { (this->*(phase_ptr))(); }

    void moveTo(long int steps) {
        if(steps > stepper.stepPos)
            stepper.dirForward();
        else
            stepper.dirBackward();

        targetStepPos = steps;
        middleStepPos = (stepper.stepPos & targetStepPos) +
                        ((stepper.stepPos ^ targetStepPos) >> 1);
        d = MAX_TICKS_PER_STEP;
        n = 0;
        phase_ptr = &accel_step;
        Counter::set(d);
        Counter::enable();
    }
};

#endif  // TRAPEZOIDAL_PROFILE_HPP

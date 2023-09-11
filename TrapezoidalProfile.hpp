#ifndef TRAPEZOIDAL_PROFILE_HPP
#define TRAPEZOIDAL_PROFILE_HPP

// based on http://ww1.microchip.com/downloads/en/AppNotes/doc8017.pdf

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
    static_assert(MIN_TICKS_PER_STEP > 0.000070 * Counter::TICKS_PER_SEC,
                  "Stepper microstepping too high, Stepper max speed too high "
                  "or Timer resolution too low");

    volatile Stepper & stepper;

    using Steps = unsigned int;

    Steps targetStepPos;
    Steps middleStepPos;
    Steps rampDownStepPos;
    unsigned int n = 0;
    float d = 0;
    void (TrapezoidalProfile::*volatile phase_ptr)(void) = nullptr;

    TrapezoidalProfile(Stepper & s) : stepper(s) {}

    void setup() {
        targetStepPos = stepper.stepPos;
        Counter::disable();
    }
    void do_step() { (this->*(phase_ptr))(); }
    bool stopped() const { return phase_ptr == nullptr; }

    void accel_step() {
        stepper.stepHIGH();
        n += 4;
        d -= (2.0 * d) / (n + 1);
        if(d <= MIN_TICKS_PER_STEP) {  // reached max speed
            d = MIN_TICKS_PER_STEP;
            phase_ptr = &TrapezoidalProfile::run_step;
            rampDownStepPos =
                middleStepPos - stepper.stepPos + middleStepPos + 1;
        }
        Counter::increment(d);
        if(stepper.stepPos == middleStepPos) {  // reached middle point
            phase_ptr = &TrapezoidalProfile::decel_step;
        }
        stepper.stepLOW();
    }

    void run_step() {
        stepper.stepHIGH();
        Counter::increment(d);
        if(stepper.stepPos == rampDownStepPos) {  // reached end of cruise
            phase_ptr = &TrapezoidalProfile::decel_step;
        }
        stepper.stepLOW();
    }

    void decel_step() {
        stepper.stepHIGH();
        n -= 4;
        d *= static_cast<float>(n + 1) / (n - 1);
        Counter::increment(d);
        if(stepper.stepPos == targetStepPos) {
            Counter::disable();
            phase_ptr = nullptr;
        }
        stepper.stepLOW();
    }

    void initMove(Steps steps) {
        stepper.stepHIGH();
        d = MAX_TICKS_PER_STEP;
        n = 0;
        phase_ptr = &TrapezoidalProfile::accel_step;
        stepper.stepLOW();
        Counter::set(d);
        Counter::enable();
    }

    void moveTo(Steps steps) {
        if(steps == targetStepPos) return;
        Steps stepsForward = steps - stepper.stepPos;
        Steps stepsBackward = stepper.stepPos - steps;
        if(steps - stepper.stepPos <= stepper.stepPos - steps) {
            stepper.dirForward();
            middleStepPos = stepper.stepPos + stepsForward / 2;
        } else {
            stepper.dirBackward();
            middleStepPos = stepper.stepPos + stepsBackward / 2;
        }
        targetStepPos = steps;

        initMove(steps);
    }

    void moveBy(Steps steps) {
        if(steps == 0) return;
        if(steps > 0)
            stepper.dirForward();
        else
            stepper.dirBackward();
        targetStepPos = stepper.stepPos + steps;
        middleStepPos = stepper.stepPos + steps / 2;

        initMove(stepper.stepPos + steps);
    }
};

#endif  // TRAPEZOIDAL_PROFILE_HPP

#ifndef TRAPEZOIDAL_PROFILE_HPP
#define TRAPEZOIDAL_PROFILE_HPP

// based on http://ww1.microchip.com/downloads/en/AppNotes/doc8017.pdf

// constexpr float constexpr_sqrt(float x, float curr, float prev) {
//     return curr == prev ? curr
//                         : constexpr_sqrt(x, 0.5 * (curr + x / curr), curr);
// }
// constexpr float constexpr_sqrt(float x) { return constexpr_sqrt(x, x, 0); }

template <typename Stepper, typename Counter>
class TrapezoidalProfile {
public:
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

    using StepsType = typename Stepper::StepsType;

    struct Data {
        float d;
        unsigned int n;
        StepsType middleStepPos;
        StepsType rampDownStepPos;
        StepsType targetStepPos;
    };

public:
    static Data data;
    static void (*volatile phase_ptr)(void);

public:
    static void Setup() {
        data.targetStepPos = Stepper::stepPos;
        Counter::Disable();
    }
    static void DoStep() { (*phase_ptr)(); }
    static bool stopped() { return phase_ptr == nullptr; }

    static void AccelStep() {
        Stepper::StepHIGH();
        data.n += 4;
        data.d -= (2.0 * data.d) / (data.n + 1);
        if(data.d <= MIN_TICKS_PER_STEP) {  // reached max speed
            data.d = MIN_TICKS_PER_STEP;
            phase_ptr = &TrapezoidalProfile::RunStep;
            data.rampDownStepPos =
                data.middleStepPos - Stepper::stepPos + data.middleStepPos + 1;
        }
        Counter::Increment(data.d);
        if(Stepper::stepPos == data.middleStepPos) {  // reached middle point
            phase_ptr = &TrapezoidalProfile::DecelStep;
        }
        Stepper::StepLOW();
    }

    static void RunStep() {
        Stepper::StepHIGH();
        Counter::Increment(data.d);
        if(Stepper::stepPos == data.rampDownStepPos) {  // reached end of cruise
            phase_ptr = &TrapezoidalProfile::DecelStep;
        }
        Stepper::StepLOW();
    }

    static void DecelStep() {
        Stepper::StepHIGH();
        data.n -= 4;
        data.d *= static_cast<float>(data.n + 1) / (data.n - 1);
        Counter::Increment(data.d);
        if(Stepper::stepPos == data.targetStepPos) {
            Counter::Disable();
            phase_ptr = nullptr;
        }
        Stepper::StepLOW();
    }

    static void InitMove() {
        Stepper::StepHIGH();
        Counter::Set(data.d = MAX_TICKS_PER_STEP);
        data.n = 0;
        phase_ptr = &TrapezoidalProfile::AccelStep;
        Counter::Enable();
        Stepper::StepLOW();
    }

    static void MoveForward(StepsType steps) {
        if(steps == 0) return;
        Stepper::DirForward();
        data.targetStepPos = Stepper::stepPos + steps;
        data.middleStepPos = Stepper::stepPos + steps / 2;
        InitMove();
    }

    static void MoveBackward(StepsType steps) {
        if(steps == 0) return;
        Stepper::DirBackward();
        data.targetStepPos = Stepper::stepPos - steps;
        data.middleStepPos = Stepper::stepPos - steps / 2;
        InitMove();
    }

    static void MoveTo(StepsType steps) {
        if(steps == data.targetStepPos) return;
        StepsType stepsForward = steps - Stepper::stepPos;
        StepsType stepsBackward = Stepper::stepPos - steps;
        if(stepsForward <= stepsBackward) {
            MoveForward(stepsForward);
        } else {
            MoveBackward(stepsBackward);
        }
    }
};

template <typename _S, typename _C>
typename TrapezoidalProfile<_S, _C>::Data TrapezoidalProfile<_S, _C>::data = {};

template <typename _S, typename _C>
void (*volatile TrapezoidalProfile<_S, _C>::phase_ptr)(void) = nullptr;

#endif  // TRAPEZOIDAL_PROFILE_HPP

#ifndef TRAPEZOIDAL_PROFILE_HPP
#define TRAPEZOIDAL_PROFILE_HPP

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
    // Safety of 100us per step (minimum mesure at 80us with ATMEGA328P)
    static_assert(MIN_TICKS_PER_STEP > 0.000080 * Counter::TICKS_PER_SEC,
                  "Stepper microstepping too high, Stepper max speed too high "
                  "or Timer resolution too low");

    using StepsType = typename Stepper::StepsType;

    struct Data {
        float d;
        unsigned int n;
        StepsType middleStepPos;
        StepsType rampDownStepPos;
        StepsType targetStepPos;
        unsigned int targetTicksPerStep;
    };

public:
    static volatile Data data;
    static void (*volatile phase_ptr)(void);

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
        if(Stepper::stepPos == data.middleStepPos) {  // reached middle point
            phase_ptr = &TrapezoidalProfile::DecelStep;
        }
        Counter::Increment(data.d);
        Stepper::StepLOW();
    }

    static void RunStep() {
        Stepper::StepHIGH();
        if(Stepper::stepPos == data.rampDownStepPos) {  // reached end of cruise
            phase_ptr = &TrapezoidalProfile::DecelStep;
        }
        Counter::Increment(data.d);
        Stepper::StepLOW();
    }

    static void DecelStep() {
        Stepper::StepHIGH();
        data.n -= 4;
        data.d *= static_cast<float>(data.n + 1) / (data.n - 1);
        if(Stepper::stepPos == data.targetStepPos) {
            Counter::Disable();
            phase_ptr = nullptr;
        }
        Counter::Increment(data.d);
        Stepper::StepLOW();
    }

    static void AccelSpeed() {
        Stepper::StepHIGH();
        data.n += 4;
        data.d -= (2.0 * data.d) / (data.n + 1);
        if(data.d <= data.targetTicksPerStep) {  // reached target speed
            data.d = data.targetTicksPerStep;
            phase_ptr = &TrapezoidalProfile::RunSpeed;
        }
        Counter::Increment(data.d);
        Stepper::StepLOW();
    }

    static void RunSpeed() {
        Stepper::StepHIGH();
        Counter::Increment(data.d);
        Stepper::StepLOW();
    }

    static void DecelSpeed() {
        Stepper::StepHIGH();
        data.n -= 4;
        data.d *= static_cast<float>(data.n + 1) / (data.n - 1);
        if(data.d >= data.targetTicksPerStep) {
            data.d = data.targetTicksPerStep;
            phase_ptr = &TrapezoidalProfile::RunSpeed;
        }
        if(data.n == 0) {
            Counter::Disable();
            phase_ptr = nullptr;
        }
        Counter::Increment(data.d);
        Stepper::StepLOW();
    }

    static void SwitchDirSpeed() {
        Stepper::StepHIGH();
        data.n -= 4;
        data.d *= static_cast<float>(data.n + 1) / (data.n - 1);
        Counter::Increment(data.d);
        Stepper::StepLOW();
        if(data.n == 0) {
            Counter::Set(data.d = MAX_TICKS_PER_STEP);
            phase_ptr = &TrapezoidalProfile::AccelSpeed;
            if(Stepper::IsForward())
                Stepper::DirBackward();
            else
                Stepper::DirForward();
        }
    }

public:
    static void Setup() {
        data.targetStepPos = Stepper::stepPos;
        Counter::Disable();
    }
    static void DoStep() { (*phase_ptr)(); }
    static bool IsMoving() { return phase_ptr != nullptr; }

    static void InitMove() {
        Stepper::StepHIGH();
        Counter::Set(data.d = MAX_TICKS_PER_STEP);
        data.n = 0;
        Counter::Enable();
        Stepper::StepLOW();
    }

    static void MoveForward(StepsType steps) {
        if(steps == 0) return;
        Stepper::DirForward();
        data.targetStepPos = Stepper::stepPos + steps;
        data.middleStepPos = Stepper::stepPos + steps / 2;
        phase_ptr = &TrapezoidalProfile::AccelStep;
        InitMove();
    }

    static void MoveBackward(StepsType steps) {
        if(steps == 0) return;
        Stepper::DirBackward();
        data.targetStepPos = Stepper::stepPos - steps;
        data.middleStepPos = Stepper::stepPos - steps / 2;
        phase_ptr = &TrapezoidalProfile::AccelStep;
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

    static void SetSpeed(float turnsPerSec) {
        if(turnsPerSec == 0) {
            data.targetTicksPerStep = Counter::MAX_VALUE;
            phase_ptr = &TrapezoidalProfile::DecelSpeed;
            return;
        }

        data.targetTicksPerStep =
            constrain(Counter::TICKS_PER_SEC /
                          (Stepper::STEPS_PER_TURN * abs(turnsPerSec)),
                      MIN_TICKS_PER_STEP, MAX_TICKS_PER_STEP);

        if(IsMoving()) {
            if((turnsPerSec > 0) == (Stepper::IsForward())) {
                if(data.d < data.targetTicksPerStep)
                    phase_ptr = &TrapezoidalProfile::DecelSpeed;
                else
                    phase_ptr = &TrapezoidalProfile::AccelSpeed;
            } else {
                phase_ptr = &TrapezoidalProfile::SwitchDirSpeed;
            }
        } else {
            if(turnsPerSec > 0)
                Stepper::DirForward();
            else
                Stepper::DirBackward();
            InitMove();
            phase_ptr = &TrapezoidalProfile::AccelSpeed;
        }
    }

    static void WaitStop() {
        while(IsMoving())
            ;
    }
};

template <typename _S, typename _C>
volatile
    typename TrapezoidalProfile<_S, _C>::Data TrapezoidalProfile<_S, _C>::data =
        {};

template <typename _S, typename _C>
void (*volatile TrapezoidalProfile<_S, _C>::phase_ptr)(void) = nullptr;

#endif  // TRAPEZOIDAL_PROFILE_HPP

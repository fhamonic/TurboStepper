#ifndef STEPPER_HPP
#define STEPPER_HPP

#include <digitalWriteFast.h>

template <uint8_t STEP_PIN, uint8_t DIR_PIN, unsigned int _STEPS_PER_TURN = 200,
          int _MAX_TURNS_PER_SEC = 4, int _MAX_TURNS_PER_SEC_PER_SEC = 8>
class Stepper {
public:
    using StepsType = unsigned int;

    static constexpr StepsType STEPS_PER_TURN = _STEPS_PER_TURN;
    static constexpr float MAX_TURNS_PER_SEC = _MAX_TURNS_PER_SEC;
    static constexpr float MAX_TURNS_PER_SEC_PER_SEC =
        _MAX_TURNS_PER_SEC_PER_SEC;

    static volatile StepsType stepPos;
    static volatile StepsType stepDir;

public:
    static void Setup() {
        pinModeFast(STEP_PIN, OUTPUT);
        pinModeFast(DIR_PIN, OUTPUT);
        DirForward();
    }
    static void StepHIGH() {
        digitalWriteFast(STEP_PIN, HIGH);
        stepPos += stepDir;
    }
    static void StepLOW() { digitalWriteFast(STEP_PIN, LOW); }
    static void DirForward() {
        digitalWriteFast(DIR_PIN, LOW);
        stepDir = 1;
    }
    static void DirBackward() {
        digitalWriteFast(DIR_PIN, HIGH);
        stepDir = -1;
    }
    static bool IsForward() {
        return stepDir == 1;
    }
    static bool IsBackward() {
        return !IsForward();
    }

public:
    void setup() { Stepper::Setup(); }
    StepsType stepsPerTurn() const { return Stepper::stepsPerTurn(); }
    float maxTurnsPerSec() const { return Stepper::maxTurnsPerSec(); }
    float maxTurnsPerSecPerSec() const {
        return Stepper::maxTurnsPerSecPerSec();
    }
    void stepHIGH() { Stepper::StepHIGH(); }
    void stepLOW() { Stepper::StepLOW(); }
    void dirForward() { Stepper::DirForward(); }
    void dirBackward() { Stepper::DirBackward(); }
};

template <uint8_t _SP, uint8_t _DP, unsigned int _SPT, int _TPS, int _SPSPS>
volatile typename Stepper<_SP, _DP, _SPT, _TPS, _SPSPS>::StepsType 
    Stepper<_SP, _DP, _SPT, _TPS, _SPSPS>::stepPos = 0;

template <uint8_t _SP, uint8_t _DP, unsigned int _SPT, int _TPS, int _SPSPS>
volatile typename Stepper<_SP, _DP, _SPT, _TPS, _SPSPS>::StepsType
    Stepper<_SP, _DP, _SPT, _TPS, _SPSPS>::stepDir = 0;

#endif  // STEPPER_HPP

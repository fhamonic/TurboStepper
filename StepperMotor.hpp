#ifndef STEPPER_MOTOR_HPP
#define STEPPER_MOTOR_HPP

#include <digitalWriteFast.h>

struct StepperMotor {
    long int stepPos;
    long int stepDir;
    virtual void setup() = 0;
    virtual unsigned int stepsPerTurn() const = 0;
    virtual float maxTurnsPerSec() const = 0;
    virtual float maxTurnsPerSecPerSec() const = 0;
    virtual void stepHIGH() = 0;
    virtual void stepLOW() = 0;
    virtual void dirForward() = 0;
    virtual void dirBackward() = 0;
};

template <uint8_t STEP_PIN, uint8_t DIR_PIN, unsigned int _STEPS_PER_TURN = 200,
          int _MAX_TURNS_PER_SEC = 4, int _MAX_TURNS_PER_SEC_PER_SEC = 8>
class Stepper : public StepperMotor {
public:
    static constexpr unsigned int STEPS_PER_TURN = _STEPS_PER_TURN;
    static constexpr float MAX_TURNS_PER_SEC = _MAX_TURNS_PER_SEC;
    static constexpr float MAX_TURNS_PER_SEC_PER_SEC = _MAX_TURNS_PER_SEC_PER_SEC;

public:
    void setup() {
        pinModeFast(STEP_PIN, OUTPUT);
        pinModeFast(DIR_PIN, OUTPUT);
    }
    unsigned int stepsPerTurn() const { return _STEPS_PER_TURN; }
    float maxTurnsPerSec() const { return _MAX_TURNS_PER_SEC; }
    float maxTurnsPerSecPerSec() const { return _MAX_TURNS_PER_SEC_PER_SEC; }
    void stepHIGH() {
        digitalWriteFast(STEP_PIN, HIGH);
        stepPos += stepDir;
    }
    void stepLOW() { digitalWriteFast(STEP_PIN, LOW); }
    void dirForward() {
        digitalWriteFast(DIR_PIN, LOW);
        stepDir = 1;
    }
    void dirBackward() {
        digitalWriteFast(DIR_PIN, HIGH);
        stepDir = -1;
    }
};

#endif  // STEPPER_MOTOR_HPP

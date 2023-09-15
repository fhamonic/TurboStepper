#ifndef SERVO_MOTOR_HPP
#define SERVO_MOTOR_HPP

#include <digitalWriteFast.h>

template <uint8_t PWM_PIN, typename Counter, uint16_t _PWM_PERIOD_US = 6000,
          uint16_t _MIN_PULSE_US = 1000, uint16_t _MAX_PULSE_US = 2000>
class Servo {
public:
    static constexpr unsigned int PWM_PERIOD_TICKS =
        static_cast<float>(Counter::TICKS_PER_SEC) / 1000000 * _PWM_PERIOD_US;
    static constexpr unsigned int MIN_PULSE_TICKS =
        static_cast<float>(Counter::TICKS_PER_SEC) / 1000000 * _MIN_PULSE_US;
    static constexpr unsigned int MAX_PULSE_TICKS =
        static_cast<float>(Counter::TICKS_PER_SEC) / 1000000 * _MAX_PULSE_US;

    //static_assert(MIN_PULSE_TICKS == 0, "Timer resolution too low");

    static_assert(PWM_PERIOD_TICKS > Counter::MAX_VALUE, "PWM period too high");

    static unsigned int pulse_ticks;
    static bool _isHIGH;

public:
    static void Setup() { pinModeFast(PWM_PIN, OUTPUT); }
    static void PwmHIGH() { digitalWriteFast(PWM_PIN, HIGH); }
    static void PwmLOW() { digitalWriteFast(PWM_PIN, LOW); }

    template <typename T>
    static void moveTo(T angle) {
        if(angle)
            pulse_ticks = MIN_PULSE_TICKS;
        else
            pulse_ticks = MAX_PULSE_TICKS;
    }

    static void move() {
        if(_isHIGH) {
            PwmLOW();
            Counter::Increment(PWM_PERIOD_TICKS - pulse_ticks);
            _isHIGH = false;
        } else {
            PwmHIGH();
            Counter::Increment(pulse_ticks);
            _isHIGH = true;
        }
    }
};

template <uint8_t _PP, typename _C, uint16_t _PPU, uint16_t _MNPU,
          uint16_t _MXPU>
unsigned int Servo<_PP, _C, _PPU, _MNPU, _MXPU>::pulse_ticks =
    Servo<_PP, _C, _PPU, _MNPU, _MXPU>::MIN_PULSE_TICKS / 2 +
    Servo<_PP, _C, _PPU, _MNPU, _MXPU>::MAX_PULSE_TICKS / 2;

template <uint8_t _PP, typename _C, uint16_t _PPU, uint16_t _MNPU,
          uint16_t _MXPU>
bool Servo<_PP, _C, _PPU, _MNPU, _MXPU>::_isHIGH = false;

#endif  // SERVO_MOTOR_HPP

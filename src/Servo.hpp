#ifndef SERVO_HPP
#define SERVO_HPP

#include <digitalWriteFast.h>

template <uint8_t PWM_PIN, typename Counter, uint16_t _PWM_PERIOD_US = 5952,
          uint16_t _MIN_PULSE_US = 384, uint16_t _MAX_PULSE_US = 2304,
          uint16_t ANGLE_RANGE = 180>
class Servo {
public:
    static constexpr unsigned int PWM_PERIOD_TICKS =
        static_cast<float>(Counter::TICKS_PER_SEC) / 1000000 * _PWM_PERIOD_US;
    static constexpr unsigned int MIN_PULSE_TICKS =
        static_cast<float>(Counter::TICKS_PER_SEC) / 1000000 * _MIN_PULSE_US;
    static constexpr unsigned int MAX_PULSE_TICKS =
        static_cast<float>(Counter::TICKS_PER_SEC) / 1000000 * _MAX_PULSE_US;

    static_assert(MIN_PULSE_TICKS > 0, "Timer resolution too low");

    static_assert(PWM_PERIOD_TICKS <= Counter::MAX_VALUE,
                  "PWM period too high");

    static unsigned int pulse_ticks;
    static bool _isHIGH;

public:
    static void Setup() {
        pinModeFast(PWM_PIN, OUTPUT);
        pulse_ticks = MIN_PULSE_TICKS / 2 + MAX_PULSE_TICKS / 2;
        Counter::Enable();
    }
    static void PwmHIGH() { digitalWriteFast(PWM_PIN, HIGH); }
    static void PwmLOW() { digitalWriteFast(PWM_PIN, LOW); }

    static void MoveTo(float angle) {
        pulse_ticks = MIN_PULSE_TICKS + (MAX_PULSE_TICKS - MIN_PULSE_TICKS) *
                                            (angle / ANGLE_RANGE);
    }

    static void DoPulse() {
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
          uint16_t _MXPU, uint16_t _AR>
unsigned int Servo<_PP, _C, _PPU, _MNPU, _MXPU, _AR>::pulse_ticks = 0;

template <uint8_t _PP, typename _C, uint16_t _PPU, uint16_t _MNPU,
          uint16_t _MXPU, uint16_t _AR>
bool Servo<_PP, _C, _PPU, _MNPU, _MXPU, _AR>::_isHIGH = false;

#endif  // SERVO_HPP

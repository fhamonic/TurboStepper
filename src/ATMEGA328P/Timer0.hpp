#ifndef ATMEGA328P_TIMER0_HPP
#define ATMEGA328P_TIMER0_HPP

#include "../ClockFrequency.hpp"

namespace ATMEGA328P {

template <ClockFrequency Clock>
class Timer0 {
public:
    struct CounterA {
        static constexpr unsigned long int MAX_VALUE = 255;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void Enable() { TIMSK0 |= (1 << OCIE0A); }
        static void Disable() { TIMSK0 &= ~(1 << OCIE0A); }
        static void Set(uint16_t ticks) { OCR0A = TCNT0 + ticks; }
        template <typename T>
        static void Increment(T ticks) {
            OCR0A += ticks;
        }
    };

    struct CounterB {
        static constexpr unsigned long int MAX_VALUE = 255;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void Enable() { TIMSK0 |= (1 << OCIE0B); }
        static void Disable() { TIMSK0 &= ~(1 << OCIE0B); }
        static void Set(unsigned long int ticks) { OCR0B = TCNT0 + ticks; }
        template <typename T>
        static void Increment(T ticks) {
            OCR0B += ticks;
        }
    };

public:
    static void Setup();
};

template <ClockFrequency Clock>
void Timer0<Clock>::Setup() {
    static_assert(Clock == 0,
                  "Invalid Timer0 clock (accepts only: C16MHz, C2MHz, C250kHz, "
                  "C62_500Hz and C15_625Hz)");
}

template <>
void Timer0<C16MHz>::Setup() {
    cli();
    TCCR0A = 0;
    TCCR0B = (1 << CS00);
    sei();
}
template <>
void Timer0<C2MHz>::Setup() {
    cli();
    TCCR0A = 0;
    TCCR0B = (1 << CS01);
    sei();
}
template <>
void Timer0<C250kHz>::Setup() {
    cli();
    TCCR0A = 0;
    TCCR0B = (1 << CS01) | (1 << CS00);
    sei();
}
template <>
void Timer0<C62_500Hz>::Setup() {
    cli();
    TCCR0A = 0;
    TCCR0B = (1 << CS02);
    sei();
}
template <>
void Timer0<C15_625Hz>::Setup() {
    cli();
    TCCR0A = 0;
    TCCR0B = (1 << CS02) | (1 << CS00);
    sei();
}

}  // namespace ATMEGA328P

#endif  // ATMEGA328P_TIMER0_HPP
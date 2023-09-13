#ifndef ATMEGA328P_TIMER1_HPP
#define ATMEGA328P_TIMER1_HPP

#include "../ClockFrequency.hpp"

namespace ATMEGA328P {

template <ClockFrequency Clock>
class Timer1 {
public:
    struct CounterA {
        static constexpr unsigned long int MAX_VALUE = 65535;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void Enable() { TIMSK1 |= (1 << OCIE1A); }
        static void Disable() { TIMSK1 &= ~(1 << OCIE1A); }
        static void Set(uint16_t ticks) { OCR1A = TCNT1 + ticks; }
        template <typename T>
        static void Increment(T ticks) {
            OCR1A += ticks;
        }
    };

    struct CounterB {
        static constexpr unsigned long int MAX_VALUE = 65535;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void Enable() { TIMSK1 |= (1 << OCIE1B); }
        static void Disable() { TIMSK1 &= ~(1 << OCIE1B); }
        static void Set(unsigned long int ticks) { OCR1B = TCNT1 + ticks; }
        template <typename T>
        static void Increment(T ticks) {
            OCR1B += ticks;
        }
    };

public:
    static void Setup();
};

template <ClockFrequency Clock>
void Timer1<Clock>::Setup() {
    static_assert(Clock == 0,
                  "Invalid Timer1 clock (accepts only: C16MHz, C2MHz, C250kHz, "
                  "C62_500Hz and C15_625Hz)");
}

template <>
void Timer1<C16MHz>::Setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS10);
    sei();
}
template <>
void Timer1<C2MHz>::Setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS11);
    sei();
}
template <>
void Timer1<C250kHz>::Setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS11) | (1 << CS10);
    sei();
}
template <>
void Timer1<C62_500Hz>::Setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS12);
    sei();
}
template <>
void Timer1<C15_625Hz>::Setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS12) | (1 << CS10);
    sei();
}

}  // namespace ATMEGA328P

#endif  // ATMEGA328P_TIMER1_HPP
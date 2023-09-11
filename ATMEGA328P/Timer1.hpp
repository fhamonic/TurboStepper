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
        static void enable() { TIMSK1 |= (1 << OCIE1A); }
        static void disable() { TIMSK1 &= ~(1 << OCIE1A); }
        static void set(uint16_t ticks) { OCR1A = TCNT1 + ticks; }
        template <typename T>
        static void increment(T ticks) {
            OCR1A += ticks;
        }
    };

    struct CounterB {
        static constexpr unsigned long int MAX_VALUE = 65535;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void enable() { TIMSK1 |= (1 << OCIE1B); }
        static void disable() { TIMSK1 &= ~(1 << OCIE1B); }
        static void set(unsigned long int ticks) { OCR1B = TCNT1 + ticks; }
        template <typename T>
        static void increment(T ticks) {
            OCR1B += ticks;
        }
    };

public:
    void setup();

    unsigned long ticksPerSec() const { return Clock; }
};

template <ClockFrequency Clock>
void Timer1<Clock>::setup() {
    static_assert(Clock == 0,
                  "Invalid Timer1 clock (accepts only: C16MHz, C2MHz, C250kHz, "
                  "C62_500Hz and C15_625Hz)");
}

template <>
void Timer1<C16MHz>::setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS10);
    sei();
}
template <>
void Timer1<C2MHz>::setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS11);
    sei();
}
template <>
void Timer1<C250kHz>::setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS11) | (1 << CS10);
    sei();
}
template <>
void Timer1<C62_500Hz>::setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS12);
    sei();
}
template <>
void Timer1<C15_625Hz>::setup() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS12) | (1 << CS10);
    sei();
}

}  // namespace ATMEGA328P

#endif  // ATMEGA328P_TIMER1_HPP
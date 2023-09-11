#ifndef ATMEGA328P_TIMER2_HPP
#define ATMEGA328P_TIMER2_HPP

#include "../ClockFrequency.hpp"

namespace ATMEGA328P {

template <ClockFrequency Clock>
class Timer2 {
public:
    struct CounterA {
        static constexpr unsigned long int MAX_VALUE = 255;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void enable() { TIMSK2 |= (1 << OCIE2A); }
        static void disable() { TIMSK2 &= ~(1 << OCIE2A); }
        static void set(uint16_t ticks) { OCR2A = TCNT2 + ticks; }
        template <typename T>
        static void increment(T ticks) {
            OCR2A += ticks;
        }
    };
    struct CounterB {
        static constexpr unsigned long int MAX_VALUE = 255;
        static constexpr unsigned long int TICKS_PER_SEC = Clock;
        static void enable() { TIMSK2 |= (1 << OCIE2B); }
        static void disable() { TIMSK2 &= ~(1 << OCIE2B); }
        static void set(uint16_t ticks) { OCR2B = TCNT2 + ticks; }
        template <typename T>
        static void increment(T ticks) {
            OCR2B += ticks;
        }
    };

public:
    void setup();

    unsigned long ticksPerSec() const { return Clock; }
};

template <ClockFrequency Clock>
void Timer2<Clock>::setup() {
    static_assert(Clock == 0,
                  "Invalid Timer2 clock (accepts only: C16MHz, C2MHz, C500kHz, "
                  "C250kHz, C125kHz, C62_500Hz and C15_625Hz)");
}

template <>
void Timer2<C16MHz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS20);
    sei();
}
template <>
void Timer2<C2MHz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS21);
    sei();
}
template <>
void Timer2<C500kHz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS21) | (1 << CS20);
    sei();
}
template <>
void Timer2<C250kHz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22);
    sei();
}
template <>
void Timer2<C125kHz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS20);
    sei();
}
template <>
void Timer2<C62_500Hz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS21);
    sei();
}
template <>
void Timer2<C15_625Hz>::setup() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    sei();
}

}  // namespace ATMEGA328P

#endif  // ATMEGA328P_TIMER2_HPP
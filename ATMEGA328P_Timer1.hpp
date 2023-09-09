#ifndef ATMEGA328P_TIMER1_HPP
#define ATMEGA328P_TIMER1_HPP

#include "TimerFreq.hpp"

template <TimerFreq Clock>
class ATMEGA328P_Timer1 {
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
        static void set(uint16_t ticks) { OCR1B = TCNT1 + ticks; }
        template <typename T>
        static void increment(T ticks) {
            OCR1B += ticks;
        }
    };

private:
    template <TimerFreq C>
    void _setup() {
        static_assert(!C, "Invalid Timer1 clock");
    }

public:
    void setup() { _setup<Clock>(); }

    unsigned long ticksPerSec() const { return Clock; }
};

template <>
template <>
void ATMEGA328P_Timer1<C16MHz>::_setup<C16MHz>() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS10);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer1<C2MHz>::_setup<C2MHz>() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS11);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer1<C250kHz>::_setup<C250kHz>() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS11) | (1 << CS10);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer1<C62_500Hz>::_setup<C62_500Hz>() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS12);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer1<C15_625Hz>::_setup<C15_625Hz>() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << CS12) | (1 << CS10);
    sei();
}

#endif  // ATMEGA328P_TIMER1_HPP
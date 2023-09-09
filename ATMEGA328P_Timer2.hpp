#ifndef ATMEGA328P_TIMER2_HPP
#define ATMEGA328P_TIMER2_HPP

#include "TimerFreq.hpp"

template <TimerFreq Clock>
class ATMEGA328P_Timer2 {
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

private:
    template <TimerFreq C>
    void _setup() {
        static_assert(!C, "Invalid Timer2 clock");
    }

public:
    void setup() { _setup<Clock>(); }

    unsigned long ticksPerSec() const { return Clock; }
};

template <>
template <>
void ATMEGA328P_Timer2<C16MHz>::_setup<C16MHz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS20);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer2<C2MHz>::_setup<C2MHz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS21);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer2<C500kHz>::_setup<C500kHz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS21) | (1 << CS20);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer2<C250kHz>::_setup<C250kHz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer2<C125kHz>::_setup<C125kHz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS20);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer2<C62_500Hz>::_setup<C62_500Hz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS21);
    sei();
}
template <>
template <>
void ATMEGA328P_Timer2<C15_625Hz>::_setup<C15_625Hz>() {
    cli();
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    sei();
}

#endif  // ATMEGA328P_TIMER2_HPP
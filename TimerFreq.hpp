#ifndef TIMER_FREQ_HPP
#define TIMER_FREQ_HPP

enum TimerFreq : long unsigned int {
    C16MHz = 6000000UL,
    C2MHz = 2000000UL,
    C500kHz = 500000UL,
    C250kHz = 250000UL,
    C125kHz = 125000UL,
    C62_500Hz = 62500UL,
    C15_625Hz = 15625UL,
};

#endif  // TIMER_FREQ_HPP
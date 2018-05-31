#pragma once

#ifndef TIMER_HPP
#define TIMER_HPP

#ifdef PROFILE
#include <chrono>

class Timer
{
public:
    Timer(const char* name);
    ~Timer();

private:
    using Clock = std::chronor::high_resolution_clock;

    const char* _name;
    Clock::time_point _start;
};

#else

class Timer
{
public:
    Timer(const char* name);
};

#endif // PROFILE

#endif //TIMER_HPP

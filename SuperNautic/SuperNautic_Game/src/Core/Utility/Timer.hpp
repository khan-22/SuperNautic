#pragma once

#ifndef PROF_TIMER_HPP
#define PROF_TIMER_HPP

#ifdef PROFILE
#include <chrono>

namespace prof
{

    class Timer
    {
    public:
        Timer(const char* name);
        ~Timer();

    private:
        using Clock = std::chrono::high_resolution_clock;
        const char* const _name;
    };

}

#else

namespace prof
{

class Timer
{
public:
    Timer(const char* name);
};

}

#endif // PROFILE

#endif //PROF_TIMER_HPP

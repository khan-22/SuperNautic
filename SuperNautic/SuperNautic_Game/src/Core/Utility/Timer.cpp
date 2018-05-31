#include "Core/Utility/Timer.hpp"
#include "Core/Utility/TimeSummary.hpp"

#ifdef PROFILE

Timer::Timer(const char* name)
: _name(name)
, _start(Clock::now())
{

}


Timer::~Timer()
{
    TimeSummary::add(
        _name,
        duration_cast<duration<double>>(_start - Clock::now());
}

#else

Timer::Timer(const char* name)
{

}

#endif // PROFILE
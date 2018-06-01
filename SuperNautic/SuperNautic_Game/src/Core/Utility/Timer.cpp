#include "Core/Utility/Timer.hpp"
#include "Core/Utility/TimeSummary.hpp"

namespace prof
{



#ifdef PROFILE

Timer::Timer(const char* name)
: _name(name)
{
  TimeSummary::startEntry(_name, Clock::now());
}



Timer::~Timer()
{
  TimeSummary::stopEntry(_name, Clock::now());
}

#else

Timer::Timer(const char* name)
{

}

#endif // PROFILE

}

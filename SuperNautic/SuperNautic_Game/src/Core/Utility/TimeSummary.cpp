#include "Core/Utility/TimeSummary.hpp"

std::vector<TimeSummary::Entry> TimeSummary::_entries;


void TimeSummary::add(const char* name, double time)
{
    _entries.emplace_back({name, time});
}

std::string TimeSummary::summarize()
{
    return "";
}
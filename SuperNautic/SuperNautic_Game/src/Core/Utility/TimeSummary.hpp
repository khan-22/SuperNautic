#pragma once

#ifndef TIME_SUMMARY_HPP
#define TIME_SUMMARY_HPP

#include <vector>
#include <string>

class TimeSummary
{
public:
    static void add(const char* name, double time);

    static std::string summarize();

private:
    struct Entry
    {
        const char* name;
        double time;
    };

    static std::vector<Entry> _entries;
};

#endif //TIME_SUMMARY_HPP

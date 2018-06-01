#pragma once

#ifndef TIME_SUMMARY_HPP
#define TIME_SUMMARY_HPP

#include <vector>
#include <string>
#include <chrono>
#include <map>

namespace prof
{

    class TimeSummary
    {
    public:
        using TimePoint = std::chrono::high_resolution_clock::time_point;

        static void startEntry(const char* name, const TimePoint& startTime);
        static void stopEntry(const char* name, const TimePoint& endTime);

        static std::string summarize();

    private:
        struct Node
        {
          const char* name = nullptr;
          double time = 0.d;

          std::vector<Node> children;
        };

        struct AccumulatedEntry
        {
          const char* name = nullptr;
          double inclusiveTime = 0.d;
          double exclusiveTime = 0.d;
          size_t count = 0;
        };

        struct Entry
        {
            enum class Type : unsigned char
            {
              START,
              STOP
            };
            Type type;
            const char* name;
            TimePoint time;
        };

        static Node createNode(const Entry*& cursor, const Entry* end);
        static void getNodes(const Node& node, std::map<const char*, std::vector<const Node*>>& nodes);

        static std::vector<Entry> _entries;
    };

}

#endif //TIME_SUMMARY_HPP

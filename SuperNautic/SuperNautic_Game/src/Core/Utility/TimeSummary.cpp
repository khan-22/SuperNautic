#include "Core/Utility/TimeSummary.hpp"

#include <map>
#include <algorithm>
#include <sstream>
#include <cassert>

namespace prof
{


std::vector<TimeSummary::Entry> TimeSummary::_entries;



void TimeSummary::startEntry(const char* name, const TimePoint& startTime)
{
    _entries.emplace_back(Entry({Entry::Type::START, name, startTime}));
}

void TimeSummary::stopEntry(const char* name, const TimePoint& endTime)
{
    _entries.emplace_back(Entry({Entry::Type::STOP, name, endTime}));
}

TimeSummary::Node TimeSummary::createNode(const Entry*& cursor, const Entry* end)
{
  assert(cursor < end - 1);
  assert(cursor && end);
  assert(cursor->type == Entry::Type::START);

  Node node;
  node.name = cursor->name;
  using namespace std::chrono;
  const TimePoint& start = cursor->time;

  cursor++;
  while(cursor < end)
  {
    if (cursor->type == Entry::Type::START)
    {
      node.children.emplace_back(createNode(cursor, end));
    }
    else if(cursor->type == Entry::Type::STOP)
    {
      node.time = duration_cast<duration<double>>(cursor->time - start).count();
      cursor++;
      return node;
    }
    else
    {
      assert(false);
    }
  }

  assert(false);
  return node;
}

void TimeSummary::getNodes(const Node& node, std::map<const char*, std::vector<const Node*>>& nodes)
{
  nodes[node.name].push_back(&node);
  for(const Node& child : node.children)
  {
    getNodes(child, nodes);
  }
}


std::string TimeSummary::summarize()
{


    const Entry* begin = _entries.data();
    const Entry* end = begin + _entries.size();
    Node root;
    root.name = "TIME_SUMMARY_ROOT";
    while(begin < end)
    {
      root.children.emplace_back(createNode(begin, end));
    }

    std::map<const char*, std::vector<const Node*>> nodes;
    getNodes(root, nodes);

    std::vector<AccumulatedEntry> accumulatedEntries;
    for(const std::pair<const char*, std::vector<const Node*>>& node : nodes)
    {
      AccumulatedEntry entry;
      entry.name = node.first;
      for(const Node* n : node.second)
      {
        entry.inclusiveTime += n->time;
        entry.exclusiveTime += n->time;
        for(const Node& child : n->children)
        {
          if(child.name == entry.name)
          {
            entry.inclusiveTime -= child.time;
          }
          entry.exclusiveTime -= child.time;
        }
        entry.count++;
      }
      accumulatedEntries.emplace_back(entry);
    }

    std::sort(accumulatedEntries.begin(),
              accumulatedEntries.end(),
              [](const AccumulatedEntry& lhs, const AccumulatedEntry& rhs)
              {
                return lhs.inclusiveTime > rhs.inclusiveTime;
              });

    std::stringstream summary;
    summary << "SUMMARY" << std::endl;;
    for(const AccumulatedEntry& entry : accumulatedEntries)
    {
      if (entry.name != root.name)
      {
        summary << entry.name << "\t" << entry.inclusiveTime << "s"
                << "\t" << entry.count << " times"
                << "\t" << entry.inclusiveTime / double(entry.count) << "s/time" << std::endl;
      }
    }


    summary << std::endl << std::endl;

    return summary.str();
}




}

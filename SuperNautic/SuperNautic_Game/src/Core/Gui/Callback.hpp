#pragma once

#include <functional>

struct Callback
{
    std::function<void()> callback;
};
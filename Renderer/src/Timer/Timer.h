#pragma once

#include <chrono>

class Timer
{
public:
    Timer()
    {
        reset();
    }

    void reset()
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    long currentTime() const
    {
        return (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    long elapsed() const
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        return (long)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
};
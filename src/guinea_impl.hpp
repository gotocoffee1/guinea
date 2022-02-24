#pragma once

#ifdef BUILD_GUINEA_BACKEND_STATIC
#define EXPORT
#else
#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif
#endif

#include "guinea_base.hpp"

#include <chrono>
#include <thread>

template<typename F>
void main_loop(int fps, F&& func) noexcept
{
    if (fps > 0)
    {
        using namespace std::chrono;
        using Framerate = duration<double, std::ratio<1, 1>>;
        auto step       = Framerate{1} / fps;
        auto next       = steady_clock::now() + step;
        // Main loop
        while (!func())
        {
            std::this_thread::sleep_until(next);
            next += step;
        }
    }
    else
    {
        while (!func())
        {
        }
    }
}

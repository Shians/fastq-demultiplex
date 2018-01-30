#pragma once
#include <string>
#include <chrono>

class Timer {
    public:
        void start();
        std::string elapsed();
        void mode(std::string mode); // mode one of "seconds", "minutes", "hours"
    private:
        std::chrono::time_point<std::chrono::steady_clock> _start_time;
        std::chrono::time_point<std::chrono::steady_clock> _end_time;
        std::string _time_mode = "seconds";
};

#include "workTimeLib.hpp"

void workTime::start()
{
    startPoint = std::chrono::high_resolution_clock::now();
}

void workTime::stop()
{
    endPoint = std::chrono::high_resolution_clock::now();
    duration = endPoint - startPoint;
}

void workTime::restart()
{
    start();
}

std::string workTime::getStringTime()
{
    {
        std::string time = std::to_string(duration.count());
        return time;
    }
}

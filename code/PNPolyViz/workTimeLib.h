// version 1.1.2
#pragma once
#ifndef __WORK_TIME_LIB_H
#define __WORK_TIME_LIB_H

#include <chrono>
#include <iostream>
#include <string>

//  Класс для подсчета времени выполнения программы.
class workTime
{
protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint, endPoint;
    std::chrono::duration<float> duration;

public:
    workTime() = default;

    ~workTime() = default;

    //  Запуск таймера.
    void start();

    //  Остановка таймера.
    void stop();

    //  Перезапуск таймера.
    void restart();

    //  Перегрузка оператора вывода.
    friend std::ostream& operator<<(std::ostream& os, const workTime& wt)
    {
        os << wt.duration.count() << "s";
        return os;
    }

    std::string getStringTime();
};

#endif //__WORK_TIME_LIB_H

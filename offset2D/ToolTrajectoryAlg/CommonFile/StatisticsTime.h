/**
  @author: lvdong
  @date: 2023-05
**/
#ifndef STATISTICSIME_H
#define STATISTICSIME_H
#include <chrono>
namespace StatisticsTime {
class Timer
{
public:
public:
    typedef std::chrono::steady_clock::time_point TimeNode;
    typedef std::chrono::nanoseconds NanoSec;
    Timer() {}
    void SetStartTime(){
        _sTime = std::chrono::high_resolution_clock::now();
    }
    void SetEndTime(){
        std::chrono::high_resolution_clock::now();
    }
    std::chrono::duration<long double> IntervalTime(){
        return _eTime - _sTime;
    }
private:
    TimeNode _sTime = 0;
    TimeNode _eTime = 0;
    std::chrono::duration<long double>_intervalTime = 0;
};
}
#endif // STATISTICSIME_H

#ifndef SIMPLE_STOPWATCH_H
#define SIMPLE_STOPWATCH_H

#include <chrono>

class Timer {
public:
    double viewSlider = 0.0;

    Timer(double seconds);

    void update();

private:
    std::chrono::duration<std::chrono::nanoseconds> period;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point now;
    std::chrono::high_resolution_clock::time_point lastUpdateTime;
    double lastSliderValue = 0.0;

    void adjustTimePoint();
    void updateViewSlider();

};

#endif // SIMPLE_STOPWATCH_H

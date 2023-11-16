#include "timer.hpp"

Timer::Timer(double seconds) {
    // Convert seconds to nanoseconds
    period = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::duration<double>(seconds)
    );

    startTime = std::chrono::high_resolution_clock::now();
    now = startTime;
}

void Timer::update() {

    now = std::chrono::high_resolution_clock::now();

    if (viewSlider != lastSliderValue) {
        adjustTimePoint();
        lastSliderValue = viewSlider;
    } else {
        // Update the viewSlider based on the elapsed time
        updateViewSlider();
        lastSliderValue = viewSlider;
    }

}

void Timer::adjustTimePoint() {
    auto now = std::chrono::high_resolution_clock::now();
    auto totalElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count();



    auto adjustedElapsed = static_cast<long long>(totalElapsed * viewSlider);
    startTime = now - std::chrono::nanoseconds(adjustedElapsed);
}

void Timer::updateViewSlider() {
    auto now = std::chrono::high_resolution_clock::now();
    auto totalElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count();
    auto currentElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(lastUpdateTime - startTime).count();

    if (totalElapsed > 0) {
        viewSlider = static_cast<double>(currentElapsed) / static_cast<double>(totalElapsed);
    }
}

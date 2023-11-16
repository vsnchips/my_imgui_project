#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <timer.hpp>
#include <imgui.h>

class TimerWidget {
public:
    explicit TimerWidget(Timer& timer);

    void render();

private:
    Timer& timer;
};

#endif // TIMERWIDGET_H

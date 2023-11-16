#include "timerWidget.hpp"

TimerWidget::TimerWidget(Timer& timer) : timer(timer) {}

void TimerWidget::render() {
    if (ImGui::SliderFloat("Timeline", &timer.currentTime, 0.0f, 100.0f)) {
        // Handle scrubbing logic here (if any)
    }

    if (ImGui::Button("Play")) {
        timer.play();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause")) {
        timer.pause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        timer.reset();
    }

    // You can add more ImGui elements here as needed
}

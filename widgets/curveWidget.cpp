#include <curveWidget.hpp>

CurveWidget::CurveWidget() {
    // Initialize keyframes or other necessary data here
    keyframes = {{100, 100}, {150, 150}, {200, 100}};
}

void CurveWidget::DrawKeyframe(ImDrawList* draw_list, ImVec2 pos) {
    draw_list->AddCircleFilled(pos, 5.0f, IM_COL32(255, 255, 255, 255));
}

void CurveWidget::Draw() {
    ImGui::Begin("Curve Editor");

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    ImVec2 p1 = ImVec2(p0.x + 400, p0.y + 200);

    // Draw a simple line as the curve (replace with your curve logic)
    draw_list->AddLine(p0, ImVec2(p1.x, p0.y), IM_COL32(255, 0, 0, 255));

    for (const auto& keyframe : keyframes) {
        DrawKeyframe(draw_list, keyframe);
    }

    // Detecting double clicks (for adding/removing keyframes, to be implemented)

    ImGui::End();
}

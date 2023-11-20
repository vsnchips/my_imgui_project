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
// draw_list->AddLine(p0, ImVec2(p1.x, p0.y), IM_COL32(255, 0, 0, 255));

    // Get the current window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = p1; ImGui::GetWindowSize();

    // Adjust line and keyframe positions based on the window
    ImVec2 lineStart, lineEnd;
    lineStart = ImVec2(0,0);
    lineEnd = ImVec2(200,100);

    ImVec2 adjustedLineStart = ImVec2(windowPos.x+lineStart.x, windowPos.y + lineStart.y);
    ImVec2 adjustedLineEnd = ImVec2(windowPos.x+lineEnd.x, windowPos.y + lineEnd.y);

    draw_list->AddLine(adjustedLineStart, adjustedLineEnd, IM_COL32(255, windowSize.x, windowSize.y, 255));

    for (const auto& keyframe : keyframes) {
        ImVec2 adjustedPos = ImVec2(windowPos.x + keyframe.x, windowPos.y + keyframe.y);
        DrawKeyframe(draw_list, adjustedPos);
    }
    // Detecting double clicks (for adding/removing keyframes, to be implemented)

    ImGui::End();
}

#ifndef _CURVE_WIDGET_HPP
#define _CURVE_WIDGET_HPP

#include "imgui.h"
#include <vector>

class CurveWidget {
public:
    CurveWidget();
    void Draw();

private:
    void DrawKeyframe(ImDrawList* draw_list, ImVec2 pos);
    std::vector<ImVec2> keyframes;
};

#endif // _CURVE_WIDGET_HPP

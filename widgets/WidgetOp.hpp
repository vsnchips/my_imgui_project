#ifndef _WIDGET_OP_HPP
#define _WIDGET_OP_HPP

#include <vector>

class WidgetOp {
public:
    virtual ~WidgetOp() {} // Virtual destructor
    virtual void doGui() = 0; // Pure virtual function

    static void registerWidget(WidgetOp* widget) {
        registeredWidgets.push_back(widget);
    }
    static void renderWidgets() {
        for (auto& widget : registeredWidgets) {
            widget->doGui(); // Call doGui on each registered widget
        }
    }

private:
    static std::vector<WidgetOp*> registeredWidgets;
};


#endif
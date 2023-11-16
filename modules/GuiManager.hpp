#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <imgui.h>
#include "inputhandler.hpp"

class GuiManager {
public:
    GuiManager(InputHandler& inputHandler);

    static void render(); // Render all GUI elements

private:
    InputHandler& inputHandler;

    static void renderPopupMenu(); // Render the popup menu
    static void renderPersistentWidgets(); // Render other persistent widgets

    // Add methods for rendering other specific widgets or GUI elements
};

#endif // GUIMANAGER_H

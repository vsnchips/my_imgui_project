#include "GuiManager.hpp"
#include "inputhandler.hpp"

void GuiManager::render() {
    if (InputHandler::shouldShowMenu()) {
        renderPopupMenu();
    }

    renderPersistentWidgets();

    // Add calls to render other GUI elements or widgets here...
}

void GuiManager::renderPopupMenu() {
    // Assuming you're using ImGui's BeginPopup/EndPopup for the popup menu
    if (!ImGui::IsPopupOpen("Popup Menu") && InputHandler::shouldShowMenu()) {
        ImGui::OpenPopup("Popup Menu");
    }

    if (ImGui::BeginPopup("Popup Menu")) {
        // Add content to the popup menu here
        // For example: ImGui::Text("Popup Menu Content");
        // You can also check for specific keypresses if needed

        ImGui::EndPopup();
    }
}

void GuiManager::renderPersistentWidgets() {
    // Example: Rendering a persistent widget with ImGui
    ImGui::Begin("Persistent Widget"); // Begin window
    // Add content to the persistent widget here
    // For example: ImGui::Text("This is a persistent widget");
    ImGui::End(); // End window
}
#include <GLFW/glfw3.h> // Make sure to include GLFW header

#include "inputhandler.hpp"

void InputHandler::update() {
    // Check for Tab key toggle
    if (isKeyPressed(GLFW_KEY_TAB)) {
        showMenu = !showMenu;
    }

    // Reset key states if necessary
}

void InputHandler::keyPressed(int key) {
    keyStates[key] = true;
}

void InputHandler::keyReleased(int key) {
    keyStates[key] = false;
}

bool InputHandler::isKeyPressed(int key) {
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        return it->second;
    }
    return false;
}

bool InputHandler::shouldShowMenu() {
    return showMenu;
}

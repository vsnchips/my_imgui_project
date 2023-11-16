#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <map>

class InputHandler {
public:
    static void update(); // Called every frame to update input states
    static void keyPressed(int key);
    static void keyReleased(int key);
    static bool isKeyPressed(int key);
    static bool shouldShowMenu();

private:
    static std::map<int, bool> keyStates;
    static bool showMenu;
};

#endif // INPUTHANDLER_H
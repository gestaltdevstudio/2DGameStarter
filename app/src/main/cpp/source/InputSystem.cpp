#include "../include/InputSystem.h"

namespace GGE
{
    InputSystem* InputSystem::instance = 0;

    InputSystem::InputSystem()
    {
        memset( &keyBuffer, 0, 256 );

        std::fill(keyBuffer, keyBuffer+sizeof(keyBuffer)/sizeof(bool), false);
    }

    InputSystem::~InputSystem()
    {
    }

    void InputSystem::destroy()
    {
	    delete instance;
        instance = NULL;
    }

    bool InputSystem::isKeyDown(KeyCode key)
    {
        return keyBuffer[key];
    }

    void InputSystem::keyPressed(KeyCode key)
    {
        keyBuffer[key] = true;
    }


    void InputSystem::keyReleased(KeyCode key)
    {
        keyBuffer[key] = false;
    }
}

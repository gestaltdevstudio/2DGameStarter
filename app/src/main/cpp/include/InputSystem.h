#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

#include "Definitions.h"
#include <cstring>

namespace GGE {

    class InputSystem
	{
        public:
            static InputSystem *getInstance()
            {
               if (!InputSystem::instance) InputSystem::instance = new InputSystem();
               return InputSystem::instance;
            }

          InputSystem();
            virtual ~InputSystem();
            bool isKeyDown(KeyCode key);
            void keyPressed(KeyCode key);
            void keyReleased(KeyCode key);
            void destroy();

        private:
            static InputSystem *instance;
            bool            keyBuffer[256];

    };

}

#endif // INPUTSYSTEM_H_INCLUDED

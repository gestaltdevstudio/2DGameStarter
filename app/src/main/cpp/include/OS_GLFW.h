#ifndef OS_GLFW_H_INCLUDED
#define OS_GLFW_H_INCLUDED

#include <vector>

#if defined (__WIN32__)
#include <windows.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "stb_image.h"
#include "Definitions.h"
#include "Graphics.h"

namespace GGE {
    class Controller;
    struct resourceFile;

    typedef struct GLFWgamepadstate JoystickState;

    class OS
	{
        public:
            static OS *getInstance()
            {
               if (!OS::instance) OS::instance = new OS();
               return OS::instance;
            }
            void destroy();
            void createWindow(std::string windowName, int _width, int _height, bool _fullScreen);
            void checkInputEvent();
            void procEvent();
            void swapBuffer();
            void resizeWindow();
            bool getRunning();
            bool isFocused();
            const Point getMouseCoord();
            float getTime();
            inline void setMouseCoord(Point p) { mouseCoord = p; }
            int alert( const char *lpCaption, const char *lpText );
            std::map<int,KeyCode>  keyConversion;
            bool getJoystickState(int joy, JoystickState* joystickState);
            bool checkJoystickPause();
            void addJoystick(int joy);
            void removeJoystick(int joy);
            inline int getJoystickCount() {return joystickCount;};
            inline bool isJoystickPresent(int joy) { return glfwJoystickPresent(joy) != GLFW_FALSE;};
            inline void setRunning(bool _yn) {running = _yn;}
            std::string getJoystickName(int joy);
            JoystickMap getJoystickMap(int joy);
            void setCursorPosition(int x, int y);
            void toggleFullScreen();
            inline bool isFullScreen() {return fullScreen; };

            Point getWindowSize();
            void updateMouseByJoystick(float delta);
            Point calculateMousePoint(Point point);

        private:
            static OS *instance;
            OS();
            Point mouseCoord;
            Point windowedPosition, windowedSize;
            float	aspectRatio;
            GLFWwindow          *window;
            bool            running;
            bool            paused;
            bool            fullScreen;
            void findJoysticks();
			GLFWmonitor* getCurrentMonitor(GLFWwindow *window);
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void frameBufferSizeCallbak(GLFWwindow* window, int width, int height);
            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void joystick_callback(int joy, int event);
            static void error_callback(int error, const char* description);
            int joysticks[NUMBER_OF_JOYSTICKS];
            int joystickCount;
            JoystickMap joystickMaps[NUMBER_OF_JOYSTICKS];
            static const std::string defaultJoysticks;
			int restoreWidth, restoreHeight;

    };

}

#endif // OS_GLFW_H_INCLUDED

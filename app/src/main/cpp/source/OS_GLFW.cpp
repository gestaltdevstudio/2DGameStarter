#include "../include/OS_GLFW.h"
#if defined(__WIN32__)
#include <algorithm>
#endif

#ifndef None
#define None                 0L
#endif

namespace GGE
{

    OS* OS::instance = NULL;

	OS::OS()
	{
        glfwSetErrorCallback(error_callback);
        if( !glfwInit() )
        {
            alert( "Error", "Failed to initialize window manager!" );
        }

        keyConversion.insert(std::make_pair(GLFW_KEY_UP, GGE_UP));
        keyConversion.insert(std::make_pair(GLFW_KEY_DOWN, GGE_DOWN));
        keyConversion.insert(std::make_pair(GLFW_KEY_LEFT, GGE_LEFT));
        keyConversion.insert(std::make_pair(GLFW_KEY_RIGHT, GGE_RIGHT));
        keyConversion.insert(std::make_pair(GLFW_KEY_SPACE, GGE_SPACE));


        keyConversion.insert(std::make_pair(GLFW_KEY_W, GGE_W));
        keyConversion.insert(std::make_pair(GLFW_KEY_A, GGE_A));
        keyConversion.insert(std::make_pair(GLFW_KEY_S, GGE_S));
        keyConversion.insert(std::make_pair(GLFW_KEY_D, GGE_D));
        keyConversion.insert(std::make_pair(GLFW_KEY_Q, GGE_Q));
        keyConversion.insert(std::make_pair(GLFW_KEY_P, GGE_P));

        keyConversion.insert(std::make_pair(GLFW_KEY_ESCAPE, GGE_ESCAPE));

        mouseCoord.x = mouseCoord.y = -SCREEN_X - 10;

        joystickCount = 0;

	}

    void OS::destroy()
	{

        glfwDestroyWindow(window);
        glfwTerminate();
	    delete instance;
        instance = NULL;
	}

	void OS::createWindow(std::string windowName, int _width, int _height, bool _fullScreen)
	{
		restoreWidth = _width;
		restoreHeight = _height;
	    fullScreen = _fullScreen;
		windowedSize.x = 0;

	    glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#if defined(__WIN32__)
		window = glfwCreateWindow(mode->width, mode->height, windowName.c_str(), monitor, NULL);
#else
		window = glfwCreateWindow(SCREEN_X, SCREEN_Y, windowName.c_str(), NULL, NULL);
#endif
        if( window == NULL ){
            alert( "Error", "Failed to create Window!.\n" );
            glfwTerminate();
        } else {


            glfwSetJoystickCallback(joystick_callback);
			glfwSetWindowAspectRatio(window, 4, 3);

            glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetFramebufferSizeCallback(window, frameBufferSizeCallbak);
            glfwSetKeyCallback(window, key_callback);

            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                alert("Cannot initialize GLAD: ", "");
            }

            glfwSwapInterval(1);

#if !defined(__WIN32__)
            fullScreen = false;
            toggleFullScreen();
#endif
			resizeWindow();

			glfwSetTime(0.0);
            running = true;
            paused = false;
            findJoysticks();
        }
	}

    float OS::getTime()
    {
        return (float) glfwGetTime();
    }

    void OS::swapBuffer()
    {

		glfwSwapBuffers(window);
		glfwPollEvents();
        fflush(stdout);
    }

    void OS::checkInputEvent()
    {
        int button_count;

		for (int joy = GLFW_JOYSTICK_1; joy <= NUMBER_OF_JOYSTICKS; joy++)
		{
			if (glfwJoystickPresent(joy))
            {
                glfwGetJoystickButtons(joy, &button_count);
            }
		}

        procEvent();
    }

    const Point OS::getMouseCoord()
    {
        const Point _return = mouseCoord;
        mouseCoord.x = mouseCoord.y = -SCREEN_X - 10;
        return _return;
    }

    std::string OS::getJoystickName(int joy)
    {
        return to_string(glfwGetJoystickName(joy));
    }

    bool OS::getJoystickState(int joy, JoystickState* joystickState)
    {

        if (!glfwJoystickPresent(joy))
        {
            return false;
        }

        glfwGetGamepadState(joy, (GLFWgamepadstate*)joystickState);
        return true;

    }

    void OS::procEvent()
    {
        if (glfwWindowShouldClose(window))
        {
            running = false;
        }
    }

    Point OS::getWindowSize()
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        Point windowSize;
        windowSize.x = width;
        windowSize.y = height;
        return windowSize;
    }

    void OS::resizeWindow()
    {
        Point windowSize = getWindowSize();
        Graphics::getInstance()->renderResize(windowSize);
    }

    bool OS::getRunning()
    {
        return running;
    }

    bool OS::isFocused()
    {
        return glfwGetWindowAttrib(window, GLFW_FOCUSED);
    }

    int OS::alert( const char *lpCaption, const char *lpText )
    {
#if defined (__WIN32__)
        return ::MessageBox( NULL, lpText, lpCaption, MB_OK );
#else
        fprintf( stderr, "Message: '%s', Detail: '%s'\n", lpCaption, lpText );
        return 0;
#endif
    }

    void OS::findJoysticks()
    {
        JoystickMap j = JoystickMap();
                    j.jump = GLFW_GAMEPAD_BUTTON_A;
                    j.axisX = GLFW_GAMEPAD_AXIS_LEFT_X;
                    j.axisY = GLFW_GAMEPAD_AXIS_LEFT_Y;
                    j.pause = GLFW_GAMEPAD_BUTTON_START;

        int joy;
        for (joy = GLFW_JOYSTICK_1;  joy <= NUMBER_OF_JOYSTICKS;  joy++)
        {
			if (glfwJoystickPresent(joy))
            {
                GLFWgamepadstate state;
                if (glfwGetGamepadState(joysticks[joy], &state))
                {
                    addJoystick(joy);
                }
                else
                {
                    char joyError[200];
                    strcpy (joyError, "Joystick \"");
                    strcpy (joyError, glfwGetJoystickName(joy));
                    strcpy (joyError, "\" has no gamepad mapping!");
                    alert("Joystick Error", joyError);
                }

                joystickMaps[joy] = j;
            }
        }
    }

    JoystickMap OS::getJoystickMap(int joy)
    {
        return joystickMaps[joy];
    }

    void OS::addJoystick(int joy)
    {
        joysticks[joystickCount++] = joy;
    }

    void OS::removeJoystick(int joy)
    {
        int i;
        for (i = 0;  i < joystickCount;  i++)
        {
            if (joysticks[i] == joy)
                break;
        }

        for (i = i + 1;  i < joystickCount;  i++)
            joysticks[i - 1] = joysticks[i];

        joystickCount = joystickCount > 0 ? joystickCount - 1 : 0;
    }

    void OS::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if ((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS))
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            Point p;
            p.x = (int) xpos;
            p.y = (int) ypos;
            OS::getInstance()->setMouseCoord(OS::getInstance()->calculateMousePoint(p));
        }
    }

    Point OS::calculateMousePoint(Point point)
    {

        Point windowSize = getWindowSize();
        Point viewportSize = windowSize;
        GraphicsUtils::calculateViewportSize(viewportSize);
        Point p;
        p.x = (int) point.x;
        p.y = (int) point.y;
        p.x = (int) (p.x - (windowSize.x / 2 - viewportSize.x / 2)) * SCREEN_X / viewportSize.x;
        p.y = (int) (p.y - (windowSize.y / 2 - viewportSize.y / 2)) * SCREEN_Y / viewportSize.y;
        p.x -= SCREEN_X/2;
        p.y = SCREEN_Y - p.y - SCREEN_Y/2;

        return p;
    }

    void OS::frameBufferSizeCallbak(GLFWwindow* window, int width, int height)
    {
        OS::getInstance()->resizeWindow();
    }

    void OS::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {

        if (action == GLFW_PRESS)
        {
            InputSystem::getInstance()->keyPressed(OS::getInstance()->keyConversion[key]);
        }

        if (action == GLFW_RELEASE)
        {
            InputSystem::getInstance()->keyReleased(OS::getInstance()->keyConversion[key]);
        }

    }

    void OS::joystick_callback(int joy, int event)
    {
        if (event == GLFW_DISCONNECTED)
        {

            OS::getInstance()->removeJoystick(joy);

        }
    }

    void OS::error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    void OS::setCursorPosition(int x, int y)
    {
        glfwSetCursorPos(window, x, y);
    }

    void OS::updateMouseByJoystick(float delta)
    {
        if (joystickCount > 0 && glfwJoystickPresent(0))
        {
            double x, y;
			double  xAdd, yAdd;
            glfwGetCursorPos(window, &x, &y);
            JoystickMap joystickMap = getJoystickMap(0);
            GLFWgamepadstate state;
            glfwGetGamepadState(0, &state);
            const float * axes = state.axes;
			xAdd = yAdd = 0;
            if (state.axes && (axes[joystickMap.axisX] > 0.2 || axes[joystickMap.axisX] < -0.2))
            {
                xAdd = (axes[joystickMap.axisX]);
            }

            if (state.axes && (axes[joystickMap.axisY] > 0.2 || axes[joystickMap.axisY] < -0.2))
            {
                yAdd = (axes[joystickMap.axisY]);
            }
            double le = sqrt(xAdd*xAdd + yAdd*yAdd);
            xAdd = (le != 0) ? xAdd / le : 0;
            yAdd = (le != 0) ? yAdd / le : 0;
            xAdd *= 500 * delta;
            yAdd *= 500 * delta;

            x += xAdd;
            y += yAdd;
            glfwSetCursorPos(window, (int) x, (int) y);

            const unsigned char * buttons = state.buttons;
            if (state.buttons > 0 &&
                 ( buttons[joystickMap.jump] == GLFW_PRESS)
                )
            {
                Point p;
                p.x = (int) x;
                p.y = (int) y;
                setMouseCoord(calculateMousePoint(p));
                usleep(400000);
            }
        }
    }

    void OS::toggleFullScreen()
    {
		if (fullScreen)
		{
			if (windowedSize.x == 0)
			{
				windowedSize.x = restoreWidth;
				windowedSize.y = restoreHeight;
				windowedPosition.x = 100;
				windowedPosition.y = 100;
			}
			glfwSetWindowMonitor(window, NULL,
				windowedPosition.x, windowedPosition.y,
				windowedSize.x, windowedSize.y, 0);
		}
		else
		{
			glfwGetWindowPos(window, &windowedPosition.x, &windowedPosition.y);
			glfwGetWindowSize(window, &windowedSize.x, &windowedSize.y);
			GLFWmonitor* monitor = getCurrentMonitor(window);
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}

        fullScreen = !fullScreen;
    }

	GLFWmonitor* OS::getCurrentMonitor(GLFWwindow *window)
	{
		int nmonitors, i;
		int wx, wy, ww, wh;
		int mx, my, mw, mh;
		int overlap, bestoverlap;
		GLFWmonitor *bestmonitor;
		GLFWmonitor **monitors;
		const GLFWvidmode *mode;

		bestoverlap = 0;
		bestmonitor = NULL;

		glfwGetWindowPos(window, &wx, &wy);
		glfwGetWindowSize(window, &ww, &wh);
		monitors = glfwGetMonitors(&nmonitors);

		for (i = 0; i < nmonitors; i++) {
			mode = glfwGetVideoMode(monitors[i]);
			glfwGetMonitorPos(monitors[i], &mx, &my);
			mw = mode->width;
			mh = mode->height;

			overlap =
				std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
				std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

			if (bestoverlap < overlap) {
				bestoverlap = overlap;
				bestmonitor = monitors[i];
			}
		}

		return bestmonitor;
	}

}

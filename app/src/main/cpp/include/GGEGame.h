#ifndef GGEGAME_H_INCLUDED
#define GGEGAME_H_INCLUDED

#include "Definitions.h"
#if defined(__ANDROID__)
 #include "OS_ANDROID.h"
#elif __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_OSX
  #include "OS_GLFW.h"
 #else
  #include "OS_iOS.h"
 #endif
#else
 #include "OS_GLFW.h"
#endif  
#if defined(__WIN32__)
#include <windows.h>
#else
#include<sys/time.h>
#endif
#include "GameScreen.h"

namespace GGE
{

    class Game
    {
    public:
        Game();
		virtual         ~Game();
        void            play();

        void            initSO(int width = SCREEN_X, int height = SCREEN_Y, bool fullScreen = true);
        void            initGraphics();
        void            initResources();
        void            initSound();
        void            initInputSystem();
        void            finishSound();
        void            finishGraphics();
        void            finishSO();
        void            finishResources();
        void            finishInputSystem();
        void            checkFocused();
        void            changeScreen(std::string screenName);

        void            addScreen(std::string name, Screen* _screen);
        void            removeScreen(std::string name);
        
        void            beforeGameLoop();
        void            gameLoop();
        void            afterGameLoop();


#if defined (__ANDROID__)
        inline void     checkInputEvent() { OS::getInstance()->checkInputEvent(); }
        bool			isAlive();
        void			showAds();
        void            setAndroidState(struct android_app* state);

#endif

    protected:
        bool running;
        bool focused;
        Point mouseCoord;
        GameScreen *gameScreen;
        std::map<std::string, Screen*> screens;
        Screen *activeScreen;
        float deltaTime;
        float nowTime, lastTime;
    };
}

#endif // GGEGAME_H_INCLUDED

#include "../include/GGEGame.h"

namespace GGE
{

    void Game::play()
    {
        beforeGameLoop();

        while (OS::getInstance()->getRunning())
        {
            gameLoop();
        }

        afterGameLoop();
    }
    
    void Game::beforeGameLoop()
    {
        initResources();
        initSO();
        //        Graphics::getInstance()->setDebugMode(true);
        initGraphics();
        initInputSystem();
        
        gameScreen = new GameScreen();
        addScreen("Game", gameScreen);
        activeScreen = gameScreen;
        activeScreen->show();
        focused = true;
    }
    
    void Game::gameLoop()
    {
        nowTime = OS::getInstance()->getTime();
        deltaTime = nowTime - lastTime;
        lastTime = nowTime;
        checkFocused();
        if (focused)
        {
            OS::getInstance()->checkInputEvent();
            activeScreen->render(deltaTime);
            
            OS::getInstance()->swapBuffer();
        }
        
    }
    
    void Game::afterGameLoop()
    {
        activeScreen->pause();
        activeScreen->finish();
        delete screens["Game"];
        finishGraphics();
        finishResources();
        finishSO();
        finishInputSystem();
    }

    void Game::checkFocused()
    {

        if (!OS::getInstance()->isFocused())
        {
            if (focused == true)
            {
                activeScreen->pause();
            }
            focused = false;

        } else {

            if (focused == false)
            {
                activeScreen->resume();
            }
            focused = true;
        }
    }

    void Game::changeScreen(std::string screenName)
    {

        activeScreen->finish();
        if (screens.find(screenName) != screens.end())
        {
            activeScreen = screens.at(screenName);
        }
        activeScreen->show();

    }

    Game::Game()
    {

		running = false;
		focused = false;

        mouseCoord = Point();
		mouseCoord.x = mouseCoord.y = -SCREEN_X - 10;
		nowTime = lastTime = 0;

    }

    Game::~Game()
    {

    }


    void Game::initSO(int width, int height, bool fullScreen)
    {
#if defined(GGE_DESKTOP)
        OS::getInstance()->createWindow("Starter", width, height, fullScreen);
#endif
    }

    void Game::initGraphics()
    {
        Graphics::getInstance()->initGraphics();
    }

    void Game::initInputSystem()
    {
        InputSystem::getInstance();
    }

    void Game::finishSO()
    {
        OS::getInstance()->destroy();
    }

    void Game::finishGraphics()
    {
        Graphics::getInstance()->destroy();
    }

    void Game::initResources()
    {

    }

    void Game::finishResources()
    {
        Resources::getInstance()->destroy();
    }

    void Game::finishInputSystem()
    {
        InputSystem::getInstance()->destroy();
    }

    void Game::addScreen(std::string name, Screen *_screen)
    {
        screens.insert(std::make_pair(name, _screen));
    }

    void Game::removeScreen(std::string name)
    {
        if (screens.find(name) != screens.end())
            screens.erase(name);
    }


#if defined (__ANDROID__)

    bool Game::isAlive()
    {
        return OS::getInstance()->isAlive();
    }

    void Game::setAndroidState(struct android_app* state)
    {
        OS::getInstance()->setAndroidApp(state);
    }

#endif

}

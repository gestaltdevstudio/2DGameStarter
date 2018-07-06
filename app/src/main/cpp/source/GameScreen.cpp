#include "../include/GameScreen.h"

namespace GGE
{

    GameScreen::GameScreen()
    {
        fixedDeltaTime = 0.016;

    }

    GameScreen::~GameScreen()
    {

    }

    void GameScreen::initScreen()
    {


        gameModel = new GameModel();
        gameView = new GameView(this, gameModel);

        gameModel->initModel();
        gameView->initView();
        accumulator = 0;
        OS::getInstance()->setRunning(true);
    }

    void GameScreen::render(float deltaTime)
    {

#if defined(GGE_DESKTOP)
        if (InputSystem::getInstance()->isKeyDown(GGE_ESCAPE) && OS::getInstance()->isFullScreen())
        {
            OS::getInstance()->toggleFullScreen();
            InputSystem::getInstance()->keyReleased(GGE_ESCAPE);
        }
#endif

        if (deltaTime > 0.25)
            deltaTime = 0.25;

        accumulator += deltaTime;

        while (accumulator >= fixedDeltaTime)
        {
            gameModel->step(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        gameModel->interpolate(accumulator / fixedDeltaTime);
        gameModel->checkCollision();
        gameView->step(deltaTime);

    }


    void GameScreen::show()
    {

        initScreen();

    }

    void GameScreen::pause()
    {

    }

    void GameScreen::resume()
    {

    }

    void GameScreen::finish()
    {

        gameView->finishView();
        gameModel->finishModel();

        delete gameView;
        delete gameModel;

    }

}

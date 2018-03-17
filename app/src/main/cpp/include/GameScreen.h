#ifndef GAMESCREEN_H_INCLUDED
#define GAMESCREEN_H_INCLUDED

#include "Screen.h"
#include "GameModel.h"
#include "GameView.h"

namespace GGE
{

    class GameScreen : public Screen
    {

        public:
            GameScreen();
            virtual ~GameScreen();
            void initScreen();
            void render(float deltaTime);

            void show();
            void pause();
            void resume();
            void finish();

        protected:
            GameModel *gameModel;
            GameView *gameView;
            float accumulator;
            float fixedDeltaTime;


    };

}

#endif // GAMESCREEN_H_INCLUDED

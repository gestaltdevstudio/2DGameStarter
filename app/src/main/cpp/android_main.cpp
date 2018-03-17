#include "gpg/gpg.h"
#include "include/GGEGame.h"

void android_main(struct android_app* state) {

    gpg::AndroidPlatformConfiguration platform_configuration;
    platform_configuration.SetActivity(state->activity->clazz);


    GGE::Game *game = new GGE::Game();
    game->setAndroidState(state);


    while (!game->isAlive())
    {
        game->checkInputEvent();
    }
    game->play();

    delete game;

}

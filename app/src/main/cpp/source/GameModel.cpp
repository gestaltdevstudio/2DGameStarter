#include "../include/GameModel.h"
#if defined(__WIN32__)
#include <algorithm>
#endif

namespace GGE
{

    GameModel::~GameModel()
    {

    }

    void GameModel::initModel()
    {

        player = new Entity("Player");
        player->setDimension(80, 170);
        player->setPosition(-SCREEN_X/2 + 200, 300);
        Graphics::getInstance()->addEntity(player);


        floor = new Entity("Floor");
        floor->setDimension(1920, 150);
        floor->setPosition(0, -SCREEN_Y/2 + floor->getDimension()->y/2);
//        Graphics::getInstance()->addEntity(floor);


        playerVelocity.x = playerVelocity.y = 0;

        jumpingTimer = 0;
        jumping = false;

    }

    void GameModel::step(float deltaTime)
    {
        const float playerMass = 1;
        const float pixelToMeters = 100.f;
        const float playerHorizontalInertia = 15.f;
        const float gravity = -30 * pixelToMeters;
        const float playerJumpDamping = 140;
        const float horizontalAcceleration = 10 / playerMass * pixelToMeters;
        const float maximumHorizontalVelocity = 4 * pixelToMeters;
        const float jumpingForce = 150 / playerMass * pixelToMeters;
        const float fallingAcceleration =  0 / playerMass * pixelToMeters;

        float playerY = player->getNextPosition()->y;

        player->setPreviousPosition(player->getNextPosition()->x, player->getNextPosition()->y);


        float xVelocity;
#if defined(GGE_DESKTOP)
        JoystickState joystickState;
        bool joystickPresent = OS::getInstance()->getJoystickState(0, &joystickState);
#endif
        if (InputSystem::getInstance()->isKeyDown(GGE_RIGHT)
#if defined(GGE_DESKTOP)
        || (joystickPresent && joystickState.axes[OS::getInstance()->getJoystickMap(0).axisX] > 0.2)
#endif
        )
        {
            xVelocity = horizontalAcceleration * deltaTime;
            playerVelocity.x += playerVelocity.x > maximumHorizontalVelocity ? 0 : xVelocity;
#if defined(GGE_DESKTOP)
            playerVelocity.x *= (joystickPresent ? std::min(1.0f, joystickState.axes[OS::getInstance()->getJoystickMap(0).axisX] + 0.4f)  : 1);
#endif
        } else

        if (InputSystem::getInstance()->isKeyDown(GGE_LEFT)
#if defined(GGE_DESKTOP)
        || (joystickPresent && joystickState.axes[OS::getInstance()->getJoystickMap(0).axisX] < -0.2)
#endif
        )
        {
            xVelocity = -horizontalAcceleration * deltaTime;
            playerVelocity.x += playerVelocity.x < -maximumHorizontalVelocity ? 0 : xVelocity;
#if defined(GGE_DESKTOP)
            playerVelocity.x *= (joystickPresent ? std::min(1.0f, -joystickState.axes[OS::getInstance()->getJoystickMap(0).axisX] + 0.4f) :1);
#endif
        }
        else
        {
            playerVelocity.x -= playerVelocity.x * playerHorizontalInertia * deltaTime;
        }

        if(playerY - player->getDimension()->y/2 <= floor->getPosition()->y + floor->getDimension()->y/2)
        {
            playerVelocity.y = 0.f;
            playerY = floor->getPosition()->y + floor->getDimension()->y/2 + player->getDimension()->y/2;
            jumpingTimer = 0;

            if (InputSystem::getInstance()->isKeyDown(GGE_UP)
#if defined(GGE_DESKTOP)
            || (joystickPresent && joystickState.buttons[OS::getInstance()->getJoystickMap(0).jump])
#endif
            )
            {
                jumpingTimer = 0.1;
                jumping = true;
            }
        }
        else if (jumping == false)
        {
            playerVelocity.y += gravity * deltaTime;
            if (playerVelocity.y < 0)
            {
                playerVelocity.y -= deltaTime * fallingAcceleration;
            }
        }

        if (jumpingTimer > 0)
        {
            jumping = true;
            if (InputSystem::getInstance()->isKeyDown(GGE_UP)
#if defined(GGE_DESKTOP)
            || (joystickPresent && joystickState.buttons[OS::getInstance()->getJoystickMap(0).jump])
#endif
            )
            {
                jumpingTimer -= deltaTime;
            }
            else
            {
                InputSystem::getInstance()->keyReleased(GGE_UP);
                jumpingTimer = 0;
            }

            playerVelocity.y += playerVelocity.y > playerJumpDamping ? playerJumpDamping : jumpingForce * deltaTime;
        }
        else
        {
            jumping = false;
        }

        player->setNextPosition(player->getPreviousPosition()->x + playerVelocity.x * deltaTime,
                            playerY + playerVelocity.y * deltaTime);

    }

    void GameModel::interpolate(float alpha)
    {
        player->setPosition(player->getNextPosition()->x * alpha + player->getPreviousPosition()->x * (1.0f - alpha),
                            player->getNextPosition()->y * alpha + player->getPreviousPosition()->y * (1.0f - alpha));

    }

    void GameModel::checkCollision()
    {
        if(player->getPosition()->y - player->getDimension()->y/2 <= floor->getPosition()->y + floor->getDimension()->y/2)
        {
            player->setPosition(player->getPosition()->x, floor->getPosition()->y + floor->getDimension()->y/2 + player->getDimension()->y/2);
        }
    }


    void GameModel::finishModel()
    {

        delete player;
        delete floor;

    }

}

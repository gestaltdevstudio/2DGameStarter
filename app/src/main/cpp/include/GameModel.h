#ifndef GAMEMODEL_H_INCLUDED
#define GAMEMODEL_H_INCLUDED

#include "Model.h"
#include "Entity.h"
#include "InputSystem.h"
#include "Graphics.h"

namespace GGE
{

    class GameModel : public Model
    {
        public:
            ~GameModel();
            void step(float deltaTime);
            void finishModel();
            void initModel();
            inline Entity* getPlayer() { return player; };
            void interpolate(float alpha);
            void checkCollision();

        protected:
            Entity *player;
            Entity *floor;

            float jumpingTimer;
            bool jumping;

            Vector2 playerVelocity;

    };

}

#endif // GAMEMODEL_H_INCLUDED

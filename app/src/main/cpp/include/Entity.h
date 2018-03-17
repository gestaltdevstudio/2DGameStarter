#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <string>
#include "Definitions.h"

namespace GGE
{

    class Entity
    {

        public:
            inline Entity(std::string _name) { name = _name; rotation = 0;
		previousPosition.x = previousPosition.y = 0;
		nextPosition.x = nextPosition.y = 0;
		};

            inline Vector2 getPosition() { return position; }
            inline Vector2 getDimension() { return dimension; }
            inline float getRotation() { return rotation; };

            inline void setPosition(float _x, float _y) { position.x = _x; position.y = _y;}
            inline void setDimension(float _x, float _y) { dimension.x = _x; dimension.y = _y; }
            inline void setRotation(float _rotation) { rotation= _rotation; };
            inline std::string getName() { return name; };
            inline void setPreviousPosition(float x, float y) { previousPosition.x = x; previousPosition.y = y; }
            inline Vector2 getPreviousPosition() { return previousPosition; }
            inline void setNextPosition(float x, float y) { nextPosition.x = x; nextPosition.y = y; }
            inline Vector2 getNextPosition() { return nextPosition; }

        protected:
            Vector2    position;
            Vector2    previousPosition;
            Vector2    nextPosition;
            Vector2    dimension;
            float      rotation;
            std::string name;
    };


}

#endif // ENTITY_H_INCLUDED

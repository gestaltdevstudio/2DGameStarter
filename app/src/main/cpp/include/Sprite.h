#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "Drawable.h"
#include "Entity.h"
#include "Animation.h"

namespace GGE
{

    class Sprite : public Drawable
    {
        public:
            Sprite();
            virtual ~Sprite();
            inline void setCurrentAnimationName(std::string _animationName, AnimationPlayMode _playMode) { currentAnimationName = _animationName; playMode = _playMode; }
            inline std::string getCurrentAnimationName() { return currentAnimationName; }
            inline Animation* getCurrentAnimation() { return animations.at(currentAnimationName); };
            inline void addAnimation(std::string animationName, Animation *animation) { animations.insert(std::make_pair(animationName, animation)); }
            inline AnimationPlayMode getAnimationPlayMode() { return playMode; }
            inline void setEntity(Entity *_entity) { entity = _entity; }
            inline Entity* getEntity() { return entity; }

        protected:
            Entity *entity;
            std::map<std::string, Animation*> animations;
            std::string currentAnimationName;
            AnimationPlayMode playMode;

    };

}

#endif // SPRITE_H_INCLUDED

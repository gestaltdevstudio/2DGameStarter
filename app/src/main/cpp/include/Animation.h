#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <vector>
#include <string>
#include "TextureAtlas.h"
#include "Shader.h"
#include "Drawable.h"

namespace GGE
{

    class Animation
    {

        public:
            Animation();
            ~Animation();
            void loadFrames(TextureAtlas *_textureAtlas, Shader *_shader, float _frameDuration, std::vector<std::string> _framesNames);


            inline void update(float deltaTime) { elapsedTime += deltaTime; }
            Drawable* getCurrentDrawable(AnimationPlayMode playMode);


        protected:
            float frameDuration;
            std::vector<Drawable*> frames;
            TextureAtlas *textureAtlas;
            Shader *shader;
            float elapsedTime;


    };


}

#endif // ANIMATION_H_INCLUDED

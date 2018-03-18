#include "../include/Animation.h"
#if defined(__WIN32__)
#include <algorithm>
#endif

namespace GGE
{


    Animation::Animation()
    {
        elapsedTime = 0;
    }

    Animation::~Animation()
    {
        for (unsigned int j=0;j<frames.size();j++)
        {
                Drawable* drawable = frames[j];
                frames[j] = 0;
                delete drawable;
        }
    }

    void Animation::loadFrames(TextureAtlas *_textureAtlas, Shader *_shader, float _frameDuration, std::vector<std::string> _framesNames)
    {

        shader = _shader;
        textureAtlas = _textureAtlas;
        frameDuration = _frameDuration;
        for(std::vector<std::string>::iterator it = _framesNames.begin(); it != _framesNames.end(); ++it) {
            std::string name = *it;
            Drawable *drawable = new Drawable();
            drawable->loadRegion(name, textureAtlas);
            drawable->setShader(shader);
            frames.push_back(drawable);
        }

    }

    Drawable* Animation::getCurrentDrawable(AnimationPlayMode playMode)
    {
        unsigned int frameIndex = (int) (elapsedTime / frameDuration);
        switch(playMode)
        {

            case ANIM_NORMAL:

                frameIndex = std::min((int)frames.size() - 1, (int) frameIndex);

            break;
            case ANIM_LOOP:

                frameIndex = frameIndex % frames.size();

            break;
            case ANIM_LOOP_PINGPONG:

                frameIndex = frameIndex % ((frames.size() * 2) - 2);
                if (frameIndex >= frames.size()) frameIndex = frames.size() - 2 - (frameIndex - frames.size());

            break;

        }
        return frames[frameIndex];
    }

}

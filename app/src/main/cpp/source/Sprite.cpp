#include "../include/Sprite.h"

namespace GGE
{

    Sprite::Sprite()
    {
        playMode = ANIM_NORMAL;

    }

    Sprite::~Sprite()
    {


        std::map<std::string, Animation*>::iterator iter;
        for (iter = animations.begin(); iter != animations.end(); ++iter)
        {

            Animation *animation = iter->second;
            iter->second = NULL;
            delete animation;

        }

        animations.clear();
    }

}

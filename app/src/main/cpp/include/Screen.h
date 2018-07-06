#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

namespace GGE
{

    class Screen
    {
        public:
        virtual ~Screen() {};
            virtual void initScreen() = 0;
            virtual void show() = 0;
            virtual void render(float deltaTime) = 0;
            virtual void pause() = 0;
            virtual void resume() = 0;
            virtual void finish() = 0;
    };


}

#endif // SCREEN_H_INCLUDED

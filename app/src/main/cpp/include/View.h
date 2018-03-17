#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "Screen.h"

namespace GGE
{

    class View
    {

        public:
            inline View(Screen *_screen) { screen = _screen; }
            inline virtual ~View() { };

            virtual void initView() = 0;
            virtual void step(float deltaTime) = 0;
            virtual void finishView() = 0;

        protected:
            Screen *screen;

    };

}

#endif // VIEW_H_INCLUDED

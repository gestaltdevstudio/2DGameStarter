#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "UIObject.h"

namespace GGE
{

    class Button : public UIObject
    {

        public:
            Button();
            virtual ~Button();
            bool isClicked(Point inputPoint);


        protected:

    };

}

#endif // BUTTON_H_INCLUDED

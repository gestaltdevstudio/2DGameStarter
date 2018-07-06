#include "../include/Button.h"

namespace GGE
{

    Button::Button()
    {
    }

    Button::~Button()
    {
    }

    bool Button::isClicked(Point inputPoint)
    {
//        if (inputPoint.x > -900 && inputPoint.x !=0)
//        std::cout << inputPoint.x << " " << inputPoint.y << " : " << position->x << " " << position->y << std::endl;

        return (inputPoint.x >= position->x - round(dimension->x/2) && inputPoint.y >= position->y - round(dimension->y/2) &&
                inputPoint.x <= position->x + round(dimension->x/2) && inputPoint.y <= position->y + round(dimension->y/2) && drawable->isVisible());

    }

}

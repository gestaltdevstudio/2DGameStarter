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

        return (inputPoint.x >= position->x - round(dimension->x/2) && inputPoint.y >= position->y - round(dimension->y/2) &&
                inputPoint.x <= position->x + round(dimension->x/2) && inputPoint.y <= position->y + round(dimension->y/2) && drawable->isVisible());

    }

}

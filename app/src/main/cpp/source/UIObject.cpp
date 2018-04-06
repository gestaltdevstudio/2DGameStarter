#include "../include/UIObject.h"

namespace GGE
{

    UIObject::UIObject()
    {
        dimension = (Point*) malloc(sizeof(Point));
        position = (Point*) malloc(sizeof(Point));
        dimension->x = dimension->y= 1;
        position->x = position->y = 0;
    }

    UIObject::~UIObject()
    {
        if (drawable)
        {
            delete drawable;
        }
        if (position)
            free(position);

        if (dimension)
            free(dimension);
    }

    void UIObject::setDrawable( Drawable *_drawable)
    {
        if (_drawable)
        {
            drawable = _drawable;
            dimension->x = drawable->getAtlasRegion()->width;
            dimension->y = drawable->getAtlasRegion()->height;
        }
    }

}

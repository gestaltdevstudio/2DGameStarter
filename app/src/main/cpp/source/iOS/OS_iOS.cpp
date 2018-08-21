#include "../../include/OS_iOS.h"

namespace GGE
{

    OS* OS::instance = 0;

    OS::OS()
    {
        handID[0]=-1;
        handID[1]=-1;

        tv0 = timeval();
        tv = timeval();
        touchCoord.x = touchCoord.y = -SCREEN_X - 10;
        inputCoord[0].x = inputCoord[0].y = -SCREEN_X - 10;
        inputCoord[1].x = inputCoord[1].y = -SCREEN_X - 10;
        running = false;
        alive = false;
        gettimeofday(&tv0, NULL);
    }

    bool OS::getRunning()
    {
        return running;
    }

    bool OS::isOnFocus()
    {
        return lostFocus;
    }

    const Point OS::getMouseCoord()
    {

        const Point _return = touchCoord;
        touchCoord.x = touchCoord.y = -SCREEN_X - 10;
        return calculateMousePoint(_return);
    }

    Point OS::calculateMousePoint(Point point)
    {
        Point windowSize = OS::getInstance()->getWindowSize();
        Point viewportSize = Graphics::getInstance()->getViewportSize();
        Point p;
        p.x = (int) point.x;
        p.y = (int) point.y;
        p.x = (int) (p.x - (windowSize.x / 2 - viewportSize.x / 2)) * SCREEN_X / viewportSize.x;
        p.y = (int) (p.y - (windowSize.y / 2 - viewportSize.y / 2 )) * SCREEN_Y / viewportSize.y;
        p.x -= SCREEN_X/2;
        p.y = SCREEN_Y - p.y - SCREEN_Y/2;
        return p;
    }

    void OS::setTouchUp(Point p)
    {
        if (calculateMousePoint(p).x > 0) {
            p.x = p.y = 0;
            handID[RIGHT_HAND] = -1;
            setInputCoord(RIGHT_HAND, calculateMousePoint(p));
        } else {
            p.x = p.y = 0;
            handID[LEFT_HAND] = -1;
            setInputCoord(LEFT_HAND, calculateMousePoint(p));
        }
    }
    
    void OS::setTouchDown(Point p)
    {
        if (calculateMousePoint(p).x > 0) {
            handID[RIGHT_HAND] = 0;
            setInputCoord(RIGHT_HAND, calculateMousePoint(p));
            setTouchCoord(p);
        } else {
            handID[LEFT_HAND] = 0;
            setInputCoord(LEFT_HAND, calculateMousePoint(p));
            setTouchCoord(p);
        }
    }
    
    void OS::setTouchMove(Point p)
    {
        setTouchDown(p);
    }
    
    void OS::resetTouchInput()
    {
        Point p;
        p.x = p.y = 0;
        handID[LEFT_HAND] = -1;
        handID[RIGHT_HAND] = -1;
    }

    void OS::swapBuffer()
    {

    }

    float OS::getTime()
    {
        gettimeofday(&tv, NULL);
        return (float)(tv.tv_sec-tv0.tv_sec) + 0.000001*((float)(tv.tv_usec-tv0.tv_usec));
    }

    void OS::checkInputEvent()
    {

    }

    void OS::destroy()
    {

        delete instance;
        instance = NULL;
    }

    Point OS::getWindowSize()
    {
        Point windowSize;
        windowSize.x = width;
        windowSize.y = height;
        return windowSize;
    }

    void OS::resizeWindow()
    {
        Point windowSize = getWindowSize();

        Graphics::getInstance()->renderResize(windowSize);
    }


    int OS::alert(const char *lpCaption, const char *lpText)
    {
        fprintf( stderr, "Message: '%s', Detail: '%s'\n", lpCaption, lpText );
        return 0;
    }

}

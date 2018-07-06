#ifndef SO_IOS_H
#define SO_IOS_H

#include <memory>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <cassert>


#include <stdio.h>
#include <stdlib.h>

#include "Definitions.h"
#include "Graphics.h"
#include <sys/time.h>


enum MobileHand {
    LEFT_HAND = 0,
    RIGHT_HAND = 1
};


namespace GGE {

    class Controller;

    struct resourceFile;

    class OS {
    public:
        static OS *getInstance() {
            if (!OS::instance) OS::instance = new OS();
            return OS::instance;
        }

        void destroy();
        void checkInputEvent();
        void procEvent();
        void swapBuffer();
        void resizeWindow();
        void resizeWindow(int _adsYOffset);
        bool getRunning();
        bool isOnFocus();
        const Point getMouseCoord();
        float getTime();
        inline void setMouseCoord(Point p) { mouseCoord = p; }
        int alert(const char *lpCaption, const char *lpText);

        inline void setRunning(bool _yn) { running = _yn; }
        inline void setLostFocus(bool _yn) { lostFocus = _yn; }

        void toggleFullScreen();

//        inline bool isFullScreen() { return fullScreen; };
        inline bool isAlive() {return alive;}
        inline void setIsAlive(bool _alive) {alive = _alive;}

        inline void setInputCoord(int index, Point p) {
            if (index > -1 && index < 2)
                inputCoord[index] = p;}

        inline void setTouchCoord(Point p) { touchCoord = p;}

        inline Point getInputCoord(int index) { return inputCoord[index]; }

        Point getWindowSize();

        Point calculateMousePoint(Point point);

        int32_t width;
        int32_t height;

        inline bool isFocused() { return !lostFocus; };

        bool killApp;
        int handID[2];
        void resetTouchInput();
        
        void setTouchDown(Point p);
        void setTouchMove(Point p);
        void setTouchUp(Point p);

    private:
        static OS *instance;

        OS();
        Point inputCoord[2];
        Point touchCoord;
        Point mouseCoord;
        Point windowedPosition, windowedSize;
        bool running;
        bool alive;
        bool lostFocus;


        int restoreWidth, restoreHeight;

        timeval     tv0, tv;
    };

}

#endif //SO_IOS_H

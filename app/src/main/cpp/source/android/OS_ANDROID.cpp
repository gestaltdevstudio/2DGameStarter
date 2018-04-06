#include "../../include/OS_ANDROID.h"

namespace GGE
{

    OS* OS::instance = 0;

    OS::OS()
    {
        context = EGL_NO_CONTEXT;
        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
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


    static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
    {
        OS* engine = (OS*)app->userData;

        if (AKeyEvent_getKeyCode(event) == AKEYCODE_BACK) {

            return 1;
        }

        Point p;

        int32_t action = AMotionEvent_getAction(event);
        int32_t touchModified = 0;
        int32_t ret = 0;

        if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_KEY) {
            ret = 1;

            switch (action & AMOTION_EVENT_ACTION_MASK) {

                case AMOTION_EVENT_ACTION_DOWN: {

                    Point p;
                    p.x = (int) (AMotionEvent_getRawX(event, 0));
                    p.y = (int) (AMotionEvent_getRawY(event, 0));
                    if (engine->calculateMousePoint(p).x > 0) {
                        engine->handID[RIGHT_HAND] = 0;
                    } else {
                        engine->handID[LEFT_HAND] = 0;
                    }

//                    LOGI("ACTION_DOWN");
                    touchModified = 1;
                }
                    break;

                case AMOTION_EVENT_ACTION_UP: {

                    if (engine->handID[LEFT_HAND] == 0) {
                        engine->handID[LEFT_HAND] = -1;
                    } else {
                        engine->handID[RIGHT_HAND] = -1;
                    }
//                    LOGI("ACTION_UP");

                    touchModified = 0;
                }
                    break;


                case AMOTION_EVENT_ACTION_POINTER_UP: {

                    int32_t iIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                                                                                        AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                    if (engine->handID[LEFT_HAND] == AMotionEvent_getPointerId(event, iIndex)) {
                        engine->handID[LEFT_HAND] = -1;
                    } else {
                        engine->handID[RIGHT_HAND] = -1;
                    }

//                    LOGI("ACTION_POINTER_UP");

                    touchModified = 0;
                }
                    break;

                case AMOTION_EVENT_ACTION_POINTER_DOWN: {

                    int32_t iIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                                                                                        AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                    Point p;
                    p.x = (int) (AMotionEvent_getRawX(event,
                                                      AMotionEvent_getPointerId(event, iIndex)));
                    p.y = (int) (AMotionEvent_getRawY(event,
                                                      AMotionEvent_getPointerId(event, iIndex)));
                    if (engine->calculateMousePoint(p).x > 0) {
                        engine->handID[RIGHT_HAND] = AMotionEvent_getPointerId(event, iIndex);
                    } else {
                        engine->handID[LEFT_HAND] = AMotionEvent_getPointerId(event, iIndex);
                    }

//                    LOGI("ACTION_POINTER_DOWN");

                    touchModified = 1;
                }
                    break;

            }
        }

        for (int i=0; i<2; i++)
        {
            if (engine->handID[i] >= 0)
            {
                p.x = (int) (AMotionEvent_getRawX(event, engine->handID[i]));
                p.y = (int) (AMotionEvent_getRawY(event, engine->handID[i]));
                engine->setInputCoord(i, engine->calculateMousePoint(p));
            }
            else
            {
                p.x = 0;
                p.y = 0;
                engine->setInputCoord(i, engine->calculateMousePoint(p));
            }
        }


        if (touchModified)
        {

            p.x = (int) (AMotionEvent_getRawX(event, 0));
            p.y = (int) (AMotionEvent_getRawY(event, 0));
            engine->setTouchCoord(p);

        }
        return ret;
    }

    void OS::resetTouchInput()
    {
        Point p;
        p.x = p.y = 0;
        handID[LEFT_HAND] = -1;
        handID[RIGHT_HAND] = -1;
    }

     static int engine_init_display(OS* engine) {

        const EGLint attribs[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_NONE
        };
        EGLint w, h;
        EGLint numConfigs;
        EGLConfig config;
        EGLSurface surface;
        EGLContext context;

        EGLDisplay display = eglGetCurrentDisplay();

        if (display == EGL_NO_DISPLAY) {

            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

            eglInitialize(display, 0, 0);

            eglChooseConfig(display, attribs, NULL, 0, &numConfigs);
            std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
            assert(supportedConfigs);
            eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
            assert(numConfigs);
            auto i = 0;
            for (; i < numConfigs; i++) {
                auto &cfg = supportedConfigs[i];
                EGLint r, g, b, d;
                if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r) &&
                    eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
                    eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b) &&
                    eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
                    r == 8 && g == 8 && b == 8 && d == 0) {

                    config = supportedConfigs[i];
                    break;
                }
            }
            if (i == numConfigs) {
                config = supportedConfigs[0];
            }

        }

        surface = engine->surface;
        if (surface == EGL_NO_SURFACE) {
            surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
            if (surface == EGL_NO_SURFACE) {
                LOGW("Unable to eglCreateWindowSurface");
                return -1;
            }
        }

        context = engine->context;
        if (context == 0)
        {

            EGLint AttribList[] =
                    {
                            EGL_CONTEXT_CLIENT_VERSION, 3,
                            EGL_NONE
                    };

            context = eglCreateContext(display, config, NULL, AttribList);
            if (context == EGL_NO_CONTEXT)
            {
                LOGW("Unable to eglCreateContext");
                return -1;
            }

        }

        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
            LOGW("Unable to eglMakeCurrent");
            return -1;
        }

        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);

        engine->display = display;
        engine->context = context;
        engine->surface = surface;
        engine->width = w;
        engine->height = h;
        engine->setRunning(true);
        engine->setIsAlive(true);
        Point p;
        p.x = p.y = 0;
        engine->setInputCoord(0, p);
        engine->setInputCoord(1, p);
        engine->setTouchCoord(p);

        auto opengl_info = { GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS };
        for (auto name : opengl_info) {
            auto info = glGetString(name);
            LOGI("OpenGL Info: %s", info);
        }

        engine->resizeWindow();

        return 0;
    }

    static void engine_draw_frame(OS* engine) {
        if (engine->display == NULL) {

            return;
        }

        eglSwapBuffers(engine->display, engine->surface);
    }


     static void engine_term_display(OS* engine) {
        if (engine->killApp) {
            if (engine->display != EGL_NO_DISPLAY) {
                eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                if (engine->context != EGL_NO_CONTEXT) {
                    eglDestroyContext(engine->display, engine->context);
                }
                if (engine->surface != EGL_NO_SURFACE) {
                    eglDestroySurface(engine->display, engine->surface);
                }
                eglTerminate(engine->display);
            }

            engine->display = EGL_NO_DISPLAY;
            engine->surface = EGL_NO_SURFACE;
            engine->context = EGL_NO_CONTEXT;
        }
        else
        {
            eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (engine->surface != EGL_NO_SURFACE) {
                eglDestroySurface(engine->display, engine->surface);
                engine->surface = EGL_NO_SURFACE;
            }

        }
        engine->setIsAlive(false);
    }

    static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
        OS* engine = (OS*)app->userData;
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:

                if (engine->app->window != NULL) {
                    engine_init_display(engine);
                    engine_draw_frame(engine);
                }
                engine->killApp = false;

                LOGI("APP_CMD_INIT_WINDOW");
            break;

            case APP_CMD_WINDOW_RESIZED:
            case APP_CMD_WINDOW_REDRAW_NEEDED:
            case APP_CMD_CONTENT_RECT_CHANGED:
            case APP_CMD_CONFIG_CHANGED:
                if (engine->app->window != NULL && ((engine->width != ANativeWindow_getWidth(app->window)) || (engine->height != ANativeWindow_getHeight(app->window)))) {
                    engine_handle_cmd(app, APP_CMD_TERM_WINDOW);
                    engine_handle_cmd(app, APP_CMD_INIT_WINDOW);
                }
                engine->resizeWindow();
//                LOGI("APP_CMD_CONFIG_CHANGED");
            break;

            case APP_CMD_PAUSE:
                engine->lostFocus = true;
//                LOGI("APP_CMD_PAUSE");
            break;

            case APP_CMD_RESUME:
                engine->lostFocus = false;
//                LOGI("APP_CMD_RESUME");
            break;
            case APP_CMD_TERM_WINDOW:

                engine_term_display(engine);
//                LOGI("APP_CMD_TERM_WINDOW");
            break;
            case APP_CMD_GAINED_FOCUS:
                engine->lostFocus = false;
//                LOGI("APP_CMD_GAINED_FOCUS");
            break;
            case APP_CMD_LOST_FOCUS:

                engine->lostFocus = true;
//                LOGI("APP_CMD_LOST_FOCUS");
            break;
        }
    }

    void OS::swapBuffer()
    {
        engine_draw_frame(this);
    }

    float OS::getTime()
    {
        gettimeofday(&tv, NULL);
        return (float)(tv.tv_sec-tv0.tv_sec) + 0.000001*((float)(tv.tv_usec-tv0.tv_usec));
    }

    void OS::checkInputEvent()
    {
        int id;
        int events;
        android_poll_source* source;

        while ((id = ALooper_pollAll(0, NULL, &events,
                                     (void**)&source)) >= 0) {

            if (source != NULL) source->process(app, source);


            if (app->destroyRequested != 0) {
                lostFocus = true;
                killApp = true;
                engine_term_display(this);
                return;
            }
        }

    }

    void OS::destroy()
    {
        alive = false;

        ANativeActivity_finish(app->activity);
        while(!killApp)
        {
            checkInputEvent();
        }
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
        LOGW(lpCaption, lpText);
        return 0;
    }


    void OS::setAndroidApp(struct android_app *_app) {

        app = _app;

        app->userData = getInstance();
        app->onAppCmd = engine_handle_cmd;
        app->onInputEvent = engine_handle_input;

    }
}
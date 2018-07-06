#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED


#if !defined(__WIN32__)
#include<unistd.h>
#endif

#include <bitset>
#include <sstream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#if defined(__ANDROID__)
 #include <GLES/gl.h>
 #include <GLES3/gl3.h>
 #include <GLES3/gl3ext.h>
#elif __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_OSX
  #include <glad/glad.h>
 #else
  #include <OpenGLES/ES3/gl.h>
 #endif
#else
 #include <glad/glad.h>
#endif

#ifndef FIXED_ASPECT_RATIO
#define FIXED_ASPECT_RATIO 1.777777777f
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SCREEN_X 1920
#define SCREEN_Y 1080

#define NUMBER_OF_JOYSTICKS 4

#if defined(__WIN32__)
// winuser.h doesn't define it...
#define VK_A 0x41
#define VK_W 0x57
#define VK_S 0x53
#define VK_D 0x44
#define VK_Q 0x51
#define VK_P 0x50
#define VK_SLASH 0xBF


#define usleep(x) Sleep(x / 1000)

#endif

#define NOTHING 6

namespace GGE
{

    struct JoystickMap {
        int jump;
        int axisX;
        int axisY;
        int pause;
    };


    struct Vector2
    {
        float x;
        float y;
    };

    struct Point {
        int x;
        int y;
    };

    enum GameState {
        GAME_RUNNING = 0,
        GAME_PREPARE = 1,
        GAME_PAUSED = 2,
        GAME_OVER = 3,
        GAME_ENDED = 4,
        MAIN_MENU = 5,
        GAME_SETTINGS = 6
    //	SELECT_DIFFICULT = 7,
    //	SELECT_VERSUS_MODE = 8
    };

    enum AnimationPlayMode {
        ANIM_NORMAL,
        ANIM_LOOP,
        ANIM_LOOP_PINGPONG,
    };

    enum KeyCode
    {
        GGE_ESCAPE      = 0x01,
        GGE_Q           = 0x02,
        GGE_W           = 0x03,
        GGE_P           = 0x04,
        GGE_RETURN      = 0x05,
        GGE_A           = 0x06,
        GGE_S           = 0x07,
        GGE_D           = 0x08,
        GGE_SPACE       = 0x09,
        GGE_UP          = 0x0A,
        GGE_LEFT        = 0x0B,
        GGE_RIGHT       = 0x0C,
        GGE_DOWN        = 0x0D
    };
    
    inline static char* stristr(const char* str1, const char* str2)
    {
        const char* p1 = str1;
        const char* p2 = str2;
        const char* r = *p2 == 0 ? str1 : 0;

        while (*p1 != 0 && *p2 != 0)
        {
            if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
            {
                if (r == 0)
                {
                    r = p1;
                }

                p2++;
            }
            else
            {
                p2 = str2;
                if (r != 0)
                {
                    p1 = r + 1;
                }

                if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
                {
                    r = p1;
                    p2++;
                }
                else
                {
                    r = 0;
                }
            }

            p1++;
        }

        return *p2 == 0 ? (char*)r : 0;
    }

}

// Workaround to std::to_string
template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

#endif // DEFINITIONS_H_INCLUDED

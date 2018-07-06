#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "Resources.h"
#if defined(__ANDROID__)
#include "OS_ANDROID.h"
#elif __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_OSX
  #include "OS_GLFW.h"
 #endif
#else
 #include "OS_GLFW.h"
#endif

namespace GGE
{

    struct FontChar {

        int id;
        int x;
        int y;
        int width;
        int height;
        int xoffset;
        int yoffset;
        int xadvance;
        int page;
        int chnl;

    };


    class Font
    {
        public:
            Font();
            ~Font();
//            std::string face;
//            std::string face;
//            int size;
//            bool bold;
//            bool italic;
//            std::string charset;
//            bool unicode;
//            int stretchH;
//            bool smooth;
//            int aa; /// TODO Review if it's float
//            int paddingUp;
//            int paddingDown;
//            int paddingLeft;
//            int paddingRight;
//            int spacingHorizontal;
//            int spacingVertical;
            void loadFont(const resourceFile *fileBuffer);
            FontChar *fontChars;
            int textureWidth;
            int textureHeight;
            inline int getOriginalSize() { return originalSize; }

        protected:
            int charCount;
            int originalSize;
//            char* fileName;

    };

}

#endif // FONT_H_INCLUDED

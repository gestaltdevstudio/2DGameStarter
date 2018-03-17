#ifndef TEXTUREATLAS_H_INCLUDED
#define TEXTUREATLAS_H_INCLUDED

#include "Resources.h"
#include <map>
#include "GraphicsUtils.h"

namespace GGE
{

    struct AtlasRegion
    {
        int index;
		int x;
		int y;
		int width;
		int height;
    };

    class TextureAtlas
    {
        public:
            TextureAtlas();
            ~TextureAtlas();
            char* imageName;
            char* format;
            char* filter1;
            char* filter2;
            std::map<std::string, AtlasRegion*> regions;
            int width;
            int height;

            void* loadTextureAtlas(const resourceFile *atlasConf, const resourceFile *atlasImage);

            GLuint textureID;
    };


}

#endif // TEXTUREATLAS_H_INCLUDED

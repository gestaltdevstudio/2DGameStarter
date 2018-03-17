#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include "TextureAtlas.h"
#include "Shader.h"

namespace GGE
{

    class Drawable
    {

    public:
        Drawable();
        virtual ~Drawable();
        void loadRegion(std::string regionName, TextureAtlas *_textureAtlas);

        inline float getX() { return x; };
        inline float getY() { return y; };
        inline float getScaleX() { return scaleX; };
        inline float getScaleY() { return scaleY; };
        inline Shader *getShader() { return shader; };
        inline AtlasRegion* getAtlasRegion() { return atlasRegion; }
        inline TextureAtlas* getTextureAtlas() { return textureAtlas; }
        inline bool isVisible() { return visible; }

        inline void setX(float _x) { x = _x; };
        inline void setY(float _y) { y = _y; };
        inline void setScaleX(float _scaleX) { scaleX = _scaleX; };
        inline void setScaleY(float _scaleY) { scaleY = _scaleY; };
        inline void setShader(Shader *_shader ) { shader = _shader; };

        inline void setIsVisible(bool _visible) { visible = _visible; }
        inline GLuint getUVBuffer() { return uvbo; }
        inline void setRotation(float r) { rotation = r;}
        inline float getRotation() { return rotation; }
        inline void setFlipedX(bool _flipedX) { flipedX = _flipedX; }
        inline bool isFlipedX() { return flipedX; }
        inline void setFlipedY(bool _flipedY) { flipedY = _flipedY; }
        inline bool isFlipedY() { return flipedY; }


    protected:
        float x;
        float y;
        float scaleX;
        float scaleY;
        float rotation;
        Shader *shader;
        AtlasRegion *atlasRegion;
        TextureAtlas *textureAtlas;
        bool visible;
        bool flipedX;
        bool flipedY;
        GLuint uvbo;
    };
}

#endif // DRAWABLE_H_INCLUDED

#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "Font.h"
#include "TextureAtlas.h"
#include "Shader.h"

namespace GGE
{
    class Font;

    class Text
    {

        public:
            Text();
            ~Text();
            void initText(const char * _textName, Font *_textFont, Shader *shader, std::string _textureRegionName, TextureAtlas* _textureAtlas, int _x, int _y);

            inline void setText(std::string _text)
            {
				if (_text.length() == 0 || _text.compare("") == 0)
				{
					_text = " ";
				}
				textToPrint = _text;
            };

            void printText();

            inline void setPosition(int _x, int _y) {position.x = _x; position.y = _y; };
            inline void setScaleX(float _x) { scaleX = _x; }
            inline void setScaleY(float _y) { scaleY = _y; }
            inline void setVisible(bool _visible) { visible = _visible; }
            inline bool isVisible() { return visible; }
            inline std::string getName() { return textName; }

            inline Point getPosition() { return position; }
            inline float getScaleX() { return scaleX; }
            inline float getScaleY() { return scaleY; }
            inline Shader* getShader() { return shader; }
            inline Font* getFont() { return font; }


        protected:
            std::string             textName;
            float                   textSize;
            float                   scaleX, scaleY;
            Font                    *font;
			std::string             textToPrint;
			std::string             textPrinted;
            GLuint                  vao;
            GLuint                  textVertexBufferID;
            GLuint                  textUVBufferID;
            std::vector<glm::vec2>  vertices;
            std::vector<glm::vec2>  UVs;
            std::string             textureRegionName;
            TextureAtlas            *textureAtlas;
            Shader                  *shader;
            Point                   position;
            bool                    visible;


    };

}

#endif // TEXT_H_INCLUDED

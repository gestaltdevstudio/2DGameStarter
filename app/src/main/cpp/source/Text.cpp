#include "../include/Text.h"

namespace GGE
{

    Text::Text()
    {
		textToPrint = "";
		textPrinted = "";
		visible = false;
    }


    Text::~Text()
    {

		if (textUVBufferID)
			glDeleteBuffers(1, &textUVBufferID);

		if (textVertexBufferID)
			glDeleteTextures(1, &textVertexBufferID);
    }

    void Text::initText(const char * _textName, Font *_textFont, Shader *_shader, std::string _textureRegionName, TextureAtlas* _textureAtlas, int _x, int _y)
    {
        if (_textureAtlas->regions.find(_textureRegionName) == _textureAtlas->regions.end())
        {
//            OS::getInstance()->alert("Can't initiate Text object", _textName);
        }
        else
        {
            textName = _textName;
            shader = _shader;
            textureAtlas = _textureAtlas;
            textureRegionName = _textureRegionName;
            position.x = _x;
            position.y = _y;
            font = _textFont;
            textSize = 96;


            glGenVertexArrays(1, &vao);

            glGenBuffers(1, &textVertexBufferID);
            glGenBuffers(1, &textUVBufferID);
        }
    }

    void Text::printText()
    {

        AtlasRegion *atlasRegion= static_cast<AtlasRegion*>(textureAtlas->regions.at(textureRegionName));
        glBindVertexArray(vao);

        if (textToPrint.compare(textPrinted) != 0) {
			textPrinted = textToPrint;
            int length = textPrinted.length();

            vertices.clear();
            UVs.clear();
            int letterX = 0;
            int y = 0;

            for ( int i=0 ; i<length ; i++ ){
                char character = textPrinted[i];
                FontChar fontChar = font->fontChars[(int)character];

                float uv_x = (float) (atlasRegion->x + (float) fontChar.x) / textureAtlas->width;
                float uv_y = (float) (atlasRegion->y + (float) fontChar.y) / textureAtlas->height;
                float yOffset = (float) fontChar.yoffset / font->textureWidth;
                float width = (float) fontChar.width / textureAtlas->width;
                float height = (float) fontChar.height / textureAtlas->height;

                float vertexWidth = (float) fontChar.width / font->textureWidth;
                float vertexHeight = (float) fontChar.height / font->textureHeight;

                glm::vec2 vertex_up_left    = glm::vec2( letterX                         , y - (vertexHeight * (textSize * scaleY)) - (yOffset * (textSize * scaleY)));
                glm::vec2 vertex_up_right   = glm::vec2( letterX + (vertexWidth * (textSize * scaleX)), y - (vertexHeight * textSize * scaleY) - (yOffset * (textSize * scaleY)));
                glm::vec2 vertex_down_right = glm::vec2( letterX + (vertexWidth * (textSize * scaleX)), y - (yOffset * (textSize * scaleY))   );
                glm::vec2 vertex_down_left  = glm::vec2( letterX                         , y - (yOffset * (textSize * scaleY))   );

                vertices.push_back(vertex_up_left   );
                vertices.push_back(vertex_down_left );
                vertices.push_back(vertex_up_right  );

                vertices.push_back(vertex_down_right);
                vertices.push_back(vertex_up_right);
                vertices.push_back(vertex_down_left);


                glm::vec2 uv_up_left    = glm::vec2( uv_x        , 1.0f- uv_y - height );
                glm::vec2 uv_up_right   = glm::vec2( uv_x + width, 1.0f- uv_y - height );
                glm::vec2 uv_down_right = glm::vec2( uv_x + width, 1.0f- uv_y );
                glm::vec2 uv_down_left  = glm::vec2( uv_x        , 1.0f- uv_y );

                UVs.push_back(uv_up_left   );
                UVs.push_back(uv_down_left );
                UVs.push_back(uv_up_right  );

                UVs.push_back(uv_down_right);
                UVs.push_back(uv_up_right);
                UVs.push_back(uv_down_left);
                letterX += (int) (vertexWidth * textSize * scaleX)
                 + (float)(((fontChar.xadvance) / font->textureWidth) * (textSize * scaleX)) ;
            }

            glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferID);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, textUVBufferID);
            glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
        }


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAtlas->textureID);

        unsigned int uniformID = glGetUniformLocation(shader->getShaderID(), "textTexture");
        glUniform1i(uniformID, 0);


        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferID);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, textUVBufferID);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        glDrawArrays(GL_TRIANGLES, 0, vertices.size() );


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    }

}

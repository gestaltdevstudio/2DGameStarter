#include "../include/GraphicsUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace GGE
{
    void GraphicsUtils::calculateViewportSize(Point &viewport)
    {
        if (viewport.x / FIXED_ASPECT_RATIO < viewport.y)
        {
            viewport.y = (int) (viewport.x / FIXED_ASPECT_RATIO);
        }
        else
        {
            viewport.x = (int) (viewport.y * FIXED_ASPECT_RATIO);
        }
    }

    GLuint GraphicsUtils::loadImage(const resourceFile *fileBuffer)
    {
		GLuint imageID;

        int width, height, format;

        stbi_set_flip_vertically_on_load(true);
        unsigned char * data = stbi_load_from_memory((unsigned char*)fileBuffer->file, fileBuffer->size, &width, &height, &format, STBI_rgb_alpha);

		glGenTextures(1,&imageID);
		glBindTexture(GL_TEXTURE_2D,imageID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
		data=NULL;

		return imageID;
	}

    GLuint GraphicsUtils::loadShaders(const resourceFile *vertexFileBuffer, const resourceFile *fragmentFileBuffer)
    {

        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compile Vertex Shader
        GLchar* tmpV = static_cast< GLchar*>(vertexFileBuffer->file);
        tmpV[vertexFileBuffer->size-1] = '\0';
        GLchar const* VertexSourcePointer = tmpV;

        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
//            LOGI("Error GL: %s" , &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        GLchar* tmpF = static_cast< GLchar*>(fragmentFileBuffer->file);
        tmpF[fragmentFileBuffer->size-1] = '\0';
        GLchar const * FragmentSourcePointer = tmpF;

        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
//            LOGI("Error GL: %s" , &FragmentShaderErrorMessage[0]);
        }

        // Link the program
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
//            LOGI("Error GL: %s" , &ProgramErrorMessage[0]);
        }

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

}

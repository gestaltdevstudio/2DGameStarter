#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

namespace GGE
{

    class Shader
    {
        public:
            virtual inline ~Shader()
            {
                if (shaderID)
                {
                    glDeleteProgram(shaderID);
                }
            }

            inline GLuint getShaderID() { return shaderID; }
            inline void setShaderID(GLuint _shaderID) { shaderID = _shaderID; }


        protected:
            std::vector<GLuint> uniformIDs;
            GLuint shaderID;

    };

}

#endif // SHADER_H_INCLUDED

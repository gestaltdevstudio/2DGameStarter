#include "../include/Font.h"


namespace GGE
{

    Font::Font()
    {
        charCount = 0;
    }

    Font::~Font()
    {

        free(fontChars);

    }

    void Font::loadFont(const resourceFile *fileBuffer)
    {

        char * file;
        file = strtok ( static_cast<char*>(fileBuffer->file),"\r\n");

        char lineHeader[130];
        int n;


            sscanf(file, "%s", lineHeader);

        if (strncmp(lineHeader, "info", strlen("info")) == 0)
        {
            char * aux = (char*) malloc(128 * sizeof(char));
            sscanf(file, "info face=\"%[^\"]\" size=%d bold=%d italic=%d charset=\"%[^\"]\" unicode=%d stretchH=%d smooth=%d aa=%d padding=%d,%d,%d,%d spacing=%d,%d",
                    aux, &originalSize, &n, &n, aux, &n, &n, &n, &n, &n, &n, &n, &n, &n, &n);
            free(aux);
        }
            file = strtok ( NULL,"\r\n");
            sscanf(file, "%s", lineHeader);

        if (strncmp(lineHeader, "common", strlen("common")) == 0)
        {
            sscanf(file, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d packed=%d", &n, &n, &textureWidth, &textureHeight, &n, &n);
        }

        file = strtok ( NULL,"\r\n");
        sscanf(file, "%s", lineHeader);

        file = strtok ( NULL,"\r\n");
        sscanf(file, "%s", lineHeader);

        if (strncmp(lineHeader, "chars", strlen("chars")) == 0)
        {
            sscanf(file, "chars count=%d", &charCount);
        }

        file = strtok ( NULL,"\r\n");

        if (charCount == 0)
        {
//            OS::getInstance()->alert("Error reading Font File", "");
        }
        else
        {

            fontChars = (FontChar*) malloc(128 * sizeof(FontChar));

            while( file != NULL)
            {
                FontChar fc;

                sscanf(file, "%s", lineHeader);
                if (strncmp(lineHeader, "char", strlen("char")) == 0)
                {

                    sscanf(file, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d",
                        &fc.id, &fc.x, &fc.y, &fc.width, &fc.height, &fc.xoffset, &fc.yoffset, &fc.xadvance, &fc.page, &fc.chnl);

                    fontChars[fc.id] = fc;

                }


                file = strtok ( NULL,"\r\n");

            }
        }

    }

}

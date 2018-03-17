#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#define DEBUG 1

#include "Definitions.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <zip.h>

namespace GGE {

struct resourceFile
{
    const char* fileName;
    char *file;
    uint64_t size;
    resourceFile()
    {
    }
    ~resourceFile()
    {
        if (file)
        {
            free(file);
            file = NULL;
        }
    }
};

    class Resources
	{
        public:

          static Resources *getInstance()
          {
             if (!Resources::instance) Resources::instance = new Resources();
             return Resources::instance;
          }
          void destroy();
          void loadZip(const char* zipFileName, const char* key);
          resourceFile* loadFile(const char* cfileName);
          resourceFile* loadCompressedFile(const char* cFileName);
          char *sgets( char * str, int num, char **input );

        private:
          Resources();

          static Resources *instance;
          zip   *zipFile;
#if defined(__ANDROID__)
          char* fileContent;
#endif
    };

}

#endif // RESOURCES_H_INCLUDED

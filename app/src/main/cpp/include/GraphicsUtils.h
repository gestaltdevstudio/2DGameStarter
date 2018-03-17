#ifndef GRAPHICSUTILS_H_INCLUDED
#define GRAPHICSUTILS_H_INCLUDED

#include <vector>
#include "Resources.h"

namespace GGE {

    class GraphicsUtils
	{
        public:

          static GLuint loadImage(const resourceFile *fileBuffer);

          static void calculateViewportSize(Point &windowSize);

          static GLuint loadShaders(const resourceFile *vertexFileBuffer, const resourceFile *fragmentFileBuffer);
    };

}

#endif // GRAPHICSUTILS_H_INCLUDED

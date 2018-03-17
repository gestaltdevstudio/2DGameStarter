#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <map>
#include <string>
#include "InputSystem.h"
#if defined(__ANDROID__)
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#elif defined(__WIN32__)
    #include "SO_GLFW.h"
#include <glad/glad.h>
#else
    #include "OS_GLFW.h"
#include <glad/glad.h>
#endif
#include "GraphicsUtils.h"
#include "Sprite.h"
#include "Entity.h"
#include "Text.h"
#include "Button.h"

namespace GGE {

    class GraphicsObject;
    class Text;


    class Graphics
	{
	public:
		static Graphics *getInstance()
		{
		   if (!Graphics::instance) Graphics::instance = new Graphics();
		   return Graphics::instance;
		}
			void            initGraphics();
		void	        onRender(float deltaTime);
		void	        onRenderDebug();
		void			onRenderUI();
		void			onRenderFinish();
		void			renderResize(Point windowSize);
		void            onSceneFinish();
		void            setupGL();
		void            addSprite(std::string name, Sprite* _object);
		void            addText(std::string name, Text* _text);
		void            removeText(std::string objectName);
		void            removeSprite(std::string objectName);
		void            addUIObject(std::string name, UIObject* _uiObject);
		void            removeUIObject(std::string objectName);
		void            doGraphics(float deltaTime);
		void            destroy();
		void            shakeCamera();
		void            cameraUpdate(float delta);
		void            setCameraPosition(float x, float y);
		glm::mat4        getProjectionMatrix();
		glm::mat4        getViewMatrix();
		void            drawCube(float size);
		void addEntity(Entity *_object);
		void removeEntity(std::string objectName);
		inline void setDebugMode(bool _debugMode) { debugMode = _debugMode; };
		inline void setANimationsPaused(bool _animationsPaused) { animationsPaused = _animationsPaused; };

		inline Point 	getViewportSize() { return viewportSize; };
	private:

		unsigned int vbo, vao, ebo;
		static Graphics *instance;
		Point viewportPosition, viewportSize;
		Graphics();

		std::map<std::string, Text*>            texts;
		std::map<std::string, Sprite*>         sprites;
		std::map<std::string, Entity*>          entities;
		std::map<std::string, UIObject*>        uiObjects;
		glm::mat4                               projectionMatrix;
		glm::mat4                               viewMatrix;

		Shader                                  *debugShader;
		bool                                    debugMode;
		bool                                    animationsPaused;


  };

}

#endif // GRAPHICS_H_INCLUDED

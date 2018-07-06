#include "../include/Graphics.h"

namespace GGE
{
    Graphics* Graphics::instance = 0;

	Graphics::Graphics()
	{
	    debugMode = false;
	    entities.clear();
	    uiObjects.clear();
	    texts.clear();
	    sprites.clear();
	    animationsPaused = false;
	    debugShader = 0;
	    vbo = ebo = 0;
	}

	void Graphics::destroy()
	{

		glFinish();

        if (vbo)
            glDeleteBuffers(1, &vbo);
        if (ebo)
            glDeleteBuffers(1, &ebo);

	    if (debugShader)
            delete debugShader;

	    delete instance;

        instance = NULL;
	}

	void Graphics::initGraphics()
	{
	     setupGL();
	     OS::getInstance()->resizeWindow();

        float fixedVertices[12] = {
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        unsigned int fixedIndexes[6] = {
            0, 1, 3,
            1, 2, 3
        };


        if (debugMode)
        {
            debugShader = new Shader();
            const resourceFile *rfVertShader = Resources::getInstance()->loadCompressedFile("debugShader.vert");
            const resourceFile *rfFragShader = Resources::getInstance()->loadCompressedFile("debugShader.frag");
            debugShader->setShaderID(GraphicsUtils::loadShaders(rfVertShader, rfFragShader));

            delete rfVertShader;
            delete rfFragShader;
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fixedVertices), fixedVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fixedIndexes), fixedIndexes, GL_STATIC_DRAW);

        glBindVertexArray(0);
	}

	void Graphics::renderResize(Point windowSize)
	{
        viewportSize = windowSize;
        GraphicsUtils::calculateViewportSize(viewportSize);

        viewportPosition.x = windowSize.x / 2 - viewportSize.x/2;
        viewportPosition.y = windowSize.y /2 - viewportSize.y/2;
        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        glEnable(GL_SCISSOR_TEST);
        glScissor(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        projectionMatrix = glm::ortho((float)- viewportSize.x/2, (float)viewportSize.x/2, (float)- viewportSize.y/2, (float)viewportSize.y/2, -1.0f, 1.0f);
        viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
	}

	glm::mat4 Graphics::getProjectionMatrix()
	{
	    return projectionMatrix;
	}

    glm::mat4 Graphics::getViewMatrix()
	{
	    return viewMatrix;
	}

	void Graphics::onRenderFinish()
	{
		glFlush();
	}
	void Graphics::onSceneFinish()
	{
	}

	void Graphics::setupGL()
	{
		glClearColor(0.00, 0.00, 0.00, 1.00);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void Graphics::doGraphics(float deltaTime)
	{
        onRender(deltaTime);
        onRenderUI();
        if (debugMode)
            onRenderDebug();
        onRenderFinish();
	}

    void Graphics::onRender(float deltaTime)
    {
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        projectionMatrix = glm::ortho((float)- viewportSize.x/2, (float)viewportSize.x/2, (float)- viewportSize.y/2, (float)viewportSize.y/2, -1.0f, 1.0f);
        glEnable(GL_SCISSOR_TEST);
        glScissor(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));


        GLenum err;


        std::map<std::string, Sprite*>::iterator iter;
        for (iter = sprites.begin(); iter != sprites.end(); ++iter)
        {
            Sprite *sprite = static_cast<Sprite*>(iter->second);
            if (sprite->isVisible()) {


                Drawable *drawable;
                if (!sprite->getCurrentAnimationName().empty())
                {
                    if (!animationsPaused) {
                        sprite->getCurrentAnimation()->update(deltaTime);
                    }
                    drawable = sprite->getCurrentAnimation()->getCurrentDrawable(sprite->getAnimationPlayMode());
                }
                else
                {
                    drawable = reinterpret_cast<Drawable*>(sprite);
                }

                glm::mat4 modelMatrix = glm::translate(glm::mat4(), glm::vec3((float) sprite->getX() * viewportSize.x / SCREEN_X,
                                                                              (float) sprite->getY() * viewportSize.y / SCREEN_Y,
                                                                              0))
                * glm::rotate((float)0.0, glm::vec3(0,0,1))
                * glm::scale(glm::vec3((float) (sprite->isFlipedX() ? -1 : 1) * sprite->getScaleX() * drawable->getAtlasRegion()->width * viewportSize.x / SCREEN_X,
                                       (float) (sprite->isFlipedY() ? -1 : 1) * sprite->getScaleY() * drawable->getAtlasRegion()->height * viewportSize.y / SCREEN_Y,
                                       0));
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUseProgram(drawable->getShader()->getShaderID());
                GLuint mvpID = glGetUniformLocation(drawable->getShader()->getShaderID(), "MVP");
                glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

                if (drawable->getTextureAtlas()->textureID)
                {

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, drawable->getTextureAtlas()->textureID);
                    unsigned int uniformID = glGetUniformLocation(drawable->getShader()->getShaderID(), "texture0");
                    glUniform1i(uniformID, 0);


                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }

                glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(
                   0,
                   3,
                   GL_FLOAT,
                   GL_FALSE,
                   0,
                   (void*)0
                );


                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, drawable->getUVBuffer());
                glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                );

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);

                glUseProgram(0);

            }
        }


        std::map<std::string, Text*>::iterator iterText;
        for (iterText = texts.begin(); iterText != texts.end(); ++iterText)
        {
            Text *text = static_cast<Text*>(iterText->second);
            if (text->isVisible())
            {

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(text->getPosition().x  * viewportSize.x / SCREEN_X,
                                                                               text->getPosition().y  * viewportSize.y / SCREEN_Y,
                                                                               0))
                * glm::rotate(glm::mat4(1), 0.0f, glm::vec3(0,0,1))
                * glm::scale(glm::mat4(1.0), glm::vec3(text->getFont()->getOriginalSize() * viewportSize.x / SCREEN_X,
                                                       text->getFont()->getOriginalSize() * viewportSize.y/SCREEN_Y,
                                                       0))
                * glm::scale(glm::mat4(1.0), glm::vec3(0.075f, 0.075f,0));
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUseProgram(text->getShader()->getShaderID());
                GLuint mvpID = glGetUniformLocation(text->getShader()->getShaderID(), "MVP");
                glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

                text->printText();
                glUseProgram(0);
            }

        }

        if ((err = glGetError()) != GL_NO_ERROR) {
            OS::getInstance()->alert("OpenGL ERR", to_string(err).c_str());
        }

    }

    void Graphics::onRenderDebug()
    {
        GLenum err;

        std::map<std::string, Entity*>::iterator iter;
        for (iter = entities.begin(); iter != entities.end(); ++iter)
        {
            Entity *entity = static_cast<Entity*>(iter->second);

            glm::mat4 modelMatrix = glm::translate(glm::mat4(), glm::vec3(entity->getPosition()->x * viewportSize.x / SCREEN_X,
                                                                          entity->getPosition()->y * viewportSize.y / SCREEN_Y,
                                                                          0))
            * glm::rotate((float)entity->getRotation(), glm::vec3(0,0,1))
            * glm::scale(glm::vec3(entity->getDimension()->x * viewportSize.x / SCREEN_X,
                                   entity->getDimension()->y * viewportSize.y / SCREEN_Y,
                                   0));
            glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

            glUseProgram(debugShader->getShaderID());
            GLuint mvpID = glGetUniformLocation(debugShader->getShaderID(), "MVP");
            glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(
               0,
               3,
               GL_FLOAT,
               GL_FALSE,
               0,
               (void*)0
            );

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glDisableVertexAttribArray(0);
            glUseProgram(0);
        }

        if ((err = glGetError()) != GL_NO_ERROR) {
            OS::getInstance()->alert("OpenGL ERR", to_string(err).c_str());
        }

    }

    void Graphics::onRenderUI()
    {
        GLenum err;

            std::map<std::string, UIObject*>::iterator iter;
        for (iter = uiObjects.begin(); iter != uiObjects.end(); ++iter)
        {
            UIObject *uiObject= reinterpret_cast<UIObject*>(iter->second);
            Drawable *drawable = uiObject->getDrawable();
            if (drawable->isVisible()) {


                glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3((float) uiObject->getPosition()->x * viewportSize.x / SCREEN_X,
                                                                               (float) uiObject->getPosition()->y * viewportSize.y / SCREEN_Y,
                                                                               0))
                * glm::rotate(glm::mat4(1), (float)drawable->getRotation(), glm::vec3(0,0,1))
                * glm::scale(glm::mat4(1.0), glm::vec3((float) drawable->getScaleX() * drawable->getAtlasRegion()->width * viewportSize.x / SCREEN_X,
                                                       (float) drawable->getScaleY() * drawable->getAtlasRegion()->height * viewportSize.y / SCREEN_Y,
                                                       0));
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUseProgram(drawable->getShader()->getShaderID());
                GLuint mvpID = glGetUniformLocation(drawable->getShader()->getShaderID(), "MVP");
                glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

                if (drawable->getTextureAtlas()->textureID)
                {

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, drawable->getTextureAtlas()->textureID);
                    unsigned int uniformID = glGetUniformLocation(drawable->getShader()->getShaderID(), "texture0");
                    glUniform1i(uniformID, 0);

                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }

                glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(
                   0,
                   3,
                   GL_FLOAT,
                   GL_FALSE,
                   0,
                   (void*)0
                );


                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, drawable->getUVBuffer());
                glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                );

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);

                glUseProgram(0);

            }
        }

        if ((err = glGetError()) != GL_NO_ERROR) {
            OS::getInstance()->alert("OpenGL ERR", to_string(err).c_str());
        }

    }

    void Graphics::addSprite(std::string objectName, Sprite *_object)
    {
         sprites.insert(std::make_pair(objectName, _object));
    }

    void Graphics::removeSprite(std::string objectName)
    {
        if (sprites.find(objectName) != sprites.end())
            sprites.erase(objectName);
    }

    void Graphics::addEntity(Entity *_object)
    {
        entities.insert(std::make_pair(_object->getName(), _object));
    }

    void Graphics::removeEntity(std::string objectName)
    {
        if (entities.find(objectName) != entities.end())
            entities.erase(objectName);
    }

    void Graphics::addText(std::string objectName, Text *_text)
    {
        texts.insert(std::make_pair(objectName, _text));
    }

    void Graphics::removeText(std::string objectName)
    {
        if (texts.find(objectName) != texts.end())
            texts.erase(objectName);
    }

    void Graphics::addUIObject(std::string objectName, UIObject *_uiObject)
    {
        uiObjects.insert(std::make_pair(objectName, _uiObject));
    }

    void Graphics::removeUIObject(std::string objectName)
    {
        if (uiObjects.find(objectName) != uiObjects.end())
            uiObjects.erase(objectName);
    }


}

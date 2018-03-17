#include "../include/GameView.h"

namespace GGE
{

    GameView::GameView(Screen *_screen, GameModel *_gameModel) : View(_screen)
    {
        gameModel = _gameModel;
    }

    void GameView::initView()
    {

        ta = new TextureAtlas();

        const resourceFile *rfMenuAtlas = Resources::getInstance()->loadCompressedFile("game.atlas");
        const resourceFile *rfMenuImage = Resources::getInstance()->loadCompressedFile("game.png");
        ta->loadTextureAtlas(rfMenuAtlas, rfMenuImage);
        delete rfMenuImage;
        delete rfMenuAtlas;

        sh = new Shader();
        const resourceFile *rfVertShader = Resources::getInstance()->loadCompressedFile("shader.vert");
        const resourceFile *rfFragShader = Resources::getInstance()->loadCompressedFile("shader.frag");
        sh->setShaderID(GraphicsUtils::loadShaders(rfVertShader, rfFragShader));
        if (!sh->getShaderID())
            return;

        delete rfVertShader;
        delete rfFragShader;

        playerSprite = new Sprite();
        Animation *animation = new Animation();
        std::vector<std::string> frames;
        frames.push_back("dogAnim2");
        frames.push_back("dogAnim3");
        frames.push_back("dogAnim4");
        frames.push_back("dogAnim5");
        frames.push_back("dogAnim6");
        frames.push_back("dogAnim7");
        frames.push_back("dogAnim8");
        frames.push_back("dogAnim9");
        animation->loadFrames(ta, sh, 1.0/8, frames);
        playerSprite->addAnimation("Idle", animation);
        playerSprite->setCurrentAnimationName("Idle", ANIM_LOOP_PINGPONG);
        playerSprite->setShader(sh);
        playerSprite->setIsVisible(true);
        playerSprite->setEntity(gameModel->getPlayer());
        Graphics::getInstance()->addSprite("test", playerSprite);

        s2 = new Sprite();
        s2->loadRegion("bone", ta);
        s2->setShader(sh);
        s2->setIsVisible(true);
        s2->setX(-200);
        s2->setY(300);
        Graphics::getInstance()->addSprite("test2", s2);


        exitButton = new Button();
        Drawable *d = new Drawable();
        d->loadRegion("exitButton", ta);
        d->setShader(sh);
        d->setIsVisible(true);
        exitButton->setDrawable(d);
        exitButton->setPosition(SCREEN_X/2 - exitButton->getDimension().x/2,
                            SCREEN_Y/2 - exitButton->getDimension().y/2);

        Graphics::getInstance()->addUIObject("Exit", exitButton);

#if defined(__ANDROID__)
        leftButton = new Button();
        Drawable *ld = new Drawable();
        ld->loadRegion("arrow", ta);
        ld->setShader(sh);
        ld->setIsVisible(true);
        leftButton->setDrawable(ld);
        leftButton->setPosition(-SCREEN_X/2 + 200,
                                -SCREEN_Y/2 + 100);

        Graphics::getInstance()->addUIObject("Left", leftButton);

        rightButton = new Button();
        Drawable *rd = new Drawable();
        rd->loadRegion("arrow", ta);
        rd->setShader(sh);
        rd->setIsVisible(true);
        rd->setRotation(180);
        rightButton->setDrawable(rd);
        rightButton->setPosition(-SCREEN_X/2 + 400,
                                -SCREEN_Y/2 + 100);

        Graphics::getInstance()->addUIObject("Right", rightButton);

        jumpButton = new Button();
        Drawable *jd = new Drawable();
        jd->loadRegion("arrow", ta);
        jd->setShader(sh);
        jd->setIsVisible(true);
        jd->setRotation(270);
        jumpButton->setDrawable(jd);
        jumpButton->setPosition(SCREEN_X/2 - 220,
                                -SCREEN_Y/2 + 100);

        Graphics::getInstance()->addUIObject("Jump", jumpButton);
#endif

        textSh = new Shader();
        const resourceFile *rfTextVertShader = Resources::getInstance()->loadCompressedFile("textShader.vert");
        const resourceFile *rfTextFragShader = Resources::getInstance()->loadCompressedFile("textShader.frag");
        textSh->setShaderID(GraphicsUtils::loadShaders(rfTextVertShader, rfTextFragShader));

        delete rfTextVertShader;
        delete rfTextFragShader;

        f = new Font();
        const resourceFile *fontFile = Resources::getInstance()->loadCompressedFile("pressStart2P.fnt");
        f->loadFont(fontFile);

        t = new Text();
        t->initText("Text", f, textSh, "pressStart2P", ta, 0, 0);
        t->setText("nsaefOUGH BFSoyjua.,PfAf");
        t->setPosition(-200, -100);
        Graphics::getInstance()->addText(t->getName(), t);
        t->setVisible(true);


        delete fontFile;


    }

    void GameView::step(float deltaTime)
    {

        Point mouseCoord = OS::getInstance()->getMouseCoord();

        playerSprite->setX(playerSprite->getEntity()->getPosition().x);
        playerSprite->setY(playerSprite->getEntity()->getPosition().y);

        if (playerSprite->getEntity()->getNextPosition().x < playerSprite->getEntity()->getPreviousPosition().x)
        {
            playerSprite->setFlipedX(true);
        }
        else if (playerSprite->getEntity()->getNextPosition().x > playerSprite->getEntity()->getPreviousPosition().x)
        {
            playerSprite->setFlipedX(false);
        }


        if (exitButton->isClicked(mouseCoord))
        {
            OS::getInstance()->setRunning(false);
        }
#if defined(__ANDROID__)
            if (leftButton->isClicked(OS::getInstance()->getInputCoord(LEFT_HAND)))
            {
                InputSystem::getInstance()->keyPressed(GGE_LEFT);
            } else {
                InputSystem::getInstance()->keyReleased(GGE_LEFT);
            }
            if (rightButton->isClicked(OS::getInstance()->getInputCoord(LEFT_HAND)))
            {
                InputSystem::getInstance()->keyPressed(GGE_RIGHT);
            } else {
                InputSystem::getInstance()->keyReleased(GGE_RIGHT);
            }
            if (jumpButton->isClicked(OS::getInstance()->getInputCoord(RIGHT_HAND)))
            {
                InputSystem::getInstance()->keyPressed(GGE_UP);
            } else {
                InputSystem::getInstance()->keyReleased(GGE_UP);
            }
#endif

        Graphics::getInstance()->doGraphics(deltaTime);

    }

    void GameView::finishView()
    {

        delete playerSprite;
        delete s2;
        delete sh;
        delete textSh;
        delete ta;
        delete f;
        delete t;
#if defined(__ANDROID__)
        delete leftButton;
        delete rightButton;
        delete jumpButton;
#endif
        delete exitButton;

    }

}

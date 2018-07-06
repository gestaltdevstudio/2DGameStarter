//
//  ViewController.m
//  Stater
//
//  Created by André Nardelli on 25/06/2018.
//

#import "ViewController.h"
#include <OpenGLES/ES3/gl.h>

@interface ViewController ()

@property (strong, nonatomic) EAGLContext *context;

@end

@implementation ViewController

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(goingToGainFocus)
                                                 name:UIApplicationWillEnterForegroundNotification object:nil];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(goingToLostFocus)
                                                 name:UIApplicationDidEnterBackgroundNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(finishApp)
                                                 name:UIApplicationWillTerminateNotification object:nil];
    
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    _ggeGame = new GGE::Game();
    
    [self setupGL];
}

- (void)viewDidLayoutSubviews {
    
}

-(void)goingToLostFocus{
    
    GGE::OS::getInstance()->setLostFocus(true);
}
-(void)goingToGainFocus{
    
    GGE::OS::getInstance()->setLostFocus(false);
}

-(void)finishApp{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    if (_ggeGame)
        _ggeGame->afterGameLoop();
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
//    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    [view bindDrawable];
    GGE::OS::getInstance()->width = (int32_t) view.drawableWidth;
    GGE::OS::getInstance()->height = (int32_t) view.drawableHeight;
    
    _ggeGame->beforeGameLoop();
    GGE::OS::getInstance()->setRunning(true);
    GGE::OS::getInstance()->setIsAlive(true);
    GGE::OS::getInstance()->setLostFocus(false);
    GGE::Point p;
    p.x = p.y = 0;
    GGE::OS::getInstance()->setInputCoord(0, p);
    GGE::OS::getInstance()->setInputCoord(1, p);
    GGE::OS::getInstance()->setTouchCoord(p);
    
    GGE::OS::getInstance()->resizeWindow();
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [self finishApp];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
//    if (GGE::OS::getInstance()->getRunning())
//    {
        _ggeGame->gameLoop();
//    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([touches count] < 1) return;
    CGPoint point = [[touches anyObject] locationInView:[self view]];
    GGE::Point p;
    p.x = point.x * ((GLKView *)self.view).contentScaleFactor;
    p.y = point.y * ((GLKView *)self.view).contentScaleFactor;
    GGE::OS::getInstance()->setTouchDown(p);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([touches count] < 1) return;
    CGPoint point = [[touches anyObject] locationInView:[self view]];
    GGE::Point p;
    p.x = point.x * ((GLKView *)self.view).contentScaleFactor;
    p.y = point.y * ((GLKView *)self.view).contentScaleFactor;
    GGE::OS::getInstance()->setTouchMove(p);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([touches count] < 1) return;
    CGPoint point = [[touches anyObject] locationInView:[self view]];
    GGE::Point p;
    p.x = point.x * ((GLKView *)self.view).contentScaleFactor;
    p.y = point.y * ((GLKView *)self.view).contentScaleFactor;
    GGE::OS::getInstance()->setTouchUp(p);
}

@end

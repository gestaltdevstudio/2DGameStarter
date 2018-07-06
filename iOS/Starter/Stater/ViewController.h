//
//  ViewController.h
//  Stater
//
//  Created by André Nardelli on 25/06/2018.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include <GGEGame.h>

@interface ViewController : GLKViewController

@property (nonatomic) GGE::Game *ggeGame;

@end


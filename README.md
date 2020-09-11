
# 2D Game Starter

2D Game Starter is a Initial Game Code created in C++ from scratch, with some basic LIBs - often presents in game engines: GLFW, GLAD, GLM, OpenAL, LibZIP, OGG and Vorbis.

## Main Concepts:

Class | Description | Files | Status | TODO
|--|--|--|--|--|
ResourcesManager | Read and manage any asset of the game and create the struct *fileResource* with attributes fileName, fileSize and content (void *) | [Resources.h](app/src/main/cpp/include/Resources.h)<br> [Resources.cpp](app/src/main/cpp/source/Resources.cpp) | Read files from filesystem by name  | Read files from zip files
GraphicsManager | Manager of the Images as Sprites, Debug Squares and UI objects  | [Graphics.h](app/src/main/cpp/include/Graphics.h)<br> [Graphics.cpp](app/src/main/cpp/source/Graphics.cpp)  | OpenGL renderer | DirectX renderer
InputSystem | Manager of the Input for Desktop | [InpusSystem.h](app/src/main/cpp/include/InputSystem.h)<br> [InpusSystem.cpp](app/src/main/cpp/source/InputSystem.cpp)  |  Done  | 
OS | Manager of the *Operating System* Window, Events, Gamepads integration, Touch detection... | **Desktop** <br> [OS_GLFW.h](app/src/main/cpp/include/OS_GLFW.h)  [OS_GLFW.cpp](app/src/main/cpp/source/OS_GLFW.cpp) <br> **Android** <br>  [OS_Android.h](app/src/main/cpp/include/OS_Android.h)  [OS_Android.cpp](app/src/main/cpp/source/android/OS_Android.cpp) <br> **iOS** <br> [OS_iOS.h](app/src/main/cpp/include/OS_iOS.h)  [OS_iOS.cpp](app/src/main/cpp/source/iOS/OS_iOS.cpp)  |  Working for Linux64, Win32, MacOS, Android and iOS  | 
Definitions | Global definitions, structures and some static functions | [Definitions.h](app/src/main/cpp/include/Definitions.h)  |   |
FontFile Parser | LibGDX Hiero Font Parser (Bitmap) | [Font.h](app/src/main/cpp/include/Font.h) <br>[Font.cpp](app/src/main/cpp/source/Font.cpp)   | Done  |
TextureAtlas Parser | LibGDX TextureAtlas File parser | [TextureAtlas.h](app/src/main/cpp/include/TextureAtlas.h) <br>[TextureAtlas.cpp](app/src/main/cpp/source/TextureAtlas.cpp)   | Done. Just limited to unrotated images |

## Building
**Linux & MacOS**<br>
Requires CMake 2.6+

    $ git clone https://github.com/gestaltdevstudio/2DGameStarter.git 
    $ cd 2DGameStarter/desktop
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

**Windows**<br>
Requires CMake 2.6+ and Visual Studio

 1. Clone the project from git and create the "build" directory

	    ...\> git clone https://github.com/gestaltdevstudio/2DGameStarter.git 
	    ...\> cd 2DGameStarter\desktop
		...\desktop> mkdir build

 2. Open CMake GUI
 3. Select the Source Code folder: "... \2DGameStarter\desktop"
 4. Select the Build folder: "... \2DGameStarter\desktop\build"
 5. Click on Configure button
 6. Select the Visual Studio version installed and click on Finish button
 7. Click on Generate Button
 8. When finished click on Open Project
 9. Right click on **Starter** project and select *Set as Startup Project*.
 10. Now you can run the project. 

**Android**<br>
Just import the Git project from *Android Studio*

**iOS**<br>
Requires the Xcode 9.4.1 (9F2000) +

    $ git clone https://github.com/gestaltdevstudio/2DGameStarter.git 
Just open the project: .../2DGameStarter/iOS/Starter/Starter.xcodeproj
  
## Motivation and Manifest 
This project was created to consolidate some ideas and concepts used in Gestalt Dev Studio games. You can start your game cloning this .git or just taking some classes, pieces of code, or concepts from here to make your own game engine, which I recommend, because this project represents one way of thinking, not the right way, or the way you'll be more confortable to work with!

Why so simple or why "to reinvent the wheel"? The main objective here is to generate conscience of what you are doing, or extending from other devs. Every little step you do results in a layer that you can enjoy and go up to the next level and so on.

## Special Thanks

 - Luiz Pestana and www.gamedev.com.br community
 - Edgard Damiani - writer of [Programação de Jogos Android](https://novatec.com.br/livros/programacao-jogos-android-2ed/)
 - [Anima](https://github.com/edamiani/Anima-Engine) team: Robson dos Santos, Regis Claus and Gregório Benatti
 - Luis Daher from [Cavylabs](http://http://www.cavylabs.com/)
 - Arnaud Masserann from www.opengl-tutorial.org
 - Joey De Vries from www.learnopengl.com

Also thank you for being here!
 
## Was this helpful?
Support my work -> https://www.patreon.com/nardelli

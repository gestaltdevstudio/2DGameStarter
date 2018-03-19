# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(GPG_SDK_PATH ${CMAKE_SOURCE_DIR}/../../../src/main/cpp/Google/gpg-cpp-sdk/android)

add_library(gpg_sdk STATIC IMPORTED)
set_target_properties(gpg_sdk PROPERTIES IMPORTED_LOCATION
    ${GPG_SDK_PATH}/lib/c++/${ANDROID_ABI}/libgpg.a)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror -Wno-deprecated-register -Wno-overloaded-virtual -Wno-shift-op-parentheses")

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
   "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(libs_DIR ${CMAKE_SOURCE_DIR}/../../../libs)

add_library(libzip SHARED IMPORTED)
set_target_properties(libzip PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libzip.so)

add_library(libbullet SHARED IMPORTED)
set_target_properties(libbullet PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libbullet.so)

add_library(libopenal SHARED IMPORTED)
set_target_properties(libopenal PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libopenal.so)

add_library(libogg SHARED IMPORTED)
set_target_properties(libogg PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libogg.so)

add_library(libvorbis SHARED IMPORTED)
set_target_properties(libvorbis PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libvorbis.so)

add_library(Starter SHARED android_main.cpp
source/android/OS_ANDROID.cpp
Google/StateManager.cpp
source/GGEGame.cpp
source/Graphics.cpp
source/GraphicsUtils.cpp
source/InputSystem.cpp
source/Resources.cpp
source/Sprite.cpp
source/Text.cpp
source/TextureAtlas.cpp
source/Animation.cpp
source/Button.cpp
source/Drawable.cpp
source/Font.cpp
source/GameModel.cpp
source/GameScreen.cpp
source/GameView.cpp
source/UIObject.cpp
)

target_include_directories(Starter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/conf
    ${ANDROID_NDK}/sources/android/native_app_glue)

include_directories(
        include/external/libzip
        include/external
        Google
        ${GPG_SDK_PATH}/include
        )



# add lib dependencies
target_link_libraries(Starter
    android
    gpg_sdk
    native_app_glue
    EGL
    GLESv3
#    GLESv2
    GLESv1_CM
    log
    libzip
    libopenal
    libogg
    libvorbis
)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
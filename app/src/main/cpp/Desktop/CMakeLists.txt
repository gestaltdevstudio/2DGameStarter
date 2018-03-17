cmake_minimum_required (VERSION 2.6)
project (Starter)

include_directories(../include ../include/external/libzip ../include/external)
file(GLOB SOURCES "../main.cpp" "../sources/*.cpp" "../sources/external/*.c")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

ADD_LIBRARY(glfw STATIC IMPORTED)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ../../libsLinux/debug/libglfw3.a)


ADD_LIBRARY(libzip STATIC IMPORTED)
SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ../../libsLinux/debug/libzip.a)


ADD_LIBRARY(libvorbisfile STATIC IMPORTED)
SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ../../libsLinux/debug/libvorbisfile.a)


ADD_LIBRARY(libvorbis STATIC IMPORTED)
SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ../../libsLinux/debug/libvorbis.a)


ADD_LIBRARY(libopenal STATIC IMPORTED)
SET_TARGET_PROPERTIES(libopenal PROPERTIES IMPORTED_LOCATION ../../libsLinux/debug/libopenal.a)


ADD_LIBRARY(libogg STATIC IMPORTED)
SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ../../libsLinux/debug/libogg.a)


add_executable(Starter ${SOURCES})
target_link_libraries (Starter glfw GL X11 Xxf86vm Xinerama Xcursor Xrandr dl libzip libvorbisfile libvorbis libopenal libogg z)


#./libsLinux/debug/libglfw3.a  ../libsLinux/debug/libzip.a
#                              ../libsLinux/debug/libvorbisfile.a ../libsLinux/debug/libvorbis.a ../libsLinux/debug/libopenal.a
#                               ../libsLinux/debug/libogg.a 

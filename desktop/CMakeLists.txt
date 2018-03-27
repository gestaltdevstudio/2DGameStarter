cmake_minimum_required (VERSION 2.6)
project (Starter)

set(CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../app/src/main/cpp")

file(COPY ${CPP_DIR}/conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
                 
if (MSVC)
  file(COPY ${CPP_DIR}/libsWin/libogg-0.dll ${CPP_DIR}/libsWin/LibVorbis.dll
        ${CPP_DIR}/libsWin/libZip.dll ${CPP_DIR}/libsWin/OpenAL32.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})      
endif (MSVC)

include_directories(${CPP_DIR}/include ${CPP_DIR}/include/external/libzip ${CPP_DIR}/include/external)
file(GLOB SOURCES "${CPP_DIR}/main.cpp" "${CPP_DIR}/source/*.cpp" "${CPP_DIR}/source/external/*.c")

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WIN32__")
endif (MSVC)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

ADD_LIBRARY(glfw STATIC IMPORTED)       
if (UNIX)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libglfw3.a)   
endif (UNIX)
   
if (MSVC)  
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/glfw3.lib)   
endif (MSVC)

ADD_LIBRARY(libzip STATIC IMPORTED)   
if (UNIX)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libzip.a) 
endif (UNIX)        
   
if (MSVC)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/libZip.lib) 
endif (MSVC)


if (UNIX)               
  ADD_LIBRARY(libvorbisfile STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libvorbisfile.a)    
endif (UNIX)

if (MSVC)              
  ADD_LIBRARY(zlib STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(zlib PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/zlib1.lib)    
endif (MSVC)


ADD_LIBRARY(libvorbis STATIC IMPORTED) 
if (UNIX)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libvorbis.a) 
endif (UNIX)

if (MSVC)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/LibVorbis.lib) 
endif (MSVC)

ADD_LIBRARY(libopenal STATIC IMPORTED)   
if (UNIX)
  SET_TARGET_PROPERTIES(libopenal PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libopenal.a)    
endif (UNIX)

if (MSVC)
  SET_TARGET_PROPERTIES(libopenal PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/OpenAL32.lib)    
endif (MSVC)


ADD_LIBRARY(libogg STATIC IMPORTED)   
if (UNIX)
  SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libogg.a) 
endif (UNIX)

if (MSVC)
SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/libogg-0.lib) 
endif (MSVC)              

add_executable(Starter ${SOURCES})

if (UNIX)
  target_link_libraries (Starter glfw GL X11 Xxf86vm Xinerama Xcursor Xrandr dl libzip libvorbisfile libvorbis libopenal libogg z)  
endif (UNIX)                  

if (MSVC)
  target_link_libraries (Starter glu32.lib opengl32.lib libzip libvorbis libopenal libogg zlib glfw )  
endif (MSVC)

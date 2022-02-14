cmake_minimum_required (VERSION 2.6)
project (Starter)

set(CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../app/src/main/cpp")

file(COPY ${CPP_DIR}/conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
                
IF(APPLE)

   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(OPENGL_LIBRARY OpenGL )
   FIND_LIBRARY(IOKIT_LIBRARY IOKit )
   FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo )
   FIND_LIBRARY(OPENAL_LIBRARY OpenAL )

   file(GLOB RSC_FILES "${CMAKE_BINARY_DIR}/conf/*")

ENDIF (APPLE)

 
if (MSVC)
  file(COPY ${CPP_DIR}/libsWin/libogg-0.dll ${CPP_DIR}/libsWin/LibVorbis.dll
        ${CPP_DIR}/libsWin/libZip.dll ${CPP_DIR}/libsWin/OpenAL32.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})      
endif (MSVC)

include_directories(${CPP_DIR}/include ${CPP_DIR}/include/external/libzip ${CPP_DIR}/include/external)
file(GLOB SOURCES "${CPP_DIR}/main.cpp" "${CPP_DIR}/source/*.cpp" "${CPP_DIR}/source/external/*.c")

if (UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif ()
 
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WIN32__")
endif (MSVC)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -DGGE_DESKTOP")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

ADD_LIBRARY(glfw STATIC IMPORTED)       
if (APPLE)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libglfw3.a)   
elseif (UNIX)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libglfw3.a)   
endif ()
   
if (MSVC)  
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/glfw3.lib)   
endif (MSVC)

ADD_LIBRARY(libzip STATIC IMPORTED)   
if (APPLE)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libzip.a) 
elseif (UNIX)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libzip.a) 
endif ()        
   
if (MSVC)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/libZip.lib) 
endif (MSVC)

if (APPLE)               
  ADD_LIBRARY(libvorbisfile STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libvorbisfile.a)    
elseif (UNIX)               
  ADD_LIBRARY(libvorbisfile STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libvorbisfile.a)    
endif ()

if (MSVC)              
  ADD_LIBRARY(zlib STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(zlib PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/zlib1.lib)    
endif (MSVC)


ADD_LIBRARY(libvorbis STATIC IMPORTED) 
if (APPLE)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libvorbis.a) 
elseif (UNIX)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libvorbis.a) 
endif ()

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
if (APPLE)
  SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libogg.a) 
elseif (UNIX)
  SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libogg.a) 
endif ()

if (MSVC)
SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/libogg-0.lib) 
endif (MSVC)              

if (APPLE)
add_executable(Starter MACOSX_BUNDLE ${SOURCES} ${RSC_FILES})
else()
add_executable(Starter ${SOURCES})
endif()

if (APPLE)
  target_link_libraries (Starter glfw ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${OPENAL_LIBRARY} z bz2 libzip libvorbisfile libvorbis libogg)  

set_source_files_properties(${RSC_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/conf)

elseif (UNIX)
  target_link_libraries (Starter glfw GL X11 Xxf86vm Xinerama Xcursor Xrandr dl libzip libvorbisfile libvorbis libopenal libogg z)  
endif ()                  

if (MSVC)
  target_link_libraries (Starter glu32.lib opengl32.lib libzip libvorbis libopenal libogg zlib glfw )  
endif (MSVC)

###############################################################################
# Declare the minimum cmake version required
###############################################################################
cmake_minimum_required(VERSION 2.8)

###############################################################################
# The name of out project
###############################################################################
project(LSSR)

###############################################################################
# Now you would normally declare additional linker and include directories.
# We don't need this at this moment, just to show you how its done
###############################################################################

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/ext/load3ds)
include_directories(${CMAKE_SOURCE_DIR}/ext/load3ds/math3d++)

link_directories("${CMAKE_SOURCE_DIR}/lib")

###############################################################################
# With cmake we can define additional compiler flags for different 
# configurations. CMAKE_CXX_FLAGS are for the default case. The debug config
# can be used to generate debug symbols for gdb. The release option uses
# special optimization flags
###############################################################################

set( CMAKE_CXX_FLAGS         "-O3 -Wall -g" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -msse3 -Wno-deprecated-declarations -Wno-unused -Wcpp" )
set( CMAKE_CXX_FLAGS_DEBUG   "-g -Wall" )

###############################################################################
# Find required libraries.Right now we need glut and OpenGL. The required 
# options forces this packages to be present. For non-mandatory packages you 
# can leave this flag out. The can then check if they were found by using
# IF(OpenGL_found) etc.
###############################################################################

FIND_PACKAGE(SDL REQUIRED)
if(SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
endif(SDL_FOUND)

FIND_PACKAGE(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
	include_directories(${SDLMIXER_INCLUDE_DIR})
endif(SDLMIXER_FOUND)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)

if(OpenGL_FOUND)
    link_directories(${OpenGL_INCLUDE_DIRS})
    include_directories(${OpenGL_LIBRARY_DIRS})
endif(OpenGL_FOUND)

set( QT_USE_QTOPENGL TRUE )
find_package( Qt4 )
if( QT4_FOUND )
    include_directories(${QT_INCLUDE_DIR})
    include_directories(${QT_QTGUI_INCLUDE_DIR})
    include_directories(${QT_QTOPENGL_INCLUDE_DIR})
    include_directories(${QT_QTCORE_INCLUDE_DIR})
    include_directories(${QT_QTXML_INCLUDE_DIR})
endif(QT4_FOUND)

###############################################################################
# Compile external libraries
###############################################################################

add_subdirectory(ext/load3ds)

###############################################################################
# Variable for the sources of the binary. In larger projects it is often a 
# good idea to use such variables because it is easy to add more source files
# and you can reuse it for several targets (maybe you want to build a library
# some day ;-)
###############################################################################

set(VIEWER_SOURCES
	io/Read3DS.cpp
	io/ReadPPM.cpp
	io/ReadTGA.cpp
	io/ReadXML.cpp
	io/ReadTXT.cpp
	io/TextureFactory.cpp
	io/TriangleMeshFactory.cpp
	io/Sound.cpp
	io/SoundManager.cpp
    io/joystick.cpp
    io/TextureManager.cpp
    view/MainWindow.cpp 
    view/Camera.cpp
    view/Galaxis.cpp
	view/RenderFrame.cpp
	view/HUD.cpp
    view/Menu.cpp
    control/keyboard.cpp
    control/joystickcontrol.cpp
    rendering/TriangleMesh.cpp
	rendering/Texture.cpp 
	rendering/Skybox.cpp
	rendering/Transformable.cpp
	rendering/TexturedMesh.cpp
    rendering/Asteorid.cpp
    rendering/Mini_Asteorid.cpp
    rendering/Fighter.cpp
    rendering/Bullet.cpp
    rendering/partikel/Emitter-Flug.cpp
    rendering/partikel/Partikel-Flug.cpp
    rendering/partikel/Emitter-Explosion.cpp
    rendering/partikel/Partikel-Explosion.cpp
    logic/Game.cpp
    Main.cpp
    math/Collision.cpp
)


###############################################################################
# META OBJECTS
###############################################################################

set(VIEWER_MOCS 
    view/RenderFrame.hpp
	view/Galaxis.hpp
	rendering/Asteorid.hpp)

qt_wrap_cpp(asteroids3D VIEWER_MOC_SRC ${VIEWER_MOCS})

###############################################################################
# The executable fpr our project
###############################################################################

add_executable(asteroids3D ${VIEWER_SOURCES} ${VIEWER_MOC_SRC} ${C3DSREADER_SOURCES})

###############################################################################
# External library dependencys
###############################################################################

message(STATUS "SDL_Mixer : " ${SDLMIXER_LIBRARY})

target_link_libraries(asteroids3D ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
target_link_libraries(asteroids3D c3dsreader) 
target_link_libraries(asteroids3D ${SDL_LIBRARY})
target_link_libraries(asteroids3D ${SDLMIXER_LIBRARY})
target_link_libraries(asteroids3D ${QGLVIEWER_LIBRARY_RELEASE})
target_link_libraries(asteroids3D ${QT_QTCORE_LIBRARIES})
target_link_libraries(asteroids3D ${QT_QTGUI_LIBRARIES})
target_link_libraries(asteroids3D ${QT_QTOPENGL_LIBRARIES})
target_link_libraries(asteroids3D ${QT_QTXML_LIBRARIES})



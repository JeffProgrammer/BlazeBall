cmake_minimum_required (VERSION 3.0)
include (CheckIncludeFiles)
project (DifViewerGame C CXX)

if (CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
	message (" * C++ compiler: Emscripten")
	set (EMSCRIPTEN 1)
else ()
	message (" * C++ cimpiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()


if (EMSCRIPTEN)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2 -s USE_PTHREADS=1")
	
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_PTHREADS=1")

	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -O1 -o test.html")
elseif (MSVC)
	# Force multiple processors to be used to compile visual studio builds.
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else ()
	# Force C++14 standard on compilers, MSVC already does this by default
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# Enable Static Linking the C++ ABI directly into the executables and libraries
if (MSVC)
	#---BEGIN COPY FROM BULLET CMAKE---
	#We statically link to reduce dependancies
	FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO )
		IF(${flag_var} MATCHES "/MD")
			STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MD")
		IF(${flag_var} MATCHES "/MDd")
			STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MDd")
	ENDFOREACH(flag_var)
	#---END COPY FROM BULLET CMAKE---
endif()

# http://stackoverflow.com/a/25286420/214063
macro(ADD_FRAMEWORK appname fwname)
	# Since CMake 2.8.12, this is really easy.
	target_link_libraries(${appname} "-framework ${fwname}")
endmacro(ADD_FRAMEWORK)

#------------------------------------------------------------------------------
# Third party projects
#------------------------------------------------------------------------------

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/zlib.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/jpeg.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mng.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tga.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dif.txt)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bullet" Bullet)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/librocket/build" LibRocket)

if (NOT EMSCRIPTEN)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2" SDL2)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/enet" ENet)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/epoxy.txt)
endif()
if (MSVC)
	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

#------------------------------------------------------------------------------
# Main Project Executable
#------------------------------------------------------------------------------

# Includes for main project
set (DIFVIEWERGAME_INCLUDEDIR
	source
	thirdparty/bullet
	thirdparty/bullet/src
	thirdparty/glm
	thirdparty/jpeg
	thirdparty/tgalib
	thirdparty/mng
	thirdparty/zlib
	thirdparty/SDL2/include
	thirdparty/srutil
	thirdparty
	dif/include
	thirdparty/assimp/include
	thirdparty/rapidjson/include
	thirdparty/enet/include
	thirdparty/enetpp/include
	thirdparty/compat
	thirdparty/librocket/Include
)

# Main project source
set (DIFVIEWERGAME_SRC
	# Main
	source/main/gameState.cpp
	source/main/gameState.h
	source/main/main.cpp

	# Base folder
	source/base/config.cpp
	source/base/config.h
	source/base/event.h
	source/base/io.cpp
	source/base/io.h
	source/base/mathFunctions.cpp
	source/base/mathFunctions.h
	source/base/math.h
	source/base/mathTypes.h
	source/base/tree.h
	source/base/types.h
	source/base/math/triangle.h

	# Game folder
	source/game/camera.cpp
	source/game/camera.h
	source/game/gameObject.cpp
	source/game/gameObject.h
	source/game/gameInterior.cpp
	source/game/gameInterior.h
	source/game/movement.h
	source/game/renderedObject.cpp
	source/game/renderedObject.h
	source/game/scriptObject.h
	source/game/scriptObject.cpp
	source/game/shape.cpp
	source/game/shape.h
	source/game/skybox.cpp
	source/game/skybox.h
	source/game/sphere.cpp
	source/game/sphere.h
	source/game/world.cpp
	source/game/world.h

	# Gui
	source/gui/renderInterface.cpp
	source/gui/renderInterface.h
	source/gui/systemInterface.cpp
	source/gui/systemInterface.h
	source/gui/elements/elementWorldView.cpp
	source/gui/elements/elementWorldView.h

	# Graphics folder
	source/graphics/gl.h

	# Networking
	source/network/bitstream.h
	source/network/charStream.h
	source/network/client.cpp
	source/network/client.h
	source/network/clientConnection.cpp
	source/network/clientConnection.h
	source/network/network.h
	source/network/netObject.cpp
	source/network/netObject.h
	source/network/server.cpp
	source/network/server.h

	# Net events
	source/network/event/netEvent.cpp
	source/network/event/netEvent.h
	source/network/event/netClientEvent.cpp
	source/network/event/netClientEvent.h
	source/network/event/netServerEvent.cpp
	source/network/event/netServerEvent.h

	# Physics folder
	source/physics/physicsBody.cpp
	source/physics/physicsBody.h
	source/physics/physicsEngine.cpp
	source/physics/physicsEngine.h
	source/physics/physicsSphere.cpp
	source/physics/physicsSphere.h

	# Bullet physics
	source/physics/bullet/btPhysicsBody.cpp
	source/physics/bullet/btPhysicsBody.h
	source/physics/bullet/btPhysicsEngine.cpp
	source/physics/bullet/btPhysicsEngine.h
	source/physics/bullet/btPhysicsInterior.cpp
	source/physics/bullet/btPhysicsInterior.h
	source/physics/bullet/btPhysicsSphere.cpp
	source/physics/bullet/btPhysicsSphere.h
	source/physics/bullet/btDebugDrawer.cpp
	source/physics/bullet/btDebugDrawer.h

	# Platform folder
	source/platform/platform.h
	source/platform/platformEvent.h
	source/platform/platformGL.h
	source/platform/platformTimer.cpp
	source/platform/platformTimer.h
	source/platform/platformWindow.h

	# SDL folder
	source/platform/SDL/SDLConfig.h
	source/platform/SDL/SDLEvent.cpp
	source/platform/SDL/SDLEvent.h
	source/platform/SDL/SDLPlatform.cpp
	source/platform/SDL/SDLPlatform.h
	source/platform/SDL/SDLTimer.h
	source/platform/SDL/SDLWindow.cpp
	source/platform/SDL/SDLWindow.h

	# Render folder
	source/render/interiorRender.cpp
	source/render/material.cpp
	source/render/material.h
	source/render/modelManager.cpp
	source/render/modelManager.h
	source/render/renderer.cpp
	source/render/renderer.h
	source/render/renderInfo.h
	source/render/renderWorld.cpp
	source/render/renderWorld.h
	source/render/shader.cpp
	source/render/shader.h
	source/render/shaderImpl.cpp
	source/render/util.h

	# Bitmap folder
	source/render/bitmap/jpegSupport.cpp
	source/render/bitmap/jpegSupport.h
	source/render/bitmap/mngSupport.cpp
	source/render/bitmap/mngSupport.h
	source/render/bitmap/tgaSupport.cpp
	source/render/bitmap/tgaSupport.h

	# Texture folder
	source/render/texture/bitmapTexture.cpp
	source/render/texture/bitmapTexture.h
	source/render/texture/cubeMapTexture.cpp
	source/render/texture/cubeMapTexture.h
	source/render/texture/cubeMapFramebufferTexture.cpp
	source/render/texture/cubeMapFramebufferTexture.h
	source/render/texture/texture.cpp
	source/render/texture/texture.h

	# ScriptEngine
	source/scriptEngine/abstractClassRep.h
	source/scriptEngine/abstractClassRep.cpp
	source/scriptEngine/concreteClassRep.h
	source/scriptEngine/scriptTypes.inl

	# Shaders folder
	source/shaders/defaultF.glsl
	source/shaders/defaultV.glsl
	source/shaders/guiF.glsl
	source/shaders/guiV.glsl
	source/shaders/interiorF.glsl
	source/shaders/interiorV.glsl
	source/shaders/modelF.glsl
	source/shaders/modelV.glsl
	source/shaders/skyboxF.glsl
	source/shaders/skyboxV.glsl
	source/shaders/sphereF.glsl
	source/shaders/sphereV.glsl
)

if (APPLE)
	set (DIFVIEWERGAME_SRC
		${DIFVIEWERGAME_SRC}

		# OpenGL specific things
		source/graphics/glMac.h
		source/graphics/glMac.mm
	)
endif()

set (DIFVIEWERGAME_LIBRARIES
	# Bullet
	BulletCollision
	BulletDynamics
	BulletSoftBody
	LinearMath

	# JPEG
	Jpeg

	# MNG
	Mng

	# TGALib
	TGALib

	# DIF
	Dif

	# Librocket things
	freetype
	RocketCore
	RocketControls
	RocketDebugger
)
if (NOT EMSCRIPTEN)
set (DIFVIEWERGAME_INCLUDEDIR
	${DIFVIEWERGAME_INCLUDEDIR}

	# Epoxy
	thirdparty/epoxy/include
)
set (DIFVIEWERGAME_SRC
	${DIFVIEWERGAME_SRC}

	# OpenGL Implementations with Epoxy
	source/graphics/glImplementation.cpp
	source/graphics/glImplementation.h
	source/graphics/GL21/gl21.cpp
	source/graphics/GL21/gl21.h
	source/graphics/GL33/gl33.cpp
	source/graphics/GL33/gl33.h
)
set (DIFVIEWERGAME_LIBRARIES
	${DIFVIEWERGAME_LIBRARIES}
	#SDL2
	SDL2-static

	#Enet
	enet

	# epoxy for now
	Epoxy
)
endif ()

if (WIN32)
set (DIFVIEWERGAME_LIBRARIES
	${DIFVIEWERGAME_LIBRARIES}

	Zlib

	Kernel32
	User32
	OpenGL32

	# These are for enet sockets
	Winmm
	ws2_32

	# Assimp
	debug ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/bin/x86/assimpd.lib
	optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/bin/x86/assimp.lib
)
elseif (APPLE)
set (DIFVIEWERGAME_LIBRARIES
	${DIFVIEWERGAME_LIBRARIES}
	z

	# Assimp
	debug ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/bin/x64/Debug/libassimpd.a
	optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/bin/x64/Release/libassimp.a
)
endif()

include_directories(include ${DIFVIEWERGAME_INCLUDEDIR})

#if (WIN32)
	#add_executable(Game WIN32 ${DIFVIEWERGAME_SRC})
#else
	add_executable(Game ${DIFVIEWERGAME_SRC})
#endif()

source_group("main"                  REGULAR_EXPRESSION main/.*)
source_group("render"                REGULAR_EXPRESSION render/.*)
source_group("base"                  REGULAR_EXPRESSION base/.*)
source_group("base\\math"            REGULAR_EXPRESSION base/math/.*)
source_group("render\\bitmap"        REGULAR_EXPRESSION bitmap/.*)
source_group("game"                  REGULAR_EXPRESSION game/.*)
source_group("objects"               REGULAR_EXPRESSION objects/.*)
source_group("game\\physics"         REGULAR_EXPRESSION physics/.*)
source_group("game\\physics\\bullet" REGULAR_EXPRESSION bullet/.*)
source_group("platform"              REGULAR_EXPRESSION platform/.*)
source_group("platform\\SDL"         REGULAR_EXPRESSION platform/SDL/.*)
source_group("scriptEngine"          REGULAR_EXPRESSION scriptEngine/.*)
source_group("render\\texture"       REGULAR_EXPRESSION render/texture/.*)
source_group("assets\\shaders"       REGULAR_EXPRESSION shaders/.*)
source_group("assets\\scripts"       REGULAR_EXPRESSION scripts/.*)
source_group("network"               REGULAR_EXPRESSION network/.*)
source_group("network\\event"        REGULAR_EXPRESSION network/event/.*)
source_group("gui"                   REGULAR_EXPRESSION gui/.*)
source_group("gui\\elements"         REGULAR_EXPRESSION gui/elements/.*)
source_group("graphics"              REGULAR_EXPRESSION graphics/.*)
source_group("graphics\\GL21"        REGULAR_EXPRESSION graphics/GL21/.*)
source_group("graphics\\GL33"        REGULAR_EXPRESSION graphics/GL33/.*)

set_property(TARGET Game PROPERTY COMPILE_FLAGS "")
target_link_libraries(Game ${DIFVIEWERGAME_LIBRARIES})
target_compile_definitions(Game PUBLIC GLEW_STATIC GLM_FORCE_RADIANS ROCKET_STATIC_LIB)

if (MSVC)
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

if (APPLE AND NOT EMSCRIPTEN)
	ADD_FRAMEWORK(Game Cocoa)
	ADD_FRAMEWORK(Game Carbon)
	ADD_FRAMEWORK(Game OpenGL)
	ADD_FRAMEWORK(Game IOKit)
	ADD_FRAMEWORK(Game ForceFeedback)
endif()

set (BUILD_DIR_FILES
	assets/DefaultNormal.png
	assets/DefaultSpec.png
	assets/noise.jpg

	assets/cube.dae
	assets/grid.png
	assets/grid.normal.png
	assets/grid.alpha.png

	source/shaders/interiorF.glsl
	source/shaders/interiorV.glsl
	source/shaders/modelV.glsl
	source/shaders/modelF.glsl

	source/scripts/main.chai
	source/levels/test.json
)

add_custom_command(TARGET Game PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Game>)

add_custom_command(TARGET Game PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/source/shaders $<TARGET_FILE_DIR:Game>)

add_custom_command(TARGET Game PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/source/scripts $<TARGET_FILE_DIR:Game>)

add_custom_command(TARGET Game PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/source/levels $<TARGET_FILE_DIR:Game>)

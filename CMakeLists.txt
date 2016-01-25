cmake_minimum_required (VERSION 3.0)
include (CheckIncludeFiles)
project (DifViewerGame C CXX)

if (NOT MSVC)
	# Force C++14 standard on compilers, MSVC already does this by default
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
	# Force multiple processors to be used to compile visual studio builds.
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
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
# Zlib
#------------------------------------------------------------------------------

set (ZLIB_SRC
	thirdparty/zlib/adler32.c
	thirdparty/zlib/compress.c
	thirdparty/zlib/crc32.c
	thirdparty/zlib/crc32.h
	thirdparty/zlib/deflate.c
	thirdparty/zlib/deflate.h
	thirdparty/zlib/gzclose.c
	thirdparty/zlib/gzguts.h
	thirdparty/zlib/gzlib.c
	thirdparty/zlib/gzread.c
	thirdparty/zlib/gzwrite.c
	thirdparty/zlib/infback.c
	thirdparty/zlib/inffast.c
	thirdparty/zlib/inffast.h
	thirdparty/zlib/inffixed.h
	thirdparty/zlib/inflate.c
	thirdparty/zlib/inflate.h
	thirdparty/zlib/inftrees.c
	thirdparty/zlib/inftrees.h
	thirdparty/zlib/trees.c
	thirdparty/zlib/trees.h
	thirdparty/zlib/uncompr.c
	thirdparty/zlib/zconf.h
	thirdparty/zlib/zlib.h
	thirdparty/zlib/zutil.c
	thirdparty/zlib/zutil.h
)

add_library(Zlib STATIC ${ZLIB_SRC})
set_property (TARGET Zlib APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zlib")

#------------------------------------------------------------------------------
# JPEG Library
#------------------------------------------------------------------------------

set (JPEG_SRC
	thirdparty/jpeg/jaricom.c
	thirdparty/jpeg/jcapimin.c
	thirdparty/jpeg/jcapistd.c
	thirdparty/jpeg/jcarith.c
	thirdparty/jpeg/jccoefct.c
	thirdparty/jpeg/jccolor.c
	thirdparty/jpeg/jcdctmgr.c
	thirdparty/jpeg/jchuff.c
	thirdparty/jpeg/jcinit.c
	thirdparty/jpeg/jcmainct.c
	thirdparty/jpeg/jcmarker.c
	thirdparty/jpeg/jcmaster.c
	thirdparty/jpeg/jcomapi.c
	thirdparty/jpeg/jcparam.c
	thirdparty/jpeg/jcprepct.c
	thirdparty/jpeg/jcsample.c
	thirdparty/jpeg/jctrans.c
	thirdparty/jpeg/jdapimin.c
	thirdparty/jpeg/jdapistd.c
	thirdparty/jpeg/jdarith.c
	thirdparty/jpeg/jdatadst.c
	thirdparty/jpeg/jdatasrc.c
	thirdparty/jpeg/jdcoefct.c
	thirdparty/jpeg/jdcolor.c
	thirdparty/jpeg/jddctmgr.c
	thirdparty/jpeg/jdhuff.c
	thirdparty/jpeg/jdinput.c
	thirdparty/jpeg/jdmainct.c
	thirdparty/jpeg/jdmarker.c
	thirdparty/jpeg/jdmaster.c
	thirdparty/jpeg/jdmerge.c
	thirdparty/jpeg/jdpostct.c
	thirdparty/jpeg/jdsample.c
	thirdparty/jpeg/jdtrans.c
	thirdparty/jpeg/jerror.c
	thirdparty/jpeg/jfdctflt.c
	thirdparty/jpeg/jfdctfst.c
	thirdparty/jpeg/jfdctint.c
	thirdparty/jpeg/jidctflt.c
	thirdparty/jpeg/jidctfst.c
	thirdparty/jpeg/jidctint.c
	thirdparty/jpeg/jquant1.c
	thirdparty/jpeg/jquant2.c
	thirdparty/jpeg/jutils.c
	thirdparty/jpeg/jmemmgr.c
)

if (APPLE)
	set(JPEG_SRC ${JPEG_SRC}
		thirdparty/jpeg/jmemnobs.c
	)
elseif (MSVC)	
	set(JPEG_SRC ${JPEG_SRC}
		thirdparty/jpeg/jmemansi.c
	)	
endif()

add_library(Jpeg STATIC ${JPEG_SRC})
set_property (TARGET Jpeg APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/jpeg")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

#------------------------------------------------------------------------------
# MNG Library
#------------------------------------------------------------------------------

set (MNG_SRC
	thirdparty/mng/libmng_callback_xs.c
	thirdparty/mng/libmng_chunk_descr.c
	thirdparty/mng/libmng_chunk_descr.h
	thirdparty/mng/libmng_chunk_io.c
	thirdparty/mng/libmng_chunk_io.h
	thirdparty/mng/libmng_chunk_prc.c
	thirdparty/mng/libmng_chunk_prc.h
	thirdparty/mng/libmng_chunk_xs.c
	thirdparty/mng/libmng_cms.c
	thirdparty/mng/libmng_cms.h
	thirdparty/mng/libmng_conf.h
	thirdparty/mng/libmng_data.h
	thirdparty/mng/libmng_display.c
	thirdparty/mng/libmng_display.h
	thirdparty/mng/libmng_dither.c
	thirdparty/mng/libmng_dither.h
	thirdparty/mng/libmng_error.c
	thirdparty/mng/libmng_error.h
	thirdparty/mng/libmng_filter.c
	thirdparty/mng/libmng_filter.h
	thirdparty/mng/libmng_hlapi.c
	thirdparty/mng/libmng_jpeg.c
	thirdparty/mng/libmng_jpeg.h
	thirdparty/mng/libmng_memory.h
	thirdparty/mng/libmng_object_prc.c
	thirdparty/mng/libmng_object_prc.h
	thirdparty/mng/libmng_objects.h
	thirdparty/mng/libmng_pixels.c
	thirdparty/mng/libmng_pixels.h
	thirdparty/mng/libmng_prop_xs.c
	thirdparty/mng/libmng_read.c
	thirdparty/mng/libmng_read.h
	thirdparty/mng/libmng_trace.c
	thirdparty/mng/libmng_trace.h
	thirdparty/mng/libmng_types.h
	thirdparty/mng/libmng_write.c
	thirdparty/mng/libmng_write.h
	thirdparty/mng/libmng_zlib.c
	thirdparty/mng/libmng_zlib.h
	thirdparty/mng/libmng.h
)

add_library(Mng STATIC ${MNG_SRC})
target_link_libraries(Mng Jpeg Zlib)
set_property (TARGET Mng APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/jpeg")

#------------------------------------------------------------------------------
# Epoxy Library
#------------------------------------------------------------------------------

set (EPOXY_SRC
	thirdparty/epoxy/src/dispatch_common.c
	thirdparty/epoxy/src/dispatch_common.h
	thirdparty/epoxy/src/gl_generated_dispatch.c
)
if (WIN32)
	#WGL
	set (EPOXY_SRC 
		${EPOXY_SRC} 
		thirdparty/epoxy/src/dispatch_wgl.c
		thirdparty/epoxy/src/wgl_generated_dispatch.c
	)
elseif (UNIX AND NOT APPLE)
	#GLX
	set (EPOXY_SRC 
		${EPOXY_SRC} 
		thirdparty/epoxy/src/dispatch_glx.c
		thirdparty/epoxy/src/glx_generated_dispatch.c
	)
endif()
add_library(Epoxy STATIC ${EPOXY_SRC})
if (WIN32)
	target_compile_definitions(Epoxy PUBLIC BUILD_WGL)
elseif (UNIX AND NOT APPLE)
	target_compile_definitions(Epoxy PUBLIC BUILD_GLX)
endif()
set_property(TARGET Epoxy APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/epoxy/include")

#------------------------------------------------------------------------------
# DIF Library
#------------------------------------------------------------------------------

set (DIF_SRC
	############################################################################
	# Sources
	############################################################################

	# Base
	dif/src/dif/base/io.cpp

	# Objects
	dif/src/dif/objects/aiSpecialNode.cpp
	dif/src/dif/objects/dif.cpp
	dif/src/dif/objects/forceField.cpp
	dif/src/dif/objects/gameEntity.cpp
	dif/src/dif/objects/interior.cpp
	dif/src/dif/objects/interiorPathFollower.cpp
	dif/src/dif/objects/staticMesh.cpp
	dif/src/dif/objects/trigger.cpp
	dif/src/dif/objects/vehicleCollision.cpp

	############################################################################
	# Headers
	############################################################################

	# Base
	dif/include/dif/base/color.h
	dif/include/dif/base/io.h
	dif/include/dif/base/point2.h
	dif/include/dif/base/point3.h
	dif/include/dif/base/types.h

	# Objects
	dif/include/dif/objects/aiSpecialNode.h
	dif/include/dif/objects/dif.h
	dif/include/dif/objects/forceField.h
	dif/include/dif/objects/gameEntity.h
	dif/include/dif/objects/interior.h
	dif/include/dif/objects/interiorPathFollower.h
	dif/include/dif/objects/staticMesh.h
	dif/include/dif/objects/trigger.h
	dif/include/dif/objects/vehicleCollision.h
)

# Create the library
add_library(Dif STATIC ${DIF_SRC})
set_property(TARGET Dif APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dif/include")

#------------------------------------------------------------------------------
# Main Project Executable
#------------------------------------------------------------------------------

# Includes for main project
set (DIFVIEWERGAME_INCLUDEDIR
	source
	thirdparty/bullet
	thirdparty/bullet/src
	thirdparty/glm/
	thirdparty/jpeg
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
	thirdparty/chaiscript/include
	thirdparty/epoxy/include
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
	
	# Graphics folder
	source/graphics/gl.h
	source/graphics/glImplementation.cpp
	source/graphics/glImplementation.h
	source/graphics/GL21/gl21.cpp
	source/graphics/GL21/gl21.h
	source/graphics/GL33/gl33.cpp
	source/graphics/GL33/gl33.h
	
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
	source/scriptEngine/scriptEngine.cpp
	source/scriptEngine/scriptEngine.h
	source/scriptEngine/scriptEngine.inl
	source/scriptEngine/scriptFunctions.h
	source/scriptEngine/scriptMath.h

	# Shaders folder
	source/shaders/defaultF.glsl
	source/shaders/defaultV.glsl
	source/shaders/interiorF.glsl
	source/shaders/interiorV.glsl
	source/shaders/modelF.glsl
	source/shaders/modelV.glsl
	source/shaders/skyboxF.glsl
	source/shaders/skyboxV.glsl
	source/shaders/sphereF.glsl
	source/shaders/sphereV.glsl
	
	# Scripts folder
	source/scripts/mainClient.chai
	source/scripts/mainServer.chai
)

#------------------------------------------------------------------------------
# SDL
#------------------------------------------------------------------------------

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2" SDL2)

#------------------------------------------------------------------------------
# Bullet Physics Library
#------------------------------------------------------------------------------

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bullet" Bullet)

#------------------------------------------------------------------------------
# ENet Networking Library
#------------------------------------------------------------------------------

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/enet" ENet)
if (MSVC)
	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

#------------------------------------------------------------------------------

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

	# DIF
	Dif
	
	#SDL2
	SDL2-static
	
	#Enet
	enet
)

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
	
	# epoxy for now
	Epoxy
)
else()
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
source_group("graphics"              REGULAR_EXPRESSION graphics/.*)
source_group("graphics\\GL21"        REGULAR_EXPRESSION graphics/GL21/.*)
source_group("graphics\\GL33"        REGULAR_EXPRESSION graphics/GL33/.*)

set_property(TARGET Game PROPERTY COMPILE_FLAGS "")
target_link_libraries(Game ${DIFVIEWERGAME_LIBRARIES})
target_compile_definitions(Game PUBLIC GLEW_STATIC GLM_FORCE_RADIANS)

if (MSVC)
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

if (APPLE)
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

cmake_minimum_required (VERSION 3.0)
include (CheckIncludeFiles)
project (DifViewerGame C CXX)

# Force C++11 standard on compilers
if (NOT MSVC)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Enable Static Linking the C++ ABI directly into the executables and libraries
if (MSVC)
	SET(CMAKE_REQUIRED_LINKER_FLAGS "/SAFESEH:NO")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_RelWithDebInfo "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_RelWithDebInfo "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# http://stackoverflow.com/a/25286420/214063
macro(ADD_FRAMEWORK appname fwname)
	# Since CMake 2.8.12, this is really easy.
	target_link_libraries(${appname} "-framework ${fwname}")
endmacro(ADD_FRAMEWORK)

#------------------------------------------------------------------------------
# Bullet Physics Library
#------------------------------------------------------------------------------

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bullet" Bullet)

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

if (MSVC)
	#------------------------------------------------------------------------------
	# GLEW Library
	#------------------------------------------------------------------------------

	add_library(GLEW STATIC thirdparty/glew/src/glew.c)
	set_property(TARGET GLEW APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glew/include")
	target_compile_definitions(GLEW PUBLIC GLEW_STATIC)

	target_link_libraries(GLEW OpenGL32)
endif()

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
	thirdparty/glew/include
	thirdparty/glm/
	thirdparty/jpeg
	thirdparty/mng
	thirdparty/zlib
	thirdparty/SDL2/include
	thirdparty/srutil
	thirdparty
	dif/include
	thirdparty/v8/include
	thirdparty/assimp/include
	thirdparty/duktape
)

# Main project source
set (DIFVIEWERGAME_SRC
   # Main
	source/main.cpp

	# Base folder
	source/base/config.cpp
	source/base/config.h
	source/base/io.cpp
	source/base/io.h
	source/base/math.cpp
	source/base/math.h
	source/base/ray.h
	source/base/types.h

	# Bitmap folder
	source/render/bitmap/jpegSupport.cpp
	source/render/bitmap/jpegSupport.h
	source/render/bitmap/mngSupport.cpp
	source/render/bitmap/mngSupport.h

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
	source/game/shape.cpp
	source/game/shape.h
	source/game/skybox.cpp
	source/game/skybox.h
	source/game/sphere.cpp
	source/game/sphere.h
	
	# Texture folder
	source/texture/bitmapTexture.cpp
	source/texture/bitmapTexture.h
	source/texture/cubeMapTexture.cpp
	source/texture/cubeMapTexture.h
	source/texture/cubeMapFramebufferTexture.cpp
	source/texture/cubeMapFramebufferTexture.h
	source/texture/texture.cpp
	source/texture/texture.h

	# Physics folder
	source/physics/physicsBody.cpp
	source/physics/physicsBody.h
	source/physics/physicsEngine.cpp
	source/physics/physicsEngine.h
	source/physics/physicsSphere.cpp
	source/physics/physicsSphere.h
	source/physics/bullet/btPhysicsBody.cpp
	source/physics/bullet/btPhysicsBody.h
	source/physics/bullet/btPhysicsEngine.cpp
	source/physics/bullet/btPhysicsEngine.h
	source/physics/bullet/btPhysicsInterior.cpp
	source/physics/bullet/btPhysicsInterior.h
	source/physics/bullet/btPhysicsSphere.cpp
	source/physics/bullet/btPhysicsSphere.h

	# Platform folder
	source/platform/event.h
	source/platform/timer.h
	source/platform/window.h
	source/platformSDL/SDLEvent.cpp
	source/platformSDL/SDLEvent.h
	source/platformSDL/SDLTimer.h
	source/platformSDL/SDLWindow.cpp
	source/platformSDL/SDLWindow.h
	source/platformSDL/SDLConfig.h
	
	# Render folder
	source/render/interiorRender.cpp
	source/render/material.cpp
	source/render/material.h
	source/render/modelManager.cpp
	source/render/modelManager.h
	source/render/renderInfo.h
	source/render/scene.cpp
	source/render/scene.h
	source/render/shader.cpp
	source/render/shader.h
	source/render/util.h
	
	# Script folder
	source/script/baseFunctions.cpp
	source/script/scriptClass.h
	source/script/scriptEngine.cpp
	source/script/scriptEngine.h
	source/script/scriptFunction.h
	source/script/scriptTemplates.h
	source/script/scriptUtil.h

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
)

#------------------------------------------------------------------------------
# duktape 1.3.0
#------------------------------------------------------------------------------

set (DUKTAPE_SRC
	thirdparty/duktape/duk_config.h
	thirdparty/duktape/duktape.h
	thirdparty/duktape/duktape.c
)

# Create the library
add_library(Duktape STATIC ${DUKTAPE_SRC})
set_property(TARGET Duktape APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/duktape/")

#------------------------------------------------------------------------------
# SDL
#------------------------------------------------------------------------------

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2" SDL2)

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
	
	# Duktape javascript engine
	Duktape
)

if (MSVC)
set (DIFVIEWERGAME_LIBRARIES
	${DIFVIEWERGAME_LIBRARIES}

	# GLEW
	GLEW

	Zlib
	
	Kernel32
	User32
	OpenGL32

	# Assimp
	debug ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/bin/x86/assimpd.lib
	optimized ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/bin/x86/assimp.lib
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

source_group("main"                  FILES source/main.cpp)
source_group("render"                REGULAR_EXPRESSION render/.*)
source_group("base"                  REGULAR_EXPRESSION base/.*)
source_group("render\\bitmap"        REGULAR_EXPRESSION bitmap/.*)
source_group("game"                  REGULAR_EXPRESSION game/.*)
source_group("objects"               REGULAR_EXPRESSION objects/.*)
source_group("game\\physics"         REGULAR_EXPRESSION physics/.*)
source_group("game\\physics\\bullet" REGULAR_EXPRESSION bullet/.*)
source_group("platform"              REGULAR_EXPRESSION platform/.*)
source_group("platform\\SDL"         REGULAR_EXPRESSION platformSDL/.*)
source_group("script"                REGULAR_EXPRESSION script/.*)
source_group("texture"               REGULAR_EXPRESSION texture/.*)
source_group("assets\\shaders"       REGULAR_EXPRESSION shaders/.*)
source_group("assets\\scripts"       REGULAR_EXPRESSION scripts/.*)

set_property(TARGET Game PROPERTY COMPILE_FLAGS "")
target_link_libraries(Game ${DIFVIEWERGAME_LIBRARIES})
target_compile_definitions(Game PUBLIC GLEW_STATIC)

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

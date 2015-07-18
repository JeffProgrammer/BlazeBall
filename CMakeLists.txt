cmake_minimum_required (VERSION 3.0)
include (CheckIncludeFiles)
project (DifViewerGame C CXX)

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

add_subdirectory("${Project_SOURCE_DIR}thirdparty/bullet" Bullet)

#------------------------------------------------------------------------------
# SDL
#------------------------------------------------------------------------------

add_subdirectory("${Project_SOURCE_DIR}thirdparty/SDL2" SDL2)

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
endif()

add_library(Jpeg STATIC ${JPEG_SRC})
set_property (TARGET Jpeg APPEND PROPERTY INCLUDE_DIRECTORIES "${Project_SOURCE_DIR}/thirdparty/jpeg")

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
target_link_libraries(Mng libJpeg)
set_property (TARGET Mng APPEND PROPERTY INCLUDE_DIRECTORIES "${Project_SOURCE_DIR}/thirdparty/jpeg")

#------------------------------------------------------------------------------
# GLEW Library
#------------------------------------------------------------------------------

add_library(GLEW STATIC thirdparty/glew/src/glew.c)
set_property(TARGET GLEW APPEND PROPERTY INCLUDE_DIRECTORIES "${Project_SOURCE_DIR}/thirdparty/glew/include")
target_compile_definitions(GLEW PUBLIC GLEW_STATIC)

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
	thirdparty/SDL2/include
	thirdparty
)

# Main project source
set (DIFVIEWERGAME_SRC
   # Main
	source/main.cpp

	# Base folder
	source/base/color.h
	source/base/Io.cpp
	source/base/Io.h
	source/base/math.cpp
	source/base/math.h
	source/base/point2.h
	source/base/point3.h
	source/base/point4.h
	source/base/ray.h
	source/base/types.h

	# Bitmap folder
	source/bitmap/jpegsupport.cpp
	source/bitmap/jpegsupport.h
	source/bitmap/mngsupport.cpp
	source/bitmap/mngsupport.h
	source/bitmap/texture.cpp
	source/bitmap/texture.h

	# Game folder
	source/game/sphere.cpp
	source/game/sphere.h

	# Objects folder
	source/objects/aiSpecialNode.cpp
	source/objects/aiSpecialNode.h
	source/objects/dif.cpp
	source/objects/dif.h
	source/objects/forceField.cpp
	source/objects/forceField.h
	source/objects/gameEntity.cpp
	source/objects/gameEntity.h
	source/objects/Interior.cpp
	source/objects/Interior.h
	source/objects/InteriorPathFollower.cpp
	source/objects/InteriorPathFollower.h
	source/objects/pathedInterior.cpp
	source/objects/pathedInterior.h
	source/objects/staticMesh.cpp
	source/objects/staticMesh.h
	source/objects/trigger.cpp
	source/objects/trigger.h
	source/objects/vehicleCollision.cpp
	source/objects/vehicleCollision.h

	# Physics folder
	source/physics/physicsBody.cpp
	source/physics/physicsBody.h
	source/physics/physicsEngine.cpp
	source/physics/physicsengine.h
	source/physics/physicsSphere.cpp
	source/physics/physicsSphere.h
	source/physics/bullet/btPhysicsBody.cpp
	source/physics/bullet/btPhysicsBody.h
	source/physics/bullet/btPhysicsEngine.cpp
	source/physics/bullet/btPhysicsEngine.h
	source/physics/bullet/btPhysicsInterior.cpp
	source/physics/bullet/btPhysicsInterior.h
	source/physics/bullet/btPhysicsSphere.cpp
	source/physics/bullet/btPhysicssphere.h

	# Platform folder
	source/platform/event.h
	source/platform/window.h

	# PlatformSDL folder
	source/platformSDL/SDLEvent.cpp
	source/platformSDL/SDLEvent.h
	source/platformSDL/SDLWindow.cpp
	source/platformSDL/SDLWindow.h
	source/platformSDL/SDLConfig.h

	# Render folder
	source/render/InteriorRender.cpp
	source/render/material.cpp
	source/render/material.h
	source/render/scene.cpp
	source/render/scene.h
	source/render/shader.cpp
	source/render/shader.h
)

set (DIFVIEWERGAME_LIBRARIES
	# Bullet
	${CMAKE_BINARY_DIR}/Bullet/src/BulletCollision/Debug/libBulletCollision.a
	${CMAKE_BINARY_DIR}/Bullet/src/BulletDynamics/Debug/libBulletDynamics.a
	${CMAKE_BINARY_DIR}/Bullet/src/BulletSoftBody/Debug/libBulletSoftBody.a
	${CMAKE_BINARY_DIR}/Bullet/src/LinearMath/Debug/libLinearMath.a
	
	# SDL2
	${CMAKE_BINARY_DIR}/SDL2/Debug/libSDL2.a

	# GLEW
	${CMAKE_BINARY_DIR}/Debug/libGLEW.a

	# JPEG
	${CMAKE_BINARY_DIR}/Debug/libJpeg.a

	# MNG
	${CMAKE_BINARY_DIR}/Debug/libMng.a
	z #Zlib
)

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

set_property(TARGET Game PROPERTY COMPILE_FLAGS "-DBUILD_PHYSICS -DGL_33")
target_link_libraries(Game ${DIFVIEWERGAME_LIBRARIES})


if (APPLE)
	ADD_FRAMEWORK(Game Cocoa)
	ADD_FRAMEWORK(Game Carbon)
	ADD_FRAMEWORK(Game OpenGL)
	ADD_FRAMEWORK(Game IOKit)
	ADD_FRAMEWORK(Game ForceFeedback)

endif()

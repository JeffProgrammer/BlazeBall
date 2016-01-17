#===================================
# Build script for libRocket =======
#===================================

# Jeff: This file has been modified from the original version.

if(APPLE)
	# This has to be before most other options so CMake properly handles the 
	# compiler variables, it MUST bebefore the project() definition
	if(IOS_PLATFORM)
		set(CMAKE_TOOLCHAIN_FILE cmake/Platform/iOS.cmake)
	endif(IOS_PLATFORM)

	option(BUILD_UNIVERSAL_BINARIES "Build universal binaries for all architectures supported" ON)
	if (NOT CMAKE_OSX_ARCHITECTURES AND BUILD_UNIVERSAL_BINARIES)
		if(IOS)
			# set the architecture for iOS 
			if (${IOS_PLATFORM} STREQUAL "OS")
				set (IOS_ARCH armv6 armv7 armv7s arm64)
				set (CMAKE_OSX_ARCHITECTURES ${IOS_ARCH} CACHE string  "Build architecture for iOS")
			else (${IOS_PLATFORM} STREQUAL "OS")
				set (IOS_ARCH i386 x86_64)
				set (CMAKE_OSX_ARCHITECTURES ${IOS_ARCH} CACHE string  "Build architecture for iOS Simulator")
			endif (${IOS_PLATFORM} STREQUAL "OS")

		else(IOS)
			# set the architectures for OS X
			set (OSXI_ARCH i386 x86_64)
			set (CMAKE_OSX_ARCHITECTURES ${OSXI_ARCH} CACHE string  "Build architecture for OS X universal binaries")		
		endif(IOS)
	endif (NOT CMAKE_OSX_ARCHITECTURES AND BUILD_UNIVERSAL_BINARIES)
endif(APPLE)

# We use the new OSX_ARCHITECTURES property
# and GNUInstallDirs module
cmake_minimum_required(VERSION 2.8.5)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0015 NEW)
endif(COMMAND cmake_policy)

# Enable the use of MACOSX_RPATH by default for CMake v3.0+; this effectively 
# allows plug 'n' play functionality, so to speak -- the resulting shared 
# library files can simply be copied over into the end-user's application 
# bundle or framework bundle. No mucking around with install_name_tool.
#
# 	See also: 
# cmake --help-policy cmp0042
# http://www.kitware.com/blog/home/post/510
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)

project(libRocket C CXX)

# paths
include(GNUInstallDirs)

set(LIBROCKET_VERSION_MAJOR 1)
set(LIBROCKET_VERSION_MINOR 3)
set(LIBROCKET_VERSION_PATCH 0)
set(LIBROCKET_VERSION_TWEAK 0)
set(PROJECT_VERSION ${LIBROCKET_VERSION_MAJOR}.${LIBROCKET_VERSION_MINOR}.${LIBROCKET_VERSION_PATCH}.${LIBROCKET_VERSION_TWEAK})

# Search in the 'cmake' directory for additional CMake modules.
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


#===================================
# Plaform specific global hacks ====
#===================================

if(APPLE)
	# Disables naked builtins from AssertMacros.h which
	# This prevents naming collisions such as those from the check()
	# function macro with LuaType::check
	add_definitions(-D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0)
endif(APPLE)

#===================================
# Build options ====================
#===================================

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

if(NOT IOS)
	option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
endif(NOT IOS)

if(APPLE)
	option(BUILD_FRAMEWORK "Build Framework bundle for OSX" OFF)
endif()

if(APPLE)
	if(IOS)
		if(BUILD_SHARED_LIBS)
			message(FATAL_ERROR "BUILD_SHARED_LIBS must be OFF for iOS builds.  iOS does not support shared libraries.")
		endif(BUILD_SHARED_LIBS)
	endif(IOS)

	if(BUILD_FRAMEWORK)
		if(NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
			message(FATAL_ERROR "You should use Xcode generator with BUILD_FRAMEWORK enabled")
		endif()
		if(NOT BUILD_SHARED_LIBS)
			message(FATAL_ERROR "BUILD_SHARED_LIBS must be ON with BUILD_FRAMEWORK enabled")
		endif()
	endif()
else(APPLE)
	if(BUILD_FRAMEWORK)
		message(FATAL_ERROR "BUILD_FRAMEWORK is only supported on Mac OS X with the Xcode generator")
	endif()
endif(APPLE)

if(NOT BUILD_SHARED_LIBS)
	add_definitions(-DROCKET_STATIC_LIB)
endif()


#===================================
# Find dependencies ================
#===================================

# FreeType
add_subdirectory(../Dependencies/freetype FreeType)
set (FREETYPE_INCLUDE_DIRS
	../Dependencies/freetype/include
	../Dependencies/freetype/include/freetype
)
set (FREETYPE_LINK_DIRS
	../Dependencies/freetype/debug
)
set (FREETYPE_LIBRARY
	freetype
)

include_directories(${FREETYPE_INCLUDE_DIRS})
link_directories(${FREETYPE_LINK_DIRS})
list(APPEND CORE_LINK_LIBS ${FREETYPE_LIBRARY})
mark_as_advanced(FREETYPE_INCLUDE_DIRS FREETYPE_LIBRARY FREETYPE_LINK_DIRECTORIES)

#===================================
# Setup paths ======================
#===================================

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/..)

include_directories(
    ${PROJECT_SOURCE_DIR}/Include
)

# Include list of source files
include(FileList)

if(NOT BUILD_FRAMEWORK)
#===================================
# Build libraries ==================
#===================================

set(LIBRARIES Core Controls Debugger)

foreach(library ${LIBRARIES})
    set(NAME Rocket${library})

    add_library(${NAME}
                        ${${library}_HDR_FILES}
                        ${${library}_PUB_HDR_FILES}
                        ${MASTER_${library}_PUB_HDR_FILES}
			${${library}_SRC_FILES}
    )

    set_target_properties(${NAME} PROPERTIES
                           VERSION ${PROJECT_VERSION}
                           SOVERSION ${LIBROCKET_VERSION_MAJOR}
    )
    
    install(TARGETS ${NAME}
            EXPORT libRocketTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    set(ROCKET_EXPORTED_TARGETS ${ROCKET_EXPORTED_TARGETS} ${NAME})
endforeach(library)

else(NOT BUILD_FRAMEWORK)
	#===================================
	# Build combined Framework =========
	#===================================

	set(NAME Rocket)

	set(MASTER_PUB_HDR_FILES
		${MASTER_Core_PUB_HDR_FILES}
		${MASTER_Controls_PUB_HDR_FILES}
		${MASTER_Debugger_PUB_HDR_FILES}
	)

	add_library(${NAME}
		${Core_HDR_FILES}
		${MASTER_Core_PUB_HDR_FILES}
		${Core_PUB_HDR_FILES}
		${Core_SRC_FILES}
		${Controls_HDR_FILES}
		${MASTER_Controls_PUB_HDR_FILES}
		${Controls_PUB_HDR_FILES}
		${Controls_SRC_FILES}
		${Debugger_HDR_FILES}
		${MASTER_Debugger_PUB_HDR_FILES}
		${Debugger_PUB_HDR_FILES}
		${Debugger_SRC_FILES}
	)

	set_target_properties(${NAME} PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${LIBROCKET_VERSION_MAJOR}
	)

		set_property(SOURCE ${MASTER_PUB_HDR_FILES}
			PROPERTY MACOSX_PACKAGE_LOCATION Headers
		)
		set_property(SOURCE ${Core_PUB_HDR_FILES}
			PROPERTY MACOSX_PACKAGE_LOCATION Headers/Core
		)
		set_property(SOURCE ${Controls_PUB_HDR_FILES}
			PROPERTY MACOSX_PACKAGE_LOCATION Headers/Controls
		)
		set_property(SOURCE ${Debugger_PUB_HDR_FILES}
			PROPERTY MACOSX_PACKAGE_LOCATION Headers/Debugger
		)
		set_target_properties(${NAME} PROPERTIES
			FRAMEWORK TRUE
			FRAMEWORK_VERSION ${PROJECT_VERSION}
			MACOSX_FRAMEWORK_IDENTIFIER com.librocketb.${NAME}
			MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${LIBROCKET_VERSION_MAJOR}.${LIBROCKET_VERSION_MINOR}.${LIBROCKET_VERSION_PATCH}.${LIBROCKET_VERSION_TWEAK}
			MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
			XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
			PUBLIC_HEADER ${MASTER_PUB_HDR_FILES}
		)

    install(TARGETS ${NAME}
            EXPORT libRocketTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            FRAMEWORK DESTINATION Library/Frameworks
    )

    set(ROCKET_EXPORTED_TARGETS ${ROCKET_EXPORTED_TARGETS} ${NAME})
endif(NOT BUILD_FRAMEWORK)

#===================================
# Link libraries ===================
#===================================

if(NOT BUILD_FRAMEWORK)
target_link_libraries(RocketCore ${CORE_LINK_LIBS})
target_link_libraries(RocketControls RocketCore)
target_link_libraries(RocketDebugger RocketCore)
else(NOT BUILD_FRAMEWORK)
target_link_libraries(Rocket ${CORE_LINK_LIBS})
endif(NOT BUILD_FRAMEWORK)

project "game"
	targetname "game"
	language "C++"
	kind "ConsoleApp"
	flags { "StaticRuntime" }
	targetdir ("../..")

	includedirs {
		"../../source",
		"../../thirdparty/glfw/include",
		"../../thirdparty/glew/include",
		"../../thirdparty",
		"../../thirdparty/jpeg",
		"../../thirdparty/glm",
		"../../thirdparty/bullet/src",
	}

	files {
		"../../source/base/**",
		"../../source/bitmap/**",
		"../../source/game/**",
		"../../source/main.cpp",
		"../../source/objects/**",
		"../../source/physics/**",
		"../../source/platform/**",
		"../../source/platformGLFW/**",
		"../../source/render/**",
	}

	defines {
		"GLEW_STATIC",
		"BUILD_PHYSICS",
		"GL_33",
	}

	links {
		"jpeg",
		"glfw",
		"glew",
		"jpeg",
		"mng",
		"bullet",
	}

	configuration "macosx"
		links {
			"CoreFoundation.framework",
			"OpenGL.framework",
			"Cocoa.framework",
		}

		buildoptions_cpp {
			"-std=c++11",
			"-stdlib=libc++",
		}

		linkoptions {
			"-stdlib=libc++",
		}
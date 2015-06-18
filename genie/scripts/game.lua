project "game"
	targetname "game"
	language "C++"
	kind "ConsoleApp"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs {
		"../../source",
		"../../thirdparty/glfw/include",
		"../../thirdparty/glew/include",
		"../../thirdparty/glm",
	}

	files {
		"../../source/base/**",
		"../../source/bitmap/**",
		"../../source/game/**",
		"../../source/main.cpp",
		"../../source/objects/**",
		-- "../../source/physics/**",
		"../../source/render/**",
	}

	defines {
		"GLEW_STATIC",
	}

	links {
		"jpeg"
	}

	configuration "macosx"
		files {
			"../../source/platformGLFW/**"
		}

		links {
			"CoreFoundation.framework",
			"OpenGL.framework",
			"Cocoa.framework",
			"glfw",
			"glew",
		}
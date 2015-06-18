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
	}
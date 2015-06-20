project "bullet"
	targetname "bullet"
	language "C"
	kind "StaticLib"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs {
		"../../thirdparty/bullet/src",
	}

	files {
		"../../thirdparty/bullet/src/**.h",
		"../../thirdparty/bullet/src/**.cpp",
	}
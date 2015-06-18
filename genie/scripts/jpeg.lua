project "jpeg"
	targetname "jpeg"
	language "C"
	kind "StaticLib"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs {
		"../../thirdparty/jpeg",
	}

	files {
		"../../thirdparty/jpeg/**",
	}
project "mng"
	targetname "mng"
	language "C"
	kind "StaticLib"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs {
		"../../thirdparty/jpeg",
		"../../thirdparty/mng",
	}

	files {
		"../../thirdparty/mng/**.c",
		"../../thirdparty/mng/**.h",
	}

	links {
		"jpeg",
	}
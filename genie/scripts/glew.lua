project "glew"
	targetname "glew"
	language "C"
	kind "StaticLib"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs { "../../thirdparty/glew/include" }

	files { "../../thirdparty/glew/src/glew.c" }

	defines { "GLEW_STATIC" }

	configuration "windows"
		defines { "_CRT_SECURE_NO_WARNINGS" }
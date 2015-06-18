project "glfw"
	targetname "glfw"
	language "C"
	kind "StaticLib"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs {
		"../../thirdparty/glfw/include",
		"../../thirdparty/glfw/deps/GL",
	}

	files {
		"../../thirdparty/glfw/src/internal.h",

		"../../thirdparty/glfw/src/context.c",
		"../../thirdparty/glfw/src/init.c",
		"../../thirdparty/glfw/src/input.c",
		"../../thirdparty/glfw/src/monitor.c",
		"../../thirdparty/glfw/src/window.c",
	}

	configuration "windows"
		-- this is in place for glfw_config.c as it's just defines
		defines {
			"_CRT_SECURE_NO_WARNINGS",
			"_GLFW_WIN32",
			"_GLFW_WGL",
			"_GLFW_USE_OPENGL",
		}

		files {
			"../../thirdparty/glfw/src/wgl_context.h",
			"../../thirdparty/glfw/src/win32_platform.h",
			"../../thirdparty/glfw/src/win32_tls.h",
			"../../thirdparty/glfw/src/winmm_joystick.h",

			"../../thirdparty/glfw/src/wgl_context.c",
			"../../thirdparty/glfw/src/win32_init.c",
			"../../thirdparty/glfw/src/win32_monitor.c",
			"../../thirdparty/glfw/src/win32_time.c",
			"../../thirdparty/glfw/src/win32_tls.c",
			"../../thirdparty/glfw/src/win32_window.c",
			"../../thirdparty/glfw/src/winmm_joystick.c",
		}

	-- TODO MacOSX and Linux

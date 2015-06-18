solution "DifViewer"
	configurations { "Release", "Debug" }

   -- Set location of build files
   BUILD_LOCATION = "";
   if os.get() == "windows" then
      BUILD_LOCATION = "../Windows";
   elseif os.get() == "macosx" then
      BUILD_LOCATION = "../OSX";
   elseif os.get() == "linux" then
      BUILD_LOCATION = "../Linux";
   else
      BUILD_LOCATION = "../Unspecified";
   end
   location (BUILD_LOCATION)

	-- main project
	dofile("game.lua")

	-- dependencies
	dofile("glew.lua")
	dofile("glfw3.lua")ß
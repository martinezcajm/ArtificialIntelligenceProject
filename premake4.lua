-- ---------------------------------
-- Toni Barella
-- Inteligencia Artificial 3º
-- ESAT 2016-2019
-- Genie Project Configuration.
-- ---------------------------------

solution ("3IA_Solution" .. _ACTION)
  configurations { "Debug", "Release" }
  platforms { "x32", "x64" }
  location ("build")
  language "C"
  kind "ConsoleApp"
	
	language "C++"
	kind "ConsoleApp"

	projects = { "PR0_Base", "PR1_AStar" }

	for i, prj in ipairs(projects) do 
		project (prj)
		targetname (prj)

		prj_path = "./build/" .. prj

		location (prj_path .. "/" .. _ACTION)
		
		includedirs {
			"./include/",			
			"./deps",
			"./deps/ESAT/include",
			"./.deps/Math",
			"./.deps/STB"
		}
		
		libdirs{ 
		"./deps/ESAT/bin"
		}

		files {
			"./deps/Math/*.h",
			"./deps/STB/*.h",
			"./deps/common_def.h",
			"./deps/platform_types.h"
		}
	
	    defines { "_CRT_SECURE_NO_WARNINGS" }
	    flags { "ExtraWarnings" }

		configuration "vs2015"
		  windowstargetplatformversion "8.1"

		configuration "vs2017"
		  --windowstargetplatformversion "10.0.15063.0"
		  windowstargetplatformversion "10.0.16299.0"
		  --windowstargetplatformversion "10.0.17134.0"
		  
		configuration "windows"
			links { "opengl32", "user32", "gdi32", "shell32"}
		
		configuration "Debug"
		   defines {"DEBUG"}
		   links {"ESAT_d", "ESAT_extra_d"}
		   targetdir "./bin/debug"
		   flags { "Symbols" }
		   implibsuffix "-d"

		configuration "Release"
		  links {"ESAT", "ESAT_extra"}
		  targetdir "./bin/release"
		  flags { "OptimizeSpeed", "No64BitChecks" }

	end

	project "PR0_Base"
		files {
		"./include/agent.h",
		"./include/path.h",
		"./include/gamestate.h",
		"./include/astar.h",
		"./include/map.h",
		"./include/path_finder.h",
		"./src/agent.cc",
		"./src/path.cc",
		"./src/path_finder.cc",
		"./src/astar.cpp",
		"./src/gamestate.cc",
		"./src/map.cc",
		"./tests/main_base.cc",
		
		}


	project "PR1_AStar"
		files {
		"./include/agent.h",
		"./include/path.h",
		"./include/gamestate.h",
		"./include/astar.h",
		"./include/map.h",
		"./include/path_finder.h",
		"./src/agent.cc",
		"./src/path.cc",
		"./src/path_finder.cc",
		"./src/astar.cpp",
		"./src/gamestate.cc",
		"./src/map.cc",
		"./tests/main_astar.cpp",
	}
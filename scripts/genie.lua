solution "IA"
  location "../build"
  configurations {
    "Debug",
    "Release"
  }

  platforms{
    "x32",
    "x64",
    "Native"
  }
  --windowstargetplatformversion "10.0.17134.0"
  language "C++"

  PROJ_DIR = path.getabsolute("..")

  project "IA"
    kind "ConsoleApp"

    includedirs {
      path.join(PROJ_DIR, "include"),
      path.join(PROJ_DIR, "deps/ESAT_rev196_vs2015/include"),
	  path.join(PROJ_DIR, "deps/Math")
    }
	
	libdirs{ 
		path.join(PROJ_DIR, "deps/ESAT_rev196_vs2015/bin")		
	}

    files {
      path.join(PROJ_DIR, "src/*.cc"),
	  path.join(PROJ_DIR, "include/*.h"),
	  path.join(PROJ_DIR, "deps/Math/*.h")
    }
   
    configuration "windows"
      links { "opengl32", "user32", "gdi32", "shell32"}
   
    configuration "Debug"
	  links {"ESAT_d", "ESAT_extra_d"}
      targetdir "../bin/debug"
      flags { "Symbols" }
      implibsuffix "-d"

    configuration "Release"
	  links {"ESAT", "ESAT_extra"}
      targetdir "../bin/release"
      flags { "OptimizeSpeed", "No64BitChecks" }

    configuration {"gmake"}
      buildoptions_cpp{
        "-x c++",
        "-std=c++14"
      }
    configuration{}
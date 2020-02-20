workspace "SpaceShooter"
	startproject "SpaceShooter"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "SpaceShooter"
	location "SpaceShooter"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/**.h",
		"%{prj.name}/vendor/**.hpp",
		"%{prj.name}/vendor/**.c"
	}

	libdirs
	{
		"SpaceShooter/vendor/openal",
		"SpaceShooter/vendor/glfw"
	}

	links
	{
		"OpenAL32",
		"glfw.win32",
		"opengl32"
	}
	
	postbuildcommands
	{
		("{COPY} OpenAL32.dll \"../bin/" .. outputdir .. "/SpaceShooter/\"")
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
workspace "UllrEngine"
	architecture "x64"
	startproject "sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Include directories relative to root folder (solution directory)
	IncludeDir = {}
	IncludeDir["spdlog"] = "ullr/vendor/spdlog/include"
	IncludeDir["glfw"] = "ullr/vendor/glfw/include"
	IncludeDir["glad"] = "ullr/vendor/glad/include"
	IncludeDir["ImGUI"] = "ullr/vendor/imgui"
	-- IncludeDir["glm"] = "Hazel/vendor/glm"

	group "dependencies"
		include "ullr/vendor/glfw"
		include "ullr/vendor/glad"
		include "ullr/vendor/imgui"

	group""

project "ullr"
	location "ullr"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ullrpch.h"
	pchsource "ullr/src/ullrpch.cpp"
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
--		"%{prj.name}/vendor/glm/glm/**.hpp",
--		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs	{
		"%{prj.name}/src", 
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGUI}",
--		"%{IncludeDir.glm}"
	}

	links {
		"glfw",
		"glad",
		"ImGUI",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		disablewarnings { "4996", "4251" }

		defines {
			"ULLR_PLATFORM_WINDOWS",
			"ULLR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		links {
			"opengl32.lib"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/sandbox/\"")
		}

	filter "system:linux"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"ULLR_PLATFORM_LINUX",
			"ULLR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		links {
			"GL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/sandbox/\"")
		}

	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"ULLR_PLATFORM_MACOS",
			"ULLR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		linkoptions { "-framework Cocoa -framework CoreVideo -framework IOKit -framework OpenGL" }

		--postbuildcommands {
		--	("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/sandbox/\"")
		--}

	filter { "system:macosx", "action:xcode4" }
		-- pchheader "src/ullrpch.h"

	filter "configurations:Debug"
		defines { "ULLR_DEBUG", "UL_ENABLE_ASSERTS" }
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ULLR_RELEASE"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ULLR_DIST"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{IncludeDir.spdlog}",
		"ullr/src"
	}

	links {
		"ullr"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		disablewarnings { "4996", "4251" }

		defines {
			"ULLR_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"ULLR_PLATFORM_LINUX"
		}

	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"ULLR_PLATFORM_MACOS"
		}

	filter "configurations:Debug"
		defines "ULLR_DEBUG"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ULLR_RELEASE"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ULLR_DIST"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

-- Fix rpaths to point to local directory on release and dist builds for macos and linux
	filter { "system:linux or system:macosx", "configurations:Release or configurations:Dist" 
}
		runpathdirs { "bin/" .. outputdir .. "/%{prj.name}" }

--	filter { "system:macosx", "configurations:Release or configurations:Dist" }
--		linkoptions {"-Wl,-rpath,./"

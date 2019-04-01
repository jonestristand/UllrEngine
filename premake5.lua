-- Implement the workspace files command for solution-scope files
require('vstudio')
premake.api.register {
  name = "workspace_files",
  scope = "workspace",
  kind = "list:string",
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
  if wks.workspace_files and #wks.workspace_files > 0 then
    premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "_global", "_global", "{' .. os.uuid("_global:"..wks.name) .. '}"')
    premake.push("ProjectSection(SolutionItems) = preProject")
    for _, path in ipairs(wks.workspace_files) do
      premake.w(path.." = "..path)
    end
    premake.pop("EndProjectSection")
    premake.pop("EndProject")
  end
  base(wks)
end)

-- Premake configuration begins here
workspace "UllrEngine"
  architecture "x64"
  startproject "sandbox"

  configurations {
    "Debug",
    "Release",
    "Dist"
  }

  workspace_files {
    ".editorconfig"
  }

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

  -- Include directories relative to root folder (solution directory)
  IncludeDir = {}
  IncludeDir["spdlog"] = "ullr/vendor/spdlog/include"
  IncludeDir["glfw"] = "ullr/vendor/glfw/include"
  IncludeDir["glad"] = "ullr/vendor/glad/include"
  IncludeDir["ImGUI"] = "ullr/vendor/imgui"
  IncludeDir["glm"] = "ullr/vendor/glm"

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
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.inl",
  }

  includedirs	{
    "%{prj.name}/src", 
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.glfw}",
    "%{IncludeDir.glad}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.ImGUI}"
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
    "ullr/src",
    "%{IncludeDir.glm}"
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

--  filter { "system:macosx", "configurations:Release or configurations:Dist" }
--    linkoptions {"-Wl,-rpath,./"

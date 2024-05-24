workspace "ApplicationFramework"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

-- Directory final files will be placed into
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "ApplicationFramework/vendor/GLFW/include"

include "ApplicationFramework/vendor/GLFW"

project "ApplicationFramework"
    location "ApplicationFramework"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "APP_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "APP_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "APP_Release"
        optimize "On"

    filter "configurations:Dist"
        defines "APP_Dist"
        optimize "On"

project "ExampleProject"
    location "ExampleProject"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files {
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp"
    }
    
    includedirs {
        "ApplicationFramework/src",
        "%{prj.name}/src"
    }

    links {
        "ApplicationFramework"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
        defines {
            "APP_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "APP_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "APP_Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "APP_Dist"
        optimize "On"
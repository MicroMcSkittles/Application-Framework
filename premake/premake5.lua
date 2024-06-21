-- This is a project template copy it into your root directory and replace the strings with the contents 
-- PROJECT_NAME_HERE with your project name if you are using the application framework as a static library.

workspace "ApplicationFramework"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

	startproject "PROJECT_NAME_HERE"

-- Directory final files will be placed into
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Libraries
include("Application-Framework/ApplicationFramework/vendor")

-- Application Framework
include("Application-Framework/ApplicationFramework")

project "PROJECT_NAME_HERE"
    location "PROJECT_NAME_HERE"
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
        "Application-Framework/ApplicationFramework/src",
        "%{prj.name}/src",
		"Application-Framework/ApplicationFramework/vendor/glm"
    }

    links {
        "ApplicationFramework",
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
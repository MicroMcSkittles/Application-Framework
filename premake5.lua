workspace "ApplicationFramework"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

	startproject "ExampleProject"

-- Directory final files will be placed into
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Library directories relative to solution directory
LibDir = {}
LibDir["GLFW"] = "ApplicationFramework/vendor/GLFW"
LibDir["GLAD"] = "ApplicationFramework/vendor/GLAD"
LibDir["glm"] = "ApplicationFramework/vendor/glm"
LibDir["STBImage"] = "ApplicationFramework/vendor/STBImage"
LibDir["TinyOBJLoader"] = "ApplicationFramework/vendor/TinyOBJLoader"

-- Include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "%{LibDir.GLFW}/include"
IncludeDir["GLAD"] = "%{LibDir.GLAD}/include"
IncludeDir["glm"] = "%{LibDir.glm}"
IncludeDir["STBImage"] = "%{LibDir.STBImage}"
IncludeDir["TinyOBJLoader"] = "%{LibDir.TinyOBJLoader}"


-- GLFW =====================================================
project "GLFW"
    location "%{LibDir.GLFW}"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{LibDir.GLFW}/include/GLFW/glfw3.h",
		"%{LibDir.GLFW}/include/GLFW/glfw3native.h",
		"%{LibDir.GLFW}/src/glfw_config.h",
		"%{LibDir.GLFW}/src/context.c",
		"%{LibDir.GLFW}/src/init.c",
		"%{LibDir.GLFW}/src/input.c",
		"%{LibDir.GLFW}/src/monitor.c",

		"%{LibDir.GLFW}/src/null_init.c",
		"%{LibDir.GLFW}/src/null_joystick.c",
		"%{LibDir.GLFW}/src/null_monitor.c",
		"%{LibDir.GLFW}/src/null_window.c",

		"%{LibDir.GLFW}/src/platform.c",
		"%{LibDir.GLFW}/src/vulkan.c",
		"%{LibDir.GLFW}/src/window.c",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"%{LibDir.GLFW}/src/win32_init.c",
			"%{LibDir.GLFW}/src/win32_joystick.c",
			"%{LibDir.GLFW}/src/win32_module.c",
			"%{LibDir.GLFW}/src/win32_monitor.c",
			"%{LibDir.GLFW}/src/win32_time.c",
			"%{LibDir.GLFW}/src/win32_thread.c",
			"%{LibDir.GLFW}/src/win32_window.c",
			"%{LibDir.GLFW}/src/wgl_context.c",
			"%{LibDir.GLFW}/src/egl_context.c",
			"%{LibDir.GLFW}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"
-- ==========================================================

-- GLAD =====================================================
project "GLAD"
    location "%{LibDir.GLAD}"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{LibDir.GLAD}/include/glad/glad.h",
		"%{LibDir.GLAD}/include/KHR/khrplatform.h",
		"%{LibDir.GLAD}/src/glad.c",
	}

    includedirs {
        "%{LibDir.GLAD}/include"
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"
-- ==========================================================

-- glm ======================================================
project "glm"
    location "%{LibDir.glm}"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{LibDir.glm}/**.hpp",
		"%{LibDir.glm}/**.h",
	}

    includedirs {
        "%{LibDir.glm}"
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"
-- ==========================================================

-- TinyOBJLoader ============================================
project "TinyOBJLoader"
    location "%{LibDir.TinyOBJLoader}"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{LibDir.TinyOBJLoader}/tiny_obj_loader.h",
		"%{LibDir.TinyOBJLoader}/tiny_obj_loader.cc",
	}

    includedirs {
        "%{LibDir.TinyOBJLoader}"
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"
-- ==========================================================

-- stb image ================================================
project "STBImage"
    location "%{LibDir.STBImage}"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{LibDir.STBImage}/stb_image.h",
	}

    includedirs {
        "%{LibDir.STBImage}"
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"
-- ==========================================================

-- Application Framework ====================================
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
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.STBImage}",
        "%{IncludeDir.TinyOBJLoader}",
    }

    links {
        "GLFW",
        "GLAD",
		"TinyOBJLoader",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "APP_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
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
-- ==========================================================

-- Example Project ==========================================
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
        "%{prj.name}/src",
		"%{IncludeDir.glm}"
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
-- ==========================================================
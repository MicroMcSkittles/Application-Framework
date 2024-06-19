project "ApplicationFramework"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.c",
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp"
    }

    includedirs {
        "src",
        "vendor/GLFW/include",
        "vendor/GLAD/include",
        "vendor/glm",
        "vendor/STBImage",
        "vendor/TinyOBJLoader",
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
			"GLFW_INCLUDE_NONE",
			"GLM_ENABLE_EXPERIMENTAL"
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
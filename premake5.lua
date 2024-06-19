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

-- Library
include("ApplicationFramework/vendor")

-- Application Framework
include("ApplicationFramework")

-- Example Project
include("ExampleProject")
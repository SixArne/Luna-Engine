project "imguiplot"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" ..outputDir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputDir.. "/%{prj.name}")

    files
    {
        "**.cpp",
        "**.h"
    }

    includedirs
    {
        "include/",
        "../imgui"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "Off"
        defines
        {
            "IMGUI_DEFINE_MATH_OPERATORS"
        }

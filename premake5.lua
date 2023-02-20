workspace "Programming-4-Engine"
    architecture "x64"
    configurations
    {
        "Debug",
        "Release"
    }


vendorFolder = "3rdParty/"
outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Minigin"
    location "Minigin"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" ..outputDir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputDir.. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/pch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        vendorFolder.."glm",
        vendorFolder.."SDL2/include",
        vendorFolder.."SDL2_image/include",
        vendorFolder.."SDL2_ttf/include",
    }

    libdirs
    {
        vendorFolder.."SDL2/lib/x64",
        vendorFolder.."SDL2_image/lib/x64",
        vendorFolder.."SDL2_ttf/lib/x64"
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
        warnings "extra"
        flags {"FatalWarnings"}

        links
        {
            "SDL2.lib",
            "SDL2main.lib",
            "SDL2_image.lib",
            "SDL2_ttf.lib"
        }

        postbuildcommands
        {
            ("{COPY} ../Data/SDL/ ../bin/"..outputDir.."/Minigin")
        }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

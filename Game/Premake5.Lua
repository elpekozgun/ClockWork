project "Game"
    kind "ConsoleApp"    
    language "C++"
    cppdialect "C++20"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
		"../ClockWork/src",
		"../ClockWork/vendor",
		"../ClockWork/%{IncludeDir.glm}",
		"../ClockWork/%{IncludeDir.Glad}",
		"../ClockWork/%{IncludeDir.ImGui}"
    }

    links
    {
        "ClockWork"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "Off"
        systemversion "latest"
        
        defines
        {
            "CW_PLATFORM_WINDOWS"
        }
 
        filter "configurations:Debug"
        defines "CW_DEBUG"
        symbols "On"

        filter "configurations:Release"
        defines "CW_RELEASE"
        optimize "On"

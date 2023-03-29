project "ClockWork"
    kind "SharedLib"    -- SharedLib for .dll
    staticruntime "Off"
    language "C++"
    cppdialect "C++20"
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "3rd/Glad/src/glad.c",
        "3rd/imgui/**.cpp",
        "3rd/imgui/**.h",
    }

    includedirs
    {
        "src",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Json}",
        "%{IncludeDir.stb}"
    }

    libdirs
    {
        "3rd/GLFW/lib-vc2022"
		--"vendor/assimp/lib"
    }

	links 
	{ 
		"glfw3",
		"opengl32.lib"
	}


    filter "system:windows"
        systemversion "latest"
        
        defines
        {
            "CW_BUILD_DLL",
            "CW_PLATFORM_WINDOWS"
        }
		
		postbuildcommands
		{
			"{COPY} %{wks.location}bin/"..outputdir.."/ClockWork/ClockWork.dll %{wks.location}/bin/"..outputdir.."/Game/"
		}
		
        filter "configurations:Debug"
        defines "CW_DEBUG"
        symbols "On"

        filter "configurations:Release"
        defines "CW_RELEASE"
        optimize "On"

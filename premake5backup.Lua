
---SOLUTION---------
workspace "ClockWork"
    architecture "x64"
	startproject "Game"
	
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

IncludeDir = {}
IncludeDir["GLFW"] = "3rd/GLFW/include"
IncludeDir["Glad"] = "3rd/Glad/include"
IncludeDir["ImGui"] = "3rd/ImGui/src"
IncludeDir["glm"] = "3rd/glm"
--IncludeDir["assimp"] = "3rd/assimp/include"

---Project ClockWork---

project "ClockWork"
    location"ClockWork"
    kind "SharedLib"    -- SharedLib for .dll
    language "C++"
	
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "3rd/Glad/src/glad.c"
    }

    includedirs
    {
        "%{wks.location}/ClockWork/src",
		"%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
		--"%{IncludeDir.assimp}"
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
		--"assimp-vc140-mt.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
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

        filter "configurations:Dist"
        defines "CW_DIST"
        symbols "On"
---END SeaRenderer---

project "Game"
    location "Game"
    kind "ConsoleApp"    
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/ClockWork/src",
        "%{wks.location}/ClockWork/3rd",
		"%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
		--"%{IncludeDir.assimp}"
    }

    links
    {
        "ClockWork"
    }

    filter "system:windows"
        cppdialect "C++17"
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

        filter "configurations:Dist"
        defines "CW_DIST"
        symbols "On"
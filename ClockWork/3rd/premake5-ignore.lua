project "ImGui"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/ClockWork/3rd/GLFW/include",
        "%{wks.location}/ClockWork/3rd/Glad/include"
    }
    
    defines
    {
        "IMGUI_IMPL_OPENGL_LOADER_GLAD",
        "CW_PLATFORM_WINDOWS"
    }


    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "Off"
        
    filter { "system:windows", "configurations:Release" }
        -- buildoptions "/MD"
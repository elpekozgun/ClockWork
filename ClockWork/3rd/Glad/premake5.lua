project "Glad"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    files
    {
        "src/**.h",
        "src/**.cpp"
    }


    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"
        staticruntime "Off"
        
    filter { "system:windows", "configurations:Release" }
        -- buildoptions "/MD"
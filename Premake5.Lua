

workspace "ClockWork" 
    architecture "x64"
	startproject "Game"
	
    configurations
    {
        "Debug",
        "Release",
    }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ClockWork/3rd/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/ClockWork/3rd/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/ClockWork/3rd/ImGui"
IncludeDir["glm"] = "%{wks.location}/ClockWork/3rd/glm"

include "ClockWork"
include "Game"

--Projects
--group "Dependencies"
    --include("Clockwork/3rd/Glad")
	--include("Clockwork/3rd/GLFW")
	--include "Clockwork/3rd/glm"
	--include "Clockwork/3rd/ImGui"
--group ""




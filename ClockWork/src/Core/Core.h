#ifndef CORE_H
#define CORE_H

enum eOS
{
	Undef = 0,
	Windows = 1,
	Linux = 2,
	Mac = 3
};

enum eRenderSpec
{
	Opengl = 0,
	Directx,
	Vulkan,
	Metal
};

#ifdef CW_PLATFORM_WINDOWS

#ifdef CW_BUILD_DLL
#define CW_API __declspec(dllexport)
#else
#define CW_API __declspec(dllimport)
#endif 
constexpr eOS OperatingSystem = eOS::Windows;
#else
#endif 

#ifdef CW_APISPEC_OPENGL
constexpr eRenderSpec RendererApi = Render_Spec::OpenGl;
#elif CW_APISPEC_DIRECTX
constexpr eRenderSpec RendererApi = Render_Spec::DirectX;
#elif CW_APISPEC_VULKAN
constexpr eRenderSpec RendererApi = Render_Spec::Vulkan;
#elif CW_APISPEC_METAL
constexpr eRenderSpec RendererApi = Render_Spec::Metal;
#elses
constexpr eRenderSpec RendererApi = eRenderSpec::Opengl;
#endif

#endif
#ifndef RENDERER_API_H
#define RENDERER_API_H

#ifdef RENDERENGINE_EXPORT
#define RENDERENGINE_USAGE __declspec(dllexport)
#else
#define RENDERENGINE_USAGE __declspec(dllimport)
#endif

#include <Windows.h>
#include <string>
#include "Definitions\GraphicalDefinitions.h"

namespace Renderer
{
	class __declspec(dllexport) API
	{
	public:
		static bool Init(HWND hwnd, unsigned int width, unsigned int height, bool fullscreen);
		static void Flush();

		static void SetView(float m[16]);
		static void SetProjection(float fov, float aspect, float nearZ, float farZ);
		static void SetProjection(float m[16]);

		static void BeginFrame();
		static bool EndFrame();

		static void RenderScene();

		static Definitions::TextureInstance* CreateTexture(std::wstring filename);

		static void RenderSprite(const Definitions::TextureInstance* sprite);

		static void SetOptions();

	};
}

#endif
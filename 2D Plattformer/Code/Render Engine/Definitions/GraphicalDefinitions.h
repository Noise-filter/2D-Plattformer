#ifndef GRAPHICAL_DEFINITIONS_H
#define GRAPHICAL_DEFINITIONS_H

#include "..\Core\Directx11Includes.h"

namespace Renderer
{
	namespace Definitions
	{
		struct Point
		{
			DirectX::XMFLOAT2 pos;
		};

		struct Vertex
		{
			DirectX::XMFLOAT4 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 tex;
		};

		struct TextureInstance
		{
			class Texture;

			Texture* texture;
			DirectX::XMFLOAT4X4 world;
		};
	}
}

#endif
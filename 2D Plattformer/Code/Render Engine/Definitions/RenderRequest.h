#ifndef RENDER_REQUEST_H
#define RENDER_REQUEST_H

#include "..\Core\Directx11Includes.h"
#include "Texture.h"

namespace Renderer
{
	struct RenderRequest
	{
		Texture* texture;
		DirectX::XMFLOAT4X4 world;
	};
}

#endif
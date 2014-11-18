#ifndef TEXTURE_H
#define TEXTURE_H

#include "..\Core\Directx11Includes.h"
#include <string>

namespace Renderer
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool Init(ID3D11Device* device, std::wstring filename);

		void Clean();

		ID3D11ShaderResourceView *const* GetTexture();

		ID3D11ShaderResourceView* texture;
	private:

	};
}

#endif
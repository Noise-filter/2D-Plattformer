#include "Texture.h"

#include "..\..\DirectXTK\Inc\DDSTextureLoader.h"

using namespace Renderer;

Texture::Texture()
{

}

Texture::~Texture()
{

}

bool Texture::Init(ID3D11Device* device, std::wstring filename)
{
	if (FAILED(DirectX::CreateDDSTextureFromFile(device, filename.c_str(), nullptr, &texture)))
	{
		return false;
	}

	return true;
}

void Texture::Clean()
{
	SAFE_RELEASE(texture);
}

ID3D11ShaderResourceView *const* Texture::GetTexture()
{
	return &texture;
}
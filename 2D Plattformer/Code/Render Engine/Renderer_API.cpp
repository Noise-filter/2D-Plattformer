#include "Renderer_API.h"
#include "Core\Core.h"
#include "Definitions\GraphicalDefinitions.h"
#include "Definitions\Texture.h"

using namespace Renderer;
using namespace Definitions;

bool API::Init(HWND hwnd, unsigned int width, unsigned int height, bool fullscreen)
{
	Core::resolution.x = width;
	Core::resolution.y = height;

	if (!Core::Init::CreateDeviceAndSwapChain(hwnd, Core::resolution, fullscreen))
	{
		return false;
	}

	if (!Core::Init::CreateDepthStencil(Core::resolution))
	{
		return false;
	}

	if (!Core::Init::CreateBackBuffer())
	{
		return false;
	}

	Core::Init::CreateViewPort(DirectX::XMUINT2(0, 0), Core::resolution);

	Core::Init::SetDefaultMatrix();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Load shaders
	if (!Core::vertexShader.Load(Core::device, L"..\\Content\\Shaders\\VertexShader.cso"))
	{
		return false;
	}

	if (!Core::geometryShader.Load(Core::device, L"..\\Content\\Shaders\\GeometryShader.cso"))
	{
		return false;
	}

	if (!Core::pixelShader.Load(Core::device, L"..\\Content\\Shaders\\PixelShader.cso"))
	{
		return false;
	}

	if (FAILED(Core::device->CreateInputLayout(layout, 1, Core::vertexShader.GetShaderBinaryData(), Core::vertexShader.GetShaderBinarySize(), &Core::inputLayout)))
	{
		return false;
	}

	Point initData[1];
	initData[0].pos = DirectX::XMFLOAT2(0.0f, 0.0f);

	Core::Buffer::BUFFER_INIT_DESC initDesc;
	initDesc.NumElements = 1;
	initDesc.Type = Core::Buffer::VERTEX_BUFFER;
	initDesc.Usage = Core::Buffer::BUFFER_DEFAULT;
	initDesc.ElementSize = sizeof(Point);
	initDesc.InitData = (void*)initData;

	if (FAILED(Core::vertexPoint.Init(initDesc)))
	{
		return false;
	}

	return true;
}

void API::Flush()
{
	Core::Flush();
}

void API::SetView(float m[16])
{
	Core::viewMatrix = DirectX::XMMATRIX(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

void API::SetProjection(float fov, float aspect, float nearZ, float farZ)
{
	DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
}

void API::SetProjection(float m[16])
{
	Core::projMatrix = DirectX::XMMATRIX(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

void API::BeginFrame()
{
	Core::deviceContext->OMSetRenderTargets(1, &Core::backBufferRTV, Core::depthStencil);
	Core::deviceContext->RSSetViewports(1, &Core::viewPort);
	Core::deviceContext->IASetInputLayout(Core::inputLayout);

	float ClearColor[4] = { 0.0f, 0.5f, 0.5f, 0.0f };
	Core::deviceContext->ClearRenderTargetView(Core::backBufferRTV, ClearColor);

	//clear depth info
	Core::deviceContext->ClearDepthStencilView(Core::depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//set topology
	Core::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//Set Shaders
	Core::vertexShader.SetShader(Core::deviceContext);
	Core::geometryShader.SetShader(Core::deviceContext);
	Core::pixelShader.SetShader(Core::deviceContext);
}

bool API::EndFrame()
{
	if (FAILED(Core::swapChain->Present(0, 0)))
		return false;

	return true;
}

void API::RenderScene()
{
	//ID3D11ShaderResourceView* asd = Core::textures.begin()->second->texture;

	Core::vertexPoint.Apply();
	Core::deviceContext->PSSetShaderResources(0, 1, &Core::textures.begin()->second->texture);
	Core::deviceContext->Draw(1, 0);
}

TextureInstance* API::CreateTexture(std::wstring filename)
{
	TextureInstance* tex = new TextureInstance();

	//Get texture from resource manager
	Texture* t = Core::textures[filename];
	
	if (t == NULL)
	{
		t = new Texture();

		if (!t->Init(Core::device, filename))
		{
			//Failed
			SAFE_DELETE(tex);
			SAFE_DELETE(t);
			return NULL;
		}

		Core::textures.at(filename) = t;
	}

	tex->texture = (Renderer::Definitions::TextureInstance::Texture*)t;
	
	return tex;
}

void API::SetOptions()
{

}
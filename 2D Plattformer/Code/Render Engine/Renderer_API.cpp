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
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Load shaders
	if (!Core::vertexShader.Load(Core::device, L"..\\Content\\Shaders\\VertexShader.cso"))
	{
		return false;
	}
	if (!Core::vertexShader2.Load(Core::device, L"..\\Content\\Shaders\\VertexShader2.cso"))
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

	if (FAILED(Core::device->CreateInputLayout(layout, 3, Core::vertexShader.GetShaderBinaryData(), Core::vertexShader.GetShaderBinarySize(), &Core::inputLayout)))
	{
		return false;
	}
	if (FAILED(Core::device->CreateInputLayout(layout2, 1, Core::vertexShader2.GetShaderBinaryData(), Core::vertexShader2.GetShaderBinarySize(), &Core::inputLayout2)))
	{
		return false;
	}

	//Create vertex buffer
	Vertex initData[6];
	initData[0].pos = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f);
	initData[1].pos = DirectX::XMFLOAT4(-0.5f, 0.5f, 0.0f, 1.0f);
	initData[2].pos = DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f);
	initData[3].pos = DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f);
	initData[4].pos = DirectX::XMFLOAT4(-0.5f, 0.5f, 0.0f, 1.0f);
	initData[5].pos = DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f);

	initData[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	initData[1].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	initData[2].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	initData[3].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	initData[4].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	initData[5].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	initData[0].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	initData[1].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	initData[2].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
	initData[3].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
	initData[4].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	initData[5].tex = DirectX::XMFLOAT2(1.0f, 1.0f);

	Core::Buffer::BUFFER_INIT_DESC initDesc;
	initDesc.NumElements = 6;
	initDesc.Type = Core::Buffer::VERTEX_BUFFER;
	initDesc.Usage = Core::Buffer::BUFFER_DEFAULT;
	initDesc.ElementSize = sizeof(Vertex);
	initDesc.InitData = (void*)initData;

	if (FAILED(Core::vertexPoint.Init(initDesc)))
	{
		return false;
	}

	DirectX::XMFLOAT4 initData2[1];
	initData[0].pos = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f);

	initDesc.NumElements = 1;
	initDesc.Type = Core::Buffer::VERTEX_BUFFER;
	initDesc.Usage = Core::Buffer::BUFFER_DEFAULT;
	initDesc.ElementSize = sizeof(DirectX::XMFLOAT4);
	initDesc.InitData = (void*)initData;

	if (FAILED(Core::vertexPoint2.Init(initDesc)))
	{
		return false;
	}

	//Create constant buffer
	//Only including a 4x4 matrix right now
	Core::Buffer::BUFFER_INIT_DESC constInitDesc;
	constInitDesc.NumElements = 1;
	//constInitDesc.InitData = NULL;
	constInitDesc.ElementSize = sizeof(DirectX::XMFLOAT4X4);
	constInitDesc.Type = Core::Buffer::CONSTANT_BUFFER_VS;
	constInitDesc.Usage = Core::Buffer::BUFFER_CPU_WRITE_DISCARD;

	if (FAILED(Core::constantBufferEveryObject.Init(constInitDesc)))
	{
		return false;
	}

	//Create constant buffer for view and proj matrix
	constInitDesc.NumElements = 1;
	//constInitDesc.InitData = NULL;
	constInitDesc.ElementSize = sizeof(DirectX::XMFLOAT4X4) * 2;
	constInitDesc.Type = Core::Buffer::CONSTANT_BUFFER_VS;
	constInitDesc.Usage = Core::Buffer::BUFFER_CPU_WRITE_DISCARD;

	if (FAILED(Core::constantBufferEveryFrame.Init(constInitDesc)))
	{
		return false;
	}

	return true;
}

void API::Flush()
{
	Core::Flush();
}

void API::SetView(float eye[3], float at[3], float up[3])
{
	DirectX::XMVECTOR eye2;
	DirectX::XMVECTOR at2;
	DirectX::XMVECTOR up2;

	eye2 = DirectX::XMVectorSet(eye[0], eye[1], eye[2], 0);
	at2 = DirectX::XMVectorSet(at[0], at[1], at[2], 0);
	up2 = DirectX::XMVectorSet(up[0], up[1], up[2], 0);

	Core::viewMatrix = DirectX::XMMatrixLookAtLH(eye2, at2, up2);
}

void API::SetView(float m[16])
{
	Core::viewMatrix = DirectX::XMMATRIX(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

void API::SetProjection(float fov, float aspect, float nearZ, float farZ)
{
	Core::projMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
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
	Core::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set Shaders
	Core::vertexShader.SetShader(Core::deviceContext);
	//Core::geometryShader.SetShader(Core::deviceContext);
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
	//DirectX::XMFLOAT4X4 m;
	//Map constant buffer with view and projection matrix
	void* res = Core::constantBufferEveryFrame.Map();

	struct
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	}m;
	
	//DirectX::XMMATRIX vp = Core::viewMatrix * Core::projMatrix;

	DirectX::XMStoreFloat4x4(&m.view, Core::viewMatrix);
	DirectX::XMStoreFloat4x4(&m.proj, Core::projMatrix);

	memcpy(res, &m, sizeof(m));

	Core::constantBufferEveryFrame.Unmap();

	Core::constantBufferEveryFrame.Apply(1);

	//Apply a single vertex point
	//The rectangle will be created in the geometry shader
	Core::vertexPoint.Apply();
	
	//Render all 'RenderRequests'
	for each(RenderRequest request in Core::renderRequests)
	{
		//Map data to the buffer
		res = Core::constantBufferEveryObject.Map();
		memcpy(res, request.world.m, sizeof(DirectX::XMFLOAT4X4));
		Core::constantBufferEveryObject.Unmap();

		Core::constantBufferEveryObject.Apply(0);

		Core::deviceContext->PSSetShaderResources(0, 1, request.texture->GetTexture());
		Core::deviceContext->Draw(6, 0);
	}

	Core::renderRequests.clear();
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

void API::Render(const Definitions::TextureInstance* sprite)
{
	RenderRequest r;
	r.texture = (Texture*)sprite->texture;
	r.world = sprite->world;

	Core::renderRequests.push_back(r);
}

void API::SetOptions()
{

}
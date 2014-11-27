#include "Core.h"

using namespace DirectX;
using namespace Renderer;

bool Core::Init::CreateDeviceAndSwapChain(HWND window, XMUINT2 resolution, bool fullscreen)
{
	UINT createDeviceFlags = 0;

	//Might give a slight improvment in speed but cannot be used when multithreading.
	createDeviceFlags = ::D3D11_CREATE_DEVICE_SINGLETHREADED;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Width = resolution.x;
	swapChainDesc.BufferDesc.Height = resolution.y;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = !fullscreen;

	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &initiatedFeatureLevel, &deviceContext)))
	{
		if (deviceContext)
		{
			SAFE_RELEASE(deviceContext);
		}

		if (device)
		{
			SAFE_RELEASE(device);
		}

		if (swapChain)
		{
			SAFE_RELEASE(swapChain);
		}
		return false;
	}

	return true;
}

bool Core::Init::CreateDepthStencil(XMUINT2 resolution)
{
	//Create depth stencil texture
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Width = resolution.x;
	textureDesc.Height = resolution.y;

	ID3D11Texture2D* depthTexture;

	if (FAILED(device->CreateTexture2D(&textureDesc, NULL, &depthTexture)))
	{
		return false;
	}

	//Create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	depthDesc.Flags = 0;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(depthTexture, &depthDesc, &depthStencil)))
	{
		SAFE_RELEASE(depthStencil);
		SAFE_RELEASE(depthTexture);
		return false;
	}

	SAFE_RELEASE(depthTexture);

	return true;
}

bool Core::Init::CreateBackBuffer()
{
	ID3D11Texture2D *pBackBuffer = NULL;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		SAFE_RELEASE(pBackBuffer);
		return false;
	}

	if (FAILED(device->CreateRenderTargetView(pBackBuffer, NULL, &Core::backBufferRTV)))
	{
		SAFE_RELEASE(pBackBuffer);
		SAFE_RELEASE(backBufferRTV);
		return false;
	}

	SAFE_RELEASE(pBackBuffer);

	return true;
}

void Core::Init::CreateViewPort(DirectX::XMUINT2 origin, DirectX::XMUINT2 resolution)
{
	viewPort.TopLeftX = (FLOAT)origin.x;
	viewPort.TopLeftY = (FLOAT)origin.x;
	viewPort.Width = (FLOAT)resolution.x;
	viewPort.Height = (FLOAT)resolution.y;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
}

void Core::Init::SetDefaultMatrix()
{
	DirectX::XMVECTOR eye;
	DirectX::XMVECTOR at;
	DirectX::XMVECTOR up;

	eye = DirectX::XMVectorSet(0, 0, -1, 0);
	at = DirectX::XMVectorSet(0, 0, 10, 0);
	up = DirectX::XMVectorSet(0, 1, 0, 0);

	Core::viewMatrix = DirectX::XMMatrixLookAtLH(eye, at, up);

	Core::viewMatrix = DirectX::XMMatrixTranspose(Core::viewMatrix);

	Core::projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI * 0.45f, (float)(Core::resolution.x) / (float)(Core::resolution.y), 0.01f, 1000.0f);

	Core::projMatrix = DirectX::XMMatrixTranspose(Core::projMatrix);


	/*DirectX::XMFLOAT4X4 ma;
	DirectX::XMStoreFloat4x4(&ma, projMatrix);
	float temp = ma._43;
	ma._43 = ma._34;
	ma._34 = temp;
	projMatrix = DirectX::XMLoadFloat4x4(&ma);
	*/
}
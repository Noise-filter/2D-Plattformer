#include "Core.h"

using namespace Renderer;

//Initiate all static variables
ID3D11Device *Core::device = NULL;
ID3D11DeviceContext *Core::deviceContext = NULL;
IDXGISwapChain* Core::swapChain = NULL;

ID3D11DepthStencilView* Core::depthStencil = NULL;
ID3D11RenderTargetView* Core::backBufferRTV = NULL;

D3D11_VIEWPORT Core::viewPort;

HWND Core::window = NULL;
DirectX::XMUINT2 Core::resolution;
bool Core::fullscreen = false;

DirectX::XMMATRIX Core::viewMatrix;
DirectX::XMMATRIX Core::projMatrix;

std::map<std::wstring, Texture*> Core::textures;

ID3D11InputLayout* Core::inputLayout;
ID3D11InputLayout* Core::inputLayout2;

Shader<ID3D11VertexShader*> Core::vertexShader;
Shader<ID3D11VertexShader*> Core::vertexShader2;
Shader<ID3D11GeometryShader*> Core::geometryShader;
Shader<ID3D11PixelShader*> Core::pixelShader;

Core::Buffer Core::vertexPoint;
Core::Buffer Core::vertexPoint2;
Core::Buffer Core::constantBufferEveryObject;
Core::Buffer Core::constantBufferEveryFrame;

std::vector<RenderRequest> Core::renderRequests;

void Core::Flush()
{
	swapChain->SetFullscreenState(FALSE, NULL);

	SAFE_RELEASE(depthStencil);
	SAFE_RELEASE(backBufferRTV);

	SAFE_RELEASE(swapChain);

	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}
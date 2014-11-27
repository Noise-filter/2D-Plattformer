#ifndef CORE_H
#define CORE_H

#include <map>
#include <string>
#include <vector>

#include "Directx11Includes.h"

#include "..\Definitions\Texture.h"
#include "..\Definitions\RenderRequest.h"

#include "..\\Shaders\Shader.h"

namespace Renderer
{
	class Core
	{
	public:
		static ID3D11Device* device;
		static ID3D11DeviceContext* deviceContext;
		static IDXGISwapChain* swapChain;

		static ID3D11DepthStencilView* depthStencil;
		static ID3D11RenderTargetView* backBufferRTV;

		static D3D11_VIEWPORT viewPort;

		static HWND window;
		static DirectX::XMUINT2 resolution;
		static bool fullscreen;

		static DirectX::XMMATRIX viewMatrix;
		static DirectX::XMMATRIX projMatrix;

		static std::map<std::wstring, Texture*> textures;

		static ID3D11InputLayout* inputLayout;

		static Shader<ID3D11VertexShader*> vertexShader;
		static Shader<ID3D11GeometryShader*> geometryShader;
		static Shader<ID3D11PixelShader*> pixelShader;

		class Buffer;
		static Buffer vertexPoint;
		static Buffer constantBufferEveryObject;
		static Buffer constantBufferEveryFrame;

		static std::vector<RenderRequest> renderRequests;

		static void Flush();

		class Buffer
		{
		public:
			enum BUFFER_TYPE
			{
				VERTEX_BUFFER,
				INDEX_BUFFER,
				CONSTANT_BUFFER_VS,
				CONSTANT_BUFFER_GS,
				CONSTANT_BUFFER_PS,
				CONSTANT_BUFFER_CS,
				STRUCTURED_BUFFER,
				BUFFER_TYPE_COUNT
			};

			enum BUFFER_USAGE
			{
				BUFFER_DEFAULT,
				BUFFER_STREAM_OUT_TARGET,
				BUFFER_CPU_WRITE,
				BUFFER_CPU_WRITE_DISCARD,
				BUFFER_CPU_READ,
				BUFFER_USAGE_COUNT,
				BUFFER_USAGE_IMMUTABLE
			};

			struct BUFFER_INIT_DESC
			{
				BUFFER_TYPE		Type;
				UINT32			NumElements;
				UINT32			ElementSize;
				BUFFER_USAGE	Usage;
				void*			InitData;

				BUFFER_INIT_DESC()
				{
					InitData = NULL;
					Usage = BUFFER_DEFAULT;
				}
			};
		protected:
			ID3D11Buffer*	mBuffer;
			BUFFER_TYPE		mType;
			BUFFER_USAGE	mUsage;

			UINT32			mElementSize;
			UINT32			mElementCount;
		public:
			Buffer();
			virtual ~Buffer();

			HRESULT Init(const BUFFER_INIT_DESC& initDesc);

			void* Map();
			void Unmap();

			operator ID3D11Buffer*();
			operator const ID3D11Buffer*() const;

			HRESULT Apply(UINT32 misc = 0) const;

			ID3D11Buffer* GetBufferPointer();
			UINT32 GetVertexSize();
			UINT32 GetElementCount();
		};

		class Init
		{
		public:
			static bool CreateDeviceAndSwapChain(HWND window, DirectX::XMUINT2 resolution, bool fullscreen);
			static bool CreateDepthStencil(DirectX::XMUINT2 resolution);
			static bool CreateBackBuffer();
			static void CreateViewPort(DirectX::XMUINT2 origin, DirectX::XMUINT2 resolution);
			static void SetDefaultMatrix();
		};
	};
}

#endif
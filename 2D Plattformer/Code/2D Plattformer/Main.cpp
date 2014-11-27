#include <Windows.h>

#include "..\Render Engine\Renderer_API.h"

using namespace Renderer;

const unsigned int SCREEN_WIDTH = 1024;
const unsigned int SCREEN_HEIGHT = 768;
const bool fullscreen = false;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool Render();
void Update(float dt);

Renderer::Definitions::TextureInstance* test;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	if (SetDllDirectory(L"..\\DLL") == FALSE)
	{
		return -1;
	}

	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"2D Plattformer",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	if (!API::Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, fullscreen))
	{
		return 0;
	}

	test = API::CreateTexture(L"..\\Content\\test.dds");
	DirectX::XMMATRIX mTest = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	mTest = DirectX::XMMatrixTranspose(mTest);

	DirectX::XMStoreFloat4x4(&test->world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&test->world, mTest);

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);


	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		__int64 currTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
		wchar_t fps[25];
		float fps2 = 1 / dt;
		swprintf(fps, sizeof(fps), L"%f", fps2);

		SetWindowText(hWnd, fps);
		Update(dt);
		Render();

		prevTimeStamp = currTimeStamp;
	}

	API::Flush();

	return 0;
}

void Update(float dt)
{
	
}

bool Render()
{
	API::BeginFrame();

	for (int i = 0; i < 1000; i++)
	{
		API::Render(test);
	}

	API::RenderScene();
	return API::EndFrame();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}